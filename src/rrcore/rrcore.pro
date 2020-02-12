TEMPLATE = lib

QT += core qml quick quickcontrols2 sql svg printsupport

CONFIG += c++17

RESOURCES += config/config.qrc \
    json/json.qrc \
    schema/schema.qrc

INCLUDEPATH += .

SOURCES += \
    database/exceptions/amountoverpaidexception.cpp \
    database/exceptions/argumentmismatchexception.cpp \
    database/exceptions/duplicateentryexception.cpp \
    database/exceptions/failedtocreateuserexception.cpp \
    database/exceptions/incorrectcredentialsexception.cpp \
    database/exceptions/incorrectpasswordexception.cpp \
    database/exceptions/invalidargumentexception.cpp \
    database/exceptions/invalidduedateexception.cpp \
    database/exceptions/missingargumentexception.cpp \
    database/exceptions/missingimplementationexception.cpp \
    database/exceptions/noexistingrecordexception.cpp \
    database/exceptions/sqlstatementfailedexception.cpp \
    database/exceptions/unexpectedresultexception.cpp \
    database/exceptions/useraccountlockedexception.cpp \
    database/exceptions/userpreviouslyarchivedexception.cpp \
    database/queryexecutor.cpp \
    queryexecutors/client/filterclients.cpp \
    queryexecutors/debtor/filterdebtors.cpp \
    queryexecutors/expense/filterexpensereport.cpp \
    queryexecutors/income/filterincomereport.cpp \
    queryexecutors/purchase/filterpurchasereport.cpp \
    queryexecutors/sales/filtersalereport.cpp \
    queryexecutors/stock/filterstockreport.cpp \
    queryexecutors/transaction/transactionexecutor.cpp \
    models/salemostsoldproductmodel.cpp \
    network/exceptions/indeterminatedestinationurlexception.cpp \
    network/networkexception.cpp \
    network/networkthread.cpp \
    network/requestlogger.cpp \
    network/serverrequest.cpp \
    network/serverresponse.cpp \
    qmlapi/qmldebtpaymentmodel.cpp \
    qmlapi/qmlexpensetransactionmodel.cpp \
    qmlapi/qmlstockproductcategorymodel.cpp \
    qmlapi/qmlstockproductcountrecord.cpp \
    qmlapi/qmlstockproductdetailrecord.cpp \
    qmlapi/qmlstockproductmodel.cpp \
    qmlapi/qmlstockproductpusher.cpp \
    qmlapi/qmluserprofile.cpp \
    queryexecutors/client/clientexecutor.cpp \
    queryexecutors/client/viewclients.cpp \
    queryexecutors/dashboard/dashboardexecutor.cpp \
    queryexecutors/dashboard/viewdashboard.cpp \
    queryexecutors/debtor/adddebtor.cpp \
    queryexecutors/debtor/debtorexecutor.cpp \
    queryexecutors/debtor/fetchdebtor.cpp \
    queryexecutors/debtor/removedebtor.cpp \
    queryexecutors/debtor/updatedebtor.cpp \
    queryexecutors/debtor/viewdebtors.cpp \
    queryexecutors/debtor/viewdebtpayments.cpp \
    queryexecutors/debtor/viewdebttransactions.cpp \
    queryexecutors/expense/addexpensetransaction.cpp \
    queryexecutors/expense/expenseexecutor.cpp \
    queryexecutors/expense/viewexpensereport.cpp \
    queryexecutors/expense/viewexpensetransactions.cpp \
    queryexecutors/income/addincometransaction.cpp \
    queryexecutors/income/incomeexecutor.cpp \
    queryexecutors/income/viewincomereport.cpp \
    queryexecutors/income/viewincometransactions.cpp \
    queryexecutors/purchase/addpurchasetransaction.cpp \
    queryexecutors/purchase/purchaseexecutor.cpp \
    queryexecutors/purchase/removepurchasedproduct.cpp \
    queryexecutors/purchase/removepurchasetransaction.cpp \
    queryexecutors/purchase/updatesuspendedpurchasetransaction.cpp \
    queryexecutors/purchase/viewpurchasecart.cpp \
    queryexecutors/purchase/viewpurchasedproducts.cpp \
    queryexecutors/purchase/viewpurchasehome.cpp \
    queryexecutors/purchase/viewpurchasereport.cpp \
    queryexecutors/purchase/viewpurchasetransactions.cpp \
    queryexecutors/sales/addsaletransaction.cpp \
    queryexecutors/sales/removesaletransaction.cpp \
    queryexecutors/sales/removesoldproduct.cpp \
    queryexecutors/sales/saleexecutor.cpp \
    queryexecutors/sales/updatesuspendedsaletransaction.cpp \
    queryexecutors/sales/viewsalecart.cpp \
    queryexecutors/sales/viewsalehome.cpp \
    queryexecutors/sales/viewsalereport.cpp \
    queryexecutors/sales/viewsaletransactionproducts.cpp \
    queryexecutors/sales/viewsaletransactions.cpp \
    queryexecutors/stock/addstockproduct.cpp \
    queryexecutors/stock/fetchstockproduct.cpp \
    queryexecutors/stock/fetchstockproductcount.cpp \
    queryexecutors/stock/filterstockcategories.cpp \
    queryexecutors/stock/filterstockcategoriesbyproduct.cpp \
    queryexecutors/stock/filterstockproductcount.cpp \
    queryexecutors/stock/filterstockproducts.cpp \
    queryexecutors/stock/removestockproduct.cpp \
    queryexecutors/stock/stockexecutor.cpp \
    queryexecutors/stock/updatestockproduct.cpp \
    queryexecutors/stock/viewstockproductcategories.cpp \
    queryexecutors/stock/viewstockproducts.cpp \
    queryexecutors/stock/viewstockreport.cpp \
    queryexecutors/user/activateuser.cpp \
    queryexecutors/user/adduser.cpp \
    queryexecutors/user/changepassword.cpp \
    queryexecutors/user/fetchuser.cpp \
    queryexecutors/user/fetchuserprivileges.cpp \
    queryexecutors/user/removeuser.cpp \
    queryexecutors/user/signinuser.cpp \
    queryexecutors/user/signoutuser.cpp \
    queryexecutors/user/signupuser.cpp \
    queryexecutors/user/updateuserprivileges.cpp \
    queryexecutors/user/userexecutor.cpp \
    queryexecutors/user/viewusers.cpp \
    database/databasethread.cpp \
    database/databaseserver.cpp \
    database/databasecreator.cpp \
    database/databaseexception.cpp \
    database/queryrequest.cpp \
    database/queryresult.cpp \
    qmlapi/qmldashboardhomemodel.cpp \
    user/businessadmin.cpp \
    user/businessstore.cpp \
    user/businessstoremodel.cpp \
    user/userprofile.cpp \
    database/databaseutils.cpp \
    models/abstractvisuallistmodel.cpp \
    pusher/abstractpusher.cpp \
    qmlapi/qmlsalecartmodel.cpp \
    qmlapi/qmlsaletransactionmodel.cpp \
    qmlapi/qmldoublevalidator.cpp \
    qmlapi/qmlsaletransactionitemmodel.cpp \
    qmlapi/qmldebtormodel.cpp \
    qmlapi/qmlsalehomemodel.cpp \
    models/saletotalrevenuemodel.cpp \
    detailrecord/abstractdetailrecord.cpp \
    models/abstracttransactionmodel.cpp \
    models/abstracttransactionitemmodel.cpp \
    user/businessdetails.cpp \
    utility/debtor/debtor.cpp \
    utility/debtor/debtpayment.cpp \
    utility/purchase/purchasecartproduct.cpp \
    utility/purchase/purchasedproduct.cpp \
    utility/purchase/purchasetransaction.cpp \
    utility/sales/salecartproduct.cpp \
    utility/sales/saletransaction.cpp \
    utility/sales/soldproduct.cpp \
    utility/stock/stockproduct.cpp \
    utility/stock/stockproductcategory.cpp \
    utility/stock/stockproductunit.cpp \
    utility/user/user.cpp \
    utility/user/userprivilege.cpp \
    widgets/dialogs.cpp \
    qmlapi/qmlclientmodel.cpp \
    qmlapi/qmldebttransactionmodel.cpp \
    qmlapi/qmldebtordetailrecord.cpp \
    models/salepaymentmodel.cpp \
    qmlapi/qmldatabasecreator.cpp \
    config/config.cpp \
    json/messagecenter.cpp \
    models/abstracthomemodel.cpp \
    qmlapi/qmlnotifier.cpp \
    qmlapi/qmlpurchasehomemodel.cpp \
    qmlapi/qmlpurchasecartmodel.cpp \
    models/purchasepaymentmodel.cpp \
    qmlapi/qmlincomehomemodel.cpp \
    qmlapi/qmlincomepusher.cpp \
    qmlapi/qmlexpensepusher.cpp \
    qmlapi/qmlexpensehomemodel.cpp \
    qmlapi/qmlcreditormodel.cpp \
    models/abstractvisualtablemodel.cpp \
    qmlapi/qmlincometransactionmodel.cpp \
    qmlapi/qmlusermodel.cpp \
    qmlapi/qmluserprivilegemodel.cpp \
    json/userprivilegecenter.cpp \
    models/userprivilegemodel.cpp \
    qmlapi/qmluserdetailrecord.cpp \
    qmlapi/qmlsettings.cpp \
    singletons/settings.cpp \
    qmlapi/qmlpurchasetransactionmodel.cpp \
    qmlapi/qmlpurchasetransactionitemmodel.cpp \
    qmlapi/qmlreceiptprinter.cpp \
    models/receiptcartmodel.cpp \
    singletons/logger.cpp \
    qmlapi/qmlstockreportmodel.cpp \
    qmlapi/qmlsalereportmodel.cpp \
    qmlapi/qmlpurchasereportmodel.cpp \
    qmlapi/qmlexpensereportmodel.cpp \
    qmlapi/qmlincomereportmodel.cpp

