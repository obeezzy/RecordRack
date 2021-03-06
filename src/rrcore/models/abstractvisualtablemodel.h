#ifndef ABSTRACTVISUALTABLEMODEL_H
#define ABSTRACTVISUALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QLoggingCategory>
#include <QQmlParserStatus>
#include <QVariant>
#include "database/queryexecutor.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "models/modelresult.h"
#include "utility/common/filtercriteria.h"
#include "utility/common/sortcriteria.h"

class QueryRequest;
class QueryResult;
class DatabaseThread;
class QueryExecutor;

class AbstractVisualTableModel : public QAbstractTableModel,
                                 public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool autoQuery READ autoQuery WRITE setAutoQuery NOTIFY
                   autoQueryChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY
                   filterTextChanged)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn NOTIFY
                   filterColumnChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY
                   sortOrderChanged)
    Q_PROPERTY(int sortColumn READ sortColumn WRITE setSortColumn NOTIFY
                   sortColumnChanged)
    Q_PROPERTY(double tableViewWidth READ tableViewWidth WRITE
                   setTableViewWidth)  // NOTE: Can only be set once!
public:
    explicit AbstractVisualTableModel(QObject* parent = nullptr);
    explicit AbstractVisualTableModel(DatabaseThread& thread,
                                      QObject* parent = nullptr);
    virtual ~AbstractVisualTableModel() override = default;

    bool autoQuery() const;
    void setAutoQuery(bool);

    bool isBusy() const;

    QString filterText() const;
    void setFilterText(const QString& filterText);

    int filterColumn() const;
    void setFilterColumn(int filterColumn);

    Qt::SortOrder sortOrder() const;
    void setSortOrder(Qt::SortOrder sortOrder);

    int sortColumn() const;
    void setSortColumn(int sortColumn);

    double tableViewWidth() const;
    void setTableViewWidth(double tableViewWidth);

    Q_INVOKABLE QVariant get(int row, int column) const;

    void classBegin() override;
    void componentComplete() override;
public slots:
    virtual void undoLastCommit();
    virtual void refresh();

protected:
    virtual void tryQuery() = 0;
    virtual bool canProcessResult(const QueryResult& result) const = 0;
    virtual void processResult(const QueryResult& result) = 0;
    virtual QString columnName(int column) const;
    virtual void filter();
    void setBusy(bool);
    const QueryRequest& lastSuccessfulRequest() const;
    Utility::FilterCriteria filterCriteria() const;
    Utility::SortCriteria sortCriteria() const;
signals:
    void execute(QueryExecutor*);
    void autoQueryChanged();
    void busyChanged();

    void filterTextChanged();
    void filterColumnChanged();

    void sortOrderChanged();
    void sortColumnChanged();

    void tableViewWidthChanged();

    void success(ModelResult result = ModelResult());
    void error(ModelResult result = ModelResult());

private:
    bool m_autoQuery{true};
    bool m_busy{false};
    Utility::FilterCriteria m_filterCriteria;
    Utility::SortCriteria m_sortCriteria;
    double m_tableViewWidth{0.0};
    QueryRequest m_lastSuccessfulRequest;

    void validateResult(const QueryResult result);
    void saveRequest(const QueryResult& result);
};

Q_DECLARE_LOGGING_CATEGORY(abstractVisualTableModel);

#endif  // ABSTRACTVISUALTABLEMODEL_H
