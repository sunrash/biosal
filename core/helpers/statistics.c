
#include "statistics.h"

#include "vector_helper.h"

#include <core/structures/vector.h>
#include <core/structures/map.h>
#include <core/structures/map_iterator.h>

#include <math.h>
#include <stdio.h>

double bsal_statistics_get_mean_int(struct bsal_vector *vector)
{
    double sum;
    int i;
    int count;

    count = bsal_vector_size(vector);

    if (count == 0) {
        return 0;
    }

    sum = 0;

    for (i = 0; i < count; i++) {
        sum += bsal_vector_at_as_int(vector, i);
    }

    return sum / count;
}

double bsal_statistics_get_standard_deviation_int(struct bsal_vector *vector)
{
    double mean;
    double sum;
    int i;
    int count;
    double difference;

    count = bsal_vector_size(vector);

    if (count == 0) {
        return 0;
    }

    mean = bsal_statistics_get_mean_int(vector);

    sum = 0;

    for (i = 0; i < count; i++) {
        difference = bsal_vector_at_as_int(vector, i) - mean;

        sum += difference * difference;
    }

    return sqrt(sum) / count;
}

int bsal_statistics_get_median_int(struct bsal_vector *vector)
{
    int count;

    count = bsal_vector_size(vector);

    if (count == 0) {
        return 0;
    }

    return bsal_vector_at_as_int(vector, count / 2);
}

int bsal_statistics_get_percentile_int(struct bsal_vector *vector, int p)
{
    int index;
    int size;
    int count;

    int other_p;
    int other_count;
#if 0
#endif

    size = bsal_vector_size(vector);

    if (size == 0) {
        return 0;
    }

    count = ((0.0 + p) / 100 ) * size;

#if 0
    /*
    other_count = size;
    other_count -= count;
    */

    if (count < other_count) {
        ++count;
    }
#endif

    index = 0;

    /* make it symmetric: it's logical that the number of
     * elements with a value <= percentile 5% be the same as the number of
     * elements with a value >= percentile 95%
     */
    if (p < 50) {
        index = count - 1;
    } else {
        /* P = 95, P' = 5 */
        other_p = 100 - p;

        /* size = 27, other_p = 5, other_count = 1 */
        other_count = ((0.0 + other_p) / 100 ) * size;
        index = size - other_count;
    }

#if 0
    printf("percentile %d size %d index %d counts: %d %d\n", p, size, index,
                    count, other_count);
#endif

    /*
    bsal_vector_print_int(vector);
    */

    return bsal_vector_at_as_int(vector, index);
}

void bsal_statistics_print_percentiles_int(struct bsal_vector *vector)
{
    int percentile_5;
    int percentile_25;
    int percentile_30;
    int percentile_40;
    int percentile_50;
    int percentile_60;
    int percentile_70;
    int percentile_75;
    int percentile_95;

    int size;

    size = bsal_vector_size(vector);

    if (size == 0) {
        printf("empty\n");
        return;
    }

    percentile_5 = bsal_statistics_get_percentile_int(vector, 5);
    percentile_25 = bsal_statistics_get_percentile_int(vector, 25);
    percentile_30 = bsal_statistics_get_percentile_int(vector, 30);
    percentile_40 = bsal_statistics_get_percentile_int(vector, 40);
    percentile_50 = bsal_statistics_get_percentile_int(vector, 50);
    percentile_60 = bsal_statistics_get_percentile_int(vector, 60);
    percentile_70 = bsal_statistics_get_percentile_int(vector, 70);
    percentile_75 = bsal_statistics_get_percentile_int(vector, 75);
    percentile_95 = bsal_statistics_get_percentile_int(vector, 95);

    printf("%d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d, %d%%: %d\n",
                    5, percentile_5,
                    25, percentile_25,
                    30, percentile_30,
                    40, percentile_40,
                    50, percentile_50,
                    60, percentile_60,
                    70, percentile_70,
                    75, percentile_75,
                    95, percentile_95);
}

float bsal_statistics_get_percentile_float(struct bsal_vector *vector, int p)
{
    int index;
    int size;

    size = bsal_vector_size(vector);

    if (size == 0) {
        return 0;
    }

    index = ((0.0 + p) / 100 ) * (size - 1);

    /*
    printf("percentile %d size %d\n", p, size);
    bsal_vector_print_int(vector);
    */

    return bsal_vector_at_as_float(vector, index);
}

void bsal_statistics_print_percentiles_float(struct bsal_vector *vector)
{
    float percentile_5;
    float percentile_25;
    float percentile_30;
    float percentile_40;
    float percentile_50;
    float percentile_60;
    float percentile_70;
    float percentile_75;
    float percentile_95;

    int size;

    size = bsal_vector_size(vector);

    if (size == 0) {
        printf("empty\n");
        return;
    }

    percentile_5 = bsal_statistics_get_percentile_float(vector, 5);
    percentile_25 = bsal_statistics_get_percentile_float(vector, 25);
    percentile_30 = bsal_statistics_get_percentile_float(vector, 30);
    percentile_40 = bsal_statistics_get_percentile_float(vector, 40);
    percentile_50 = bsal_statistics_get_percentile_float(vector, 50);
    percentile_60 = bsal_statistics_get_percentile_float(vector, 60);
    percentile_70 = bsal_statistics_get_percentile_float(vector, 70);
    percentile_75 = bsal_statistics_get_percentile_float(vector, 75);
    percentile_95 = bsal_statistics_get_percentile_float(vector, 95);

    printf("%d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f, %d%%: %f\n",
                    5, percentile_5,
                    25, percentile_25,
                    30, percentile_30,
                    40, percentile_40,
                    50, percentile_50,
                    60, percentile_60,
                    70, percentile_70,
                    75, percentile_75,
                    95, percentile_95);
}

int bsal_statistics_get_percentile_int_map(struct bsal_map *map, int percentile)
{
    struct bsal_vector keys;
    struct bsal_map_iterator iterator;
    int key;
    int frequency;
    int total;
    int keys_before;
    int keys_so_far;
    int i;
    int size;

    bsal_map_iterator_init(&iterator, map);
    total = 0;
    bsal_vector_init(&keys, sizeof(int));

    while (bsal_map_iterator_get_next_key_and_value(&iterator, &key, &frequency)) {
        bsal_vector_push_back(&keys, &key);
        total += frequency;
    }

    bsal_map_iterator_destroy(&iterator);

    keys_before = round((percentile / 100.0) * total);

    bsal_vector_sort_int(&keys);

    i = 0;
    keys_so_far = 0;
    size = bsal_vector_size(&keys);

    key = bsal_vector_at_as_int(&keys, size - 1);

    /*
     * example:
     *
     * 31 2
     * 3000 1
     *
     * 31 31 3000
     *
     * N = 3
     *
     * P30 -> 30.0/100*3 = 0.8999999999999999 = 0 (with round), so P30 is 31
     * P70 -> 70.0/100*3 = 2.0999999999999996 = 2 (with round), so P70 is 31
     * P95 -> 95.0/100*3 = 2.8499999999999996 = 3 (with round), so P95 = 3000
     */
    while (i < size) {
        key = bsal_vector_at_as_int(&keys, i);
        bsal_map_get_value(map, &key, &frequency);

        if (keys_so_far <= keys_before
                        && keys_before <= keys_so_far + frequency) {
            break;
        }

        keys_so_far += frequency;

        ++i;
    }

    bsal_vector_destroy(&keys);

    return key;
}
