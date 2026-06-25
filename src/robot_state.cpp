#include "coverage_demo/robot_state.hpp"

RobotState nextState(
    RobotState current,
    bool obstacle,
    bool reset)
{
    if (current == RobotState::ERROR && reset)
        return RobotState::RECOVERY;

    if (obstacle)
        return RobotState::ERROR;

    if (current == RobotState::IDLE)
        return RobotState::DRIVING;

    return current;
}
