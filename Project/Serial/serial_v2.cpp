#include<bits/stdc++.h>

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

// double **tableau;
// ofstream log_file;

class Compare_Max {
    public :
    double val = 0;
    int index = 0;
};

class Compare_Min {
    public:
    double val = HUGE_VAL;
    int index = 0;
};

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
  //cout<<argv[1]<<endl;
    char *pch = strtok(argv[1], "x/_");
    
    while (pch != NULL) {

        if (i == 1) {
            dimension[0] = atoi(pch);
        }
        //cout<<atoi(pch)<< " " <<i<<endl;
        if (i == 2) {
            dimension[1] = atoi(pch);
        }
       
        pch = strtok(NULL, "x/_");
        
        i++;
    }
    
    cout<<dimension[0]<<endl<<dimension[1]<<endl;
    nL = dimension[0] + 1;
    nC = dimension[0] + dimension[1] + 1;
 //   cout<<nL<<endl<<nC<<endl;
}


int main(int argc, char** argv) {
    int numbThreads, constraintNumb, colNumb, ni = 0, chunk = 1,nL,nC;
    int count = 0, conta = 0;
    ifstream file(argv[1]);
    get_dimension(argv, nL, nC);
    vector<vector<double>> tableau(nL+1,vector<double> (nC+1,0));
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
        //   if(value==NULL)
        //     break;  
          tableau[lin][i]=value;
          //cout<<tableau[lin][i]<<" ";
          i++;
        }
        // cout<<i<<endl;
        //cout<<endl;
        lin++;
        
    }
//     cout<<"read complete\n";
    
    
    constraintNumb=nL;
    colNumb=nC;
    
    constraintNumb--;
    colNumb--;

//     ni = 0;

    struct timespec timeTotalInit, timeTotalEnd;

bool flag=true;
    Compare_Max max;
    Compare_Min min;

    if (clock_gettime(CLOCK_REALTIME, &timeTotalInit)) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }


    double pivot, pivot2, pivot3;
    int j;

//cout<<"loop will start\n";

//comment1 from here


    for (j = 0; j <= colNumb; j++)
        if (tableau[constraintNumb][j] < 0.0 && max.val < (-tableau[constraintNumb][j])) {
            max.val = -tableau[constraintNumb][j];
            max.index = j;
        }
//   cout<<min.index<<" "<<max.index<<endl;


    max.val = 0;
//cout<<"do will start"<<endl;
    do {

//         //cout<<constraintNumb<<endl;
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
    
//       // cout<<"First for ended\n";
//       //  cout<<count<<constraintNumb<<endl;
    
        if (count == constraintNumb) {
            cout<<"gibberish"<<endl;
            flag=false;
            //exit(1);
        } 
        else
            count = 0;
            
        conta = 0;
    
//      //   cout<<min.index<<" "<<max.index<<endl;
        
        pivot = tableau[min.index][max.index];
        pivot3 = -tableau[constraintNumb][max.index];

     //  cout<<"pivot found\n";
        for (j = 0; j <= (colNumb); j++) {
            tableau[min.index][j] = tableau[min.index][j] / pivot;
        }
        
//      //   cout<<"second for ended\n";

        for (i = 0; i < constraintNumb; i++) {
            if (i != min.index) {
                pivot2 = -tableau[i][max.index];

                for (j = 0; j <= colNumb; j++) {
                    tableau[i][j] = (pivot2 * tableau[min.index][j]) + tableau[i][j];
                }
            }
        }
//      //   cout<<"third for ended\n";

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
//     // cout<<"fourth for ended\n";

    
        ni++;
        max.val = 0.0;
        min.val = HUGE_VAL;
        cout<<ni<<endl;
    } while (conta && ni<1000*NMAX  && flag);
    

    if (clock_gettime(CLOCK_REALTIME, &timeTotalEnd)) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

    double ONE_SECOND_IN_NANOSECONDS = 1000000000;
    struct timespec time = My_diff(timeTotalInit, timeTotalEnd);
    double processTime = (time.tv_sec + (double) time.tv_nsec / ONE_SECOND_IN_NANOSECONDS);

    printf("%f %f ", processTime / ni, processTime);
    printf("%d %f \n", ni, tableau[constraintNumb][colNumb]);

    //delete_matrix(tableau, constraintNumb);
    // log_file.close();
    return 0;

}