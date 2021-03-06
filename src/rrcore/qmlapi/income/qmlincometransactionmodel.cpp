#include "qmlincometransactionmodel.h"
#include <QDateTime>
#include "database/databasethread.h"
#include "query/income/viewincometransactions.h"

QMLIncomeTransactionModel::QMLIncomeTransactionModel(QObject* parent)
    : QMLIncomeTransactionModel(DatabaseThread::instance(), parent)
{}

QMLIncomeTransactionModel::QMLIncomeTransactionModel(DatabaseThread& thread,
                                                     QObject* parent)
    : AbstractTransactionModel(thread, parent)
{}

int QMLIncomeTransactionModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_transactions.count();
}

int QMLIncomeTransactionModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLIncomeTransactionModel::data(const QModelIndex& index,
                                         int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case TransactionIdRole:
            return m_transactions.at(index.row()).id;
        case ClientIdRole:
            return m_transactions.at(index.row()).client.id;
        case ClientNameRole:
            return m_transactions.at(index.row()).client.preferredName;
        case AmountRole:
            return m_transactions.at(index.row()).amount.toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLIncomeTransactionModel::roleNames() const
{
    return {{TransactionIdRole, "transaction_id"},
            {ClientNameRole, "client_name"},
            {AmountRole, "amount"}};
}

QVariant QMLIncomeTransactionModel::headerData(int section,
                                               Qt::Orientation orientation,
                                               int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
                case TransactionIdColumn:
                    return tr("Transaction ID");
                case ClientNameColumn:
                    return tr("Client name");
                case AmountColumn:
                    return tr("Amount");
                case ActionColumn:
                    return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
                case TransactionIdColumn:
                    return Qt::AlignRight;
                case ClientNameColumn:
                    return Qt::AlignLeft;
                case AmountColumn:
                    return Qt::AlignRight;
                case ActionColumn:
                    return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
                case TransactionIdColumn:
                    return 120;
                case ClientNameColumn:
                    return 330;
                case AmountColumn:
                    return 160;
                case ActionColumn:
                    return 125;
            }
        }
    }

    return section + 1;
}

void QMLIncomeTransactionModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Income::ViewIncomeTransactions(
        dateTimeSpan(), Utility::RecordGroup::None, this));
}

bool QMLIncomeTransactionModel::canProcessResult(
    const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLIncomeTransactionModel::processResult(const QueryResult& result)
{
    setBusy(false);
    beginResetModel();
    if (result.isSuccessful()) {
        m_transactions = Utility::Income::IncomeTransactionList{
            result.outcome().toMap().value("transactions").toList()};
        emit success();
    } else {
        emit error();
    }
    endResetModel();
}
