#include<stdio.h>

int main()
{   
    FILE *fptr;
    char a[100];
    fptr=fopen("trial1.txt","w");
    // fscanf(fptr,"%s",&a);
    // printf(a);

    for(int i=8;i<210;i*=2)
        fprintf(fptr,"%d,%f\n",i,i);
    fclose(fptr);
    return 0;
}

