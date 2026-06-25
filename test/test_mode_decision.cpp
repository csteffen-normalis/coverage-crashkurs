#include <gtest/gtest.h>

#include "coverage_demo/mode_decision.hpp"

TEST(DecideModeTest, ReturnsAuto)
{
    EXPECT_EQ(decideMode(true, false), Mode::AUTO);
}
