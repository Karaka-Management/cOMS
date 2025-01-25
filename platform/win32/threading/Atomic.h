/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_ATOMIC_H
#define TOS_PLATFORM_WIN32_THREADING_ATOMIC_H

#include "../../../stdlib/Types.h"
#include <windows.h>

// WARNING: Windows doesn't really have relaxed, release, acquire function on x86_64.
// You can see that by checking out how they are defined

// We need the following helper types to "cast" between float and long.
// We can't just perform a "normal" cast since that re-interprets the bits. We need to maintain the bits
typedef union { f32 f; LONG l; } _atomic_32;
typedef union { f64 f; LONG64 l; } _atomic_64;

inline void atomic_set_relaxed(void** target, void* new_pointer) { InterlockedExchangePointerNoFence(target, new_pointer); }
inline void* atomic_get_relaxed(void** target) { return InterlockedCompareExchangePointerNoFence(target, NULL, NULL); }
inline void atomic_set_relaxed(volatile int32* value, int32 new_value) { InterlockedExchangeNoFence((long *) value, new_value); }
inline void atomic_set_relaxed(volatile int64* value, int64 new_value) { InterlockedExchangeNoFence64((LONG64 *) value, (LONG64) new_value); }
inline void atomic_set_relaxed(volatile f32* value, f32 new_value) { _atomic_32 temp = {.f = new_value}; InterlockedExchangeNoFence((long *) value, (long) temp.l); }
inline void atomic_set_relaxed(volatile f64* value, f64 new_value) { _atomic_64 temp = {.f = new_value}; InterlockedExchangeNoFence64((LONG64 *) value, (LONG64) temp.l); }
inline int32 atomic_fetch_set_relaxed(volatile int32* value, int32 new_value) { return (int32) InterlockedExchangeNoFence((long *) value, new_value); }
inline int64 atomic_fetch_set_relaxed(volatile int64* value, int64 new_value) { return (int64) InterlockedExchangeNoFence64((LONG64 *) value, (LONG64) new_value); }
inline int32 atomic_get_relaxed(volatile int32* value) { return (int32) InterlockedCompareExchangeNoFence((long *) value, 0, 0); }
inline int64 atomic_get_relaxed(volatile int64* value) { return (int64) InterlockedCompareExchangeNoFence64((LONG64 *) value, 0, 0); }
inline f32 atomic_get_relaxed(volatile f32* value) { _atomic_32 temp = {.l = InterlockedCompareExchangeNoFence((long *) value, 0, 0)}; return temp.f; }
inline f64 atomic_get_relaxed(volatile f64* value) { _atomic_64 temp = {.l = InterlockedCompareExchangeNoFence64((LONG64 *) value, 0, 0)}; return temp.f; }
inline void atomic_increment_relaxed(volatile int32* value) { InterlockedIncrementNoFence((long *) value); }
inline void atomic_decrement_relaxed(volatile int32* value) { InterlockedDecrementNoFence((long *) value); }
inline void atomic_increment_relaxed(volatile int64* value) { InterlockedIncrementNoFence64((LONG64 *) value); }
inline void atomic_decrement_relaxed(volatile int64* value) { InterlockedDecrementNoFence64((LONG64 *) value); }
inline void atomic_add_relaxed(volatile int32* value, int32 increment) { InterlockedAddNoFence((long *) value, increment); }
inline void atomic_sub_relaxed(volatile int32* value, int32 decrement) { InterlockedAddNoFence((long *) value, -decrement); }
inline void atomic_add_relaxed(volatile int64* value, int64 increment) { InterlockedAddNoFence64((LONG64 *) value, (LONG64) increment); }
inline void atomic_sub_relaxed(volatile int64* value, int64 decrement) { InterlockedAddNoFence64((LONG64 *) value, -((LONG64) decrement)); }
inline f32 atomic_compare_exchange_weak_relaxed(volatile f32* value, f32* expected, f32 desired) { _atomic_32 temp = {.l = InterlockedCompareExchangeNoFence((long *) value, (long) desired, (long) *expected) }; return temp.f; }
inline f64 atomic_compare_exchange_weak_relaxed(volatile f64* value, f64* expected, f64 desired) { _atomic_64 temp = {.l = InterlockedCompareExchangeNoFence64((LONG64 *) value, (LONG64) desired, (LONG64) *expected) }; return temp.f; }
inline int32 atomic_compare_exchange_weak_relaxed(volatile int32* value, int32* expected, int32 desired) { return (int32) InterlockedCompareExchangeNoFence((long *) value, desired, *expected); }
inline int64 atomic_compare_exchange_weak_relaxed(volatile int64* value, int64* expected, int64 desired) { return (int64) InterlockedCompareExchangeNoFence64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline int32 atomic_fetch_add_relaxed(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddNoFence((long *) value, operand); }
inline int32 atomic_fetch_sub_relaxed(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddNoFence((unsigned long *) value, -((long) operand)); }
inline int64 atomic_fetch_add_relaxed(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAddNoFence64((LONG64 *) value, (LONG64) operand); }
inline int64 atomic_fetch_sub_relaxed(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_set_relaxed(volatile uint32* value, uint32 new_value) { InterlockedExchangeNoFence((long *) value, new_value); }
inline void atomic_set_relaxed(volatile uint64* value, uint64 new_value) { InterlockedExchangeNoFence64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_fetch_set_relaxed(volatile uint32* value, uint32 new_value) { return (uint32) InterlockedExchangeNoFence((long *) value, new_value); }
inline uint64 atomic_fetch_set_relaxed(volatile uint64* value, uint64 new_value) { return (uint64) InterlockedExchangeNoFence64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_get_relaxed(volatile uint32* value) { return (uint32) InterlockedCompareExchangeNoFence((long *) value, 0, 0); }
inline uint64 atomic_get_relaxed(volatile uint64* value) { return (uint64) InterlockedCompareExchangeNoFence64((LONG64 *) value, 0, 0); }
inline void atomic_increment_relaxed(volatile uint32* value) { InterlockedIncrementNoFence((long *) value); }
inline void atomic_decrement_relaxed(volatile uint32* value) { InterlockedDecrementNoFence((long *) value); }
inline void atomic_increment_relaxed(volatile uint64* value) { InterlockedIncrementNoFence64((LONG64 *) value); }
inline void atomic_decrement_relaxed(volatile uint64* value) { InterlockedDecrementNoFence64((LONG64 *) value); }
inline void atomic_add_relaxed(volatile uint32* value, uint32 increment) { InterlockedAddNoFence((long *) value, increment); }
inline void atomic_sub_relaxed(volatile uint32* value, uint32 decrement) { InterlockedAddNoFence((long *) value, -1 * ((int32) decrement)); }
inline void atomic_add_relaxed(volatile uint64* value, uint64 increment) { InterlockedAddNoFence64((LONG64 *) value, (LONG64) increment); }
inline  void atomic_sub_relaxed(volatile uint64* value, uint64 decrement) { InterlockedAddNoFence64((LONG64 *) value, -((LONG64) decrement)); }
inline uint32 atomic_compare_exchange_weak_relaxed(volatile uint32* value, uint32* expected, uint32 desired) { return (uint32) InterlockedCompareExchangeNoFence((long *) value, desired, *expected); }
inline uint64 atomic_compare_exchange_weak_relaxed(volatile uint64* value, uint64* expected, uint64 desired) { return (uint64) InterlockedCompareExchangeNoFence64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline uint32 atomic_fetch_add_relaxed(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddNoFence((long *) value, operand); }
inline uint32 atomic_fetch_sub_relaxed(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddNoFence((unsigned long *) value, -((long) operand)); }
inline uint64 atomic_fetch_add_relaxed(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAddNoFence64((LONG64 *) value, (LONG64) operand); }
inline uint64 atomic_fetch_sub_relaxed(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_and_relaxed(volatile uint32* value, uint32 mask) { InterlockedAndNoFence((volatile LONG *) value, mask); }
inline void atomic_and_relaxed(volatile int32* value, int32 mask) { InterlockedAndNoFence((volatile LONG *) value, (LONG)mask); }
inline void atomic_and_relaxed(volatile uint64* value, uint64 mask) { InterlockedAnd64NoFence((volatile LONG64 *) value, mask); }
inline void atomic_and_relaxed(volatile int64* value, int64 mask) { InterlockedAnd64NoFence((volatile LONG64 *) value, mask); }
inline void atomic_or_relaxed(volatile uint32* value, uint32 mask) { InterlockedOrNoFence((volatile LONG *) value, mask); }
inline void atomic_or_relaxed(volatile int32* value, int32 mask) { InterlockedOrNoFence((volatile LONG *) value, (LONG)mask); }
inline void atomic_or_relaxed(volatile uint64* value, uint64 mask) { InterlockedOr64NoFence((volatile LONG64 *) value, mask); }
inline void atomic_or_relaxed(volatile int64* value, int64 mask) { InterlockedOr64NoFence((volatile LONG64 *) value, mask); }

inline void atomic_set_acquire(void** target, void* new_pointer) { InterlockedExchangePointerAcquire(target, new_pointer); }
inline void* atomic_get_acquire(void** target) { return InterlockedCompareExchangePointerAcquire(target, NULL, NULL); }
inline void atomic_set_acquire(volatile int32* value, int32 new_value) { InterlockedExchangeAcquire((long *) value, new_value); }
inline void atomic_set_acquire(volatile int64* value, int64 new_value) { InterlockedExchangeAcquire64((LONG64 *) value, (LONG64) new_value); }
inline void atomic_set_acquire(volatile f32* value, f32 new_value) { _atomic_32 temp = {.f = new_value}; InterlockedExchangeAcquire((long *) value, (long) temp.l); }
inline void atomic_set_acquire(volatile f64* value, f64 new_value) { _atomic_64 temp = {.f = new_value}; InterlockedExchangeAcquire64((LONG64 *) value, (LONG64) temp.l); }
inline int32 atomic_fetch_set_acquire(volatile int32* value, int32 new_value) { return (int32) InterlockedExchangeAcquire((long *) value, new_value); }
inline int64 atomic_fetch_set_acquire(volatile int64* value, int64 new_value) { return (int64) InterlockedExchangeAcquire64((LONG64 *) value, (LONG64) new_value); }
inline int32 atomic_get_acquire(volatile int32* value) { return (int32) InterlockedCompareExchangeAcquire((long *) value, 0, 0); }
inline int64 atomic_get_acquire(volatile int64* value) { return (int64) InterlockedCompareExchangeAcquire64((LONG64 *) value, 0, 0); }
inline f32 atomic_get_acquire(volatile f32* value) { _atomic_32 temp = {.l = InterlockedCompareExchangeAcquire((long *) value, 0, 0)}; return temp.f; }
inline f64 atomic_get_acquire(volatile f64* value) { _atomic_64 temp = {.l = InterlockedCompareExchangeAcquire64((LONG64 *) value, 0, 0)}; return temp.f; }
inline void atomic_increment_acquire(volatile int32* value) { InterlockedIncrementAcquire((long *) value); }
inline void atomic_decrement_acquire(volatile int32* value) { InterlockedDecrementAcquire((long *) value); }
inline void atomic_increment_acquire(volatile int64* value) { InterlockedIncrementAcquire64((LONG64 *) value); }
inline void atomic_decrement_acquire(volatile int64* value) { InterlockedDecrementAcquire64((LONG64 *) value); }
inline void atomic_add_acquire(volatile int32* value, int32 increment) { InterlockedAddAcquire((long *) value, increment); }
inline void atomic_sub_acquire(volatile int32* value, int32 decrement) { InterlockedAddAcquire((long *) value, -decrement); }
inline void atomic_add_acquire(volatile int64* value, int64 increment) { InterlockedAddAcquire64((LONG64 *) value, (LONG64) increment); }
inline void atomic_sub_acquire(volatile int64* value, int64 decrement) { InterlockedAddAcquire64((LONG64 *) value, -((LONG64) decrement)); }
inline f32 atomic_compare_exchange_weak_acquire(volatile f32* value, f32* expected, f32 desired) { _atomic_32 temp = {.l = InterlockedCompareExchangeAcquire((long *) value, (long) desired, (long) *expected) }; return temp.f; }
inline f64 atomic_compare_exchange_weak_acquire(volatile f64* value, f64* expected, f64 desired) { _atomic_64 temp = {.l = InterlockedCompareExchangeAcquire64((LONG64 *) value, (LONG64) desired, (LONG64) *expected) }; return temp.f; }
inline int32 atomic_compare_exchange_weak_acquire(volatile int32* value, int32* expected, int32 desired) { return (int32) InterlockedCompareExchangeAcquire((long *) value, desired, *expected); }
inline int64 atomic_compare_exchange_weak_acquire(volatile int64* value, int64* expected, int64 desired) { return (int64) InterlockedCompareExchangeAcquire64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline int32 atomic_fetch_add_acquire(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddAcquire((long *) value, operand); }
inline int32 atomic_fetch_sub_acquire(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddAcquire((unsigned long *) value, -((long) operand)); }
inline int64 atomic_fetch_add_acquire(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAddAcquire64((LONG64 *) value, (LONG64) operand); }
inline int64 atomic_fetch_sub_acquire(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_set_acquire(volatile uint32* value, uint32 new_value) { InterlockedExchangeAcquire((long *) value, new_value); }
inline void atomic_set_acquire(volatile uint64* value, uint64 new_value) { InterlockedExchangeAcquire64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_fetch_set_acquire(volatile uint32* value, uint32 new_value) { return (uint32) InterlockedExchangeAcquire((long *) value, new_value); }
inline uint64 atomic_fetch_set_acquire(volatile uint64* value, uint64 new_value) { return (uint64) InterlockedExchangeAcquire64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_get_acquire(volatile uint32* value) { return (uint32) InterlockedCompareExchangeAcquire((long *) value, 0, 0); }
inline uint64 atomic_get_acquire(volatile uint64* value) { return (uint64) InterlockedCompareExchangeAcquire64((LONG64 *) value, 0, 0); }
inline void atomic_increment_acquire(volatile uint32* value) { InterlockedIncrementAcquire((long *) value); }
inline void atomic_decrement_acquire(volatile uint32* value) { InterlockedDecrementAcquire((long *) value); }
inline void atomic_increment_acquire(volatile uint64* value) { InterlockedIncrementAcquire64((LONG64 *) value); }
inline void atomic_decrement_acquire(volatile uint64* value) { InterlockedDecrementAcquire64((LONG64 *) value); }
inline void atomic_add_acquire(volatile uint32* value, uint32 increment) { InterlockedAddAcquire((long *) value, increment); }
inline void atomic_sub_acquire(volatile uint32* value, uint32 decrement) { InterlockedAddAcquire((long *) value, -1 * ((int32) decrement)); }
inline void atomic_add_acquire(volatile uint64* value, uint64 increment) { InterlockedAddAcquire64((LONG64 *) value, (LONG64) increment); }
inline  void atomic_sub_acquire(volatile uint64* value, uint64 decrement) { InterlockedAddAcquire64((LONG64 *) value, -((LONG64) decrement)); }
inline uint32 atomic_compare_exchange_weak_acquire(volatile uint32* value, uint32* expected, uint32 desired) { return (uint32) InterlockedCompareExchangeAcquire((long *) value, desired, *expected); }
inline uint64 atomic_compare_exchange_weak_acquire(volatile uint64* value, uint64* expected, uint64 desired) { return (uint64) InterlockedCompareExchangeAcquire64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline uint32 atomic_fetch_add_acquire(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddAcquire((long *) value, operand); }
inline uint32 atomic_fetch_sub_acquire(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddAcquire((unsigned long *) value, -((long) operand)); }
inline uint64 atomic_fetch_add_acquire(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAddAcquire64((LONG64 *) value, (LONG64) operand); }
inline uint64 atomic_fetch_sub_acquire(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_and_acquire(volatile uint32* value, uint32 mask) { InterlockedAndAcquire((volatile LONG *) value, mask); }
inline void atomic_and_acquire(volatile int32* value, int32 mask) { InterlockedAndAcquire((volatile LONG *) value, (LONG)mask); }
inline void atomic_and_acquire(volatile uint64* value, uint64 mask) { InterlockedAnd64Acquire((volatile LONG64 *) value, mask); }
inline void atomic_and_acquire(volatile int64* value, int64 mask) { InterlockedAnd64Acquire((volatile LONG64 *) value, mask); }
inline void atomic_or_acquire(volatile uint32* value, uint32 mask) { InterlockedOrAcquire((volatile LONG *) value, mask); }
inline void atomic_or_acquire(volatile int32* value, int32 mask) { InterlockedOrAcquire((volatile LONG *) value, (LONG)mask); }
inline void atomic_or_acquire(volatile uint64* value, uint64 mask) { InterlockedOr64Acquire((volatile LONG64 *) value, mask); }
inline void atomic_or_acquire(volatile int64* value, int64 mask) { InterlockedOr64Acquire((volatile LONG64 *) value, mask); }

inline void atomic_set_release(void** target, void* new_pointer) { InterlockedExchangePointer(target, new_pointer); }
inline void* atomic_get_release(void** target) { return InterlockedCompareExchangePointerRelease(target, NULL, NULL); }
inline void atomic_set_release(volatile int32* value, int32 new_value) { InterlockedExchange((long *) value, new_value); }
inline void atomic_set_release(volatile int64* value, int64 new_value) { InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline void atomic_set_release(volatile f32* value, f32 new_value) { _atomic_32 temp = {.f = new_value}; InterlockedExchange((long *) value, (long) temp.l); }
inline void atomic_set_release(volatile f64* value, f64 new_value) { _atomic_64 temp = {.f = new_value}; InterlockedExchange64((LONG64 *) value, (LONG64) temp.l); }
inline int32 atomic_fetch_set_release(volatile int32* value, int32 new_value) { return (int32) InterlockedExchange((long *) value, new_value); }
inline int64 atomic_fetch_set_release(volatile int64* value, int64 new_value) { return (int64) InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline int32 atomic_get_release(volatile int32* value) { return (int32) InterlockedCompareExchangeRelease((long *) value, 0, 0); }
inline int64 atomic_get_release(volatile int64* value) { return (int64) InterlockedCompareExchangeRelease64((LONG64 *) value, 0, 0); }
inline f32 atomic_get_release(volatile f32* value) { _atomic_32 temp = {.l = InterlockedCompareExchangeRelease((long *) value, 0, 0)}; return temp.f; }
inline f64 atomic_get_release(volatile f64* value) { _atomic_64 temp = {.l = InterlockedCompareExchangeRelease64((LONG64 *) value, 0, 0)}; return temp.f; }
inline void atomic_increment_release(volatile int32* value) { InterlockedIncrementRelease((long *) value); }
inline void atomic_decrement_release(volatile int32* value) { InterlockedDecrementRelease((long *) value); }
inline void atomic_increment_release(volatile int64* value) { InterlockedIncrementRelease64((LONG64 *) value); }
inline void atomic_decrement_release(volatile int64* value) { InterlockedDecrementRelease64((LONG64 *) value); }
inline void atomic_add_release(volatile int32* value, int32 increment) { InterlockedAddRelease((long *) value, increment); }
inline void atomic_sub_release(volatile int32* value, int32 decrement) { InterlockedAddRelease((long *) value, -decrement); }
inline void atomic_add_release(volatile int64* value, int64 increment) { InterlockedAddRelease64((LONG64 *) value, (LONG64) increment); }
inline void atomic_sub_release(volatile int64* value, int64 decrement) { InterlockedAddRelease64((LONG64 *) value, -((LONG64) decrement)); }
inline f32 atomic_compare_exchange_weak_release(volatile f32* value, f32* expected, f32 desired) { _atomic_32 temp = {.l = InterlockedCompareExchangeRelease((long *) value, (long) desired, (long) *expected) }; return temp.f; }
inline f64 atomic_compare_exchange_weak_release(volatile f64* value, f64* expected, f64 desired) { _atomic_64 temp = {.l = InterlockedCompareExchangeRelease64((LONG64 *) value, (LONG64) desired, (LONG64) *expected) }; return temp.f; }
inline int32 atomic_compare_exchange_weak_release(volatile int32* value, int32* expected, int32 desired) { return (int32) InterlockedCompareExchangeRelease((long *) value, desired, *expected); }
inline int64 atomic_compare_exchange_weak_release(volatile int64* value, int64* expected, int64 desired) { return (int64) InterlockedCompareExchangeRelease64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline int32 atomic_fetch_add_release(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddRelease((long *) value, operand); }
inline int32 atomic_fetch_sub_release(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAddRelease((unsigned long *) value, -((long) operand)); }
inline int64 atomic_fetch_add_release(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAddRelease64((LONG64 *) value, (LONG64) operand); }
inline int64 atomic_fetch_sub_release(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_set_release(volatile uint32* value, uint32 new_value) { InterlockedExchange((long *) value, new_value); }
inline void atomic_set_release(volatile uint64* value, uint64 new_value) { InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_fetch_set_release(volatile uint32* value, uint32 new_value) { return (uint32) InterlockedExchange((long *) value, new_value); }
inline uint64 atomic_fetch_set_release(volatile uint64* value, uint64 new_value) { return (uint64) InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_get_release(volatile uint32* value) { return (uint32) InterlockedCompareExchangeRelease((long *) value, 0, 0); }
inline uint64 atomic_get_release(volatile uint64* value) { return (uint64) InterlockedCompareExchangeRelease64((LONG64 *) value, 0, 0); }
inline void atomic_increment_release(volatile uint32* value) { InterlockedIncrementRelease((long *) value); }
inline void atomic_decrement_release(volatile uint32* value) { InterlockedDecrementRelease((long *) value); }
inline void atomic_increment_release(volatile uint64* value) { InterlockedIncrementRelease64((LONG64 *) value); }
inline void atomic_decrement_release(volatile uint64* value) { InterlockedDecrementRelease64((LONG64 *) value); }
inline void atomic_add_release(volatile uint32* value, uint32 increment) { InterlockedAddRelease((long *) value, increment); }
inline void atomic_sub_release(volatile uint32* value, uint32 decrement) { InterlockedAddRelease((long *) value, -1 * ((int32) decrement)); }
inline void atomic_add_release(volatile uint64* value, uint64 increment) { InterlockedAddRelease64((LONG64 *) value, (LONG64) increment); }
inline  void atomic_sub_release(volatile uint64* value, uint64 decrement) { InterlockedAddRelease64((LONG64 *) value, -((LONG64) decrement)); }
inline uint32 atomic_compare_exchange_weak_release(volatile uint32* value, uint32* expected, uint32 desired) { return (uint32) InterlockedCompareExchangeRelease((long *) value, desired, *expected); }
inline uint64 atomic_compare_exchange_weak_release(volatile uint64* value, uint64* expected, uint64 desired) { return (uint64) InterlockedCompareExchangeRelease64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline uint32 atomic_fetch_add_release(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddRelease((long *) value, operand); }
inline uint32 atomic_fetch_sub_release(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAddRelease((unsigned long *) value, -((long) operand)); }
inline uint64 atomic_fetch_add_release(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAddRelease64((LONG64 *) value, (LONG64) operand); }
inline uint64 atomic_fetch_sub_release(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_and_release(volatile uint32* value, uint32 mask) { InterlockedAndRelease((volatile LONG *) value, mask); }
inline void atomic_and_release(volatile int32* value, int32 mask) { InterlockedAndRelease((volatile LONG *) value, (LONG)mask); }
inline void atomic_and_release(volatile uint64* value, uint64 mask) { InterlockedAnd64Release((volatile LONG64 *) value, mask); }
inline void atomic_and_release(volatile int64* value, int64 mask) { InterlockedAnd64Release((volatile LONG64 *) value, mask); }
inline void atomic_or_release(volatile uint32* value, uint32 mask) { InterlockedOrRelease((volatile LONG *) value, mask); }
inline void atomic_or_release(volatile int32* value, int32 mask) { InterlockedOrRelease((volatile LONG *) value, (LONG)mask); }
inline void atomic_or_release(volatile uint64* value, uint64 mask) { InterlockedOr64Release((volatile LONG64 *) value, mask); }
inline void atomic_or_release(volatile int64* value, int64 mask) { InterlockedOr64Release((volatile LONG64 *) value, mask); }

inline void atomic_set_acquire_release(void** target, void* new_pointer) { InterlockedExchangePointer(target, new_pointer); }
inline void* atomic_get_acquire_release(void** target) { return InterlockedCompareExchangePointer(target, NULL, NULL); }
inline void atomic_set_acquire_release(volatile int32* value, int32 new_value) { InterlockedExchange((long *) value, new_value); }
inline void atomic_set_acquire_release(volatile int64* value, int64 new_value) { InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline void atomic_set_acquire_release(volatile f32* value, f32 new_value) { _atomic_32 temp = {.f = new_value}; InterlockedExchange((long *) value, (long) temp.l); }
inline void atomic_set_acquire_release(volatile f64* value, f64 new_value) { _atomic_64 temp = {.f = new_value}; InterlockedExchange64((LONG64 *) value, (LONG64) temp.l); }
inline int32 atomic_fetch_set_acquire_release(volatile int32* value, int32 new_value) { return (int32) InterlockedExchange((long *) value, new_value); }
inline int64 atomic_fetch_set_acquire_release(volatile int64* value, int64 new_value) { return (int64) InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline int32 atomic_get_acquire_release(volatile int32* value) { return (int32) InterlockedCompareExchange((long *) value, 0, 0); }
inline int64 atomic_get_acquire_release(volatile int64* value) { return (int64) InterlockedCompareExchange64((LONG64 *) value, 0, 0); }
inline f32 atomic_get_acquire_release(volatile f32* value) { _atomic_32 temp = {.l = InterlockedCompareExchange((long *) value, 0, 0)}; return temp.f; }
inline f64 atomic_get_acquire_release(volatile f64* value) { _atomic_64 temp = {.l = InterlockedCompareExchange64((LONG64 *) value, 0, 0)}; return temp.f; }
inline void atomic_increment_acquire_release(volatile int32* value) { InterlockedIncrement((long *) value); }
inline void atomic_decrement_acquire_release(volatile int32* value) { InterlockedDecrement((long *) value); }
inline void atomic_increment_acquire_release(volatile int64* value) { InterlockedIncrement64((LONG64 *) value); }
inline void atomic_decrement_acquire_release(volatile int64* value) { InterlockedDecrement64((LONG64 *) value); }
inline void atomic_add_acquire_release(volatile int32* value, int32 increment) { InterlockedAdd((long *) value, increment); }
inline void atomic_sub_acquire_release(volatile int32* value, int32 decrement) { InterlockedAdd((long *) value, -decrement); }
inline void atomic_add_acquire_release(volatile int64* value, int64 increment) { InterlockedAdd64((LONG64 *) value, (LONG64) increment); }
inline void atomic_sub_acquire_release(volatile int64* value, int64 decrement) { InterlockedAdd64((LONG64 *) value, -((LONG64) decrement)); }
inline f32 atomic_compare_exchange_weak_acquire_release(volatile f32* value, f32* expected, f32 desired) { _atomic_32 temp = {.l = InterlockedCompareExchange((long *) value, (long) desired, (long) *expected) }; return temp.f; }
inline f64 atomic_compare_exchange_weak_acquire_release(volatile f64* value, f64* expected, f64 desired) { _atomic_64 temp = {.l = InterlockedCompareExchange64((LONG64 *) value, (LONG64) desired, (LONG64) *expected) }; return temp.f; }
inline int32 atomic_compare_exchange_weak_acquire_release(volatile int32* value, int32* expected, int32 desired) { return (int32) InterlockedCompareExchange((long *) value, desired, *expected); }
inline int64 atomic_compare_exchange_weak_acquire_release(volatile int64* value, int64* expected, int64 desired) { return (int64) InterlockedCompareExchange64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline int32 atomic_fetch_add_acquire_release(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAdd((long *) value, operand); }
inline int32 atomic_fetch_sub_acquire_release(volatile int32* value, int32 operand) { return (int32) InterlockedExchangeAdd((unsigned long *) value, -((long) operand)); }
inline int64 atomic_fetch_add_acquire_release(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAdd64((LONG64 *) value, (LONG64) operand); }
inline int64 atomic_fetch_sub_acquire_release(volatile int64* value, int64 operand) { return (int64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_set_acquire_release(volatile uint32* value, uint32 new_value) { InterlockedExchange((long *) value, new_value); }
inline void atomic_set_acquire_release(volatile uint64* value, uint64 new_value) { InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_fetch_set_acquire_release(volatile uint32* value, uint32 new_value) { return (uint32) InterlockedExchange((long *) value, new_value); }
inline uint64 atomic_fetch_set_acquire_release(volatile uint64* value, uint64 new_value) { return (uint64) InterlockedExchange64((LONG64 *) value, (LONG64) new_value); }
inline uint32 atomic_get_acquire_release(volatile uint32* value) { return (uint32) InterlockedCompareExchange((long *) value, 0, 0); }
inline uint64 atomic_get_acquire_release(volatile uint64* value) { return (uint64) InterlockedCompareExchange64((LONG64 *) value, 0, 0); }
inline void atomic_increment_acquire_release(volatile uint32* value) { InterlockedIncrement((long *) value); }
inline void atomic_decrement_acquire_release(volatile uint32* value) { InterlockedDecrement((long *) value); }
inline void atomic_increment_acquire_release(volatile uint64* value) { InterlockedIncrement64((LONG64 *) value); }
inline void atomic_decrement_acquire_release(volatile uint64* value) { InterlockedDecrement64((LONG64 *) value); }
inline void atomic_add_acquire_release(volatile uint32* value, uint32 increment) { InterlockedAdd((long *) value, increment); }
inline void atomic_sub_acquire_release(volatile uint32* value, uint32 decrement) { InterlockedAdd((long *) value, -1 * ((int32) decrement)); }
inline void atomic_add_acquire_release(volatile uint64* value, uint64 increment) { InterlockedAdd64((LONG64 *) value, (LONG64) increment); }
inline  void atomic_sub_acquire_release(volatile uint64* value, uint64 decrement) { InterlockedAdd64((LONG64 *) value, -((LONG64) decrement)); }
inline uint32 atomic_compare_exchange_weak_acquire_release(volatile uint32* value, uint32* expected, uint32 desired) { return (uint32) InterlockedCompareExchange((long *) value, desired, *expected); }
inline uint64 atomic_compare_exchange_weak_acquire_release(volatile uint64* value, uint64* expected, uint64 desired) { return (uint64) InterlockedCompareExchange64((LONG64 *) value, (LONG64) desired, (LONG64) *expected); }
inline uint32 atomic_fetch_add_acquire_release(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAdd((long *) value, operand); }
inline uint32 atomic_fetch_sub_acquire_release(volatile uint32* value, uint32 operand) { return (uint32) InterlockedExchangeAdd((unsigned long *) value, -((long) operand)); }
inline uint64 atomic_fetch_add_acquire_release(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAdd64((LONG64 *) value, (LONG64) operand); }
inline uint64 atomic_fetch_sub_acquire_release(volatile uint64* value, uint64 operand) { return (uint64) InterlockedExchangeAdd64((LONG64 *) value, -((LONG64) operand)); }
inline void atomic_and_acquire_release(volatile uint32* value, uint32 mask) { InterlockedAnd((volatile LONG *) value, mask); }
inline void atomic_and_acquire_release(volatile int32* value, int32 mask) { InterlockedAnd((volatile LONG *) value, (LONG)mask); }
inline void atomic_and_acquire_release(volatile uint64* value, uint64 mask) { InterlockedAnd64((volatile LONG64 *) value, mask); }
inline void atomic_and_acquire_release(volatile int64* value, int64 mask) { InterlockedAnd64((volatile LONG64 *) value, mask); }
inline void atomic_or_acquire_release(volatile uint32* value, uint32 mask) { InterlockedOr((volatile LONG *) value, mask); }
inline void atomic_or_acquire_release(volatile int32* value, int32 mask) { InterlockedOr((volatile LONG *) value, (LONG)mask); }
inline void atomic_or_acquire_release(volatile uint64* value, uint64 mask) { InterlockedOr64((volatile LONG64 *) value, mask); }
inline void atomic_or_acquire_release(volatile int64* value, int64 mask) { InterlockedOr64((volatile LONG64 *) value, mask); }

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_acquire() MemoryBarrier();

// Check out the intrinsic functions fence_memory and fence_write
// These are much faster and could accomplish what you are doing
#define atomic_fence_release() MemoryBarrier();

#endif