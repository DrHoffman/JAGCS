#include "mission_service.h"

// Qt
#include <QMap>

// Internal
#include "mission.h"

using namespace domain;

class MissionService::Impl
{
public:
    QMap<uint8_t, Mission*> missions;
};

MissionService::MissionService(QObject* parent):
    QObject(parent),
    d(new Impl())
{
    this->requestMission(0);
}

MissionService::~MissionService()
{
    delete d;
}

Mission* MissionService::mission(uint8_t id) const
{
    return d->missions.value(id, nullptr);
}

uint8_t MissionService::missionId(Mission* mission) const
{
    return d->missions.key(mission);
}

QList<Mission*> MissionService::missions() const
{
    return d->missions.values();
}

QList<uint8_t> MissionService::missionIds() const
{
    return d->missions.keys();
}

Mission* MissionService::requestMission(uint8_t id)
{
    if (!d->missions.contains(id))
    {
        d->missions[id] = new Mission(this);
        emit missionAdded(id);
    }

    return d->missions[id];
}

void MissionService::removeMission(uint8_t id)
{
    d->missions.remove(id);
    emit missionRemoved(id);
}

void MissionService::deleteMission(uint8_t id)
{
    Mission* mission = d->missions.value(id, nullptr);
    if (!mission) return;

    this->removeMission(id);
    mission->deleteLater();
}