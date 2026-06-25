#include <gtest/gtest.h>

#include "coverage_demo/robot_state.hpp"

TEST(StateMachineTest, StartsDrivingFromIdle)
{
    EXPECT_EQ(
        nextState(RobotState::IDLE, false, false),
        RobotState::DRIVING);
}

TEST(StateMachineTest, ObstacleCausesError)
{
    EXPECT_EQ(
        nextState(RobotState::DRIVING, true, false),
        RobotState::ERROR);
}
