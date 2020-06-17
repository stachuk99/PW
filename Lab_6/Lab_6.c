#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


int triangles(int n, int* A) {
int i,j,k,t = 0;
 for (i=0;i<n;i++)
  for (j=0;j<n;j++) if (A[n*i+j])
   for (k=0;k<n;k++) { if (A[n*i+k] && A[n*j+k]) t++; }
return t/6; 
}

int triangle_task(int n, int* A, int i) 
{   
    int t = 0;
    for (int j=0;j<n;j++) if (A[n*i+j])
        for (int k=0;k<n;k++) { if (A[n*i+k] && A[n*j+k]) t++; }  
return t;
} 

int triangles_omp(int n, int* A) 
{
    int t = 0;
#pragma omp parallel for shared (t)
    for (int i=0;i<n;i++)
    {
        t+=triangle_task(n, A,i);
    }
return t/6; 
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    int n=1000;
    if (argc>1) {n =strtol(argv[1],NULL,10);}
    struct timespec start, stop;
	int* intMatrix = (int *)malloc(n*n* sizeof(int)); 
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            intMatrix[n*i+j] = (int)(rand() % 10 <= 5);
        }
    }

	clock_gettime(CLOCK_MONOTONIC, &start);

    int t = triangles(n, intMatrix);
	
	clock_gettime(CLOCK_MONOTONIC, &stop);
	double time_1 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("%lf ", time_1);


    clock_gettime(CLOCK_MONOTONIC, &start);

    int t2 = triangles_omp(n, intMatrix);

    clock_gettime(CLOCK_MONOTONIC, &stop);
	time_1 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("%lf\n", time_1);
}

