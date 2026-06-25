#pragma once

enum class Mode
{
    AUTO,
    MANUAL,
    ERROR
};

Mode decideMode(bool sensor_ok, bool manual_mode);
