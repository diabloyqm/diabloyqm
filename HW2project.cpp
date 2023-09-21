#include <iostream>
#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

void allocate_matrix(double **a, int rows, int columns)
{
    
    // You need to do the dynamic allocation on the heap
    int temp = rows *columns;
    
    *a = new double [temp];
}

void initialize_a(double *a, int rows, int columns)

{ //You need to initialize a
    /* We want a to look like the following
    *      1  2  3
    * a =  1  1  1
    *      1  1  1
    */
    for (int i = 0; i < rows; i++)
    {for (int j = 0; j < columns; j++)
        {if (i == 0)
            {a[i * columns + j] = j + 1;
            }
        else
            {a[i * columns + j] = 1;
            }
        }
    }
}

void initialize_b(double *a, int rows, int columns)
{//You need to initialize b
    /* We want b to look like the following
    * b =  1  1  1
    */
    for (int i = 0; i < rows; i++)
    {for (int j = 0; j < columns; j++)
        {a[i * columns + j] = 1;
        }
    }
}


void initialize_c(double *a, int rows, int columns)
{
     //You need to initialize c
    /* We want c to look like the following
    * c =  0 0 0
    */
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // a[i * columns + j] = 0;
        }
        a[i] = 0;
    }
}



void product_sum(double (*a)[3], double *b, double *c)
{
    /* We want a vector s.t.
    * c = b * a + b
    */
    for (int j = 0; j < 3; j++)
    {
        c[j] = 0; 
        for (int i = 0; i < 3; i++)
        {
            c[j] += b[i] * a[i][j];
        }
        c[j] += b[j];
    }
}
void display_matrix_3(double (*c)[3])
{
    for (int i=0; i<3;i++) {
        for (int k = 0; k < 3; k++)
            cout << c[i][k] << " ";
        cout << endl;
    }

}

void display_vector(double *c, int size)
{
    for (int i=0; i<size;i++) {
        cout << c[i] << " ";
    }
}

void test_allocate_matrix() {
    string arg;
    getline(cin, arg);
    int rowSize = stoi(ltrim(rtrim(arg)));
    
    getline(cin, arg);
    int colSize = stoi(ltrim(rtrim(arg)));
    
    double * mat = NULL;
    allocate_matrix(&mat,rowSize,colSize);
    
}

void test_initialize_a() {
    double * mat;
    allocate_matrix(&mat,3,3);
    initialize_a(mat,3,3);
    display_matrix_3((double (*)[3])mat);  
}

void test_initialize_b() {
    double * vec;   
    allocate_matrix(&vec,3,3);
    initialize_b(vec,1,3);
    display_vector(vec, 3);   
}

void test_initialize_c() {
    double * vec;
    allocate_matrix(&vec,3,3);
    initialize_c(vec,1,3);
    display_vector(vec, 3);    
}

void test_product_sum() {
    double * matA = NULL;
    allocate_matrix(&matA,3,3);
    initialize_a(matA, 3, 3);
    double * vecB = NULL;
    allocate_matrix(&vecB,3,3);
    initialize_b(vecB, 1, 3);
    double * vecC = NULL;
    allocate_matrix(&vecC,3,3);
    initialize_b(vecC, 1, 3);
    product_sum((double(*)[3])matA, vecB, vecC);
    display_vector(vecC, 3);
}
