#include "qmlusermodel.h"
#include "database/databasethread.h"
#include "models/abstractvisualtablemodel.h"
#include "query/user/activateuser.h"
#include "query/user/removeuser.h"
#include "query/user/viewusers.h"
#include "utility/user/user.h"

QMLUserModel::QMLUserModel(QObject* parent)
    : QMLUserModel(DatabaseThread::instance(), parent)
{}

QMLUserModel::QMLUserModel(DatabaseThread& thread, QObject* parent)
    : AbstractVisualTableModel(thread, parent)
{}

int QMLUserModel::keys() const
{
    return m_keys;
}

void QMLUserModel::setKeys(int keys)
{
    if (m_keys == keys)
        return;

    m_keys = keys;
    emit keysChanged();
}

int QMLUserModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_users.count();
}

int QMLUserModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnCount;
}

QVariant QMLUserModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case UserIdRole:
            return m_users.at(index.row()).id;
        case UserRole:
            return m_users.at(index.row()).user;
        case ActiveRole:
            return m_users.at(index.row())
                .flags.testFlag(Utility::RecordGroup::Active);
        case PresetRole:
            return m_users.at(index.row()).preset;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLUserModel::roleNames() const
{
    return {{UserIdRole, "user_id"},
            {UserRole, "user"},
            {ActiveRole, "active"},
            {PresetRole, "preset"}};
}

QVariant QMLUserModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
                case UserColumn:
                    return tr("User");
                case ActiveColumn:
                    return tr("Active");
                case PresetColumn:
                    return tr("Preset");
                case ActionColumn:
                    return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
                case UserColumn:
                case ActiveColumn:
                case PresetColumn:
                    return Qt::AlignLeft;
                case ActionColumn:
                    return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
                case UserColumn:
                    return tableViewWidth() - 100 - 100 - 130;
                case ActiveColumn:
                    return 100;
                case PresetColumn:
                    return 100;
                case ActionColumn:
                    return 130;
            }
        }
    }

    return section + 1;
}

void QMLUserModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::User::ViewUsers(Utility::RecordGroup::None, this));
}

bool QMLUserModel::canProcessResult(const QueryResult& result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLUserModel::processResult(const QueryResult& result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == Query::User::ActivateUser::COMMAND) {
            emit success(ModelResult{ActivateUserSuccess});
        } else {
            beginResetModel();
            if (result.request().command() == Query::User::ViewUsers::COMMAND) {
                m_users = Utility::User::UserList{
                    result.outcome().toMap().value("users").toList()};
                emit success(ModelResult{ViewUsersSuccess});
            } else if (result.request().command() ==
                       Query::User::RemoveUser::COMMAND) {
                const auto user = Utility::User::User{result.outcome().toMap()};
                removeUserFromModel(user);
                emit success(ModelResult{RemoveUserSuccess});
            } else if (result.request().command() ==
                       Query::User::RemoveUser::UNDO_COMMAND) {
                emit success(ModelResult{UndoRemoveUserSuccess});
            }
            endResetModel();
        }
    } else {
        emit error();
    }
}

void QMLUserModel::removeUserFromModel(const Utility::User::User& user)
{
    beginRemoveRows(QModelIndex(), user.row, user.row);
    m_users.removeAt(user.row);
    endRemoveRows();
}

void QMLUserModel::removeUser(int row)
{
    setBusy(true);
    auto& user{m_users[row]};
    user.row = row;
    emit execute(new Query::User::RemoveUser(user, this));
}

void QMLUserModel::activateUser(int row, bool active)
{
    setBusy(true);
    auto& user{m_users[row]};
    user.row = row;
    emit execute(new Query::User::ActivateUser(active, user, this));
}
