#include "coverage_demo/safety_monitor.hpp"

void SafetyMonitor::laserCallback(bool obstacle_detected)
{
    if (obstacle_detected)
        emergency_stop_ = true;
}

bool SafetyMonitor::emergencyStop() const
{
    return emergency_stop_;
}
