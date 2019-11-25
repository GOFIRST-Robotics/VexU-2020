#pragma once

#include "api.h"
#include <map> 
#include <vector>
#include <memory>
#include "utils.h"

typedef void (*comm_handler_ptr)(uint8_t packetId, int32_t value);

class CommHandler {
    public:
        static CommHandler& getInstance() {
            static CommHandler instance;
            return instance;
        }
    private:
        CommHandler() {}
        std::map<uint8_t, std::vector<comm_handler_ptr>> handlers;    
        std::shared_ptr<pros::Task> listenTask;  

        void runTask(void* arg);
    public:
        CommHandler(CommHandler const&) = delete;
        void operator=(CommHandler const&) = delete;

        void init();
        void registerPacketHandler(uint8_t id, comm_handler_ptr handler);
        void sendPacket(uint8_t id, int32_t data);

};
