#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/hash.h"

#define maxn 19
#define TEST_MAX (sizeof(void *)*128)

int main(int argc, char **argv) {
    void *ptr;
    int i = 0;
    int min = 0, max = 0;
    int res_mine[maxn] = {0};
    int res_fugue[maxn] = {0};
    if(argc >= 2) {
        if(!strcmp("mine", argv[1])) {
            printf("Testing my hash (hashing %d times)...\n", TEST_MAX);
            while(i < TEST_MAX) {
                //printf("hash> ");
                //scanf("%p", &ptr);
                //printf(">>> %u (%u)\n", hash_pointer(ptr), compress_hash(hash_pointer(ptr), 11));
                res_mine[compress_hash(hash_pointer((void *)i), maxn)]++;
                //while(getc(stdin) != '\n');
                i++;
            }
            i=0;
            if(argc >= 3) {
                while(i < maxn) {
                    printf("%d : %d\n", i, res_mine[i]);
                    min = res_mine[i] < res_mine[min]? i : min;
                    max = res_mine[i] > res_mine[max]? i : max;
                    i++;
                }
                printf("min (%d): %d; max (%d): %d; range: %d (%.2f%%)\n",
                        min, res_mine[min], max, res_mine[max],
                        res_mine[max]-res_mine[min],
                        ((double)res_mine[max]-(double)res_mine[min])/(double)TEST_MAX*100);
            }
        } else if(!strcmp("fugue", argv[1])) {
            i=0;
            printf("Testing fugue hash (hashing %d times)...\n", TEST_MAX);
            while(i < TEST_MAX) {
                res_fugue[compress_hash(hash_pointer_using((void *)i, &fugue_hash_wrapper), maxn)]++;
                i++;
            }
            i=0;
            if(argc >= 3) {
                while(i < maxn) {
                    printf("%d : %d\n", i, res_fugue[i]);
                    min = res_fugue[i] < res_fugue[min]? i : min;
                    max = res_fugue[i] > res_fugue[max]? i : max;
                    i++;
                }
                printf("min (%d): %d; max (%d): %d; range: %d (%.2f%%)\n",
                        min, res_fugue[min], max, res_fugue[max],
                        res_fugue[max]-res_fugue[min],
                        ((double)res_fugue[max]-(double)res_fugue[min])/(double)TEST_MAX*100);
            }
        }
    }
}
