/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 * @see https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
 */
#ifndef PHYSICS_COLLISION_DETECTION_H
#define PHYSICS_COLLISION_DETECTION_H

#include "../Stdlib/Intrinsics.h"
#include "../Stdlib/SIMD/SIMD_F32.h"
#include "../Utils/MathUtils.h"

namespace Physics
{
    struct CollisionPoint {
        union {
            struct {
                float x;
                float y;
                float z;
            };

            float v[3];
        };
    };

    struct CollisionBox {
        union {
            struct {
                float x_min;
                float y_min;
                float z_min;

                float x_max;
                float y_max;
                float z_max;
            };

            struct {
                float v_min[3];
                float v_max[3];
            };
        };
    };

    struct CollisionSphere {
        union {
            struct {
                float x;
                float y;
                float z;
                float radius;
            };

            float v[4];
        };
    };

    /**
     * WARNING: Very inefficient, the actual implementation should use SIMD in the context of the actual
     *      programm. Usually you can group together multiple collision detections and perfom the
     *      algorithms below on multiple elements at the same time
     */
    inline
    bool is_point_inside_AABB(CollisionPoint* point, CollisionBox* box)
    {
        return point->x >= box->x_min
            && point->x <= box->x_max
            && point->y >= box->y_min
            && point->y <= box->y_max
            && point->z >= box->z_min
            && point->z <= box->z_max;
    }

    inline
    bool is_box_box_intersecting(CollisionBox* a, CollisionBox* b)
    {
        return a->x_min <= b->x_max
            && a->x_max >= b->x_min
            && a->y_min <= b->y_max
            && a->y_max >= b->y_min
            && a->z_min <= b->z_max
            && a->z_max >= b->z_min;
    }

    inline
    bool is_point_inside_sphere(CollisionPoint* point, CollisionSphere* sphere)
    {
        return Stdlib::Intrinsics::sqrt(
            (point->x - sphere->x) * (point->x - sphere->x)
            + (point->y - sphere->y) * (point->y - sphere->y)
            + (point->z - sphere->z) * (point->z - sphere->z)
        ) <= sphere->radius;
    }

    inline
    bool is_sphere_sphere_intersecting(CollisionSphere* a, CollisionSphere* b)
    {
        return Stdlib::Intrinsics::sqrt(
            (a->x - b->x) * (a->x - b->x)
            + (a->y - b->y) * (a->y - b->y)
            + (a->z - b->z) * (a->z - b->z)
        ) <= a->radius + b->radius;
    }

    inline
    bool is_sphere_box_intersecting(CollisionSphere* sphere, CollisionBox* box)
    {
        float x_max = OMS_MAX(box->x_min, OMS_MAX(sphere->x, box->x_max));
        float y_max = OMS_MAX(box->y_min, OMS_MAX(sphere->y, box->y_max));
        float z_max = OMS_MAX(box->z_min, OMS_MAX(sphere->z, box->z_max));

        return Stdlib::Intrinsics::sqrt(
            (x_max - sphere->x) * (x_max - sphere->x)
            + (y_max - sphere->y) * (y_max - sphere->y)
            + (z_max - sphere->z) * (z_max - sphere->z)
        ) <= sphere->radius;
    }
}
#endif