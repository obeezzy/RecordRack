#include "viewdashboard.h"
#include "json/messagecenter.h"
#include "database/databaseexception.h"

using namespace DashboardQuery;

ViewDashboard::ViewDashboard(QObject *receiver) :
    DashboardExecutor(COMMAND, { }, receiver)
{

}

QueryResult ViewDashboard::execute()
{
    QueryResult result{ request() };
    result.setSuccessful(true);

    QVariantList homeRecords;

    try {

        fetchWelcomeMessage(homeRecords);
        fetchEmptyStockMessage(homeRecords);
        result.setOutcome(QVariantMap {
                              { "records", homeRecords },
                              { "record_count", homeRecords.count() }
                          });

        return result;
    } catch (DatabaseException &) {
        throw;
    }
}

void ViewDashboard::fetchWelcomeMessage(QVariantList &homeRecords)
{
    MessageCenter center;
    homeRecords.append(center.getMessage("welcome"));
}

void ViewDashboard::fetchEmptyStockMessage(QVariantList &homeRecords)
{
    MessageCenter center;
    homeRecords.append(center.getMessage("empty_stock"));
}
