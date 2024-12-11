/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_ATOMIC_H
#define TOS_PLATFORM_LINUX_THREADING_ATOMIC_H

#include <stdatomic.h>
#include "../../../stdlib/Types.h"

inline
void atomic_set(void** target, void* value) {
    __atomic_store_n(target, value, __ATOMIC_SEQ_CST);
}

inline
void* atomic_get(void** target) {
    return __atomic_load_n(target, __ATOMIC_SEQ_CST);
}

inline
void atomic_set(volatile int32* value, int32 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_set(volatile int64* value, int64 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_set_fetch(volatile int32* value, int32 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_set_fetch(volatile int64* value, int64 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    __atomic_store((volatile __int128 *) value, (__int128 *) data, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_get(volatile int32* value)
{
    return __atomic_load_n((int32 *) value, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_get(volatile int64* value)
{
    return __atomic_load_n((int64 *) value, __ATOMIC_SEQ_CST);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    __atomic_load((volatile __int128 *) value, (__int128 *) data, __ATOMIC_SEQ_CST);
}

inline
void atomic_increment(volatile int32* value) {
    __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_decrement(volatile int32* value) {
    __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_increment(volatile int64* value) {
    __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_decrement(volatile int64* value) {
    __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_add(volatile int32* value, int32 increment) {
    __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST);
}

inline
void atomic_sub(volatile int32* value, int32 decrement) {
    __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST);
}

inline
void atomic_add(volatile int64* value, int64 increment) {
    __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST);
}

inline
void atomic_sub(volatile int64* value, int64 decrement) {
    __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_compare_exchange_weak(volatile int32* value, int32* expected, int32 desired) {
    __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return *expected;
}

inline
int32 atomic_add_fetch(volatile int32* value, int32 operand) {
    return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
int32 atomic_sub_fetch(volatile int32* value, int32 operand) {
    return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_add_fetch(volatile int64* value, int64 operand) {
    return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
int64 atomic_sub_fetch(volatile int64* value, int64 operand) {
    return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
void atomic_set(volatile uint32* value, uint32 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_set(volatile uint64* value, uint64 new_value)
{
    __atomic_store_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
uint32 atomic_set_fetch(volatile uint32* value, uint32 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
uint64 atomic_set_fetch(volatile uint64* value, uint64 new_value) {
    return __atomic_exchange_n(value, new_value, __ATOMIC_SEQ_CST);
}

inline
void atomic_get(volatile byte* value, byte data[16])
{
    __atomic_store((volatile __uint128 *) value, (__uint128 *) data, __ATOMIC_SEQ_CST);
}

inline
uint32 atomic_get(volatile uint32* value)
{
    return __atomic_load_n((uint32 *) value, __ATOMIC_SEQ_CST);
}

inline
uint64 atomic_get(volatile uint64* value)
{
    return __atomic_load_n((uint64 *) value, __ATOMIC_SEQ_CST);
}

inline
void atomic_increment(volatile uint32* value) {
    __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_decrement(volatile uint32* value) {
    __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_increment(volatile uint64* value) {
    __atomic_add_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_decrement(volatile uint64* value) {
    __atomic_sub_fetch(value, 1, __ATOMIC_SEQ_CST);
}

inline
void atomic_add(volatile uint32* value, uint32 increment) {
    __atomic_add_fetch(value, increment, __ATOMIC_SEQ_CST);
}

inline
void atomic_sub(volatile uint32* value, uint32 decrement) {
    __atomic_sub_fetch(value, decrement, __ATOMIC_SEQ_CST);
}

inline
uint32 atomic_compare_exchange_weak(volatile uint32* value, uint32* expected, uint32 desired) {
    __atomic_compare_exchange_n(value, expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return *expected;
}

inline
uint32 atomic_add_fetch(volatile uint32* value, uint32 operand) {
    return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
uint32 atomic_sub_fetch(volatile uint32* value, uint32 operand) {
    return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
uint64 atomic_add_fetch(volatile uint64* value, uint64 operand) {
    return __atomic_add_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
uint64 atomic_sub_fetch(volatile uint64* value, uint64 operand) {
    return __atomic_sub_fetch(value, operand, __ATOMIC_SEQ_CST);
}

inline
void atomic_and(volatile uint32* value, uint32 mask) {
    __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_and(volatile int32* value, int32 mask) {
    __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_and(volatile uint64* value, uint64 mask) {
    __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_and(volatile int64* value, int64 mask) {
    __atomic_fetch_and(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_or(volatile uint32* value, uint32 mask) {
    __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_or(volatile int32* value, int32 mask) {
    __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_or(volatile uint64* value, uint64 mask) {
    __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST);
}

inline
void atomic_or(volatile int64* value, int64 mask) {
    __atomic_fetch_or(value, mask, __ATOMIC_SEQ_CST);
}
#endif