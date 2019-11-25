#include "gopher/system.h"

System::System(const std::string& name) {
    this->runTask = new pros::Task((pros::task_fn_t) &System::run, (void*)this, name.c_str());
    this->time = std::make_shared<okapi::TimeUtil>(okapi::Supplier<std::unique_ptr<okapi::AbstractTimer>>([]() { return std::make_unique<okapi::Timer>(); }),
                                                   okapi::Supplier<std::unique_ptr<okapi::AbstractRate>>([]() { return std::make_unique<okapi::Rate>(); }),
                                                   okapi::Supplier<std::unique_ptr<okapi::SettledUtil>>([]() { return std::make_unique<okapi::SettledUtil>(std::make_unique<okapi::Timer>()); }));
}

System::~System() {
    delete this->runTask;
}

void System::run() {
    while (this->doRunLoop) {
        this->loop();

        this->time->getRate()->delayUntil(this->loopDelay);
    }
}