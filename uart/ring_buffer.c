#include "ring_buffer.h"

/*
最好是2的幂次方长度，
 */

#define RB_MASK(rb) ((rb)->size - 1U)


void ring_buffer_init(ring_buffer_t *rb, uint8_t *mem, uint32_t size)
{
    if (rb == NULL || mem == NULL || size == 0U)
        return;

    /* size must be power of 2 */
    if ((size & (size - 1U)) != 0U)
        return;

    rb->mem  = mem;
    rb->size = size;
    rb->head = 0U;
    rb->tail = 0U;
}


bool ring_buffer_push(ring_buffer_t *rb, uint8_t byte)
{
    if (rb == NULL || ring_buffer_is_full(rb))
        return false;

    rb->mem[rb->head & RB_MASK(rb)] = byte;
    rb->head++;

    return true;
}


uint32_t ring_buffer_write(ring_buffer_t *rb,
                           const uint8_t *data,
                           uint32_t len)
{
    uint32_t i;
    uint32_t write_len;

    if (rb == NULL || data == NULL || len == 0U)
        return 0U;

    write_len = ring_buffer_free_space(rb);

    if (len < write_len)
        write_len = len;

    for (i = 0U; i < write_len; i++)
    {
        rb->mem[rb->head & RB_MASK(rb)] = data[i];
        rb->head++;
    }

    return write_len;
}


/*
 * Zero-copy write begin
 *
 * IMPORTANT:
 * reserve only returns address.
 * It does NOT move head.
 * Call ring_buffer_commit() after data is written.
 */
uint8_t *ring_buffer_reserve(ring_buffer_t *rb, uint32_t len)
{
    if (rb == NULL || len == 0U)
        return NULL;

    if (ring_buffer_free_space(rb) < len)
        return NULL;

    return &rb->mem[rb->head & RB_MASK(rb)];
}


void ring_buffer_commit(ring_buffer_t *rb, uint32_t len)
{
    if (rb == NULL || len == 0U)
        return;

    if (len <= ring_buffer_free_space(rb))
    {
        rb->head += len;
    }
}


bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *byte)
{
    if (rb == NULL || byte == NULL || ring_buffer_is_empty(rb))
        return false;

    *byte = rb->mem[rb->tail & RB_MASK(rb)];
    rb->tail++;

    return true;
}


uint32_t ring_buffer_read(ring_buffer_t *rb,
                          uint8_t *dst,
                          uint32_t len)
{
    uint32_t i;
    uint32_t read_len;

    if (rb == NULL || dst == NULL || len == 0U)
        return 0U;

    read_len = ring_buffer_available(rb);

    if (len < read_len)
        read_len = len;

    for (i = 0U; i < read_len; i++)
    {
        dst[i] = rb->mem[rb->tail & RB_MASK(rb)];
        rb->tail++;
    }

    return read_len;
}


bool ring_buffer_peek(ring_buffer_t *rb, uint8_t *byte)
{
    if (rb == NULL || byte == NULL || ring_buffer_is_empty(rb))
        return false;

    *byte = rb->mem[rb->tail & RB_MASK(rb)];

    return true;
}


void ring_buffer_drain(ring_buffer_t *rb, uint32_t len)
{
    uint32_t available;

    if (rb == NULL || len == 0U)
        return;

    available = ring_buffer_available(rb);

    if (len > available)
        len = available;

    rb->tail += len;
}


uint32_t ring_buffer_available(ring_buffer_t *rb)
{
    uint32_t used;

    if (rb == NULL)
        return 0U;

    used = rb->head - rb->tail;

    /* protection against corrupted index */
    if (used > rb->size)
        return 0U;

    return used;
}


uint32_t ring_buffer_free_space(ring_buffer_t *rb)
{
    uint32_t used;

    if (rb == NULL)
        return 0U;

    used = rb->head - rb->tail;

    if (used >= rb->size)
        return 0U;

    return rb->size - used;
}


bool ring_buffer_is_empty(ring_buffer_t *rb)
{
    if (rb == NULL)
        return true;

    return (rb->head == rb->tail);
}


bool ring_buffer_is_full(ring_buffer_t *rb)
{
    if (rb == NULL)
        return true;

    return ((rb->head - rb->tail) >= rb->size);
}
