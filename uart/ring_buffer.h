 

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
  最好是2的幂次方长度，比如16，32，128.否则可能出错
 */
typedef struct ring_buffer {
    uint8_t  *mem;        /* Pointer to backing storage array           */
    size_t    size;       /* Total capacity (must be power of 2)        */
    volatile  uint32_t  head;       /* Write index (advanced by producer)         */
    volatile  uint32_t  tail;       /* Read index  (advanced by consumer)         */
} ring_buffer_t;

/* ==================== API ==================== */

/**
 * Initialize a ring buffer
 * @param rb     Pointer to ring_buffer_t (must be stack or global allocated)
 * @param mem    Pointer to backing storage array (caller-allocated)
 * @param size   Size of backing storage in bytes (recommended: power of 2)
 */
void ring_buffer_init(ring_buffer_t *rb, uint8_t *mem, uint32_t size);

/* ===== Producer API (push / write) ===== */
/*  写入函数。。。。write 跟 push   */
/** Push a single byte. Returns false if full. */
bool ring_buffer_push(ring_buffer_t *rb, uint8_t byte);


uint32_t ring_buffer_write(ring_buffer_t *rb, const uint8_t *data, uint32_t len);

uint8_t *ring_buffer_reserve(ring_buffer_t *rb, uint32_t len);

/** Commit previously reserved bytes (advance write pointer). */
void ring_buffer_commit(ring_buffer_t *rb, uint32_t len);


/*   读取函数 pop 跟 read*/

bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *byte);

uint32_t ring_buffer_read(ring_buffer_t *rb, uint8_t *dst, uint32_t len);

/**
 * 读取是否满了.
 * Returns false if empty.
 */
bool ring_buffer_peek(ring_buffer_t *rb, uint8_t *byte);

/** Remove and discard n bytes (advance read pointer). */
void ring_buffer_drain(ring_buffer_t *rb, uint32_t len);

/* ===== Status ===== */

/** Number of unread bytes available to consumer. */
uint32_t ring_buffer_available(ring_buffer_t *rb);

/** Number of free bytes available for producer to write. */
uint32_t ring_buffer_free_space(ring_buffer_t *rb);

/** True if no data available to read. */
bool ring_buffer_is_empty(ring_buffer_t *rb);

/** True if no free space for writing. */
bool ring_buffer_is_full(ring_buffer_t *rb);

#endif /* RING_BUFFER_H */
