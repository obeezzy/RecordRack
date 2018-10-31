#include "stocksqlmanager.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlDriver>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QSqlError>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include "singletons/userprofile.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

StockSqlManager::StockSqlManager(const QString &connectionName)
    : AbstractSqlManager(connectionName)
{

}

QueryResult StockSqlManager::execute(const QueryRequest &request)
{
    QueryResult result(request);

    try {
        if (request.command() == "add_new_stock_item")
            addNewStockItem(request);
        else if (request.command() == "update_stock_item")
            updateStockItem(request);
        else if (request.command() == "view_stock_items")
            viewStockItems(request, result);
        else if (request.command() == "view_stock_item_details")
            viewStockItemDetails(request, result);
        else if (request.command() == "view_stock_categories")
            viewStockCategories(request, result);
        else if (request.command() == "remove_stock_item")
            removeStockItem(request, result);
        else if (request.command() == "undo_remove_stock_item")
            undoRemoveStockItem(request, result);
        else
            throw DatabaseException(DatabaseException::RRErrorCode::CommandNotFound, QString("Command not found: %1").arg(request.command()));

        result.setSuccessful(true);
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
    }

    return result;
}

void StockSqlManager::addNewStockItem(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QDateTime &currentDateTime = QDateTime::currentDateTime();
    int categoryNoteId = 0;
    int itemNoteId = 0;
    int categoryId = 0;
    int itemId = 0;
    int unitId = 0;

    QSqlQuery q(connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // Insert category note
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            q.prepare("INSERT INTO note (note, created, last_edited, user_id) VALUES (:note, :created, :last_edited, :user_id)");
            q.bindValue(":note", params.value("category_note").toString());
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert category note.");

            categoryNoteId = q.lastInsertId().toInt();

            if (!categoryNoteId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Invalid category note ID returned.");
        }

        // Insert item note
        if (!params.value("item_note").toString().trimmed().isEmpty()) {
            q.prepare("INSERT INTO note (note, created, last_edited, user_id) VALUES (:note, :created, :last_edited, :user_id)");
            q.bindValue(":note", params.value("item_note").toString());
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert item note.");

            itemNoteId = q.lastInsertId().toInt();
            if (!itemNoteId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Invalid item note ID returned.");
        }


        // Insert category
        q.prepare("INSERT IGNORE INTO category (category, short_form, note_id, archived, created, last_edited, user_id) "
                  "VALUES (:category, :short_form, :note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":category", params.value("category").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":note_id", categoryNoteId > 0 ? categoryNoteId : QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert category.");

        if (q.numRowsAffected() > 0) {
            categoryId = q.lastInsertId().toInt();
            if (!categoryId)
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Invalid category ID returned.");
        } else {
            // Insert category
            q.prepare("SELECT id FROM category WHERE category = :category");
            q.bindValue(":category", params.value("category").toString());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert category.");

            if (!q.first())
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure,
                                        q.lastError().text(),
                                        QString("Expected category ID for category '%1'.")
                                        .arg(params.value("category").toString()));

            categoryId = q.value("id").toInt();
        }

        // Insert item
        q.prepare("INSERT INTO item (category_id, item, short_form, description, barcode, divisible, image, "
                  "note_id, archived, created, last_edited, user_id) "
                  "VALUES (:category_id, :item, :short_form, :description, :barcode, :divisible, :image, "
                  ":note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":category_id", categoryId);
        q.bindValue(":item", params.value("item").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":description", params.value("description").toString());
        q.bindValue(":barcode", QVariant(QVariant::String));
        q.bindValue(":divisible", params.value("divisible").toBool());
        q.bindValue(":image", DatabaseUtils::imageToByteArray(params.value("image_source").toString())); // Store image as BLOB
        q.bindValue(":note_id", itemNoteId > 0 ? itemNoteId : QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec()) {
            if (q.lastError().number() == int(DatabaseException::MySqlErrorCode::DuplicateEntryError))
                throw DatabaseException(DatabaseException::RRErrorCode::DuplicateEntryFailure,
                                        q.lastError().text(), "Failed to insert item because item already exists.");
            else
                throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert item.");
        }

        itemId = q.lastInsertId().toInt();
        if (!itemId)
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Invalid item ID returned.");


        // Insert unit
        q.prepare("INSERT INTO unit (item_id, unit, short_form, base_unit_equivalent, cost_price, retail_price, preferred, currency, note_id, "
                  "archived, created, last_edited, user_id) "
                  "VALUES (:item_id, :unit, :short_form, :base_unit_equivalent, :cost_price, :retail_price, :preferred, :currency, :note_id, "
                  ":archived, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":unit", params.value("unit").toString());
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":base_unit_equivalent", 1);
        q.bindValue(":cost_price", params.value("cost_price").toString());
        q.bindValue(":retail_price", params.value("retail_price").toString());
        q.bindValue(":preferred", true);
        q.bindValue(":currency", "NGN");
        q.bindValue(":note_id", QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Failed to insert unit.");

        unitId = q.lastInsertId().toInt();
        if (!unitId)
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure, q.lastError().text(), "Invalid unit ID returned.");


        // Insert quantity into initial_quantity table
        q.prepare("INSERT INTO initial_quantity (item_id, quantity, unit_id, reason, archived, created, last_edited, user_id) "
                  "VALUES (:item_id, :quantity, :unit_id, :reason, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":quantity", params.value("quantity").toDouble());
        q.bindValue(":unit_id", unitId);
        q.bindValue(":reason", request.command());
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure,
                                    q.lastError().text(), "Failed to insert quantity into initial_quantity.");


        // Insert unit into current_quantity table
        q.prepare("INSERT INTO current_quantity (item_id, quantity, unit_id, created, last_edited, user_id) "
                  "VALUES (:item_id, :quantity, :unit_id, :created, :last_edited, :user_id)");
        q.bindValue(":item_id", itemId);
        q.bindValue(":quantity", params.value("quantity").toDouble());
        q.bindValue(":unit_id", unitId);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::AddItemFailure,
                                    q.lastError().text(), "Failed to insert quantity into current_quantity.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void StockSqlManager::updateStockItem(const QueryRequest &request)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();
    const QDateTime &currentDateTime = QDateTime::currentDateTime();
    int categoryId = 0;

    QSqlQuery q(connection);

    if (params.contains("quantity"))
        qWarning() << Q_FUNC_INFO << "-> This function is not responsible for updating quantity. Quantity will be ignored.";

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        // STEP: Update category note.
        if (!params.value("category_note").toString().trimmed().isEmpty()) {
            q.prepare("UPDATE note SET note = :note, last_edited = :last_edited, user_id = :user_id WHERE id = :category_note_id");
            q.bindValue(":note", params.value("category_note"));
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":category_note_id", params.value("category_note_id"));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(),
                                        QStringLiteral("Failed to update category note."));
        }

        // STEP: Insert item note.
        if (!params.value("item_note").toString().trimmed().isEmpty()) {
            q.prepare("UPDATE note SET note = :note, last_edited = :last_edited, user_id = :user_id WHERE id = :item_note_id");
            q.bindValue(":note", params.value("item_note").toString());
            q.bindValue(":created", currentDateTime);
            q.bindValue(":last_edited", currentDateTime);
            q.bindValue(":user_id", UserProfile::instance().userId());
            q.bindValue(":item_note_id", params.value("item_note_id"));

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(),
                                        QStringLiteral("Failed to update item note."));
        }

        // STEP: Insert category if it doesn't exist. Else, get the category ID.
        q.prepare("INSERT IGNORE INTO category (category, short_form, note_id, archived, created, last_edited, user_id) "
                  "VALUES (:category, :short_form, :note_id, :archived, :created, :last_edited, :user_id)");
        q.bindValue(":category", params.value("category").toString());
        q.bindValue(":short_form", params.value("short_form", QVariant(QVariant::String)));
        q.bindValue(":note_id", params.value("category_note_id", QVariant(QVariant::Int)));
        q.bindValue(":archived", false);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(),
                                    QStringLiteral("Failed to insert new category."));

        if (q.numRowsAffected() > 0) {
            categoryId = q.lastInsertId().toInt();
            if (!categoryId)
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(),
                                        QStringLiteral("Invalid category ID returned."));
        } else {
            q.prepare("SELECT id FROM category WHERE category = :category");
            q.bindValue(":category", params.value("category").toString());

            if (!q.exec())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(),
                                        QStringLiteral("Failed to find category ID."));

            if (!q.first())
                throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure,
                                        q.lastError().text(),
                                        QString("Expected category ID for category '%1'.")
                                        .arg(params.value("category").toString()));

            categoryId = q.value("id").toInt();
        }

        // STEP: Update item.
        q.prepare("UPDATE item SET category_id = :category_id, item = :item, short_form = :short_form, description = :description, "
                  "barcode = :barcode, divisible = :divisible, image = :image, note_id = :note_id, archived = :archived, "
                  "last_edited = :last_edited, user_id = :user_id "
                  "WHERE item.id = :item_id");
        q.bindValue(":category_id", categoryId);
        q.bindValue(":item", params.value("item"));
        q.bindValue(":short_form", params.value("short_form", QVariant(QVariant::String)));
        q.bindValue(":description", params.value("description", QVariant(QVariant::String)));
        q.bindValue(":barcode", params.value("barcode", QVariant(QVariant::String)));
        q.bindValue(":divisible", params.value("divisible"));
        q.bindValue(":image", DatabaseUtils::imageToByteArray(params.value("image_source").toString())); // Store image as BLOB
        q.bindValue(":note_id", params.value("item_note_id", QVariant(QVariant::Int)));
        q.bindValue(":archived", false);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":item_id", params.value("item_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to update item."));

        // STEP: Update unit.
        q.prepare("UPDATE unit SET unit = :unit, short_form = :short_form, "
                  "base_unit_equivalent = :base_unit_equivalent, cost_price = :cost_price, retail_price = :retail_price, "
                  "preferred = :preferred, currency = :currency, note_id = :note_id, archived = :archived, "
                  "last_edited = :last_edited, user_id = :user_id WHERE item_id = :item_id");
        q.bindValue(":unit", params.value("unit"));
        q.bindValue(":short_form", QVariant(QVariant::String));
        q.bindValue(":base_unit_equivalent", 1);
        q.bindValue(":cost_price", params.value("cost_price"));
        q.bindValue(":retail_price", params.value("retail_price"));
        q.bindValue(":preferred", true);
        q.bindValue(":currency", "NGN");
        q.bindValue(":note_id", QVariant(QVariant::Int));
        q.bindValue(":archived", false);
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());
        q.bindValue(":item_id", params.value("item_id"));

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UpdateItemFailure, q.lastError().text(), "Failed to update unit.");

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void StockSqlManager::viewStockItems(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params = request.params();

    try {
        const QList<QSqlRecord> records(callProcedure("ViewStockItems", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_column",
                                                              params.value("filter_column")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "filter_text",
                                                              params.value("filter_text")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "sort_column",
                                                              params.value("sort_column", QStringLiteral("category"))
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::In,
                                                              "sort_order",
                                                              params.value("sort_order").toInt() == Qt::DescendingOrder
                                                              ? "descending" : "ascending"
                                                          }
                                                      }));


        QStringList categories;
        QVariantList itemGroups;
        int itemCount = 0;
        for (int i = 0; i < records.count(); ++i) {
            auto record = records[i];
            const auto categoryId = record.value("category_id").toInt();
            const auto &category = record.value("category").toString();

            QVariantList items;

            while ((i < records.count()) && categoryId == record.value("category_id").toInt()) {
                QVariantMap itemRecord;
                itemRecord.insert("item_id", record.value("item_id"));
                itemRecord.insert("category_id", categoryId);
                itemRecord.insert("category", category);
                itemRecord.insert("item", record.value("item"));
                itemRecord.insert("description", record.value("description"));
                itemRecord.insert("divisible", record.value("divisible"));
                itemRecord.insert("image", DatabaseUtils::byteArrayToImage(record.value("image").toByteArray()));
                itemRecord.insert("quantity", record.value("quantity"));
                itemRecord.insert("unit", record.value("unit"));
                itemRecord.insert("unit_id", record.value("unit_id"));
                itemRecord.insert("cost_price", record.value("cost_price"));
                itemRecord.insert("retail_price", record.value("retail_price"));
                itemRecord.insert("currency", record.value("currency"));
                itemRecord.insert("created", record.value("created"));
                itemRecord.insert("last_edited", record.value("last_edited"));
                itemRecord.insert("user", record.value("user"));

                items.append(itemRecord);
                itemCount++;

                if ((i + 1) < records.count() && categoryId == records.at(i + 1).value("category_id").toInt())
                    record = records[i++];
                else
                    break;
            }

            categories.append(category);
            itemGroups.append(QVariant(items));
        }

        /*
        qInfo() << "--------------------Outcome-----------------------";
        QMapIterator<QString, QVariant> categoryIter(categoryRecords);
        while (categoryIter.hasNext()) {
            categoryIter.next();
            qInfo() << categoryIter.key();
            qInfo() << "-------------------------------------------------";
            const QVariantList &items = categoryIter.value().toList();

            for (const QVariant &itemRecord : items) {
                qInfo() << "Item name:" << itemRecord.toMap().value("item").toString();
            }

            qInfo() << "-------------------------------------------------";
        }
        */

        if (categories.count() != itemGroups.count())
            throw DatabaseException(DatabaseException::RRErrorCode::ResultMismatch,
                                    QString("Category count (%1) and item group count (%2) are unequal.")
                                    .arg(categories.count()).arg(itemGroups.count()));

        result.setOutcome(QVariantMap { { "categories", categories },
                                        { "item_groups", itemGroups },
                                        { "record_count", itemCount },
                                        { "total_items", itemCount }
                          });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockItemDetails(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap &params = request.params();

    try {
        enforceArguments({ "item_id" }, params);

        const QList<QSqlRecord> records(callProcedure("ViewStockItemDetails", {
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::InOut,
                                                              "item",
                                                              params.value("item_id")
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "category_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "category",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "item",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "description",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "divisible",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "image",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "quantity",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "unit_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "unit",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "cost_price",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "retail_price",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "currency",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "created",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "last_edited",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "user_id",
                                                              {}
                                                          },
                                                          ProcedureArgument {
                                                              ProcedureArgument::Type::Out,
                                                              "user",
                                                              {}
                                                          }
                                                      }));

        QVariantMap itemInfo;
        if (!records.isEmpty())
            itemInfo = recordToMap(records.first());
        else
            throw DatabaseException(DatabaseException::RRErrorCode::ViewStockItemDetailsFailed,
                                    QString(),
                                    QString("Item details do not exists for item '%1'.").arg(params.value("item_id").toString()));

        result.setOutcome(QVariantMap { { "item", itemInfo }, { "record_count", 1 } });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::viewStockCategories(const QueryRequest &request, QueryResult &result)
{
    const QVariantMap &params(request.params());

    try {
        QList<QSqlRecord> records(callProcedure("ViewStockCategories", {
                                                    ProcedureArgument {
                                                        ProcedureArgument::Type::In,
                                                        "sort_order",
                                                        params.value("sort_order")
                                                    }
                                                }));

        QVariantList categories;
        for (const auto &record : records) {
            categories.append(recordToMap(record));
        }

        result.setOutcome(QVariantMap { { "categories", categories }, { "record_count", categories.count() } });
    } catch (DatabaseException &) {
        throw;
    }
}

void StockSqlManager::removeStockItem(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        if (params.value("item_id").toInt() <= 0)
            throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, QString(), "Item ID is null.");
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        QSqlQuery q(connection);
        q.prepare("UPDATE item SET archived = 1, last_edited = :last_edited, user_id = :user_id WHERE id = :item_id");
        q.bindValue(":item_id", params.value("item_id"));
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(), "Failed to remove stock item.");

        q.prepare("SELECT category.id as category_id, item.id as item_id "
                  "FROM item "
                  "INNER JOIN category ON item.category_id = category.id "
                  "WHERE item.id = :item_id");
        q.bindValue(":item_id", params.value("item_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(), "Failed to fetch category ID.");

        if (!q.first())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(), "Failed to retrieve category ID.");
        else {
            QVariantMap outcome;
            outcome.insert("category_id", q.value("category_id"));
            outcome.insert("item_id", q.value("item_id"));

            result.setOutcome(outcome);
        }

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}

