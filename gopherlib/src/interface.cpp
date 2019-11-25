#include "display/lvgl.h"
#include "gopher/utils.h"
#include <string>
#include <vector>

ScreenInterface::ScreenInterface(std::vector<std::pair<std::string, auton_ptr>> * autons) 
    : System("ScreenInterface") {
    std::cout << "this is the ScreenInterface constructor" << std::endl;
    this->autons = autons;
    this->loopDelay = 250_ms; // We don't need to spend a lot of time just watching pose
}

void ScreenInterface::initialize() {
    // Setup auton chooser
    lv_obj_t * auton_ddlist = lv_ddlist_create(lv_scr_act(), NULL);            /*Create a drop down list*/
    lv_obj_align(auton_ddlist, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_set_top(auton_ddlist, true);
    // Build string of options
    std::string options = "";
    for (auto const& x : *autons) {
        options = options.append(x.first);
        options = options.append("\n");
    }
    options = options.substr(0, options.length()-1); // Remove the last \n
    lv_ddlist_set_options(auton_ddlist, options.c_str()); /*Set the options*/
    lv_obj_set_free_ptr(auton_ddlist, this);
    lv_ddlist_set_action(auton_ddlist, &chooseAuton); /*Set function to call on new option is chosen*/

    // Setup color chooser
    lv_obj_t * color_ddlist = lv_ddlist_create(lv_scr_act(), NULL);            /*Create a drop down list*/
    lv_obj_align(color_ddlist, auton_ddlist, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);
    lv_obj_set_top(color_ddlist, true);
    lv_ddlist_set_options(color_ddlist, "Blue\nRed"); /*Set the options*/
    lv_obj_set_free_ptr(color_ddlist, this);
    lv_ddlist_set_action(color_ddlist, &chooseColor); /*Set function to call on new option is chosen*/

    this->setSelectedAuton(0);
    this->_init = true;
}

lv_res_t chooseAuton(lv_obj_t * ddlist) {
    uint16_t opt = lv_ddlist_get_selected(ddlist);      /*Get the id of selected option*/
    ScreenInterface* interface = static_cast<ScreenInterface*>(lv_obj_get_free_ptr(ddlist));
    interface->setSelectedAuton(opt);

    return LV_RES_OK;
}

lv_res_t chooseColor(lv_obj_t * ddlist) {
    uint16_t opt = lv_ddlist_get_selected(ddlist);      /*Get the id of selected option*/
    ScreenInterface* interface = static_cast<ScreenInterface*>(lv_obj_get_free_ptr(ddlist));

    interface->setColor(opt);

    return LV_RES_OK;
}

void ScreenInterface::setSelectedAuton(uint16_t opt) {
    this->auton = this->autons->at(opt).second;
}

void ScreenInterface::setColor(uint16_t opt) {
    this->color = static_cast<AllianceColor>(opt);
}

auton_ptr ScreenInterface::getSelectedAuton() {
    return this->auton;
}

AllianceColor ScreenInterface::getColor() {
    return this->color;
}

void ScreenInterface::loop() {
    if (this->_init) {
        
    }
}