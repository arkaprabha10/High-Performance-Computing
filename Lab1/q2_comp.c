#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdio.h>

int main()
{
    long long int minSize = pow(2, 8);
    long long int maxSize = pow(2, 29);
    long long int total = maxSize;

    FILE *outfile,*outfile2;
    outfile = fopen("results_add.txt", "w");
    outfile2 = fopen("results_div.txt", "w");

    int power = 8;
    long long int size;
    for(size = minSize; size < maxSize; size = size*2)
    {
        long long int RUNS = total/size;

        double *A = (double*)malloc(size * sizeof(double));
        double *B = (double*)malloc(size * sizeof(double));
        double *C = (double*)malloc(size * sizeof(double));
        double *D = (double*)malloc(size * sizeof(double));
        double *E = (double*)malloc(size * sizeof(double));
        double *F = (double*)malloc(size * sizeof(double));
		
        long long int i;
        long long int j;
        for(i = 0; i < size; i++)
        {
            B[i] = 100+rand()/100;
            C[i] = 100+rand()/100;
            D[i] = 100+rand()/100;
            E[i] = 100+rand()/100;
            if(E[i]==0)
              E[i]+=100;
        }

        clock_t start_time = clock();

        for(i = 0; i < RUNS; i++)
        {
            for(j = 0; j < size; j++)
            {
                A[j] = B[j] + C[j];
            }
            if(((double)i)==33.33)
              i=i;
        }

        clock_t end_time = clock();
        double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

        double throughput_add = (sizeof(double)*2*total)/cpu_time_used;

        printf("\npower: %d\t throughput_add: %.2f\n", power, throughput_add);
        
        fprintf(outfile, "%.2f,", throughput_add);
        
        
        
        clock_t start_time2 = clock();

        for(i = 0; i < RUNS; i++)
        {
            for(j = 0; j < size; j++)
            {
                F[j] = D[j]/E[j];
            }
            if(((double)i)==33.33)
              i=i;
        }

        clock_t end_time2 = clock();
        double cpu_time_used2 = ((double) (end_time2 - start_time2)) / CLOCKS_PER_SEC;

        double throughput_div = (sizeof(double)*2*total)/cpu_time_used2;

        printf("\npower: %d\t throughput_div: %.2f\n", power, throughput_div);
        
        fprintf(outfile2, "%.2f,", throughput_div);

        power += 1;

        free(A);
        free(B);
        free(C);
        free(D);
        free(E);
        free(F);

        printf("\n");
    }

    fclose(outfile);

    return 0;
}