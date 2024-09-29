/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_CAMERA_H
#define TOS_CAMERA_H

#include "../stdlib/Types.h"

#include "../math/matrix/MatrixFloat32.h"

#include "CameraMovement.h"

#define CAMERA_MAX_INPUTS 4

// @todo Please check out if we can switch to quaternions. We tried but failed.
// The functions with a 2 at the end are our current backup solution which shouldn't be used (probably)

struct Camera {
    v3_f32 location;
    v4_f32 orientation;

    v3_f32 front;
    v3_f32 right;
    v3_f32 up;
    v3_f32 world_up;

    float speed;
    float sensitivity;
    float zoom;

    float fov;
    float znear;
    float zfar;
    float aspect;
};

void
camera_update_vectors(Camera* camera)
{
    camera->front.x = cosf(OMS_DEG2RAD(camera->orientation.x)) * cosf(OMS_DEG2RAD(camera->orientation.y));
    camera->front.y = sinf(OMS_DEG2RAD(camera->orientation.x));
    camera->front.z = cosf(OMS_DEG2RAD(camera->orientation.x)) * sinf(OMS_DEG2RAD(camera->orientation.y));
    vec3_normalize_f32(&camera->front);

    vec3_cross(&camera->right, &camera->front, &camera->world_up);
    vec3_normalize_f32(&camera->right);

    vec3_cross(&camera->up, &camera->right, &camera->front);
    vec3_normalize_f32(&camera->up);
}

void camera_rotate(Camera* camera, int32 dx, int32 dy, float dt)
{
    camera->orientation.x += dy * camera->sensitivity;
    camera->orientation.y -= dx * camera->sensitivity;

    if (true) {
        if (camera->orientation.x > 89.0f) {
            camera->orientation.x = 89.0f;
        } else if (camera->orientation.x < -89.0f) {
            camera->orientation.x = -89.0f;
        }

        if (camera->orientation.y > 360.0f) {
            camera->orientation.y -= 360.0f;
        } else if (camera->orientation.y < -360.0f) {
            camera->orientation.y += 360.0f;
        }
    }

    camera_update_vectors(camera);
}

