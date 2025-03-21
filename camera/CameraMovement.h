/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_CAMERA_MOVEMENT_H
#define COMS_CAMERA_MOVEMENT_H

enum CameraMovement {
    CAMERA_MOVEMENT_NONE,

    CAMERA_MOVEMENT_FORWARD,
    CAMERA_MOVEMENT_BACK,

    CAMERA_MOVEMENT_LEFT,
    CAMERA_MOVEMENT_RIGHT,

    CAMERA_MOVEMENT_UP,
    CAMERA_MOVEMENT_DOWN,

    CAMERA_MOVEMENT_FREE_ORIENTATION,

    CAMERA_MOVEMENT_PITCH_UP,
    CAMERA_MOVEMENT_PITCH_DOWN,

    CAMERA_MOVEMENT_ROLL_LEFT,
    CAMERA_MOVEMENT_ROLL_RIGHT,

    CAMERA_MOVEMENT_YAW_LEFT,
    CAMERA_MOVEMENT_YAW_RIGHT,

    CAMERA_MOVEMENT_ZOOM_IN,
    CAMERA_MOVEMENT_ZOOM_OUT
};

#endif