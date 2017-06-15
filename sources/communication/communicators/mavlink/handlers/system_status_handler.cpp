#include "system_status_handler.h"

// MAVLink
#include <mavlink.h>

// Qt
#include <QVariant>

// Internal
#include "mavlink_protocol_helpers.h"

#include "telemetry_service.h"
#include "telemetry.h"

using namespace comm;
using namespace domain;

SystemStatusHandler::SystemStatusHandler(TelemetryService* telemetryService,
                                         MavLinkCommunicator* communicator):
    AbstractMavLinkHandler(communicator),
    m_telemetryService(telemetryService)
{}

void SystemStatusHandler::processMessage(const mavlink_message_t& message)
{
    if (message.msgid != MAVLINK_MSG_ID_SYS_STATUS) return;

    Telemetry* node = m_telemetryService->nodeByMavId(message.sysid);
    if (!node) return;

    mavlink_sys_status_t status;
    mavlink_msg_sys_status_decode(&message, &status);

    node->setParameter({ Telemetry::Ahrs, Telemetry::Present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_AHRS);
    node->setParameter({ Telemetry::Ahrs, Telemetry::Enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_AHRS);
    node->setParameter({ Telemetry::Ahrs, Telemetry::Operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_AHRS);

    node->setParameter({ Telemetry::Satellite, Telemetry::Present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_GPS);
    node->setParameter({ Telemetry::Satellite, Telemetry::Enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_GPS);
    node->setParameter({ Telemetry::Satellite, Telemetry::Operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_GPS);

    node->setParameter({ Telemetry::Compass, Telemetry::Present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_3D_MAG);
    node->setParameter({ Telemetry::Compass, Telemetry::Enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_3D_MAG);
    node->setParameter({ Telemetry::Compass, Telemetry::Operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_3D_MAG);

    node->setParameter({ Telemetry::Barometric, Telemetry::Present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);
    node->setParameter({ Telemetry::Barometric, Telemetry::Enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);
    node->setParameter({ Telemetry::Barometric, Telemetry::Operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE);

    node->setParameter({ Telemetry::Pitot, Telemetry::Present },
                       status.onboard_control_sensors_present & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);
    node->setParameter({ Telemetry::Pitot, Telemetry::Enabled },
                       status.onboard_control_sensors_enabled & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);
    node->setParameter({ Telemetry::Pitot, Telemetry::Operational },
                       status.onboard_control_sensors_health & MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE);

    node->notify();

// TODO: MAV_SYS_STATUS_SENSOR_3D_ACCEL, MAV_SYS_STATUS_SENSOR_3D_GYRO, MAV_SYS_STATUS_SENSOR_LASER_POSITION
// TODO: battery
}
