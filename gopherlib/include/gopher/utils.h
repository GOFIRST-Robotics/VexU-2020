#include "system.h"
#include <stdlib.h>
#include "display/lvgl.h"

#pragma once

#define EPS 1e-15

typedef void (*auton_ptr)();

enum AllianceColor {
    blue = 0,
    red 
};

class AutoRoutine {
    public:
    virtual void execute();
};

class ScreenInterface : public System {
    private:
    auton_ptr auton;
    AllianceColor color;

    std::vector<std::pair<std::string, auton_ptr>> * autons;
    lv_obj_t * pose_label;

    void loop() override;

    bool _init = false;

    public:
    ScreenInterface(std::vector<std::pair<std::string, auton_ptr>> * autons);
    auton_ptr getSelectedAuton();
    AllianceColor getColor();
    void setSelectedAuton(uint16_t opt);
    void setColor(uint16_t opt);
    void initialize();
};

lv_res_t chooseAuton(lv_obj_t * ddlist);
lv_res_t chooseColor(lv_obj_t * ddlist);

int32_t getVelocityFromGearset(okapi::AbstractMotor::gearset gearset);
int32_t getEncoderTicksFromGearset(okapi::AbstractMotor::gearset gearset);