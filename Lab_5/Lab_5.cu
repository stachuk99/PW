#include <stdio.h>
#include <time.h>

__global__ void matrixMultiply(int *matrix1, int *matrix2, int *matrix3, int m, int p) {
    int sum=0;
    int i =  blockIdx.x*64 + threadIdx.x; 
    for (int j = 0; j < p; j++) 
    {
        for (int k = 0; k < m; k++) 
        {
            sum = sum + matrix1[m*i+k]*matrix2[p*k+j];
        }
        matrix3[p*i+j] = sum;
        sum = 0;
     }    
}

int main(int argc, char **argv)
{   
    clock_t start_cpu,stop_cpu,start_gpu,stop_gpu;
    srand(time(NULL));
    int n = 0,m,p;
    if (argc>1) {n=strtol(argv[1],NULL,10);}
    m=n;
    p=n;
    //if (argc>2) {m=strtol(argv[2],NULL,10);}
    //if (argc>3) {p=strtol(argv[3],NULL,10);}
    //printf("n: %d, m:%d, p:%d\n",n,m,p);
    int *mat1 = (int *)malloc(n*m*sizeof(int));
    int *mat2 = (int *)malloc(m*p*sizeof(int));
    int *mat3 = (int *)malloc(n*p*sizeof(int));
        
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            mat1[m*i+j] = (int) rand()%5;
        }
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            mat2[p*i+j] = (int) rand()%5;
        }
    }
    int *c_mat1;
    cudaMalloc(&c_mat1,n*m*sizeof(int));
    int *c_mat2;
    cudaMalloc(&c_mat2,m*p*sizeof(int));
    int *c_mat3;
    cudaMalloc(&c_mat3,n*p*sizeof(int));
    cudaMemcpy(c_mat1, mat1, n*m*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(c_mat2, mat2, m*p*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(c_mat3, mat3, n*p*sizeof(int), cudaMemcpyHostToDevice);
    start_cpu = clock();
    int sum=0;
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < p; j++) 
        {
            for (int k = 0; k < m; k++) 
            {
                sum = sum + mat1[m*i+k]*mat2[p*k+j];
            }
            mat3[p*i+j] = sum;
            //printf("%d, ",mat3[p*i+j]);
            sum = 0;
        }
        //printf("\n");
    }
    stop_cpu = clock();
    int time_spent = (int)(1000 * (stop_cpu - start_cpu) / CLOCKS_PER_SEC);
    printf("%d  ",time_spent);
    start_gpu = clock();
    int bloki = n / 64;
    int watki =  64;
    if (n%64 != 0 ) bloki++;   
    matrixMultiply<<<bloki,watki>>>(c_mat1,c_mat2,c_mat3,m,p);
    cudaDeviceSynchronize();
    stop_gpu = clock();
    int time_spent2 = (int)(1000 * (stop_gpu - start_gpu) / CLOCKS_PER_SEC);
    printf("%d\n",time_spent2);
    cudaMemcpy(mat3, c_mat3, n*p*sizeof(int), cudaMemcpyDeviceToHost);
    /*for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < p; j++) 
        {
            printf("%d, ",mat3[p*i+j]);
        }
        printf("\n");
    }*/
 return 0;
}

