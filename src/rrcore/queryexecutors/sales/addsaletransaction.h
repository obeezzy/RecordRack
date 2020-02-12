﻿#ifndef ADDSALETRANSACTION_H
#define ADDSALETRANSACTION_H

#include "saleexecutor.h"
#include "utility/saleutils.h"

namespace SaleQuery {
class AddSaleTransaction : public SaleExecutor
{
    Q_OBJECT
public:
    static inline const QString COMMAND = "add_sale_transaction";
    static inline const QString UNDO_COMMAND = "undo_add_sale_transaction";

    explicit AddSaleTransaction(const SaleTransaction &transaction,
                                QObject *receiver);
    explicit AddSaleTransaction(const QueryRequest &request,
                                QObject *receiver);
    QueryResult execute() override;
private:
    QueryResult undoAddSaleTransaction();

    void affirmSaleTransactionIdIsValid();
    void archiveSaleTransaction();
    void archiveDebtTransaction();
    void archiveCreditTransaction();
    void revertProductQuantityUpdate();
};
}

#endif // ADDSALETRANSACTION_H
