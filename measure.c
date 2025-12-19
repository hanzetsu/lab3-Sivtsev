#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "container.h"
#include "struct.h"
#include <math.h>   
#include "sort.h"

#define NUMBER_OF_RUNS 3

int main()
{
    srand(time(NULL));

    int test_sizes[] = {100, 500, 2000, 4000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    printf("   N\t\tВставками (мс)\t\t\tБыстрая (мс)\t\tУскорение\n");
    printf("\t\tсреднее ± дисперсия\tсреднее ± дисперсия\n");
    printf("--------------------------------------------------------------------------------------\n");

    for (int k = 0; k < num_tests; k++)
    {
        int n = test_sizes[k];

        double insertion_times[NUMBER_OF_RUNS];
        double quick_times[NUMBER_OF_RUNS];
        
        for (int j = 0; j < NUMBER_OF_RUNS; j++)
        {
            container *cont_insertion = container_create(sizeof(struct house));
            container *cont_quick = container_create(sizeof(struct house));

            for (int i = 0; i < n; i++)
            {
                struct house h = generate_random_house();
                container_push_back(&h, cont_insertion);
                container_push_back(&h, cont_quick);
            }
            
            clock_t start, end;

            start = clock();
            container_sort(cont_insertion, comparator_asc);
            end = clock();
            insertion_times[j] = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            start = clock();
            container_sort_quick(cont_quick, comparator_asc);
            end = clock();
            quick_times[j] = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            container_destroy(cont_insertion);
            container_destroy(cont_quick);
        }
        
        double sum_insertion = 0, sum_quick = 0;
        for (int j = 0; j < NUMBER_OF_RUNS; j++)
        {
            sum_insertion += insertion_times[j];
            sum_quick += quick_times[j];
        }
        
        double avg_insertion = sum_insertion / NUMBER_OF_RUNS;
        double avg_quick = sum_quick / NUMBER_OF_RUNS;
        
        double variance_insertion = 0, variance_quick = 0;
        for (int j = 0; j < NUMBER_OF_RUNS; j++)
        {
            variance_insertion += pow(insertion_times[j] - avg_insertion, 2);
            variance_quick += pow(quick_times[j] - avg_quick, 2);
        }
        
        double variance_insertion_final = variance_insertion / NUMBER_OF_RUNS;
        double variance_quick_final = variance_quick / NUMBER_OF_RUNS;
        
        double speedup = avg_insertion / avg_quick;

        printf("%5d\t\t%7.2f ± %-5.2f\t\t%7.2f ± %7.2f\t%15.2fx\n", 
               n, avg_insertion, variance_insertion_final, 
               avg_quick, variance_quick_final, speedup);
    }

    return 0;
}