HEADERS += \
    database/databaseerror.h \
    database/exceptions/amountoverpaidexception.h \
    database/exceptions/argumentmismatchexception.h \
    database/exceptions/duplicateentryexception.h \
    database/exceptions/exceptions.h \
    database/exceptions/failedtocreateuserexception.h \
    database/exceptions/incorrectcredentialsexception.h \
    database/exceptions/incorrectpasswordexception.h \
    database/exceptions/invalidargumentexception.h \
    database/exceptions/invalidduedateexception.h \
    database/exceptions/missingargumentexception.h \
    database/exceptions/missingimplementationexception.h \
    database/exceptions/noexistingrecordexception.h \
    database/exceptions/sqlstatementfailedexception.h \
    database/exceptions/unexpectedresultexception.h \
    database/exceptions/useraccountlockedexception.h \
    database/exceptions/userpreviouslyarchivedexception.h \
    database/queryexecutor.h \
    queryexecutors/client/filterclients.h \
    queryexecutors/debtor/filterdebtors.h \
    queryexecutors/expense/filterexpensereport.h \
    queryexecutors/income/filterincomereport.h \
    queryexecutors/purchase/filterpurchasereport.h \
    queryexecutors/sales/filtersalereport.h \
    queryexecutors/stock/filterstockreport.h \
    queryexecutors/transaction/transactionexecutor.h \
    models/salemostsoldproductmodel.h \
    network/exceptions/exceptions.h \
    network/exceptions/indeterminatedestinationurlexception.h \
    network/networkerror.h \
    network/networkexception.h \
    network/networkthread.h \
    network/networkurl.h \
    network/requestlogger.h \
    network/serverrequest.h \
    network/serverresponse.h \
    qmlapi/qmldebtpaymentmodel.h \
    qmlapi/qmlexpensetransactionmodel.h \
    qmlapi/qmlstockproductcategorymodel.h \
    qmlapi/qmlstockproductcountrecord.h \
    qmlapi/qmlstockproductdetailrecord.h \
    qmlapi/qmlstockproductmodel.h \
    qmlapi/qmlstockproductpusher.h \
    qmlapi/qmluserprofile.h \
    queryexecutors/client.h \
    queryexecutors/client/clientexecutor.h \
    queryexecutors/client/viewclients.h \
    queryexecutors/dashboard.h \
    queryexecutors/dashboard/dashboardexecutor.h \
    queryexecutors/dashboard/viewdashboard.h \
    queryexecutors/debtor.h \
    queryexecutors/debtor/adddebtor.h \
    queryexecutors/debtor/debtorexecutor.h \
    queryexecutors/debtor/fetchdebtor.h \
    queryexecutors/debtor/removedebtor.h \
    queryexecutors/debtor/updatedebtor.h \
    queryexecutors/debtor/viewdebtors.h \
    queryexecutors/debtor/viewdebtpayments.h \
    queryexecutors/debtor/viewdebttransactions.h \
    queryexecutors/expense.h \
    queryexecutors/expense/addexpensetransaction.h \
    queryexecutors/expense/expenseexecutor.h \
    queryexecutors/expense/viewexpensereport.h \
    queryexecutors/expense/viewexpensetransactions.h \
    queryexecutors/income.h \
    queryexecutors/income/addincometransaction.h \
    queryexecutors/income/incomeexecutor.h \
    queryexecutors/income/viewincomereport.h \
    queryexecutors/income/viewincometransactions.h \
    queryexecutors/purchase.h \
    queryexecutors/purchase/addpurchasetransaction.h \
    queryexecutors/purchase/purchaseexecutor.h \
    queryexecutors/purchase/removepurchasedproduct.h \
    queryexecutors/purchase/removepurchasetransaction.h \
    queryexecutors/purchase/updatesuspendedpurchasetransaction.h \
    queryexecutors/purchase/viewpurchasecart.h \
    queryexecutors/purchase/viewpurchasedproducts.h \
    queryexecutors/purchase/viewpurchasehome.h \
    queryexecutors/purchase/viewpurchasereport.h \
    queryexecutors/purchase/viewpurchasetransactions.h \
    queryexecutors/sales.h \
    queryexecutors/sales/addsaletransaction.h \
    queryexecutors/sales/removesaletransaction.h \
    queryexecutors/sales/removesoldproduct.h \
    queryexecutors/sales/saleexecutor.h \
    queryexecutors/sales/updatesuspendedsaletransaction.h \
    queryexecutors/sales/viewsalecart.h \
    queryexecutors/sales/viewsalehome.h \
    queryexecutors/sales/viewsalereport.h \
    queryexecutors/sales/viewsaletransactionproducts.h \
    queryexecutors/sales/viewsaletransactions.h \
    queryexecutors/stock.h \
    queryexecutors/stock/addstockproduct.h \
    queryexecutors/stock/fetchstockproduct.h \
    queryexecutors/stock/fetchstockproductcount.h \
    queryexecutors/stock/filterstockcategories.h \
    queryexecutors/stock/filterstockcategoriesbyproduct.h \
    queryexecutors/stock/filterstockproductcount.h \
    queryexecutors/stock/filterstockproducts.h \
    queryexecutors/stock/removestockproduct.h \
    queryexecutors/stock/stockexecutor.h \
    queryexecutors/stock/updatestockproduct.h \
    queryexecutors/stock/viewstockproductcategories.h \
    queryexecutors/stock/viewstockproducts.h \
    queryexecutors/stock/viewstockreport.h \
    queryexecutors/user.h \
    queryexecutors/user/activateuser.h \
    queryexecutors/user/adduser.h \
    queryexecutors/user/changepassword.h \
    queryexecutors/user/fetchuser.h \
    queryexecutors/user/fetchuserprivileges.h \
    queryexecutors/user/removeuser.h \
    queryexecutors/user/signinuser.h \
    queryexecutors/user/signoutuser.h \
    queryexecutors/user/signupuser.h \
    queryexecutors/user/updateuserprivileges.h \
    queryexecutors/user/userexecutor.h \
    queryexecutors/user/viewusers.h \
    schema/schema.h \
    database/databasethread.h \
    database/databaseserver.h \
    database/databasecreator.h \
    database/databaseexception.h \
    database/queryrequest.h \
    database/queryresult.h \
    qmlapi/qmldashboardhomemodel.h \
    user/businessadmin.h \
    user/businessstore.h \
    user/businessstoremodel.h \
    user/userprofile.h \
    database/databaseutils.h \
    models/abstractvisuallistmodel.h \
    pusher/abstractpusher.h \
    qmlapi/qmlsalecartmodel.h \
    qmlapi/qmlsaletransactionmodel.h \
    qmlapi/qmldoublevalidator.h \
    qmlapi/qmlsaletransactionitemmodel.h \
    qmlapi/qmldebtormodel.h \
    qmlapi/qmlsalehomemodel.h \
    models/saletotalrevenuemodel.h \
    detailrecord/abstractdetailrecord.h \
    models/abstracttransactionmodel.h \
    models/abstracttransactionitemmodel.h \
    user/businessdetails.h \
    utility/commonutils.h \
    utility/debtor/debtor.h \
    utility/debtor/debtpayment.h \
    utility/debtor/debttransaction.h \
    utility/purchase/purchasecartproduct.h \
    utility/purchase/purchasedproduct.h \
    utility/purchase/purchasepayment.h \
    utility/purchase/purchasetransaction.h \
    utility/purchaseutils.h \
    utility/sales/salecartproduct.h \
    utility/sales/salepayment.h \
    utility/sales/saletransaction.h \
    utility/sales/soldproduct.h \
    utility/stock/stockproduct.h \
    utility/stock/stockproductcategory.h \
    utility/stock/stockproductunit.h \
    utility/stockutils.h \
    utility/user/user.h \
    utility/user/userprivilege.h \
    utility/userutils.h \
    widgets/dialogs.h \
    qmlapi/qmlclientmodel.h \
    qmlapi/qmldebttransactionmodel.h \
    qmlapi/qmldebtordetailrecord.h \
    utility/debtorutils.h \
    utility/saleutils.h \
    models/salepaymentmodel.h \
    qmlapi/qmldatabasecreator.h \
    config/config.h \
    json/messagecenter.h \
    models/abstracthomemodel.h \
    qmlapi/qmlnotifier.h \
    qmlapi/qmlpurchasehomemodel.h \
    qmlapi/qmlpurchasecartmodel.h \
    models/purchasepaymentmodel.h \
    qmlapi/qmlincomehomemodel.h \
    qmlapi/qmlincomepusher.h \
    qmlapi/qmlexpensepusher.h \
    qmlapi/qmlexpensehomemodel.h \
    qmlapi/qmlcreditormodel.h \
    models/abstractvisualtablemodel.h \
    qmlapi/qmlincometransactionmodel.h \
    qmlapi/qmlusermodel.h \
    qmlapi/qmluserprivilegemodel.h \
    json/userprivilegecenter.h \
    models/userprivilegemodel.h \
    qmlapi/qmluserdetailrecord.h \
    qmlapi/qmlsettings.h \
    singletons/settings.h \
    qmlapi/qmlpurchasetransactionmodel.h \
    qmlapi/qmlpurchasetransactionitemmodel.h \
    qmlapi/qmlreceiptprinter.h \
    models/receiptcartmodel.h \
    singletons/logger.h \
    qmlapi/qmlstockreportmodel.h \
    qmlapi/qmlsalereportmodel.h \
    qmlapi/qmlpurchasereportmodel.h \
    qmlapi/qmlexpensereportmodel.h \
    qmlapi/qmlincomereportmodel.h

DISTFILES +=
