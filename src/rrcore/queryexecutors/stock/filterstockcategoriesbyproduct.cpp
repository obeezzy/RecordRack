#include "filterstockcategoriesbyproduct.h"
#include "database/databaseexception.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace StockQuery;

FilterStockProductCategoriesByProduct::FilterStockProductCategoriesByProduct(const FilterCriteria &filterCriteria,
                                                                             const SortCriteria &sortCriteria,
                                                                             const RecordGroup::Flags &flags,
                                                                             QObject *receiver) :
    StockExecutor(COMMAND, {
                    { "filter_text", filterCriteria.text },
                    { "sort_order", sortCriteria.orderAsString() },
                    { "archived", flags.testFlag(RecordGroup::Archived) }
                  }, receiver)
{

}

QueryResult StockQuery::FilterStockProductCategoriesByProduct::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);
    const QVariantMap &params(request().params());

    try {
        const auto &records(callProcedure("FilterStockProductCategoriesByProduct", {
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "filter_text",
                                                  params.value("filter_text")
                                              },
                                              ProcedureArgument {
                                                  ProcedureArgument::Type::In,
                                                  "sort_order",
                                                  params.value("sort_order")
                                              }
                                          }));

        QVector<int> categoryIds;
        QVariantList categories;
        for (const auto &record : records) {
            if (!categoryIds.contains(record.value("product_category_id").toInt())) {
                categories.append(recordToMap(record));
                categoryIds.append(record.value("product_category_id").toInt());
            }
        }

        result.setOutcome(QVariantMap {
                              { "categories", categories },
                              { "record_count", categories.count() }
                          });
        return result;
    } catch (DatabaseException &) {
        throw;
    }
}
