#include "qmlpurchasereportmodel.h"
#include "database/databasethread.h"
#include "query/purchase/viewpurchasereport.h"

QMLPurchaseReportModel::QMLPurchaseReportModel(QObject* parent)
    : QMLPurchaseReportModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseReportModel::QMLPurchaseReportModel(DatabaseThread& thread,
                                               QObject* parent)
    : AbstractReportModel(thread, parent)
{}

QVariant QMLPurchaseReportModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case CategoryRole:
            return m_transactions.at(index.row()).category.category;
        case ProductRole:
            return m_transactions.at(index.row()).product.product;
        case QuantityBoughtRole:
            return m_transactions.at(index.row()).quantityBought.toDouble();
        case UnitRole:
            return m_transactions.at(index.row()).product.unit.unit;
        case TotalExpenditureRole:
            return m_transactions.at(index.row()).totalExpenditure.toDouble();
    }

    return QVariant();
}

int QMLPurchaseReportModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
}

int QMLPurchaseReportModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QHash<int, QByteArray> QMLPurchaseReportModel::roleNames() const
{
    return {{CategoryRole, "category"},
            {ProductRole, "product"},
            {QuantityBoughtRole, "quantity_bought"},
            {TotalExpenditureRole, "total_expenditure"},
            {UnitRole, "unit"}};
}

QVariant QMLPurchaseReportModel::headerData(int section,
                                            Qt::Orientation orientation,
                                            int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
                case CategoryColumn:
                    return tr("Category");
                case ProductColumn:
                    return tr("Product");
                case QuantityBoughtColumn:
                    return tr("Qty bought");
                case TotalAmountColumn:
                    return tr("Total amount");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
                case CategoryColumn:
                case ProductColumn:
                    return Qt::AlignLeft;
                case QuantityBoughtColumn:
                case TotalAmountColumn:
                    return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
                case CategoryColumn:
                    return 180;
                case ProductColumn:
                    return tableViewWidth() - 180 - 180 - 180;
                case QuantityBoughtColumn:
                    return 180;
                case TotalAmountColumn:
                    return 180;
            }
        }
    }

    return section + 1;
}

void QMLPurchaseReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Purchase::ViewPurchaseReport(dateTimeSpan(), this));
}

bool QMLPurchaseReportModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLPurchaseReportModel::processResult(const QueryResult& result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() ==
            Query::Purchase::ViewPurchaseReport::COMMAND) {
            beginResetModel();
            m_transactions = Utility::Purchase::PurchaseReportTransactionList{
                result.outcome().toMap().value("transactions").toList()};
            endResetModel();
            emit success();
        } else {
            emit error();
        }
    } else {
    }
}
