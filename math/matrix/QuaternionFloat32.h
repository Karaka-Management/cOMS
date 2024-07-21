/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MATH_MATRIX_QUATERNION_FLOAT32_H
#define TOS_MATH_MATRIX_QUATERNION_FLOAT32_H

#include "../../stdlib/Intrinsics.h"
#include "../../stdlib/Mathtypes.h"
#include "../../utils/MathUtils.h"

void quaternion_from_euler(v4_f32* quat, float pitch, float yaw, float roll)
{
    float y = OMS_RAD2DEG(yaw * 0.5f);
    float cy = cosf_approx(y);
    float sy = sinf_approx(y);

    float p = OMS_RAD2DEG(pitch * 0.5f);
    float cp = cosf_approx(p);
    float sp = sinf_approx(p);

    float r = OMS_RAD2DEG(roll * 0.5f);
    float cr = cosf_approx(r);
    float sr = sinf_approx(r);

    quat->w = cr * cp * cy + sr * sp * sy;
    quat->x = sr * cp * cy - cr * sp * sy;
    quat->y = cr * sp * cy + sr * cp * sy;
    quat->z = cr * cp * sy - sr * sp * cy;
}

void quaternion_from_euler(v4_f32* quat, const v3_f32* v)
{
    float y = OMS_RAD2DEG(v->y * 0.5f);
    float cy = cosf_approx(y);
    float sy = sinf_approx(y);

    float p = OMS_RAD2DEG(v->x * 0.5f);
    float cp = cosf_approx(p);
    float sp = sinf_approx(p);

    float r = OMS_RAD2DEG(v->z * 0.5f);
    float cr = cosf_approx(r);
    float sr = sinf_approx(r);

    quat->w = cr * cp * cy + sr * sp * sy;
    quat->x = sr * cp * cy - cr * sp * sy;
    quat->y = cr * sp * cy + sr * cp * sy;
    quat->z = cr * cp * sy - sr * sp * cy;
}

void euler_from_quaternion(const v4_f32* quat, v3_f32* v) {
    // Pitch
    float sinp = 2.0f * (quat->w * quat->x + quat->y * quat->z);
    float cosp = 1.0f - 2.0f * (quat->x * quat->x + quat->y * quat->y);
    v->pitch = atan2f_approx(sinp, cosp);

    // Yaw
    float siny = 2.0f * (quat->w * quat->y - quat->z * quat->x);

    if (siny >= 1.0f) {
        v->yaw = OMS_PI_OVER_TWO;
    } else if (siny <= -1.0f ) {
        v->yaw = -OMS_PI_OVER_TWO;
    } else {
        v->yaw = asinf_approx(siny);
    }

    // Roll
    float sinr = 2.0f * (quat->w * quat->z + quat->x * quat->y);
    float cosr = 1.0f - 2.0f * (quat->y * quat->y + quat->z * quat->z);
    v->roll = atan2f_approx(sinr, cosr);
}

void quaternion_multiply(v4_f32* quat, const v4_f32* quat1, const v4_f32* quat2)
{
    quat->w = quat1->w * quat2->w - quat1->x * quat2->x - quat1->y * quat2->y - quat1->z * quat2->z;
    quat->x = quat1->w * quat2->x + quat1->x * quat2->w + quat1->y * quat2->z - quat1->z * quat2->y;
    quat->y = quat1->w * quat2->y - quat1->x * quat2->z + quat1->y * quat2->w + quat1->z * quat2->x;
    quat->z = quat1->w * quat2->z + quat1->x * quat2->y - quat1->y * quat2->x + quat1->z * quat2->w;
}

void quaternion_inverse(v4_f32* quat, const v4_f32* quat_origin) {
    float norm = quat_origin->w * quat_origin->w
        + quat_origin->x * quat_origin->x
        + quat_origin->y * quat_origin->y
        + quat_origin->z * quat_origin->z;

    quat->w = quat_origin->w / norm;
    quat->x = -quat_origin->x / norm;
    quat->y = -quat_origin->y / norm;
    quat->z = -quat_origin->z / norm;
}

inline
void quaternion_rotate_euler(v4_f32* quat, const v4_f32* quat_origin, const v4_f32* euler)
{
    quaternion_multiply(quat, quat_origin, euler);
}

inline
void quaternion_rotate_quaternion(v4_f32* quat, const v4_f32* quat_origin, const v4_f32* quat_rot)
{
    v4_f32 quat_tmp;
    quaternion_multiply(&quat_tmp, quat_rot, quat_origin);

    v4_f32 rot_;
    quaternion_inverse(&rot_, quat_rot);

    quaternion_multiply(quat, &quat_tmp, &rot_);
}

#endif