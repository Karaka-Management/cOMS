#ifndef TOS_ENVIRONMENT_GLOBE_H
#define TOS_ENVIRONMENT_GLOBE_H

#include "../stdlib/Types.h"
#include <math.h>

// day_length in seconds
f32 time_of_day(f64 time, f32 day_length) {
    f64 t = time;
    t = t / day_length;
    t = t - (int32) t;

    return (f32) t;
}

// time can either be actual time in seconds OR relative time to progra start
f32 daylight_get(f64 time, f32 day_length) {
    f32 timer = time_of_day(time, day_length);

    if (timer < 0.5) {
        f32 t = (timer - 0.25) * 100;

        return 1 / (1 + powf(2, -t));
    }

    f32 t = (timer - 0.85) * 100;

    return 1 - 1 / (1 + powf(2, -t));
}

#endif