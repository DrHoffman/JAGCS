#ifndef PRESSURE_HANDLER_H
#define PRESSURE_HANDLER_H

#include "abstract_mavlink_handler.h"

namespace domain
{
    class TelemetryService;
}

namespace comm
{
    class PressureHandler: public AbstractMavLinkHandler
    {
    public:
        explicit PressureHandler(MavLinkCommunicator* communicator);

        void processMessage(const mavlink_message_t& message) override;

    private:
        const domain::TelemetryService* m_telemetryService;
    };
}

#endif // PRESSURE_HANDLER_H
