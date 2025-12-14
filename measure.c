#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "container.h"
#include "struct.h"
#include "sort.h"

int main() {
    srand(time(NULL));
    
    int test_sizes[] = {100, 500, 1000, 2000, 4000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    printf("Сравнение производительности сортировок\n");
    printf("N\tВставками (мс)\tБыстрая (мс)\tУскорение\n");
    printf("-------------------------------------------------\n");
    
    for (int k = 0; k < num_tests; k++) {
        int n = test_sizes[k];
        
        container* cont_insertion = container_create(sizeof(struct house));
        container* cont_quick = container_create(sizeof(struct house));
        
        for (int i = 0; i < n; i++) {
            struct house h = generate_random_house();
            container_push_back(&h, cont_insertion);
            container_push_back(&h, cont_quick);
        }
        
        clock_t start, end;
        double time_insertion, time_quick;
        
        start = clock();
        container_sort(cont_insertion, comparator, 0);
        end = clock();
        time_insertion = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        
        start = clock();
        container_sort_quick(cont_quick, comparator, 0);
        end = clock();
        time_quick = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        
        double speedup = time_insertion / time_quick;
        printf("%d\t%.2f\t\t%.2f\t\t%.2fx\n", n, time_insertion, time_quick, speedup);
        
        container_destroy(cont_insertion);
        container_destroy(cont_quick);
    }
    
    return 0;
}