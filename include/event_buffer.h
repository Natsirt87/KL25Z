/* 
    Circular event buffer for handling task-based event handling & communication 
*/

#include "event.h"

#define EVENT_BUFFER_SIZE 10

typedef struct {
    event_t events[EVENT_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} event_buffer_t;

void EventBuffer_Init(event_buffer_t *buffer);
bool EventBuffer_Push(event_buffer_t *buffer, event_t *event);
bool EventBuffer_Pop(event_buffer_t *buffer, event_t *event);