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

TEST(StateMachineTest, RecoversFromErrorOnReset)
{
    EXPECT_EQ(
        nextState(RobotState::ERROR, false, true),
        RobotState::RECOVERY);
}

TEST(StateMachineTest, ExecutesRecoveryWithCheckingResult)
{
    EXPECT_EQ(
        nextState(RobotState::ERROR, false, true),
        RobotState::RECOVERY);
}
