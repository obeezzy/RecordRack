#include "userprivilege.h"

UserPrivilege::UserPrivilege() :
    id(-1),
    userId(-1)
{

}

UserPrivilege::UserPrivilege(int userId) :
    id(-1),
    userId(userId)
{

}

UserPrivilege::UserPrivilege(const QVariantMap &map) :
    id(map.value("user_privilege_id").toInt()),
    userId(map.value("user_id").toInt())
{

}

QVariantMap UserPrivilege::toVariantMap() const
{
    return { };
}
