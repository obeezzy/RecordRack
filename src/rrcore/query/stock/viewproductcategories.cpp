#include "viewproductcategories.h"

#include "database/databaseexception.h"
#include "utility/common/recordgroup.h"
#include "utility/common/sortcriteria.h"

using namespace Query::Stock;

ViewProductCategories::ViewProductCategories(
    const Utility::SortCriteria& sortCriteria,
    const Utility::RecordGroup::Flags& flags, QObject* receiver)
    : StockExecutor(
          COMMAND,
          {{"sort_order", sortCriteria.orderAsString()},
           {"archived", flags.testFlag(Utility::RecordGroup::Archived)}},
          receiver)
{}

QueryResult ViewProductCategories::execute()
{
    QueryResult result{request()};

    result.setSuccessful(true);
    const QVariantMap& params(request().params());

    try {
        const auto& records(callProcedure(
            "ViewProductCategories",
            {ProcedureArgument{ProcedureArgument::Type::In, "sort_order",
                               params.value("sort_order")},
             ProcedureArgument{ProcedureArgument::Type::In, "archived",
                               params.value("archived", false)}}));

        QVariantList categories;
        for (const auto& record : records)
            categories.append(recordToMap(record));

        result.setOutcome(QVariantMap{{"categories", categories},
                                      {"record_count", categories.count()}});
    } catch (const DatabaseException&) {
        throw;
    }

    return result;
}
