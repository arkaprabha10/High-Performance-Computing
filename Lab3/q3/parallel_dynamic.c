
#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))
//  Using the MONOTONIC clock 
#define CLK CLOCK_MONOTONIC

static long num_steps=100000; 
double step;
/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);

/* 
   Function to computes the difference between two time instances

   Taken from - http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/ 

   Further reading:
http://stackoverflow.com/questions/6749621/how-to-create-a-high-resolution-timer-in-linux-to-measure-program-performance
http://stackoverflow.com/questions/3523442/difference-between-clock-realtime-and-clock-monotonic
 */
struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if((end.tv_nsec-start.tv_nsec)<0){
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


int main(int argc, char* argv[])
{
	struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
	/* Should start before anything else */
	clock_gettime(CLK, &start_e2e);

	/* Check if enough command-line arguments are taken in. */
	if(argc < 3){
		printf( "Usage: %s n p \n", argv[0] );
		return -1;
	}

	int N=atoi(argv[1]);	/* size of input array */
	int P=atoi(argv[2]);	/* number of processors*/
	char *problem_name = "q3";
	char *approach_name = "MATRIX_MULTIPLICATION_SERIAL_UNBLOCKED";
//	char buffer[10];
//	FILE* inputFile;
	FILE* outputFile;
	//	inputFile = fopen(argv[3],"r");

	char outputFileName[50];		
	sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);

	//***************
	int *A = (int *)malloc((N*N) * sizeof(int));
    	int *B = (int *)malloc((N*N) * sizeof(int));
    	int *C = (int *)malloc((N*N) * sizeof(int));
    	int a,b;
    	for(a=0;a<N;a++){
    		for(b=0;b<N;b++){
    			*(A + a*N + b) = 1;
    			*(B + a*N + b) = 1;
    			*(C + a*N + b) = 0;
    		}
    	}
	//**********************
	
	omp_set_num_threads(P);
	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	/*----------------------Core algorithm starts here----------------------------------------------*/
	#pragma omp parallel
    { int i,j,k;
    #pragma omp for collapse(3) schedule(dynamic,4)
    for(i=0;i<N;i++)
	  {
		
        for(j=0;j<N;j++)
    		{
    			for(k=0;k<N;k++)
    			{
    				*(C + i*N + j) += (*(A + i*N + k) * (*(B + k*N + j)));
    			}
    		}
	  }
    }		
	//***************


	clock_gettime(CLK, &end_alg);	/* Start the algo timer */


	/* Ensure that only the algorithm is present between these two
	   timers. Further, the whole algorithm should be present. */


	/* Should end before anything else (printing comes later) */
	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);

	free(A);
	free(B);
	free(C);
	/* problem_name,approach_name,n,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
	   Change problem_name to whatever problem you've been assigned
	   Change approach_name to whatever approach has been assigned
	   p should be 0 for serial codes!! 
	 */
	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, N, P, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);

	return 0;

}
