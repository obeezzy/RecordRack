#ifndef SALETRANSACTION_H
#define SALETRANSACTION_H

#include <QDateTime>
#include <QString>
#include "salecartproduct.h"
#include "salepayment.h"
#include "singletons/settings.h"
#include "utility/common/customer.h"
#include "utility/common/note.h"

namespace Utility {
namespace Sales {
struct SaleTransaction
{
    int id{0};
    Customer customer;
    SaleMonies monies;
    SaleCartProductList products;
    SalePaymentList payments;
    RecordGroup::Flags flags;
    QDateTime dueDateTime;
    QString action;
    Note note;
    RecordTimestamp timestamp;
    User::User user;
    int row{-1};

    explicit SaleTransaction() = default;
    explicit SaleTransaction(const QVariantMap& map);
    explicit SaleTransaction(int id);
    explicit SaleTransaction(int id, const Customer& customer,
                             const SaleMonies& monies,
                             const SaleCartProductList& products,
                             const SalePaymentList& payments,
                             const RecordGroup::Flags& flags,
                             const QDateTime& dueDateTime,
                             const QString& action, const Note& note);
    explicit SaleTransaction(int id, const Customer& customer,
                             const SaleMonies& monies,
                             const RecordGroup::Flags& flags,
                             const Note& note = Note());

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const SaleTransaction& transaction)
    {
        debug.nospace() << "SaleTransaction("
                        << "id=" << transaction.id
                        << ", customer=" << transaction.customer
                        << ", totalCost=" << transaction.monies.totalCost
                        << ", amountPaid=" << transaction.monies.amountPaid
                        << ", balance=" << transaction.monies.balance
                        << ", discount=" << transaction.monies.discount
                        << ", payments=" << transaction.payments
                        << ", products=" << transaction.products
                        << ", flags=" << transaction.flags
                        << ", dueDateTime=" << transaction.dueDateTime
                        << ", action=" << transaction.action
                        << ", note=" << transaction.note
                        << ", row=" << transaction.row << ")";

        return debug.nospace();
    }
};

class SaleTransactionList : public QList<SaleTransaction>
{
public:
    explicit SaleTransactionList() = default;
    SaleTransactionList(std::initializer_list<SaleTransaction> transactions)
        : QList<SaleTransaction>(transactions)
    {}
    explicit SaleTransactionList(const QVariantList& list)
        : QList<SaleTransaction>()
    {
        for (const auto& variant : list)
            append(SaleTransaction{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}  // namespace Sales
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Sales::SaleTransaction, Q_PRIMITIVE_TYPE);

#endif  // SALETRANSACTION_H
