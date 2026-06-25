#include <gtest/gtest.h>

#include "coverage_demo/safety_monitor.hpp"

TEST(SafetyMonitorTest, ObjectCanBeCreated)
{
    SafetyMonitor monitor;

    EXPECT_FALSE(monitor.emergencyStop());
}
