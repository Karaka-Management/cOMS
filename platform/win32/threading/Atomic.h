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

#include <windows.h>
#include "../../../stdlib/Types.h"

inline
void atomic_set(void** target, void* new_pointer)
{
    InterlockedExchangePointer(target, new_pointer);
}

inline
void* atomic_get(void** target)
{
    return InterlockedCompareExchangePointer(target, NULL, NULL);
}

inline
void atomic_set(volatile int32* value, int32 new_value)
{
    InterlockedExchange((long *) value, new_value);
}

inline
void atomic_set(volatile int64* value, int64 new_value)
{
    InterlockedExchange((long *) value, (long) new_value);
}

inline
void atomic_set(volatile f32* value, f32 new_value)
{
    InterlockedExchange((long *) value, (long) new_value);
}

inline
void atomic_set(volatile f64* value, f64 new_value)
{
    InterlockedExchange((long *) value, (long) new_value);
}

inline
int32 atomic_set_fetch(volatile int32* value, int32 new_value)
{
    return (int32) InterlockedExchange((long *) value, new_value);
}

inline
int64 atomic_set_fetch(volatile int64* value, int64 new_value)
{
    return (int64) InterlockedExchange((long *) value, (long) new_value);
}

inline
void atomic_set(volatile byte* value, const byte new_value[16])
{
    int64* value64 = (int64*) value;
    const int64* new_value64 = (const int64*) new_value;

    int64 expected_low, expected_high;

    do {
        expected_low = value64[0];
        expected_high = value64[1];
    } while (
        !InterlockedCompareExchange128(
            (volatile long long *) value,
            new_value64[1],
            new_value64[0],
            &expected_low
        )
    );
}

