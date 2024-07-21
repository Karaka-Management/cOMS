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
#include "../stdlib/Mathtypes.h"

#include "../math/matrix/QuaternionFloat32.h"

#include "CameraMovement.h"

struct Camera {
    // left handed cartesian coordinates
    v3_f32 location;
    v4_f32 orientation;

    float speed;
    float zoom;
};

// you can have up to 4 camera movement inputs at the same time
void camera_movement(Camera* camera, CameraMovement* movement, float dt)
{
    f32 velocity = camera->speed * dt;

    bool has_pos = false;
    v4_f32 pos;

    bool has_view = false;
    v3_f32 view;
    v4_f32 quaternion;

    for (int i = 0; i < 4; i++) {
        switch(movement[i]) {
            case CAMERA_MOVEMENT_FORWARD: {
                    pos.z = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_BACK: {
                    pos.z = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_LEFT: {
                    pos.x = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_RIGHT: {
                    pos.x = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_UP: {
                    pos.y = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_DOWN: {
                    pos.y = velocity;
                    has_pos = true;
                } break;
            case CAMERA_MOVEMENT_PITCH_UP: {
                    view.pitch += velocity;
                    has_view = true;
                } break;
            case CAMERA_MOVEMENT_PITCH_DOWN: {
                    view.pitch -= velocity;
                    has_view = true;
                } break;
            case CAMERA_MOVEMENT_ROLL_LEFT: {
                    view.roll += velocity;
                    has_view = true;
                } break;
            case CAMERA_MOVEMENT_ROLL_RIGHT: {
                    view.roll -= velocity;
                    has_view = true;
                } break;
            case CAMERA_MOVEMENT_YAW_LEFT: {
                    view.yaw += velocity;
                    has_view = true;
                } break;
            case CAMERA_MOVEMENT_YAW_RIGHT: {
                    view.yaw -= velocity;
                    has_view = true;
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

    // A position change updates the position AND the quaternion
    if (has_pos) {
        // @question this might be wrong/bad since pos is not a normalized vector
        v4_f32 quat_temp = camera->orientation;
        quaternion_rotate_euler(&camera->orientation, &quat_temp, &pos);

        camera->location.x += pos.x;
        camera->location.y += pos.y;
        camera->location.z += pos.z;
    }

    // A view change only updates the quaternion
    if (has_view) {
        v4_f32 quat_temp = camera->orientation;
        quaternion_from_euler(&quaternion, &view);
        quaternion_multiply(&camera->orientation, &quat_temp, &quaternion);
    }
}

#endif