  #include<stdio.h>
  #include<stdlib.h>
  #include<time.h>
  #include<math.h>
  
  
  // call this function to start a nanosecond-resolution timer
  struct timespec timer_start()
  {
      struct timespec start_time;
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
      return start_time;
  }
  
  // call this function to end a timer, returning nanoseconds elapsed as a long
  long timer_end(struct timespec start_time){
      struct timespec end_time;
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
      // printf("%lf",end_time.tv_nsec);
      long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
      return diffInNanos;
  }
  
  /// /timespec diff(timespec start, timespec end);
  
  int main()
  {
    clock_t start_t, end_t, total_t;
      int minSize = pow(2,8);
      long long int maxSize=  pow(2,29),total;
      // printf("%d",minSize);
      // printf("%d",maxSize);
      FILE *fptr;
      fptr=fopen("data.txt","w");
      //for(int mm=0;mm<3;mm++){
      total = maxSize;
      //timespec time1, time2;
      for(long int s=minSize;s<maxSize;s*=2)
      {   
          // printf("%d",s);
          // create a dynamic list
          
          float *a,*b,*c,*d;
          a = (float*) malloc(s * sizeof(float));
          b = (float*) malloc(s * sizeof(float));
          c = (float*) malloc(s * sizeof(float));
          d = (float*) malloc(s * sizeof(float));
          for(int i=0;i<s;i++)
          {   a[i]=0;
              b[i]=3;
              c[i]=2;
              d[i]=1;
          }
          int RUNS = total/s ;//initialise arrays
          
          struct timespec start_time=timer_start();//end_time;
          //printf("%lf",start_time.tv_nsec);
          // //// a.tv_sec=;
          // //// a.tv_nsec=;
          
          
          // //clock_t end_time;
          start_t = clock();
          for(int run=0;run<RUNS;run++)
          {
              for (int i=0;i<s;i++)
               {   
                  a[i] = b[i] + c[i]*d[i];
                  
               }
              
          }
          end_t = clock();
          total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
          double end_time = timer_end(start_time);
          //// long double wall_time=(end_time-start_time)/((double)CLOCKS_PER_SEC);
          printf("%lf: ",end_time);
          
          double throughput = (double)(sizeof(double)*2*total)/total_t;
          fprintf(fptr,"%f, %d\n",throughput,s);
          free(a);
          free(b);
          free(c);
          
          
      }
      fclose(fptr);
      return 0;       
  }
  