inline
int32 atomic_get(volatile int32* value)
{
    return (int32) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
int64 atomic_get(volatile int64* value)
{
    return (int64) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
f32 atomic_get(volatile f32* value)
{
    return (f32) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
f64 atomic_get(volatile f64* value)
{
    return (f64) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    InterlockedCompareExchange128((volatile long long *) value, 0, 0, (long long *) data);
}

inline
void atomic_increment(volatile int32* value) {
    InterlockedIncrement((long *) value);
}

inline
void atomic_decrement(volatile int32* value) {
    InterlockedDecrement((long *) value);
}

inline
void atomic_increment(volatile int64* value) {
    InterlockedIncrement((long *) value);
}

inline
void atomic_decrement(volatile int64* value) {
    InterlockedDecrement((long *) value);
}

inline
void atomic_add(volatile int32* value, int32 increment) {
    InterlockedAdd((long *) value, increment);
}

inline
void atomic_sub(volatile int32* value, int32 decrement) {
    InterlockedAdd((long *) value, -decrement);
}

inline
void atomic_add(volatile int64* value, int64 increment) {
    InterlockedAdd((long *) value, (long) increment);
}

inline
void atomic_sub(volatile int64* value, int64 decrement) {
    InterlockedAdd((long *) value, -1 * ((long) decrement));
}

inline
f32 atomic_compare_exchange_weak(volatile f32* value, f32* expected, f32 desired) {
    return (f32) InterlockedCompareExchange((long *) value, (long) desired, (long) *expected);
}

inline
f64 atomic_compare_exchange_weak(volatile f64* value, f64* expected, f64 desired) {
    return (f64) InterlockedCompareExchange((long *) value, (long) desired, (long) *expected);
}

inline
int32 atomic_compare_exchange_weak(volatile int32* value, int32* expected, int32 desired) {
    return (int32) InterlockedCompareExchange((long *) value, desired, *expected);
}

inline
int64 atomic_compare_exchange_weak(volatile int64* value, int64* expected, int64 desired) {
    return (int64) InterlockedCompareExchange((long *) value, (long) desired, (long) *expected);
}

inline
int32 atomic_fetch_add(volatile int32* value, int32 operand) {
    return (int32) InterlockedExchangeAdd((long *) value, operand);
}

inline
int32 atomic_fetch_sub(volatile int32* value, int32 operand) {
    return (int32) InterlockedExchangeSubtract((unsigned long *) value, operand);
}

inline
int64 atomic_fetch_add(volatile int64* value, int64 operand) {
    return (int64) InterlockedExchangeAdd((long *) value, (long) operand);
}

inline
int64 atomic_fetch_sub(volatile int64* value, int64 operand) {
    return (int64) InterlockedExchangeSubtract((unsigned long *) value, (long) operand);
}

inline
void atomic_set(volatile uint32* value, uint32 new_value)
{
    InterlockedExchange((long *) value, new_value);
}

inline
void atomic_set(volatile uint64* value, uint64 new_value)
{
    InterlockedExchange((long *) value, (long) new_value);
}

inline
uint32 atomic_set_fetch(volatile uint32* value, uint32 new_value)
{
    return (uint32) InterlockedExchange((long *) value, new_value);
}

inline
uint64 atomic_set_fetch(volatile uint64* value, uint64 new_value)
{
    return (uint64) InterlockedExchange((long *) value, (long) new_value);
}

inline
uint32 atomic_get(volatile uint32* value)
{
    return (uint32) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
uint64 atomic_get(volatile uint64* value)
{
    return (uint64) InterlockedCompareExchange((long *) value, 0, 0);
}

inline
void atomic_increment(volatile uint32* value) {
    InterlockedIncrement((long *) value);
}

inline
void atomic_decrement(volatile uint32* value) {
    InterlockedDecrement((long *) value);
}

inline
void atomic_increment(volatile uint64* value) {
    InterlockedIncrement((long *) value);
}

inline
void atomic_decrement(volatile uint64* value) {
    InterlockedDecrement((long *) value);
}

inline
void atomic_add(volatile uint32* value, uint32 increment) {
    InterlockedAdd((long *) value, increment);
}

inline
void atomic_sub(volatile uint32* value, uint32 decrement) {
    InterlockedAdd((long *) value, -1 * ((int32) decrement));
}

inline
void atomic_add(volatile uint64* value, uint64 increment) {
    InterlockedAdd((long *) value, (long) increment);
}

inline
void atomic_sub(volatile uint64* value, uint64 decrement) {
    InterlockedAdd((long *) value, -1 * ((long) decrement));
}

inline
uint32 atomic_compare_exchange_weak(volatile uint32* value, uint32* expected, uint32 desired) {
    return (uint32) InterlockedCompareExchange((long *) value, desired, *expected);
}

inline
uint64 atomic_compare_exchange_weak(volatile uint64* value, uint64* expected, uint64 desired) {
    return (uint64) InterlockedCompareExchange((unsigned long long *) value, (unsigned long long) desired, (unsigned long long) *expected);
}

inline
uint32 atomic_fetch_add(volatile uint32* value, uint32 operand) {
    return (uint32) InterlockedExchangeAdd((long *) value, operand);
}

inline
uint32 atomic_fetch_sub(volatile uint32* value, uint32 operand) {
    return (uint32) InterlockedExchangeSubtract((unsigned long *) value, operand);
}

inline
uint64 atomic_fetch_add(volatile uint64* value, uint64 operand) {
    return (uint64) InterlockedExchangeAdd((long *) value, (long) operand);
}

inline
uint64 atomic_fetch_sub(volatile uint64* value, uint64 operand) {
    return (uint64) InterlockedExchangeSubtract((unsigned long *) value, (long) operand);
}

inline
void atomic_and(volatile uint32* value, uint32 mask) {
    InterlockedAnd((volatile LONG *) value, mask);
}

inline
void atomic_and(volatile int32* value, int32 mask) {
    InterlockedAnd((volatile LONG *) value, (LONG)mask);
}

inline
void atomic_and(volatile uint64* value, uint64 mask) {
    InterlockedAnd64((volatile LONG64 *) value, mask);
}

inline
void atomic_and(volatile int64* value, int64 mask) {
    InterlockedAnd64((volatile LONG64 *) value, mask);
}

inline
void atomic_or(volatile uint32* value, uint32 mask) {
    InterlockedOr((volatile LONG *) value, mask);
}

inline
void atomic_or(volatile int32* value, int32 mask) {
    InterlockedOr((volatile LONG *) value, (LONG)mask);
}

inline
void atomic_or(volatile uint64* value, uint64 mask) {
    InterlockedOr64((volatile LONG64 *) value, mask);
}

inline
void atomic_or(volatile int64* value, int64 mask) {
    InterlockedOr64((volatile LONG64 *) value, mask);
}

#endif