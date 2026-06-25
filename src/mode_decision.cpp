#include "coverage_demo/mode_decision.hpp"

Mode decideMode(bool sensor_ok, bool manual_mode)
{
    if (!sensor_ok)
        return Mode::ERROR;

    if (manual_mode)
        return Mode::MANUAL;

    return Mode::AUTO;
}
