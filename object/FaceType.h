/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_FACE_TYPE_H
#define TOS_OBJECT_FACE_TYPE_H

enum FaceType {
    FACE_TYPE_VERTICES = 1,
    FACE_TYPE_TEXTURES = 2,
    FACE_TYPE_NORMALS = 4,
    FACE_TYPE_COLORS = 8,
    FACE_TYPE_ALL = 15
};

#endif