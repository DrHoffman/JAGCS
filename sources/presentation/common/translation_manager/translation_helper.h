#ifndef TRANSLATION_HELPER_H
#define TRANSLATION_HELPER_H

// Qt
#include <QObject>

// Internal
#include "mission_item.h"
#include "vehicle.h"
#include "vehicle_types.h"

namespace presentation
{
    class TranslationHelper : public QObject
    {
        Q_OBJECT

    public:
        explicit TranslationHelper(QObject* parent = nullptr);

        Q_INVOKABLE QString translateCommand(dao::MissionItem::Command command);
        Q_INVOKABLE QString translateVehicleType(dao::Vehicle::Type type);
        Q_INVOKABLE QString translateMode(domain::vehicle::Mode mode);
    };
}

#endif // TRANSLATION_HELPER_H
