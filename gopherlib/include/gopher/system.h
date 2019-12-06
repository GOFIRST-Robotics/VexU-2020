#pragma once
#include "okapi/api.hpp"

using namespace okapi::literals;

#define SYSTEM_LOOP_DELAY 10_ms

class System {
    public:
    System(const std::string& name);
    ~System();
    
    private:
    virtual void loop() = 0;
    void run();

    protected:
    bool doRunLoop = true;
    okapi::QTime loopDelay = SYSTEM_LOOP_DELAY;
    pros::Task* runTask;
    std::shared_ptr<okapi::TimeUtil> time;
};