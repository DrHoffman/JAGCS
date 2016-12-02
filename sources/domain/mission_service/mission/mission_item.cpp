#include "mission_item.h"

using namespace domain;

MissionItem::MissionItem(QObject* parent):
    QObject(parent),
    m_current(false)
{}

QGeoCoordinate MissionItem::coordinate() const
{
    return m_coordinate;
}

bool MissionItem::isCurrent() const
{
    return m_current;
}

void MissionItem::setCoordinate(const QGeoCoordinate& coordinate)
{
    if (m_coordinate == coordinate) return;

    m_coordinate = coordinate;
    emit coordinateChanged(coordinate);
}

void MissionItem::setCurrent(bool current)
{
    if (m_current == current) return;

    m_current = current;
    emit currentChanged(current);
}
