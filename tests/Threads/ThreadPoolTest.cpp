/**
 * Jingga
 *
 * @package   Test
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#include <stdint.h>
#include <stdio.h>

#include "../../Threads/Thread.h"
#include "../../Utils/TestUtils.h"

static const size_t num_threads = 4;
static const size_t num_items   = 10;

// increase value by 100
void worker(void *arg)
{
    Threads::Job *job = (Threads::Job *) arg;

    int *val  = (int *) job->arg;
    *val     += 100;

    if (*val % 2) {
        sleep(1);
    }

    job->state = 1;
}

int main(int argc, char **argv)
{
    printf("Threads:\n\n");
    printf("ThreadPool:\n");

    int i;
    Threads::ThreadPool *pool = Threads::pool_create(num_threads);
    int *vals                 = (int *) calloc(num_items, sizeof(int));
    Threads::Job **works      = (Threads::Job **) calloc(num_items, sizeof(Threads::Job));

    for (i = 0; i < num_items; ++i) {
        vals[i]  = i;
        works[i] = Threads::pool_add_work(pool, worker, vals + i);
    }

    // @bug wait is not working as expected
    //      I thought wait works similarly to what the do/while construct below does
    // Threads::pool_wait(pool);

    bool finished = false;
    do {
        finished = true;
        for (i = 0; i < num_items; ++i) {
            finished = finished && (works[i]->state == 1);
        }
    } while (!finished);

    bool test = true;

    for (i = 0; i < num_items; ++i) {
        ASSERT_EQUALS(vals[i], 100 + i, "%d", "%d");
    }

    free(vals);
    free(works);
    Threads::pool_destroy(pool);

    printf("\n\n");

    return 0;
}
