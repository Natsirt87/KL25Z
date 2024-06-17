// Definitions for event type
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EVENT_NONE,
    EVENT_BTN_MODE,
    EVENT_BTN_TIMING,
    EVENT_SENSOR_READ
} event_id_t;

typedef struct {
    event_id_t id;
    union {
        // Each possible field is named after the corresponding event id
        bool btn_mode; // true = flash, false = sequence
        bool btn_timing; // true = fast, false = slow
        struct {
            uint32_t sensor_data1;
            uint32_t sensor_data2;
        } sensor_read;
    } data;
} event_t;