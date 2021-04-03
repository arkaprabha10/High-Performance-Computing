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

class Compare_Max {
    public:
    double val = 0;
    int index = 0;
};

class Compare_Min {
    public:
    double val = HUGE_VAL;
    int index = 0;
};
#pragma omp declare reduction(minimo : Compare_Min : omp_out = omp_in.val < omp_out.val ? omp_in : omp_out)
#pragma omp declare reduction(maximo : Compare_Max : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)


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
    cout<<argv[1]<<endl;
    char *pch = strtok(argv[1], "x/_");
    cout<<argv[1]<<endl;
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
    cout<<argv[1]<<endl;
}


int main(int argc, char** argv) {
    int numbThreads, constraintNumb, colNumb, ni = 0, chunk = 1,nL,nC;
    int count = 0, conta = 0;
    ifstream file(argv[1]);
    get_dimension(argv, nL, nC);
    vector<vector<double>> tableau(nL,vector<double> (nC,0));
    cout<<nL<<"  "<<nC<<endl; 
    int NMAX=(nL + nC);
    
    

    
    cout<<"begin reading\n";
    constraintNumb=nL;
    colNumb=nC;
    
    string line;
    int lin=0,i=0;
    
    cout<<"begin while\n";
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
    cout<<"read complete\n";
      
    constraintNumb--;
    colNumb--;
printf("%d %f \n", ni, tableau[constraintNumb][colNumb]);
    
  numbThreads=atoi(argv[2]);
  omp_set_num_threads(numbThreads);
    
    ni = 0;

    Compare_Max max;
    Compare_Min min;
    cout<<max.index<<" "<<max.val<<" "<<min.index<<" "<<min.val<<endl;
    struct timespec timeTotalInit, timeTotalEnd;
    bool flag=true;

    chunk=atoi(argv[3]);


    if (clock_gettime(CLOCK_REALTIME, &timeTotalInit)) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

#pragma omp parallel default(none) shared(min,max,chunk,numbThreads,count,tableau,conta,colNumb,constraintNumb,ni,NMAX,flag)
    {
        double pivot, pivot2, pivot3;
        int i, j;

#pragma omp for schedule(guided,chunk) reduction(maximo:max) 
        for (j = 0; j <= colNumb; j++)
            if (tableau[constraintNumb][j] < 0.0 && max.val < (-tableau[constraintNumb][j])) {
                max.val = -tableau[constraintNumb][j];
                max.index = j;
            }

printf("%d",max.index);
#pragma omp single nowait
        max.val = 0;

        do {

#pragma omp for reduction(+:count),reduction(minimo:min) schedule(guided,chunk) 
            for (i = 0; i < constraintNumb; i++) {
                if (tableau[i][max.index] > 0.0) {
                    pivot = tableau[i][colNumb] / tableau[i][max.index];
                    if (min.val > pivot) {
                        min.val = pivot;
                        min.index = i;
                    }
                } else
                    count++;
            }

#pragma omp single nowait 
            {
                if (count == constraintNumb) {
                    printf("Solution not found\n");
                    // printf("random");
                    flag=false;
                } else
                    count = 0;
                conta = 0;
            }

            pivot = tableau[min.index][max.index];
            // printf("%f",pivot);
            pivot3 = -tableau[constraintNumb][max.index];

#pragma omp barrier 
#pragma omp for 
            for (j = 0; j <= (colNumb); j++) {
                tableau[min.index][j] = tableau[min.index][j] / pivot;
            }

#pragma omp for nowait 
            for (i = 0; i < constraintNumb; i++) {
                if (i != min.index) {
                    pivot2 = -tableau[i][max.index];
//#pragma simd  //GCC ivdep
                    for (j = 0; j <= colNumb; j++) {
                        tableau[i][j] = (pivot2 * tableau[min.index][j]) + tableau[i][j];
                    }
                }
            }
#pragma omp barrier

//keep the above barrier
#pragma omp for reduction(+:conta),reduction(maximo:max) schedule(guided,chunk)
            for (j = 0; j <= colNumb; j++) {
                tableau[constraintNumb][j] = (pivot3 * tableau[min.index][j]) + tableau[constraintNumb][j];
                if (j < colNumb && tableau[constraintNumb][j] < 0.0) {
                    conta++;
                    if (max.val < (-tableau[constraintNumb][j])) {
                        max.val = -tableau[constraintNumb][j];
                        max.index = j;
                    }
                }
            }

#pragma omp single 
            {
                ni++;
                max.val = 0.0;
                min.val = HUGE_VAL;
                // printf("%d",ni);
            }

        } while (conta && ni<100*NMAX && flag);
    }


    if (clock_gettime(CLOCK_REALTIME, &timeTotalEnd)) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
    cout<<"end file\n";

    double ONE_SECOND_IN_NANOSECONDS = 1000000000;
    struct timespec time = My_diff(timeTotalInit, timeTotalEnd);
    double processTime = (time.tv_sec + (double) time.tv_nsec / ONE_SECOND_IN_NANOSECONDS);

    printf("%f %f ", processTime / ni, processTime);
    printf("%d %f \n", ni, tableau[constraintNumb][colNumb]);

    // tableau.erase(tableau.begin(),tableau.end());
    tableau.clear();
    tableau.shrink_to_fit();
    
    return 0;
}