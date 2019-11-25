#include "gopher/comm.h"
#include "gopher/utils.h"
#include "gopher/uart.h"
#include "pros/apix.h"
#include <fcntl.h>

void CommHandler::init() {
    pros::c::serctl(SERCTL_DISABLE_COBS, NULL);
    this->listenTask = std::make_shared<pros::Task>((pros::task_fn_t) &CommHandler::runTask, (void*)this, "USBComm");
}

void CommHandler::registerPacketHandler(uint8_t id, comm_handler_ptr handler) {
    this->handlers.at(id).push_back(handler);
}

void CommHandler::sendPacket(uint8_t id, int32_t data) {
    writeUart(id, data);
}

void CommHandler::runTask(void * arg) {
    uint32_t time = pros::millis();
    while (true) {
        uint8_t packetId;
        int32_t data;
        readUart(&packetId, &data);

        // Loop through handlers
        std::vector<comm_handler_ptr> handlers = this->handlers.at(packetId);
        std::vector<comm_handler_ptr>::iterator begin = handlers.begin();
        for (std::vector<comm_handler_ptr>::iterator ptr = handlers.begin(); ptr < handlers.end(); ptr++) {
            (*ptr)(packetId, data);
        }

        this->listenTask->delay_until(&time, 10);
    }
}