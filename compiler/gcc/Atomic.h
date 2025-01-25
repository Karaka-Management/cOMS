/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_COMPILER_GCC_ATOMIC_H
#define TOS_COMPILER_GCC_ATOMIC_H

#include "../../stdlib/Types.h"
#include <stdatomic.h>

inline void atomic_set_relaxed(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_RELAXED); }
inline void* atomic_get_relaxed(void** target) { return __atomic_load_n(target, __ATOMIC_RELAXED); }
inline void atomic_set_relaxed(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
inline void atomic_set_relaxed(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
inline int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
inline int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
inline int32 atomic_get_relaxed(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_RELAXED); }
inline int64 atomic_get_relaxed(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_RELAXED); }
inline void atomic_increment_relaxed(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_decrement_relaxed(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_increment_relaxed(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_decrement_relaxed(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_add_relaxed(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
inline void atomic_sub_relaxed(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
inline void atomic_add_relaxed(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
inline void atomic_sub_relaxed(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
inline int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
inline int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
inline int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
inline int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
inline int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
inline void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
inline void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
inline uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
inline uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
inline uint32 atomic_get_relaxed(volatile uint32* value) { return __atomic_load_n((uint32 *) value, __ATOMIC_RELAXED); }
inline uint64 atomic_get_relaxed(volatile uint64* value) { return __atomic_load_n((uint64 *) value, __ATOMIC_RELAXED); }
inline void atomic_increment_relaxed(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_decrement_relaxed(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_increment_relaxed(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_decrement_relaxed(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
inline void atomic_add_relaxed(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
inline void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
inline uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
inline uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
inline uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
inline uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
inline uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
inline void atomic_and_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
inline void atomic_and_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
inline void atomic_and_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
inline void atomic_and_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
inline void atomic_or_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
inline void atomic_or_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
inline void atomic_or_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
inline void atomic_or_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }

inline void atomic_set_acquire(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_ACQUIRE); }
inline void* atomic_get_acquire(void** target) { return __atomic_load_n(target, __ATOMIC_ACQUIRE); }
inline void atomic_set_acquire(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
inline void atomic_set_acquire(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
inline int32 atomic_fetch_set_acquire(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
inline int64 atomic_fetch_set_acquire(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
inline int32 atomic_get_acquire(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_ACQUIRE); }
inline int64 atomic_get_acquire(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_ACQUIRE); }
inline void atomic_increment_acquire(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_decrement_acquire(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_increment_acquire(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_decrement_acquire(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_add_acquire(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
inline void atomic_sub_acquire(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
inline void atomic_add_acquire(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
inline void atomic_sub_acquire(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
inline int32 atomic_compare_exchange_weak_acquire(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
inline int32 atomic_fetch_add_acquire(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline int32 atomic_fetch_sub_acquire(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline int64 atomic_fetch_add_acquire(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline int64 atomic_fetch_sub_acquire(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline void atomic_set_acquire(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
inline void atomic_set_acquire(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
inline uint32 atomic_fetch_set_acquire(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
inline uint64 atomic_fetch_set_acquire(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
inline uint32 atomic_get_acquire(volatile uint32* value) { return __atomic_load_n((uint32 *) value, __ATOMIC_ACQUIRE); }
inline uint64 atomic_get_acquire(volatile uint64* value) { return __atomic_load_n((uint64 *) value, __ATOMIC_ACQUIRE); }
inline void atomic_increment_acquire(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_decrement_acquire(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_increment_acquire(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_decrement_acquire(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
inline void atomic_add_acquire(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
inline void atomic_sub_acquire(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
inline uint32 atomic_compare_exchange_weak_acquire(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
inline uint32 atomic_fetch_add_acquire(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline uint32 atomic_fetch_sub_acquire(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline uint64 atomic_fetch_add_acquire(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline uint64 atomic_fetch_sub_acquire(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
inline void atomic_and_acquire(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_and_acquire(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_and_acquire(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_and_acquire(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_or_acquire(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_or_acquire(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_or_acquire(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
inline void atomic_or_acquire(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }

inline void atomic_set_release(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_RELEASE); }
inline void* atomic_get_release(void** target) { return __atomic_load_n(target, __ATOMIC_RELEASE); }
inline void atomic_set_release(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
inline void atomic_set_release(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
inline int32 atomic_fetch_set_release(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
inline int64 atomic_fetch_set_release(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
inline int32 atomic_get_release(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_RELEASE); }
inline int64 atomic_get_release(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_RELEASE); }
inline void atomic_increment_release(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_decrement_release(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_increment_release(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_decrement_release(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_add_release(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
inline void atomic_sub_release(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
inline void atomic_add_release(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
inline void atomic_sub_release(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
inline int32 atomic_compare_exchange_weak_release(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
inline int32 atomic_fetch_add_release(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
inline int32 atomic_fetch_sub_release(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
inline int64 atomic_fetch_add_release(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
inline int64 atomic_fetch_sub_release(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
inline void atomic_set_release(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
inline void atomic_set_release(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
inline uint32 atomic_fetch_set_release(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
inline uint64 atomic_fetch_set_release(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
inline uint32 atomic_get_release(volatile uint32* value) { return __atomic_load_n((uint32 *) value, __ATOMIC_RELEASE); }
inline uint64 atomic_get_release(volatile uint64* value) { return __atomic_load_n((uint64 *) value, __ATOMIC_RELEASE); }
inline void atomic_increment_release(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_decrement_release(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_increment_release(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_decrement_release(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
inline void atomic_add_release(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
inline void atomic_sub_release(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
inline uint32 atomic_compare_exchange_weak_release(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
inline uint32 atomic_fetch_add_release(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
inline uint32 atomic_fetch_sub_release(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
inline uint64 atomic_fetch_add_release(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
inline uint64 atomic_fetch_sub_release(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
inline void atomic_and_release(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
inline void atomic_and_release(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
inline void atomic_and_release(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
inline void atomic_and_release(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
inline void atomic_or_release(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
inline void atomic_or_release(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
inline void atomic_or_release(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
inline void atomic_or_release(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }

inline void atomic_set_acquire_release(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_SEQ_CST); }
inline void* atomic_get_acquire_release(void** target) { return __atomic_load_n(target, __ATOMIC_SEQ_CST); }
inline void atomic_set_acquire_release(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
inline void atomic_set_acquire_release(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
inline int32 atomic_fetch_set_acquire_release(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
inline int64 atomic_fetch_set_acquire_release(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
inline int32 atomic_get_acquire_release(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_SEQ_CST); }
inline int64 atomic_get_acquire_release(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_SEQ_CST); }
inline void atomic_increment_acquire_release(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_decrement_acquire_release(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_increment_acquire_release(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_decrement_acquire_release(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_add_acquire_release(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
inline void atomic_sub_acquire_release(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
inline void atomic_add_acquire_release(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
inline void atomic_sub_acquire_release(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
inline int32 atomic_compare_exchange_weak_acquire_release(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
inline int32 atomic_fetch_add_acquire_release(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline int32 atomic_fetch_sub_acquire_release(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline int64 atomic_fetch_add_acquire_release(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline int64 atomic_fetch_sub_acquire_release(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline void atomic_set_acquire_release(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
inline void atomic_set_acquire_release(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
inline uint32 atomic_fetch_set_acquire_release(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
inline uint64 atomic_fetch_set_acquire_release(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
inline uint32 atomic_get_acquire_release(volatile uint32* value) { return __atomic_load_n((uint32 *) value, __ATOMIC_SEQ_CST); }
inline uint64 atomic_get_acquire_release(volatile uint64* value) { return __atomic_load_n((uint64 *) value, __ATOMIC_SEQ_CST); }
inline void atomic_increment_acquire_release(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_decrement_acquire_release(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_increment_acquire_release(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_decrement_acquire_release(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
inline void atomic_add_acquire_release(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
inline void atomic_sub_acquire_release(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
inline uint32 atomic_compare_exchange_weak_acquire_release(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
inline uint32 atomic_fetch_add_acquire_release(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline uint32 atomic_fetch_sub_acquire_release(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline uint64 atomic_fetch_add_acquire_release(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline uint64 atomic_fetch_sub_acquire_release(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
inline void atomic_and_acquire_release(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_and_acquire_release(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_and_acquire_release(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_and_acquire_release(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_or_acquire_release(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_or_acquire_release(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_or_acquire_release(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
inline void atomic_or_acquire_release(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_acquire() __atomic_thread_fence(__ATOMIC_ACQUIRE)

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_release() __atomic_thread_fence(__ATOMIC_RELEASE)


#endif