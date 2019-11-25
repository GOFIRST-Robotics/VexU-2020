#pragma once

#include "api.h"

void writeUart(uint8_t packet_id, int32_t value);
void readUart(uint8_t *packet_id, int32_t *value);