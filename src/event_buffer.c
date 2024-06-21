#include "event_buffer.h"

void EventBuffer_Init(volatile event_buffer_t *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
}

bool EventBuffer_Push(volatile event_buffer_t *buffer, event_t *event) {
    uint8_t next_head = (buffer->head + 1) % EVENT_BUFFER_SIZE;
    if (next_head == buffer->tail) {
        // Buffer is full, advance tail to overwrite the oldest entry
        buffer->tail = (buffer->tail + 1) % EVENT_BUFFER_SIZE;
    }
    buffer->events[buffer->head] = *event;
    buffer->head = next_head;
    return true;
}

bool EventBuffer_Pop(volatile event_buffer_t *buffer, event_t *event) {
    if (buffer->head == buffer->tail) {
        // Buffer is empty
        return false;
    }
    *event = buffer->events[buffer->tail];
    buffer->tail = (buffer->tail + 1) % EVENT_BUFFER_SIZE;
    return true;
}