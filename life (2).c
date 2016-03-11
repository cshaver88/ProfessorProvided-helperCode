 /* File:     life.c
 * Purpose:  Conway's Game of Life 
 *
 * Input:    n:  process dimensions (size of the process grid)?
 *				For example, if there are twelve MPI processes, 
 * 				the user can organize them as a 1 x 12 grid, 
 * 				a 2 x 6 grid, a 3 x 4 grid, etc.
 * Output:   State of the Life grid after each generation.
 *
 * Compile:  mpicc -g -Wall -o l life.c -lm
 * Usage:    mpiexec -n <p> ./l <n>
 *           p:  number of MPI processes
 *           n:  max int to test for primality
 *
 * Notes:
 * 1.  User should input MAX.
 * 2.  Process 0 can handle all Input and Output.
 * 3.  Each process should store its subgrid as a 1-Dimensional Array
 * 4.  Processes need to exchange information about thier local grids to update
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <mpi.h>

 /*
 Prototypes
 */ 
 int Proc_valid(int proc, int row, int col);
 int Life_valid(int p_row, int p_col, int l_row, int l_col);
 void Print_Array(int size, int l_col, int l_row);
 void Split(int rank, int local_row_size, int local_col_size,int l_row, int l_col);
 void Alive(int i, int j,int l_row, int l_col);
 void Dead(int i, int j, int l_row, int l_col);
 int Neighbors(int i, int j, int l_row, int l_col);

/*

Game Board Break Down

Process Grid -- 6 specified so block partition means 2 x 3
[0] [1] [2] 
[3] [4] [5]

Life Grid -- 4 x 6 specified by the user
proc 0 -- [0] [0] [1] [1] [2] [2]
proc 1 -- [0] [0] [1] [1] [2] [2]
proc 2 -- [3] [3] [4] [4] [5] [5]
proc 3 -- [3] [3] [4] [4] [5] [5]

*/
 /*
 Globals
 */ 
int* life_array;
int* temp_array;


 /*
 Main
 */ 
 int main (int argc, char* argv[]){
 	int procs;
 	int rank = 0;
 	if(argc != 7){
 		printf("You have entered an invalid number of input parameters.\n");
 		return -1;
 	}
 	else{
 		MPI_Comm comm;

		MPI_Init(&argc, &argv);
		comm = MPI_COMM_WORLD;
		MPI_Comm_size(comm, &procs);
		MPI_Comm_rank(comm, &rank);


	 	int p_row = atoi(argv[2]);  // taken from procs block partition  (2)
	 	int p_col = atoi(argv[3]);  // taken from procs block partition  (3)

	 	int l_row = atoi(argv[4]);  // user specified rows  (4)
	 	int l_col = atoi(argv[5]);  // user specified cols  (6)

	    // int MAX = atoi(argv[6]);  // user specified maximum number of generations  (100)
	 	int MAX = 1;    //for testing purposes

	    int proc_grid = 0;
	    int life_grid = 0;

	    proc_grid = Proc_valid(procs, p_row, p_col);
	    life_grid = Life_valid(p_row, p_col, l_row, l_col);

	    if(proc_grid == 0 || life_grid == 0){
	    	printf("You have entered an invalid grid information.\n");
	    	return -1;
	    }
	    else{
	    	int size = l_row * l_col;
	    	life_array = malloc(size * sizeof(int));
	    	temp_array = malloc(size * sizeof(int));
	    	if(rank == 0){
		    	for(int i = 0; i < size; i ++){
		    		scanf("%d", &life_array[i]);
		    		// printf("i = %d, life_array = %d\n", i, life_array[i]);
		    	}
		    }
	    	if(rank == 0){
	    		printf("Starting Array of Cells\n");
	    		Print_Array(size, l_col, l_row);
	    	}

	    	int local_row_size = l_row / p_row;
	    	int local_col_size = l_col / p_col;

	    	MPI_Barrier(comm);
	    	Split(rank, local_row_size, local_col_size,l_row, l_col);
	    	MPI_Barrier(comm);

	    	if(rank == 0){
	    		printf("End Array of Cells\n");
	    		Print_Array(size, l_col, l_row);
	    	}

		    MPI_Finalize();
		    return 0;

	    }

	}
}   /* main */



