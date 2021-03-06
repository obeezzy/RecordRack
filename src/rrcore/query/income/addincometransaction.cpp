#include "addincometransaction.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "user/userprofile.h"
#include "utility/income/incometransaction.h"

using namespace Query::Income;

AddIncomeTransaction::AddIncomeTransaction(
    const Utility::Income::IncomeTransaction& transaction, QObject* receiver)
    : IncomeExecutor(COMMAND,
                     {{"client_name", transaction.client.preferredName},
                      {"purpose", transaction.purpose},
                      {"amount", transaction.amount.toDouble()},
                      {"payment_method", transaction.paymentMethod.toString()},
                      {"currency", transaction.amount.currency().isoCode()}},
                     receiver)
{}

QueryResult Query::Income::AddIncomeTransaction::execute()
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params{request().params()};
    const QString& note = params.value("note").toString();
    int noteId = 0;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::beginTransaction(q);

        noteId = QueryExecutor::addNote(note, QStringLiteral("income"),
                                        ExceptionPolicy::DisallowExceptions);

        callProcedure(
            "AddIncomeTransaction",
            {ProcedureArgument{ProcedureArgument::Type::In, "client_id",
                               params.value("client_id")},
             ProcedureArgument{ProcedureArgument::Type::In, "name",
                               params.value("client_name")},
             ProcedureArgument{ProcedureArgument::Type::In, "purpose",
                               params.value("purpose")},
             ProcedureArgument{ProcedureArgument::Type::In, "amount",
                               params.value("amount")},
             ProcedureArgument{ProcedureArgument::Type::In, "payment_method",
                               params.value("payment_method")},
             ProcedureArgument{ProcedureArgument::Type::In, "currency",
                               params.value("currency")},
             ProcedureArgument{ProcedureArgument::Type::In, "note_id", noteId},
             ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                               UserProfile::instance().userId()}});

        QueryExecutor::commitTransaction(q);
        return result;
    } catch (const DatabaseException&) {
        QueryExecutor::rollbackTransaction(q);
        throw;
    }
}
