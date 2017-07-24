#include "mission_line_map_item_model.h"

// Qt
#include <QGeoCoordinate>
#include <QDebug>

// Internal
#include "db_facade.h"
#include "mission.h"
#include "mission_item.h"

using namespace presentation;

MissionLineMapItemModel::MissionLineMapItemModel(db::DbFacade* dbFacade, QObject* parent):
    QAbstractListModel(parent),
    m_dbFacade(dbFacade)
{
    connect(dbFacade, &db::DbFacade::missionAdded,
            this, &MissionLineMapItemModel::onMissionAdded);
    connect(dbFacade, &db::DbFacade::missionRemoved,
            this, &MissionLineMapItemModel::onMissionRemoved);
    connect(dbFacade, &db::DbFacade::missionItemChanged,
            this, &MissionLineMapItemModel::onMissionItemChanged);

    for (const db::MissionPtr& item: dbFacade->missions())
    {
        this->onMissionAdded(item);
    }
}

int MissionLineMapItemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_missions.count();
}

QVariant MissionLineMapItemModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_missions.count()) return QVariant();

    const db::MissionPtr& mission = m_missions.at(index.row());

    switch (role)
    {
    case MissionPathRole:
    {
        QVariantList line;
        for (const db::MissionItemPtr& item: m_dbFacade->missionItems(mission->id()))
        {
            if (item->command() == db::MissionItem::Waypoint ||
                item->command() == db::MissionItem::Takeoff ||
                item->command() == db::MissionItem::Landing ||
                item->command() == db::MissionItem::LoiterAltitude ||
                item->command() == db::MissionItem::LoiterTurns)
            {
                QGeoCoordinate coordinate(item->latitude(), item->longitude());
                if (coordinate.isValid())
                {
                    line.append(QVariant::fromValue(coordinate));
                }
            }
            else if (item->command() == db::MissionItem::Return && !line.isEmpty())
            {
                line.append(line[0]); // Return to home line
            }
        }
        return line;
    }
    default:
        return QVariant();
    }
}

void MissionLineMapItemModel::onMissionAdded(const db::MissionPtr& mission)
{
    this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
    m_missions.append(mission);
    this->endInsertRows();
}

void MissionLineMapItemModel::onMissionRemoved(const db::MissionPtr& mission)
{
    int row = m_missions.indexOf(mission);

    this->beginRemoveRows(QModelIndex(), row, row);
    m_missions.removeOne(mission);
    this->endRemoveRows();
}

void MissionLineMapItemModel::onMissionItemChanged(const db::MissionItemPtr& item)
{
    db::MissionPtr mission = m_dbFacade->mission(item->missionId());
    QModelIndex index = this->index(m_missions.indexOf(mission));
    if (index.isValid()) emit dataChanged(index, index, { MissionPathRole });
}

QHash<int, QByteArray> MissionLineMapItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[MissionPathRole] = "missionPath";

    return roles;
}

QModelIndex MissionLineMapItemModel::missionIndex(const db::MissionPtr& mission) const
{
    return this->index(m_missions.indexOf(mission));
}
