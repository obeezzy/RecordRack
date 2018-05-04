#include "stockitemmodel.h"
#include <QDateTime>

StockItemModel::StockItemModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_category(QString()),
    m_categoryId(-1),
    m_itemRecords(QVariantList())
{

}

int StockItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_itemRecords.count();
}

QVariant StockItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_itemRecords.at(index.row()).toMap().value("category_id").toInt();
        break;
    case ItemIdRole:
        return m_itemRecords.at(index.row()).toMap().value("item_id").toString();
        break;
    case ItemRole:
        return m_itemRecords.at(index.row()).toMap().value("item").toString();
        break;
    case DescriptionRole:
        return m_itemRecords.at(index.row()).toMap().value("description").toString();
        break;
    case DivisibleRole:
        return m_itemRecords.at(index.row()).toMap().value("divisible").toBool();
        break;
    case ImageRole:
        return m_itemRecords.at(index.row()).toMap().value("image").toString();
        break;
    case QuantityRole:
        return m_itemRecords.at(index.row()).toMap().value("quantity").toDouble();
        break;
    case UnitRole:
        return m_itemRecords.at(index.row()).toMap().value("unit").toString();
        break;
    case UnitIdRole:
        return m_itemRecords.at(index.row()).toMap().value("unit_id").toInt();
        break;
    case CostPriceRole:
        return m_itemRecords.at(index.row()).toMap().value("cost_price").toDouble();
        break;
    case RetailPriceRole:
        return m_itemRecords.at(index.row()).toMap().value("retail_price").toDouble();
        break;
    case CurrencyRole:
        return m_itemRecords.at(index.row()).toMap().value("currency").toString();
        break;
    case CreatedRole:
        return m_itemRecords.at(index.row()).toMap().value("created").toDateTime();
        break;
    case LastEditedRole:
        return m_itemRecords.at(index.row()).toMap().value("last_edited").toDateTime();
        break;
    case UserRole:
        return m_itemRecords.at(index.row()).toMap().value("user").toString();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> StockItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(QAbstractListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(ItemIdRole, "item_id");
    roles.insert(ItemRole, "item");
    roles.insert(DescriptionRole, "description");
    roles.insert(DivisibleRole, "divisible");
    roles.insert(ImageRole, "image");
    roles.insert(QuantityRole, "quantity");
    roles.insert(UnitRole, "unit");
    roles.insert(UnitIdRole, "unit_id");
    roles.insert(CostPriceRole, "cost_price");
    roles.insert(RetailPriceRole, "retail_price");
    roles.insert(CurrencyRole, "currency");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserRole, "user");

    return roles;
}

QString StockItemModel::category() const
{
    return m_category;
}

void StockItemModel::setCategory(const QString &category)
{
    m_category = category;
}

int StockItemModel::categoryId() const
{
    return m_categoryId;
}

void StockItemModel::setCategoryId(int categoryId)
{
    m_categoryId = categoryId;
}

QVariantList StockItemModel::items() const
{
    return m_itemRecords;
}

void StockItemModel::setItems(const QVariantList &items)
{
    m_itemRecords = items;
}

bool StockItemModel::addItem(int itemId, const QVariantMap &itemInfo, Qt::SortOrder sortOrder)
{
    if (itemId <= 0 || itemInfo.isEmpty())
        return false;

    for (int i = 0; i < rowCount(); ++i) {
        if (itemInfo.value("item").toString().toLower() > data(index(i), ItemRole).toString().toLower() && sortOrder == Qt::AscendingOrder) {
            beginInsertRows(QModelIndex(), i, i);
            m_itemRecords.insert(i, itemInfo);
            endInsertRows();

            return true;
        } else if (itemInfo.value("item").toString().toLower() < data(index(i), ItemRole).toString().toLower() && sortOrder == Qt::DescendingOrder) {
            beginInsertRows(QModelIndex(), i, i);
            m_itemRecords.insert(i, itemInfo);
            endInsertRows();

            return true;
        }
    }

    return false;
}

bool StockItemModel::removeItem(int itemId)
{
    if (itemId <= 0)
        return false;

    for (int i = 0; i < rowCount(); ++i) {
        if (itemId == data(index(i), ItemIdRole).toInt()) {
            beginRemoveRows(QModelIndex(), i, i);
            m_itemRecords.removeAt(i);
            endRemoveRows();

            return true;
        }
    }

    return false;
}

void StockItemModel::tryQuery()
{

}

void StockItemModel::processResult(const QueryResult &result)
{
    Q_UNUSED(result)
}