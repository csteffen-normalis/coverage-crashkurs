#pragma once

enum class RobotState
{
    IDLE,
    DRIVING,
    ERROR,
    RECOVERY
};

RobotState nextState(
    RobotState current,
    bool obstacle,
    bool reset);
