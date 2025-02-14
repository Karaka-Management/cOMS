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
#include "CompilerUtils.h"
#include <stdatomic.h>

FORCE_INLINE void atomic_set_relaxed(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_RELAXED); }
FORCE_INLINE void* atomic_get_relaxed(void** target) { return __atomic_load_n(target, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile int8* value, int8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile int16* value, int16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE int8 atomic_fetch_set_relaxed(volatile int8* value, int8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE int16 atomic_fetch_set_relaxed(volatile int16* value, int16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE int8 atomic_get_relaxed(volatile int8* value) { return __atomic_load_n((int8 *) value, __ATOMIC_RELAXED); }
FORCE_INLINE int16 atomic_get_relaxed(volatile int16* value) { return __atomic_load_n((int16 *) value, __ATOMIC_RELAXED); }
FORCE_INLINE int32 atomic_get_relaxed(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_RELAXED); }
FORCE_INLINE int64 atomic_get_relaxed(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile int8* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile int16* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile int8* value, int8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile int8* value, int8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile int16* value, int16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile int16* value, int16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE f32 atomic_compare_exchange_weak_relaxed(volatile f32* value, f32* expected, f32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE f64 atomic_compare_exchange_weak_relaxed(volatile f64* value, f64* expected, f64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE int64 atomic_compare_exchange_weak_relaxed(volatile int64* value, int64* expected, int64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE int8 atomic_fetch_add_relaxed(volatile int8* value, int8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int8 atomic_fetch_sub_relaxed(volatile int8* value, int8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int16 atomic_fetch_add_relaxed(volatile int16* value, int16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int16 atomic_fetch_sub_relaxed(volatile int16* value, int16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile uint8* value, uint8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile uint16* value, uint16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE uint8 atomic_fetch_set_relaxed(volatile uint8* value, uint8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE uint16 atomic_fetch_set_relaxed(volatile uint16* value, uint16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELAXED); }
FORCE_INLINE uint8 atomic_get_relaxed(volatile uint8* value) { return __atomic_load_n(value, __ATOMIC_RELAXED); }
FORCE_INLINE uint16 atomic_get_relaxed(volatile uint16* value) { return __atomic_load_n(value, __ATOMIC_RELAXED); }
FORCE_INLINE uint32 atomic_get_relaxed(volatile uint32* value) { return __atomic_load_n(value, __ATOMIC_RELAXED); }
FORCE_INLINE uint64 atomic_get_relaxed(volatile uint64* value) { return __atomic_load_n(value, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint8* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint16* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile uint8* value, uint8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint8* value, uint8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile uint16* value, uint16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint16* value, uint16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_add_relaxed(volatile uint64* value, uint64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint64* value, uint64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELAXED); }
FORCE_INLINE uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE uint64 atomic_compare_exchange_weak_relaxed(volatile uint64* value, uint64* expected, uint64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); return *expected; }
FORCE_INLINE uint8 atomic_fetch_add_relaxed(volatile uint8* value, uint8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint8 atomic_fetch_sub_relaxed(volatile uint8* value, uint8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint16 atomic_fetch_add_relaxed(volatile uint16* value, uint16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint16 atomic_fetch_sub_relaxed(volatile uint16* value, uint16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_and_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }
FORCE_INLINE void atomic_or_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELAXED); }

FORCE_INLINE void atomic_set_relaxed(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void* atomic_get_relaxed(void** target) { return __atomic_load_n(target, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile int8* value, int8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile int16* value, int16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int8 atomic_fetch_set_relaxed(volatile int8* value, int8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int16 atomic_fetch_set_relaxed(volatile int16* value, int16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int8 atomic_get_relaxed(volatile int8* value) { return __atomic_load_n((int8 *) value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int16 atomic_get_relaxed(volatile int16* value) { return __atomic_load_n((int16 *) value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int32 atomic_get_relaxed(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_ACQUIRE); }
FORCE_INLINE int64 atomic_get_relaxed(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int8* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int16* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile int8* value, int8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int8* value, int8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile int16* value, int16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int16* value, int16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE f32 atomic_compare_exchange_weak_relaxed(volatile f32* value, f32* expected, f32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE f64 atomic_compare_exchange_weak_relaxed(volatile f64* value, f64* expected, f64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE int64 atomic_compare_exchange_weak_relaxed(volatile int64* value, int64* expected, int64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE int8 atomic_fetch_add_relaxed(volatile int8* value, int8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int8 atomic_fetch_sub_relaxed(volatile int8* value, int8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int16 atomic_fetch_add_relaxed(volatile int16* value, int16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int16 atomic_fetch_sub_relaxed(volatile int16* value, int16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint8* value, uint8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint16* value, uint16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint8 atomic_fetch_set_relaxed(volatile uint8* value, uint8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint16 atomic_fetch_set_relaxed(volatile uint16* value, uint16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint8 atomic_get_relaxed(volatile uint8* value) { return __atomic_load_n(value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint16 atomic_get_relaxed(volatile uint16* value) { return __atomic_load_n(value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint32 atomic_get_relaxed(volatile uint32* value) { return __atomic_load_n(value, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint64 atomic_get_relaxed(volatile uint64* value) { return __atomic_load_n(value, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint8* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint16* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint8* value, uint8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint8* value, uint8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint16* value, uint16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint16* value, uint16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint64* value, uint64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint64* value, uint64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE uint64 atomic_compare_exchange_weak_relaxed(volatile uint64* value, uint64* expected, uint64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); return *expected; }
FORCE_INLINE uint8 atomic_fetch_add_relaxed(volatile uint8* value, uint8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint8 atomic_fetch_sub_relaxed(volatile uint8* value, uint8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint16 atomic_fetch_add_relaxed(volatile uint16* value, uint16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint16 atomic_fetch_sub_relaxed(volatile uint16* value, uint16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_and_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }
FORCE_INLINE void atomic_or_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_ACQUIRE); }

FORCE_INLINE void atomic_set_relaxed(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_RELEASE); }
FORCE_INLINE void* atomic_get_relaxed(void** target) { return __atomic_load_n(target, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile int8* value, int8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile int16* value, int16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE int8 atomic_fetch_set_relaxed(volatile int8* value, int8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE int16 atomic_fetch_set_relaxed(volatile int16* value, int16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE int8 atomic_get_relaxed(volatile int8* value) { return __atomic_load_n((int8 *) value, __ATOMIC_RELEASE); }
FORCE_INLINE int16 atomic_get_relaxed(volatile int16* value) { return __atomic_load_n((int16 *) value, __ATOMIC_RELEASE); }
FORCE_INLINE int32 atomic_get_relaxed(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_RELEASE); }
FORCE_INLINE int64 atomic_get_relaxed(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int8* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int16* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile int8* value, int8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int8* value, int8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile int16* value, int16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int16* value, int16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE f32 atomic_compare_exchange_weak_relaxed(volatile f32* value, f32* expected, f32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE f64 atomic_compare_exchange_weak_relaxed(volatile f64* value, f64* expected, f64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE int64 atomic_compare_exchange_weak_relaxed(volatile int64* value, int64* expected, int64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE int8 atomic_fetch_add_relaxed(volatile int8* value, int8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int8 atomic_fetch_sub_relaxed(volatile int8* value, int8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int16 atomic_fetch_add_relaxed(volatile int16* value, int16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int16 atomic_fetch_sub_relaxed(volatile int16* value, int16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint8* value, uint8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint16* value, uint16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE uint8 atomic_fetch_set_relaxed(volatile uint8* value, uint8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE uint16 atomic_fetch_set_relaxed(volatile uint16* value, uint16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_RELEASE); }
FORCE_INLINE uint8 atomic_get_relaxed(volatile uint8* value) { return __atomic_load_n(value, __ATOMIC_RELEASE); }
FORCE_INLINE uint16 atomic_get_relaxed(volatile uint16* value) { return __atomic_load_n(value, __ATOMIC_RELEASE); }
FORCE_INLINE uint32 atomic_get_relaxed(volatile uint32* value) { return __atomic_load_n(value, __ATOMIC_RELEASE); }
FORCE_INLINE uint64 atomic_get_relaxed(volatile uint64* value) { return __atomic_load_n(value, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint8* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint16* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint8* value, uint8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint8* value, uint8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint16* value, uint16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint16* value, uint16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_add_relaxed(volatile uint64* value, uint64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint64* value, uint64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_RELEASE); }
FORCE_INLINE uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE uint64 atomic_compare_exchange_weak_relaxed(volatile uint64* value, uint64* expected, uint64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_RELEASE, __ATOMIC_RELEASE); return *expected; }
FORCE_INLINE uint8 atomic_fetch_add_relaxed(volatile uint8* value, uint8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint8 atomic_fetch_sub_relaxed(volatile uint8* value, uint8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint16 atomic_fetch_add_relaxed(volatile uint16* value, uint16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint16 atomic_fetch_sub_relaxed(volatile uint16* value, uint16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_and_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }
FORCE_INLINE void atomic_or_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_RELEASE); }

FORCE_INLINE void atomic_set_relaxed(void** target, void* value) { __atomic_store_n(target, value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void* atomic_get_relaxed(void** target) { return __atomic_load_n(target, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile int8* value, int8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile int16* value, int16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile int32* value, int32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile int64* value, int64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int8 atomic_fetch_set_relaxed(volatile int8* value, int8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int16 atomic_fetch_set_relaxed(volatile int16* value, int16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int8 atomic_get_relaxed(volatile int8* value) { return __atomic_load_n((int8 *) value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int16 atomic_get_relaxed(volatile int16* value) { return __atomic_load_n((int16 *) value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int32 atomic_get_relaxed(volatile int32* value) { return __atomic_load_n((int32 *) value, __ATOMIC_SEQ_CST); }
FORCE_INLINE int64 atomic_get_relaxed(volatile int64* value) { return __atomic_load_n((int64 *) value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile int8* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile int16* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile int32* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile int64* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile int64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile int8* value, int8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile int8* value, int8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile int16* value, int16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile int16* value, int16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile int32* value, int32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile int32* value, int32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile int64* value, int64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile int64* value, int64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE f32 atomic_compare_exchange_weak_relaxed(volatile f32* value, f32* expected, f32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE f64 atomic_compare_exchange_weak_relaxed(volatile f64* value, f64* expected, f64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE int64 atomic_compare_exchange_weak_relaxed(volatile int64* value, int64* expected, int64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE int8 atomic_fetch_add_relaxed(volatile int8* value, int8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int8 atomic_fetch_sub_relaxed(volatile int8* value, int8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int16 atomic_fetch_add_relaxed(volatile int16* value, int16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int16 atomic_fetch_sub_relaxed(volatile int16* value, int16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile uint8* value, uint8 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile uint16* value, uint16 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint8 atomic_fetch_set_relaxed(volatile uint8* value, uint8 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint16 atomic_fetch_set_relaxed(volatile uint16* value, uint16 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint8 atomic_get_relaxed(volatile uint8* value) { return __atomic_load_n(value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint16 atomic_get_relaxed(volatile uint16* value) { return __atomic_load_n(value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint32 atomic_get_relaxed(volatile uint32* value) { return __atomic_load_n(value, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint64 atomic_get_relaxed(volatile uint64* value) { return __atomic_load_n(value, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint8* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint8* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint16* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint16* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint32* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint32* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_increment_relaxed(volatile uint64* value) { __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_decrement_relaxed(volatile uint64* value) { __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile uint8* value, uint8 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint8* value, uint8 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile uint16* value, uint16 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint16* value, uint16 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile uint32* value, uint32 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_add_relaxed(volatile uint64* value, uint64 increment) { __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_sub_relaxed(volatile uint64* value, uint64 decrement) { __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE uint64 atomic_compare_exchange_weak_relaxed(volatile uint64* value, uint64* expected, uint64 desired) { __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); return *expected; }
FORCE_INLINE uint8 atomic_fetch_add_relaxed(volatile uint8* value, uint8 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint8 atomic_fetch_sub_relaxed(volatile uint8* value, uint8 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint16 atomic_fetch_add_relaxed(volatile uint16* value, uint16 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint16 atomic_fetch_sub_relaxed(volatile uint16* value, uint16 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_and_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile uint8* value, uint8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile int8* value, int8 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile uint16* value, uint16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile int16* value, int16 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile uint32* value, uint32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile int32* value, int32 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile uint64* value, uint64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }
FORCE_INLINE void atomic_or_relaxed(volatile int64* value, int64 mask) { __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST); }

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_acquire() __atomic_thread_fence(__ATOMIC_ACQUIRE)

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_release() __atomic_thread_fence(__ATOMIC_RELEASE)


#endif