#ifndef STOCKPRODUCTUNIT_H
#define STOCKPRODUCTUNIT_H

#include "utility/common/recordgroup.h"
#include <QString>
#include <QDebug>

namespace Utility {
struct StockProductUnit
{
    int id {0};
    QString unit;
    RecordGroup::Flags flags;
    double baseUnitEquivalent {1.0};

    explicit StockProductUnit() = default;
    explicit StockProductUnit(int id,
                              const QString &unit);
    explicit StockProductUnit(const QString &unit,
                              double baseUnitEquivalent);

    friend QDebug operator<<(QDebug debug, const StockProductUnit &unit)
    {
        debug.nospace() << "StockProductUnit("
                        << "id=" << unit.id
                        << ", product=" << unit.unit
                        << ", baseUnitEquivalent=" << unit.baseUnitEquivalent
                        << ")";

        return debug.nospace();
    }
};
}
Q_DECLARE_TYPEINFO(Utility::StockProductUnit, Q_PRIMITIVE_TYPE);

#endif // STOCKPRODUCTUNIT_H