void StockSqlManager::undoRemoveStockItem(const QueryRequest &request, QueryResult &result)
{
    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    const QVariantMap params = request.params();
    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery q(connection);

    try {
        if (params.value("item_id").toInt() <= 0)
            throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments, QString(), "Item ID is null.");
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed, q.lastError().text(), "Failed to start transation.");

        q.prepare("UPDATE item SET archived = 0, last_edited = :last_edited, user_id = :user_id WHERE id = :item_id");
        q.bindValue(":item_id", params.value("item_id"));
        q.bindValue(":last_edited", currentDateTime);
        q.bindValue(":user_id", UserProfile::instance().userId());

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::UndoFailed, q.lastError().text(), "Failed to undo stock item removal.");

        q.prepare("SELECT item.id AS item_id, category.id AS category_id, category.category, item.item, item.description, "
                  "item.divisible, item.image, current_quantity.quantity, "
                  "unit.id as unit_id, unit.unit, unit.cost_price, "
                  "unit.retail_price, unit.currency, item.created, item.last_edited, item.user_id, item.user_id AS user "
                  "FROM item "
                  "INNER JOIN category ON item.category_id = category.id "
                  "INNER JOIN unit ON item.id = unit.item_id "
                  "INNER JOIN current_quantity ON item.id = current_quantity.item_id "
                  "LEFT JOIN user ON item.user_id = user.id "
                  "WHERE item.archived = 0 AND unit.base_unit_equivalent = 1 "
                  "AND item.id = :item_id");
        q.bindValue(":item_id", params.value("item_id"), QSql::Out);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(), "Failed to fetch category ID.");

        if (!q.first()) {
            throw DatabaseException(DatabaseException::RRErrorCode::RemoveStockItemFailed, q.lastError().text(), "Failed to retrieve category ID.");
        } else {
            QVariantMap outcome;
            outcome.insert("category_id", q.value("category_id"));
            outcome.insert("item_id", q.value("item_id"));
            outcome.insert("item_info", recordToMap(q.record()));

            result.setOutcome(outcome);
        }

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::CommitTransationFailed, q.lastError().text(), "Failed to commit.");
    } catch (DatabaseException &) {
        if (!DatabaseUtils::rollbackTransaction(q))
            qCritical("Failed to rollback failed transaction! %s", q.lastError().text().toStdString().c_str());

        throw;
    }
}
