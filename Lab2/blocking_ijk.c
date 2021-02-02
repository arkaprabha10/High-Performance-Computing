#include <stdio.h>
#include<time.h>

#include<math.h>
#include<stdlib.h>

int main()
{
    //int N=300;
    
    //
    // ... v[] and s[][] --assumed to contain valid data
    //
    double start_time,end_time;
    
    for(int t=128;t<514;t*=2){  
        int r=t,c=t,N=t,B=32,p;
        int **mat ,**a,**b,*ptr,*ptr2,*ptr3;
        
        int count = 0,i,j,k,jj,kk,len=0; 
        
      
        len = sizeof(double *) * r + sizeof(double) * c * r; 
        mat = (int **)malloc(len); 
        a = (int **)malloc(len); 
        b = (int **)malloc(len); 
        ptr = (int *)(mat + r); 
        ptr2 = (int *)(a + r); 
        ptr3 = (int *)(b + r); 
        for(i = 0; i < r; i++) {
            mat[i] = (ptr + c * i);
            a[i] = (ptr2 + c * i);
            b[i] = (ptr3 + c * i);
        }
            
            
        p=B*(N/B);
        start_time=clock();
        for(jj=0;jj<p;jj+=B)
        {
            
            for(kk=0;kk<p;kk+=B)
            {
            
                for(i=0; i<N ; ++i) 
                {
                    
                    for(j=jj; j<jj+B; ++j) 
                    {
                        count=0.0;
            
                        for(k=kk;k<kk+B;k++)
                        {
                            mat[i][j]+=a[i][k]*b[k][j];
                        }
                        
            
                    }
                }
            }
        }
        
        end_time=clock();
        printf("%lf ,",(end_time-start_time)/CLOCKS_PER_SEC);
        free(mat);
        free(a);
        free(b);
    }

    return 0;
}
