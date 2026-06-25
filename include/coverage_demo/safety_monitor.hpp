#pragma once

class SafetyMonitor
{
public:
    void laserCallback(bool obstacle_detected);
    bool emergencyStop() const;

private:
    bool emergency_stop_ = false;
};
