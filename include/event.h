// Definitions for event type

#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef enum {
    EVENT_BTN_MODE,
    EVENT_BTN_TIMING,
    EVENT_SENSOR_READ
} event_id_t;

typedef struct {
    event_id_t id;

    // Each possible field is named after the corresponding event id
    union {
        // 0 = sequence, 1 = flash
        uint8_t btn_mode; 

        // 0 = slow, 1 = fast
        uint8_t btn_timing; 

        // Example of complex data
        struct {
            uint32_t sensor_data1;
            uint32_t sensor_data2;
        } sensor_read;
    } data;
} event_t;

#endif