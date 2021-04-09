#include<stdio.h>
#include <mpi.h>

float Trap(float local_a,float local_b,int n,float h)
{
    float ans=0.0;
    // int n = (int)(1/h);
    for(int ii=local_a*n;ii<local_b*n;ii++)
		{
			float x=(ii+0.5)*h;
			ans+=4.0/(1.0 + x*x);
        }
    return ans*h;
}

int main(int argc , char** argv)
{
int my_rank,p,n=1024,local_n,source,dest=0,tag=0;
float a=0.0,b=1.0,h,local_a,local_b,integral,total;
double start, finish, loc_elapsed, elapsed;

MPI_Status status;
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&p);
h=(b-a)/n;
local_n = n/p;
local_a = a + my_rank*local_n*h;
local_b = local_a + local_n*h;
start = MPI_Wtime();
integral = Trap(local_a,local_b,n,h);
finish = MPI_Wtime();
loc_elapsed = finish-start;
MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
if(my_rank==0)
{
    total = integral;
    for(source=1;source<p;source++)
    {
        MPI_Recv(&integral,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
        total = total+integral;

    }
}
else
{
    MPI_Send(&integral,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
}
if(my_rank==0)
{
    printf("With n= %d trapezoids, our estimate \n",n);
    printf("of the integral from %f to %f = %f \n",a,b,total);
    printf("Elapsed time = %f milliseconds \n", elapsed * 1000);
}
MPI_Finalize();
return 0;
}
