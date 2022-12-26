/**
 * Karaka
 *
 * @package   Test
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#include <stdio.h>

#include "../../Threads/Thread.h"
#include "../../Utils/TestUtils.h"

static const size_t num_threads = 4;
static const size_t num_items   = 10;

void worker(void *arg)
{
    int *val = (int *) arg;
    int  old = *val;

    *val += 100;
    // printf("tid=%p, old=%d, val=%d\n", (void *) pthread_self(), old, *val);

    if (*val % 2) {
        sleep(1);
    }
}

int main(int argc, char** argv)
{
    printf("Threads:\n\n");
    printf("ThreadPool:\n");

    int i;
    Threads::ThreadPool *pool = Threads::pool_create(num_threads);
    int                 *vals = (int *) calloc(num_items, sizeof(int));

    for (i = 0; i < num_items; ++i) {
        vals[i] = i;
        Threads::pool_add_work(pool, worker, vals + i);
    }

    Threads::pool_wait(pool);
    sleep(1);

    bool test = true;

    for (i = 0; i < num_items; ++i) {
        // printf("%d\n", vals[i]);
        test = test && 100 + i == vals[i];
    }

    ASSERT_EQUALS(test, true, "%d", "%d");

    free(vals);
    Threads::pool_destroy(pool);

    printf("\n\n");

    return 0;
}