// you can have up to 4 camera movement inputs at the same time
void camera_movement(Camera* camera, CameraMovement* movement, float dt, bool relative_to_world = true)
{
    f32 velocity = camera->speed * dt;

    if (relative_to_world) {
        for (int i = 0; i < CAMERA_MAX_INPUTS; i++) {
            switch(movement[i]) {
                case CAMERA_MOVEMENT_FORWARD: {
                        camera->location.z += velocity;
                    } break;
                case CAMERA_MOVEMENT_BACK: {
                        camera->location.z -= velocity;
                    } break;
                case CAMERA_MOVEMENT_LEFT: {
                        camera->location.x -= velocity;
                    } break;
                case CAMERA_MOVEMENT_RIGHT: {
                        camera->location.x += velocity;
                    } break;
                case CAMERA_MOVEMENT_UP: {
                        camera->location.y += velocity;
                    } break;
                case CAMERA_MOVEMENT_DOWN: {
                        camera->location.y -= velocity;
                    } break;
                case CAMERA_MOVEMENT_PITCH_UP: {
                        camera->orientation.x += velocity;
                    } break;
                case CAMERA_MOVEMENT_PITCH_DOWN: {
                        camera->orientation.x -= velocity;
                    } break;
                case CAMERA_MOVEMENT_ROLL_LEFT: {
                        camera->orientation.z += velocity;
                    } break;
                case CAMERA_MOVEMENT_ROLL_RIGHT: {
                        camera->orientation.z -= velocity;
                    } break;
                case CAMERA_MOVEMENT_YAW_LEFT: {
                        camera->orientation.y += velocity;
                    } break;
                case CAMERA_MOVEMENT_YAW_RIGHT: {
                        camera->orientation.y -= velocity;
                    } break;
                case CAMERA_MOVEMENT_ZOOM_IN: {
                        camera->zoom += velocity;
                    } break;
                case CAMERA_MOVEMENT_ZOOM_OUT: {
                        camera->zoom -= velocity;
                    } break;
                default: {}
            }
        }
    } else {
        v3_f32 forward = camera->front;

        v3_f32 right;
        vec3_cross(&right, &camera->world_up, &forward);
        vec3_normalize_f32(&right);

        v3_f32 up;
        vec3_cross(&up, &right, &forward);
        vec3_normalize_f32(&up);

        for (int i = 0; i < CAMERA_MAX_INPUTS; i++) {
            switch(movement[i]) {
                case CAMERA_MOVEMENT_FORWARD: {
                        camera->location.x += forward.x * velocity;
                        camera->location.y += forward.y * velocity;
                        camera->location.z += forward.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_BACK: {
                        camera->location.x -= forward.x * velocity;
                        camera->location.y -= forward.y * velocity;
                        camera->location.z -= forward.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_LEFT: {
                        camera->location.x -= right.x * velocity;
                        camera->location.y -= right.y * velocity;
                        camera->location.z -= right.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_RIGHT: {
                        camera->location.x += right.x * velocity;
                        camera->location.y += right.y * velocity;
                        camera->location.z += right.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_UP: {
                        camera->location.x += up.x * velocity;
                        camera->location.y += up.y * velocity;
                        camera->location.z += up.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_DOWN: {
                        camera->location.x -= up.x * velocity;
                        camera->location.y -= up.y * velocity;
                        camera->location.z -= up.z * velocity;
                    } break;
                case CAMERA_MOVEMENT_PITCH_UP: {
                        camera->orientation.x += velocity;
                    } break;
                case CAMERA_MOVEMENT_PITCH_DOWN: {
                        camera->orientation.x -= velocity;
                    } break;
                case CAMERA_MOVEMENT_ROLL_LEFT: {
                        camera->orientation.z += velocity;
                    } break;
                case CAMERA_MOVEMENT_ROLL_RIGHT: {
                        camera->orientation.z -= velocity;
                    } break;
                case CAMERA_MOVEMENT_YAW_LEFT: {
                        camera->orientation.z += velocity;
                    } break;
                case CAMERA_MOVEMENT_YAW_RIGHT: {
                        camera->orientation.z -= velocity;
                    } break;
                case CAMERA_MOVEMENT_ZOOM_IN: {
                        camera->zoom += velocity;
                    } break;
                case CAMERA_MOVEMENT_ZOOM_OUT: {
                        camera->zoom -= velocity;
                    } break;
                default: {}
            }
        }
    }
}

// @question should we remove the fov for this or not, fov shouldn't matter for UI!?
inline
void camera_orth_matrix_lh(const Camera* __restrict camera, float* __restrict orth)
{
    mat4_identity_sparse(orth);

    float ymax, xmax;
    ymax = camera->znear * tanf(camera->fov * 0.5f);
    xmax = ymax * camera->aspect;

    mat4_ortho_sparse_lh(orth, -xmax, xmax, -ymax, ymax, camera->znear, camera->zfar);
}

inline
void camera_orth_matrix_rh(const Camera* __restrict camera, float* __restrict orth)
{
    mat4_identity_sparse(orth);

    float ymax, xmax;
    ymax = camera->znear * tanf(camera->fov * 0.5f);
    xmax = ymax * camera->aspect;

    mat4_ortho_sparse_rh(orth, -xmax, xmax, -ymax, ymax, camera->znear, camera->zfar);
}

inline
void camera_projection_matrix_lh(const Camera* __restrict camera, float* __restrict projection)
{
    mat4_identity_sparse(projection);
    mat4_perspective_sparse_lh(
        projection,
        camera->fov,
        camera->aspect,
        camera->znear,
        camera->zfar
    );
}

inline
void camera_projection_matrix_rh(const Camera* __restrict camera, float* __restrict projection)
{
    mat4_identity_sparse(projection);
    mat4_perspective_sparse_rh(
        projection,
        camera->fov,
        camera->aspect,
        camera->znear,
        camera->zfar
    );
}

// This is usually not used, since it is included in the view matrix
// expects the identity matrix
inline
void camera_translation_matrix_sparse_rh(const Camera* __restrict camera, float* translation)
{
    translation[12] = camera->location.x;
    translation[13] = camera->location.y;
    translation[14] = camera->location.z;
}

inline
void camera_translation_matrix_sparse_lh(const Camera* __restrict camera, float* translation)
{
    translation[3] = camera->location.x;
    translation[7] = camera->location.y;
    translation[11] = camera->location.z;
}

void
camera_view_matrix_lh(const Camera* __restrict camera, float* __restrict view)
{
    v3_f32 zaxis = { camera->front.x, camera->front.y, camera->front.z };

    v3_f32 xaxis;
    vec3_cross(&xaxis, &camera->world_up, &zaxis);
    vec3_normalize_f32(&xaxis);

    v3_f32 yaxis;
    vec3_cross(&yaxis, &zaxis, &xaxis);

    view[0] = xaxis.x;
    view[1] = yaxis.x;
    view[2] = zaxis.x;
    view[3] = 0.0f;
    view[4] = xaxis.y;
    view[5] = yaxis.y;
    view[6] = zaxis.y;
    view[7] = 0.0f;
    view[8] = xaxis.z;
    view[9] = yaxis.z;
    view[10] = zaxis.z;
    view[11] = 0;
    view[12] = -vec3_dot(&xaxis, &camera->location);
    view[13] = -vec3_dot(&yaxis, &camera->location);
    view[14] = -vec3_dot(&zaxis, &camera->location);
    view[15] = 1.0f;
}

void
camera_view_matrix_rh(const Camera* __restrict camera, float* __restrict view)
{
    v3_f32 zaxis = { -camera->front.x, -camera->front.y, -camera->front.z };

    v3_f32 xaxis;
    vec3_cross(&xaxis, &zaxis, &camera->world_up);
    vec3_normalize_f32(&xaxis);

    v3_f32 yaxis;
    vec3_cross(&yaxis, &zaxis, &xaxis);

    view[0] = xaxis.x;
    view[1] = yaxis.x;
    view[2] = zaxis.x;
    view[3] = 0.0f;
    view[4] = xaxis.y;
    view[5] = yaxis.y;
    view[6] = zaxis.y;
    view[7] = 0.0f;
    view[8] = xaxis.z;
    view[9] = yaxis.z;
    view[10] = zaxis.z;
    view[11] = 0;
    view[12] = -vec3_dot(&xaxis, &camera->location);
    view[13] = -vec3_dot(&yaxis, &camera->location);
    view[14] = -vec3_dot(&zaxis, &camera->location);
    view[15] = 1.0f;
}

#endif