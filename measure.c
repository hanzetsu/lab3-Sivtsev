#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "container.h"
#include "struct.h"
#include <math.h>   
#include "sort.h"

#define NUMBER_OF_RUNS 10

int main()
{
    srand(time(NULL));

    int test_sizes[] = {100, 500, 2000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    printf("   N\t\tВставками (мс)\t\t\tБыстрая (мс)\t\tУскорение\n");
    printf("\t\tсреднее ± ст.откл.\tсреднее ± ст.откл.\n");
    printf("--------------------------------------------------------------------------------------\n");

    for (int k = 0; k < num_tests; k++)
    {
        int n = test_sizes[k];

        double insertion_times[NUMBER_OF_RUNS];
        double quick_times[NUMBER_OF_RUNS];
        
        struct house *houses = malloc(n * sizeof(struct house));
        if (!houses) {
            printf("Ошибка выделения памяти\n");
            return 1;
        }
        
        for (int i = 0; i < n; i++) {
            houses[i] = generate_random_house();
        }
        
        for (int j = 0; j < NUMBER_OF_RUNS; j++)
        {
            container *cont_insertion = container_create(sizeof(struct house));
            container *cont_quick = container_create(sizeof(struct house));

            for (int i = 0; i < n; i++)
            {
                container_push_back(&houses[i], cont_insertion);
                container_push_back(&houses[i], cont_quick);
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
        
        free(houses);
        
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
        

        double variance_insertion_final = variance_insertion / (NUMBER_OF_RUNS - 1);
        double variance_quick_final = variance_quick / (NUMBER_OF_RUNS - 1);
        
        double std_insertion = sqrt(variance_insertion_final);
        double std_quick = sqrt(variance_quick_final);
        
        double speedup = avg_insertion / avg_quick;

        printf("%5d\t\t%7.2f ± %-5.2f\t\t%7.2f ± %-7.2f\t%10.2fx\n", 
               n, avg_insertion, std_insertion, 
               avg_quick, std_quick, speedup);
    }

    return 0;
}