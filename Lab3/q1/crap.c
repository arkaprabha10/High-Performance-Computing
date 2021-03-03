
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

	int N=10;atoi(argv[1]);	/* size of input array */
	int P=2;atoi(argv[2]);	/* number of processors*/

	//**********************
	float count = 0;
 int i;
 unsigned int s=2;
 unsigned int *seed=&s;
 double r=1,x,y;
 srand(s);
  //seed(0,-r,r);
	//**********************
	omp_set_num_threads(P);
	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	/*----------------------Core algorithm starts here----------------------------------------------*/
	
  ///////////////////////////////////////////////////////////////
  
	#pragma omp parallel for private(i,x,y) shared(seed) reduction(+:count)
		for(i=0;i<N;i++){
			x = (double)rand_r(seed)/(double)RAND_MAX;
			y = (double)rand_r(seed)/(double)RAND_MAX;

			printf("%d %d %d %f %f\n",seed,omp_get_thread_num(),i,x,y);
			if((x*x + y*y) < 1)
			  count=count+1;
		}
  printf("%f",4.0*count/N);

	
	

	
	/*----------------------Core algorithm finished--------------------------------------------------*/

	
	return 0;

}
