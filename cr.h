/***************************************************************************
*   Copyright (C) 2006 by martin lukac   				  *
*   lukacm@ece.pdx.edu   						  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
//Uncomment to allow multi-threaded computation for smaller circuits 8 < qubits
//#define __SSIZE__
//Uncoment to allow QMDD representation for circuits up to 7 qubits
//#define __QMDD__
//Uncoment to unleash the standard things
//#define __STDR__
//#define __TIMING__

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <complex>
#include <sstream>

//#include <cutil.h>
//#include <cutil_inline.h>
//#include "cublas.h"
//#include "cuda.h"

//#include <gsl_cblas.h>
//#include <gsl_complex_math.h>
//#include <gsl_permutation.h>
//#include <gsl_blas.h>
//#include <gsl_linalg.h>

using namespace std;

// Thread block size
#define MAXGATEINPUT 20
#define BLOCK_SIZE 2

// max number of of inputs
const int MAXVARS = 100;
// max number of generated base functions
const int MAXGEN = 1000000;	

//int base_functions;
// input file stream
static ifstream in_stream;	
// output file stream
static ofstream out_stream;	

/*typedef struct Bfunction {
	gsl_matrix * matrix; //original matrix
	gsl_matrix * inverse; //inverse matrix
	gsl_matrix * ludecomp; //LU decomposition
	int size; //number of inputs
	double det; // determinant
	gsl_permutation * perm; // permutation matrix
} Bfunction;
*/
typedef struct qGate
{
// number of input/output wires
	int numIO;		
	int realIO;		
// cost of the gate
	int Cost;		
// representation
	int representation;
// matrix representing this gate
	int *gateMatrix1;	
//The String Name of this gate
	string my_string;	
	int valuedness;
	int restrictions_number;
	//specifies wich wires can be used as placement for this gate
	int restrictions[MAXGATEINPUT];
	int connections[MAXGATEINPUT];
} qGate;

typedef struct MCT {
	short *controls;
	short target;
} MCT;

typedef struct Kmap {
//	gsl_matrix * function;
	int inputs;
} Kmap;


typedef struct Mresult
{
	int results;
//	gsl_matrix ** resulting_bfunctions;
//	gsl_matrix ** resulting_inv_bfunctions;
//	gsl_matrix ** resulting_bfunction_kmap;
} Mresult;

typedef struct result
{
	float avFitness [100000];
	float avError[100000];
	float avCost[100000];
	int counter;
	int step;
} result;

typedef struct pattern
{
	

} pattern;

typedef struct Solution
{
	int ioNumber;
	float error;
	float fitness;
	float cost;
	string my_string;
} Solution;

typedef struct Node
{
	char data;
	int id,jd;
	Node *left;
	Node *right;
} Node;

void read_pla(string);
int read_pla_file(const char*, int*, int**, int**);	
void order_level_tree(int, int, int, int, int, int**, int, int**);
int minimize_pla(int, int*, int***, int***, char**, char**);
int decompose_pla_v(int, int, int*, int**, int**, int***, char**);
int process_pla(int, int, int*, int**, int**, int*);
int calculate_entropy(int, int**, int*, int*);
int calculate_stats(int, int**, int*, int*);
int row_swap(int, int**, int*, int*);
int main(int, char**);
int remove_top_bit_common(int, int***, int*, int*, char**);
int get_top_bottom_cc(int, int*, int***, int***, int*, int*);
int get_top_bottom_cc_two(int, int*, char**, int*, int*, int*);
int merge_compat_cols(int, int*, int*, int***, char**);
int merge_compat_cols_top_lvl(int, int, int*, int*, int*, int***, char**);
int transform_tof_to_cvc(int, int, int*, int*, int*, int***, char**, char** );
int reintr_dontcrs(int, int***, char**, int*, int*);

