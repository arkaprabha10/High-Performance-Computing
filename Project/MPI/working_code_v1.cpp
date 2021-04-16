#include<iostream>
#include<string>
#include<cmath>
#include<unordered_map>
#include<unordered_set>
#include<bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <string>
#include <omp.h>
#include <mpi.h>



/*
matrix format:
 A  b
-c  0 

filename should be shape of A i.e., with A of dimension 56x137 keep file name 56x137.txt
nL = 57 
nC = 195

to compile  $g++ -std=c++11 serial.cpp
to execute  $./a.out 56x137.txt 

*/
using namespace std;



// ofstream log_file;

struct Compare_Max {
    
    double val = 0;
    int index = 0;
}local_max,global_max;

struct Compare_Min {
    
    double val = HUGE_VAL;
    int index = 0;
}local_min,global_min;

struct timespec My_diff(struct timespec start, struct timespec end) {
    struct timespec temp;

    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void get_dimension(char** argv, int &nL, int &nC) {
    int dimension[2] = {0, 0}, i = 0;
    // cout<<argv[1]<<endl;
    char *pch = strtok(argv[1], "x/_");
    // cout<<argv[1]<<endl;
    while (pch != NULL) {

        if (i == 1) {
            dimension[0] = atoi(pch);
        }
        if (i == 2) {
            dimension[1] = atoi(pch);
        }
        pch = strtok(NULL, "x/_");
        i++;
    }
    nL = dimension[0] + 1;
    nC = dimension[0] + dimension[1] + 1;
    // cout<<argv[1]<<endl;
}


int main(int argc, char** argv) {
    int numbThreads, constraintNumb, colNumb, ni = 0, chunk = 1,nL,nC;
    int count = 0, conta = 0;
    ifstream file(argv[1]);
    double start1;
    int p,s,my_rank;
    get_dimension(argv, nL, nC);
    vector<vector<double> > tableau(nL,vector<double> (nC,0));
    // cout<<nL<<"  "<<nC<<endl; 
    int NMAX=(nL + nC);
    
    

    
    // cout<<"begin reading\n";
    constraintNumb=nL;
    colNumb=nC;
    
    string line;
    int lin=0,i=0;
    double pivot, pivot2, pivot3;
    double start_time,end_time,global_time;
    int  j;
    
    // cout<<"begin while\n";
    while (getline(file, line)) {
        istringstream iss(line);
        double value;
        i=0;
        while (iss >> value && i<nC)
        {
          tableau[lin][i]=value;
          //cout<<tableau[lin][i]<<" ";
          i++;
        }
        //cout<<endl;
        lin++;
        
    }
    // cout<<"read complete\n";
      
    constraintNumb--;
    colNumb--;

    
  numbThreads=atoi(argv[2]);

    
    ni = 0;
    struct timespec timeTotalInit, timeTotalEnd;
    int flag=0,local_flag=0;

int temp=0;


// define mpi 

MPI_Init(&argc,&argv);
start_time=MPI_Wtime();
MPI_Status status;
MPI_Request request = MPI_REQUEST_NULL;
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Comm_size(MPI_COMM_WORLD,&p);

// if(my_rank==0)
//     vector<vector<double>> temp(nL,vector<double> (nC,0));

//#LOOP 1
int start,end,local_conta;
start=my_rank*(colNumb+1)/p;
end=(my_rank+1)*(colNumb+1)/p;


for (j = start; j < end; j++)
    if (tableau[constraintNumb][j] < 0.0 && global_max.val < (-tableau[constraintNumb][j])) {
        local_max.val = -tableau[constraintNumb][j];
        local_max.index = j;
    }

MPI_Barrier(MPI_COMM_WORLD);
MPI_Allreduce(&local_max.val, &global_max.val, 1, MPI_DOUBLE, MPI_MAX,MPI_COMM_WORLD);

if(my_rank==0){

    for(int i=0;i<colNumb+1;i++)
    {
        double temp=-tableau[constraintNumb][i];
        if(global_max.val==temp)
            global_max.index=i;

    }
    
}
MPI_Bcast(&global_max.index,1,MPI_INT,0,MPI_COMM_WORLD);
///done


global_max.val = 0;
local_max=global_max;


do {

// //LOOP 2
int local_count=0;
start = my_rank*(constraintNumb)/p;
end = (my_rank+1)*(constraintNumb)/p;
for (i = start; i < end; i++) {
    if (tableau[i][global_max.index] > 0.0) {
        pivot = tableau[i][colNumb] / tableau[i][global_max.index];
        if (local_min.val > pivot) {
            local_min.val = pivot;
            local_min.index = i;
        }
    } else
        local_count++;
}


MPI_Barrier(MPI_COMM_WORLD);
MPI_Allreduce(&local_min.val, &global_min.val, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
if(my_rank==0){

    for(int i=0;i<constraintNumb;i++)
    {
        double temp=tableau[i][colNumb] / tableau[i][global_max.index];
        if(global_min.val==temp)
            global_min.index=i;

    }
    
}
MPI_Bcast(&global_min.index,1,MPI_INT,0,MPI_COMM_WORLD);
MPI_Allreduce(&local_count, &count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
local_min=global_min;

// ///DONE


if (count == constraintNumb) {
    printf("Solution not found\n");
    local_flag=1;

} else
    count = 0;
conta = 0;
MPI_Allreduce(&local_flag, &flag, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


pivot = tableau[global_min.index][global_max.index];

pivot3 = -tableau[constraintNumb][global_max.index];

// cout<<pivot<<endl;
MPI_Barrier(MPI_COMM_WORLD);
// //done

// //UPDATE PIVOT ROW
start = my_rank*(colNumb+1)/p;
end = (my_rank+1)*(colNumb+1)/p;
for (j = start; j < end; j++) {
    tableau[global_min.index][j] = tableau[global_min.index][j] / pivot;
}
MPI_Barrier(MPI_COMM_WORLD);

// ///change it to single row tranfer
if(my_rank==0)
{
    vector<vector<double>> temp_3(nL,vector<double> (nC,0));
     for(int source=1;source<p;source++)
    {
        MPI_Probe(source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&temp_3[0][0],temp_3.size()*temp_3[0].size(),MPI_DOUBLE,source,0,MPI_COMM_WORLD,&status);
        start = source*(colNumb+1)/p;
        end = (source+1)*(colNumb+1)/p;
        for (j = start; j < end; j++) {
            tableau[global_min.index][j] = temp_3[global_min.index][j];
        }
    }
    temp_3.clear();
}
else
{
    MPI_Send(&tableau[0][0],nL*nC,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
   
}
MPI_Barrier(MPI_COMM_WORLD);
MPI_Bcast(&tableau[0][0], nL*nC, MPI_DOUBLE, 0, MPI_COMM_WORLD);



//  for(int i=0;i<nL;i++)
//     {
//         for(int j=0;j<nC;j++)
//             cout<<tableau[i][j]<<" ";
//         cout<<endl;
//     }
//     cout<<endl<<endl;

// /////////////////////////////////



// //UPDATE ALL OTHER ROWS EXCEPT PIVOT ROW
start =   my_rank*(constraintNumb)/p;
end =  (my_rank+1)*(constraintNumb)/p;
for (i = start; i < end; i++) 
    {
        if (i != global_min.index) 
            {
                pivot2 = -tableau[i][global_max.index];
                for (j = 0; j <= colNumb; j++) 
                {
                    tableau[i][j] = (pivot2 * tableau[global_min.index][j]) + tableau[i][j];
                }
            }
    }

MPI_Barrier(MPI_COMM_WORLD);
// // // ////////////////////////////////////////////////
if(my_rank!=0)
     MPI_Send(&tableau[0][0],nL*nC,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
// MPI_Barrier(MPI_COMM_WORLD);

if(my_rank==0)
{
    vector<vector<double>> temp_2(nL,vector<double> (nC,0));
    for(int source=1;source<p;source++)
    {
        MPI_Probe(source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&temp_2[0][0],temp_2.size()*temp_2[0].size(),MPI_DOUBLE,source,0,MPI_COMM_WORLD,&status);
        start =  source*(constraintNumb)/p;
        end =  (source+1)*(constraintNumb)/p;
        for (i = start; i < end; i++) 
        {
            if (i != global_min.index) 
                {
                    pivot2 = -temp_2[i][global_max.index];
                    
                    for (j = 0; j <= colNumb; j++) 
                    {
                        tableau[i][j] = (pivot2 * temp_2[global_min.index][j]) + temp_2[i][j];
                        
                    }
                }
        }
        // cout<<source<<endl;
    }
    temp_2.clear();
    
}
MPI_Barrier(MPI_COMM_WORLD);

// else
// {
//     MPI_Send(&tableau[0][0],nL*nC,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
// }

MPI_Bcast(&tableau[0][0], nL*nC, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Barrier(MPI_COMM_WORLD);

for(int i=0;i<nL;i++)
{
    for(int j=0;j<nC;j++)
        cout<<tableau[i][j]<<" ";
    cout<<endl;
}
cout<<endl<<endl;


// // //keep the above barrier
// start = my_rank*(colNumb+1)/p;
// end = (my_rank+1)*(colNumb+1)/p;
// local_conta=0;
// for (j = start; j < end; j++) {
//     tableau[constraintNumb][j] = (pivot3 * tableau[global_min.index][j]) + tableau[constraintNumb][j];
//     if (j < colNumb && tableau[constraintNumb][j] < 0.0) {
//         local_conta++;
//         if (local_max.val < (-tableau[constraintNumb][j])) {
//             local_max.val = -tableau[constraintNumb][j];
//             local_max.index = j;
//         }
//     }
// }

// ///////////////////////////
// if(my_rank==0)
// {
//     vector<vector<double>> temp_1(nL,vector<double> (nC,0));
 
//     for(int source=1;source<p;source++)
//     {
//         MPI_Recv(&temp_1[0][0],temp_1.size()*temp_1[0].size(),MPI_DOUBLE,source,0,MPI_COMM_WORLD,&status);
//         start = source*(colNumb+1)/p;
//         end = (source+1)*(colNumb+1)/p;
//         for (i = start; i < end; i++) 
//         {
//             tableau[constraintNumb][i] = temp_1[constraintNumb][i];
            
//         }
//     }
//     temp_1.clear();
// }
// else 
// {
//     MPI_Send(&tableau[0][0],nL*nC,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
// }
// ////////////////////////
// MPI_Bcast(&tableau[0][0], nL*nC, MPI_DOUBLE, 0, MPI_COMM_WORLD);
// MPI_Barrier(MPI_COMM_WORLD);







// MPI_Allreduce(&local_max.val, &global_max.val, 1, MPI_DOUBLE, MPI_MAX,MPI_COMM_WORLD);
// if(my_rank==0){

//     for(int i=0;i<tableau[constraintNumb].size();i++)
//     {
//         if(tableau[constraintNumb][i]==global_max.val)
//             global_max.index=i;
//     }
    
// }
// MPI_Bcast(&global_max.index,1,MPI_INT,0,MPI_COMM_WORLD);

// MPI_Barrier(MPI_COMM_WORLD);
// cout<<"globalmax:"<<global_max.index<<"  "<<global_max.val<<endl;

// MPI_Allreduce(&local_conta, &conta, 1, MPI_INT, MPI_SUM,MPI_COMM_WORLD);
// MPI_Barrier(MPI_COMM_WORLD);
// cout<<"globalmax:"<<global_max.index<<"  "<<global_max.val<<endl;
if(my_rank==0)
{   
    ni++;
}
MPI_Bcast(&ni, 1, MPI_INT, 0, MPI_COMM_WORLD);
// MPI_Barrier(MPI_COMM_WORLD);

// global_max.val = 0.0;
// global_min.val = HUGE_VAL;
// local_max = global_max;
// local_min = global_min;

} while (conta && ni<10);


    end_time=MPI_Wtime();
    double time=end_time-start_time;
    MPI_Allreduce(&time, &global_time, 1, MPI_DOUBLE, MPI_MAX,MPI_COMM_WORLD);
    
    // MPI_Bcast(&tableau[constraintNumb][colNumb], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(my_rank==0)
    {
        cout<<global_time<<endl;
        cout<<"ans:"<<tableau[constraintNumb][colNumb]<<endl;
    }
    
    // end MPI region
    MPI_Finalize();
    
    cout<<"end file\n";
    tableau.clear();
    tableau.shrink_to_fit();
    
    return 0;
}