/*-------------------------------------------------------------------
 * Function:    Proc_valid
 * Purpose:     Get the input value n
 * Input args:  
 */
int Proc_valid(int proc, int row, int col){
	int valid = 0;
	if(proc != row * col){
		printf("You have entered invalid Process Grid parameters.\n");
	}
	else{
		valid = 1;
	}
	return valid;

}  /* Proc_valid */

/*-------------------------------------------------------------------
 * Function:    Life_valid
 * Purpose:     Get the input value n
 * Input args:  
 */
int Life_valid(int p_row, int p_col, int l_row, int l_col){
	int valid = 0;

	if (l_row % p_row == 0 && l_col % p_col == 0){
		valid = 1;
	}
	else{
		printf("You have entered invalid Life Grid parameters.\n");
	}
	return valid;


}  /* Life_valid */


/*-------------------------------------------------------------------
 * Function:    Print_Array
 * Purpose:     Get the input value n
 * Input args:  
 */
void Print_Array(int size, int l_col, int l_row){
		for(int i = 0; i < l_row; i ++){
			printf("\n");
			for(int j = 0; j < l_col; j ++){
				printf("%d\t", life_array[i*l_col + j]);
			}
		}
		printf("\n");

}  /* Print_Array */


/*-------------------------------------------------------------------
 * Function:    Split
 * Purpose:     Get the input value n
 * Input args:  
 */
void Split(int rank, int local_row_size, int local_col_size, int l_row, int l_col){
	int local_x;
	int local_y;

	local_x = rank / l_col;
	local_y = rank % l_col;

	for(int i = local_x; i < l_row; i ++){
		for(int j = local_y; j < l_col; j ++){
			if (life_array[j * l_col + i] == 1){
				Alive(i, j, l_row, l_col);
			}
			else{
				Dead(i, j, l_row, l_col);
			}
		}

	}
	//set the temp to be the new life_array
	//int* crap = life_array;
	life_array = temp_array;
	temp_array = life_array;

}  /* Split */


/*-------------------------------------------------------------------
 * Function:    Alive
 * Purpose:     Get the input value n
 * Input args:  
 */
void Alive(int i, int j, int l_row, int l_col){
	int count = Neighbors(i, j, l_row, l_col);

	if (count < 2 || count > 3){
		temp_array[i * l_col + j] = 0;
		//set the temp to dead
	}
	else{
		temp_array[i * l_col + j] = 1;
		//set the temp to alive
	}

}  /* Alive */


/*-------------------------------------------------------------------
 * Function:    Dead
 * Purpose:     Get the input value n
 * Input args:  
 */
void Dead(int i, int j, int l_row, int l_col){
	int count = Neighbors(i, j, l_row, l_col);

	if (count == 3){
		temp_array[i * l_col + j] = 1;
		//set temp to alive
	}
	else{
		temp_array[i * l_col + j] = 0;
		//set temp to dead
	}
}  /* Dead */


/*-------------------------------------------------------------------
 * Function:    Neighbors
 * Purpose:     Get the input value n
 * Input args:  
 */
int Neighbors(int i, int j, int l_row, int l_col){
	int count = 0;

	for(int k = -1; k <= 1; k ++){
		for(int l = -1; l <= 1; l ++){
			int row = i + l;
			int col = j + k;
			if(row < 0 || col < 0 || row > l_row || col > l_col){
				continue;
			}
			if(life_array[row * l_col + col] == 1){
				count ++;
			}
		}
	}
	return count;
}  /* Neighbors */












