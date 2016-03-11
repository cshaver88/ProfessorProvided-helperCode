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
 * Usage:    mpiexec -n <p> ./l 2 3 4 6 100
 * 			 mpiexec -n 6 ./l 2 3 4 6 100 < life.txt
 *           p:  number of MPI processes
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
 void Join(int local_size, int l_row, int l_col);
 int Proc_valid(int proc, int row, int col);
 int Life_valid(int p_row, int p_col, int l_row, int l_col);
 void Print_Grid(int size, int l_row, int l_col);
 void Print_Array(int* array, int local_size, int rank);
 void Split(int rank, int local_size, int l_row, int l_col, 
 	int local_row_size, int local_col_size, int p_row, int p_col, 
 	MPI_Comm comm, int* local_array);
 void Neighbors(int rank, int p_row, int p_col, int* local_array, 
 	int local_row_size, int local_col_size, MPI_Comm comm);
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
 	int local_row_size;
 	int local_col_size;
 	int* local_array;
 	int p_row = 2;  //default
 	int p_col = 3;  //default
 	int l_row = 4;  //default
 	int l_col = 6;  //default
 	int MAX = 100;  //default
 	int size = l_row * l_col;

 	MPI_Comm comm;

	MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &procs);
	MPI_Comm_rank(comm, &rank);

 	if(argc != 6){
 		printf("You have entered an invalid number of input parameters.\n");
                int i;
                for (i = 0; i < argc; i++)
                   printf("Proc %d > argv[%d] = %s\n", rank, i, argv[i]);
        MPI_Finalize();
 		return -1;
 	}
 	else {
 		if(rank == 0){
		 	p_row = atoi(argv[1]);  // taken from procs block partition  (2)
		 	p_col = atoi(argv[2]);  // taken from procs block partition  (3)

		 	l_row = atoi(argv[3]);  // user specified rows  (4)
		 	l_col = atoi(argv[4]);  // user specified cols  (6)

		    MAX = atoi(argv[5]);  // user spec maximum # generations  (100)
		 	// int MAX = 1;    //for testing purposes

		    int proc_grid = 0;
		    int life_grid = 0;

		    proc_grid = Proc_valid(procs, p_row, p_col);
		    life_grid = Life_valid(p_row, p_col, l_row, l_col);


		    if(proc_grid == 0 || life_grid == 0){
		    	printf("You have entered an invalid grid information.\n");
		    	MPI_Finalize();
		    	return -1;
		    }
		    else{
		    	life_array = malloc(size * sizeof(int));
			    for(int i = 0; i < size; i ++){
			    	scanf("%d", &life_array[i]);
			    	// printf("i = %d, life_array = %d\n", i, life_array[i]);
			    }
		    	printf("Starting Array of Cells\n");
		    	Print_Grid(size, l_row, l_col);

		    	local_row_size = l_row / p_row;
		    	local_col_size = l_col / p_col;

		    	int mini = local_row_size * local_col_size;
		    	temp_array = malloc(mini * sizeof(int));
		    }
	    }
	    //this is for all of the processes not just process 0
	    	MPI_Bcast(&local_row_size, 1, MPI_INT, 0, comm);
		    MPI_Bcast(&local_col_size, 1, MPI_INT, 0, comm);
		    
		    int local_size = local_row_size * local_col_size;
		    local_array = malloc(local_size * sizeof(int));
			
			// printf("local_size: %d, rank: %d\n", local_size, rank);
		    Split(rank, local_size, l_row, l_col, local_row_size, 
		    	local_col_size, p_row, p_col, comm, local_array);

		    for (int m = 0; m < MAX; m ++){
		    	printf("RANK: %d\n", rank);
			    Neighbors(rank, p_row, p_col, local_array, local_row_size, 
			    	local_col_size, comm);

			    MPI_Barrier(comm);
			    
			    //have all processes send their local arrays to 0
			    //0 needs to take all that data and put it into life_array

			    MPI_Send(local_array, local_size, MPI_INT, 0, 0, comm);
			    MPI_Recv(temp_array, local_size, MPI_INT, rank, 0, 
			    	comm, MPI_STATUS_IGNORE);

			    Join(local_size, l_row, l_col);

			    if(rank == 0){
			    	printf("Life Grid after a generation.\n");
			    	Print_Grid(size, l_row, l_col);
			    }
			}

		    MPI_Finalize();
		    return 0;
	}
}   /* main */


/*-------------------------------------------------------------------
 * Function:    Join
 * Purpose:     Collect the mini arrays from procs to 0
 * Input args:  local size = size of the mini array
 *				l row = # of rows in the life grid
 *				l col = # of columns in the life grid
 */
void Join(int local_size, int l_row, int l_col){

	for (int i = 0; i < l_row; i ++){
		for (int j = 0; j < l_col; j ++){
			int n = i * l_col + j;
			for (int k = 0; k < local_size; k ++){
				life_array[n] = temp_array[k];
			}
		}
	}
}  /* Join */



/*-------------------------------------------------------------------
 * Function:    Proc_valid
 * Purpose:     Determine whether or not this is a valid # of 
 * 				processes and grid size
 * Input args:  proc = # processes
 *				row = # rows given by user
 *				col = # columns given by user
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
 * Purpose:     Determine whether or not this is a valid # of 
 * 				rows and cols for the # of processes to be block partitioned
 * Input args:  p row = # rows in the process grid
 *				p col = # columns in the process grid
 *				l row = # rows in the life grid 
 *				l col = # columns in the life grid 
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
 * Function:    Print_Grid
 * Purpose:     Print out the life grid
 * Input args:  size = size of the life grid
 * 				l row = # of rows in the life grid
 * 				l col = # of columns in the life grid
 */
void Print_Grid(int size, int l_row, int l_col){
		for(int i = 0; i < l_row; i ++){
			printf("\n");
			for(int j = 0; j < l_col; j ++){
				printf("%d\t", life_array[i*l_col + j]);
			}
		}
		printf("\n");

}  /* Print_Grid */


/*-------------------------------------------------------------------
 * Function:    Print_Array
 * Purpose:     Print the individual arrays of a process for testing
 * Input args:  array = local array held by the process
 * 				local size = size of the array
 * 				rank = process #
 */
void Print_Array(int* array, int local_size, int rank){
		for(int i = 0; i < local_size; i ++){
			printf("Process %d %d :  %d\t", rank, i, array[i]);
		}
		printf("\n");
}  /* Print_Array */



/*-------------------------------------------------------------------
 * Function:    Split
 * Purpose:     Take the life grid and break it into blocks for each process 
 * 				to work with 
 * Input args:  rank = process #
 * 				local size = size of the process array
 * 				l row = # of rows in the life grid
 * 				l col = # of cols in the life grid
 * 				local row size = # of rows in the process local array
 * 				local col size = # of cols in the process local array
 * 				p row = # rows in the proc grid
 * 				p col = # cols in the proc grid
 * 				comm = MPI communicator
 * 				local array = the actual array held by a proc
 */
void Split(int rank, int local_size, int l_row, int l_col, int local_row_size, 
	int local_col_size, int p_row, int p_col, MPI_Comm comm, int* local_array){

	if (rank == 0){
		int count = 0;
		for (int i = 0; i < l_row; i ++){
			for (int j = 0; j < l_col; j ++){
				int temp_r = i / local_row_size;  
				int temp_c = j / local_col_size;  
				int dest = temp_r * p_col + temp_c;
				int n = i * l_col + j;
				if (dest != 0){
					MPI_Send(&life_array[n], 1, MPI_INT, dest, 0, comm);
				}
				else{
					local_array[count] = life_array[n];
					count ++;
				}
			}
		}
	}
	else{
		for(int i = 0; i < local_size; i ++){
			MPI_Recv(&local_array[i], 1, MPI_INT, 0, 0, comm, MPI_STATUS_IGNORE);
		}
	}
}  /* Split */



/*-------------------------------------------------------------------
 * Function:    Neighbors
 * Purpose:     Get the processes next to the given process, look at the
 * 				portion of the array that is relevant to the given process
 * 				and count how many 1's are within that greater block of 
 * 				information. Depending on the # of 1's and 0's the given 
 * 				process' local array will either change or stay the same
 * Input args:  rank = process #
 * 				p row = # rows in the process grid
 * 				p col = # cols in the process grid
 * 				local array = the process' local array
 * 				local row size = # of rows in the local array
 * 				local col size = # of cols in the local array
 * 				comm = MPI communicator
 */
void Neighbors(int rank, int p_row, int p_col, int* local_array, 
	int local_row_size, int local_col_size, MPI_Comm comm){
	int count = 0;  //counts how many alive cells there are

	int a = rank / p_col;   //gets the "middle" of the enlarged square
	int b = rank % p_col;

	int new1 = local_row_size + 2; //creates the enlarged square space
	int new2 = local_col_size + 2;
	int size = new1 * new2; 
	int* temp = malloc(size * sizeof(int));

//sending and receiving only
	int partner;

	for(int k = -1; k <= 1; k ++){
		for(int l = -1; l <= 1; l ++){
			int row = a + l;  //this gets the outer area around the mini square
			int col = b + k;  //this gets the outer area around the mini square
			// printf("row: %d  col: %d \n", row, col);
			if(row < 0 || col < 0 || row > p_row || col > p_col){  
				//this checks to see if it is one of the outer parts of the life grid, if so 
				//Do no sending and no receiving since there is nothing to send to or receive from
				continue;
			}
			else{

				partner = row * p_col + col;
				printf("Partner: %d\n", partner);
				//This is supposed to 1. send and 2. receive for the non edge cases
				if(row == 0 && col == 0){
					//Should this be my else and have the case 2 from notes be here instead?
					//Center
					//it is the center piece don't do anything
					continue;
				}
				else if (row < 0 && col == 0){
					//Top Middle
					MPI_Send(local_array, local_col_size, MPI_INT, partner, 
						0, comm);
					MPI_Recv(temp, local_col_size, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
					
				}
				else if (row > 0 && col == 0){
					//Bottom Middle
					MPI_Send(local_array, local_col_size, MPI_INT, partner, 
						0, comm);
					MPI_Recv(temp, local_col_size, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				else if (row == 0 && col < 0){
					//Left Middle
					MPI_Send(local_array, local_row_size, MPI_INT, partner, 
						0, comm);
					MPI_Recv(temp, local_col_size, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				//not getting this
				else if (row == 0 && col > 0){
					//Right Middle
					MPI_Send(local_array, local_row_size, MPI_INT, partner, 
						0, comm);
					MPI_Recv(temp, local_col_size, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				//Corners
				else if (row < 0 && col < 0){
					//Top Left Corner
					MPI_Send(local_array, 1, MPI_INT, partner, 0, comm);
					MPI_Recv(temp, 1, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				//not getting this
				else if (row < 0 && col > 0){
					// Top Right Corner
					MPI_Send(local_array, 1, MPI_INT, partner, 0, comm);
					MPI_Recv(temp, 1, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				else if (row > 0 && col < 0){
					//Bottom Left Corner
					MPI_Send(local_array, 1, MPI_INT, partner, 0, comm);
					MPI_Recv(temp, 1, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
				//not getting this
				else{
					//Bottom Right Corner
					MPI_Send(local_array, 1, MPI_INT, partner, 0, comm);
					MPI_Recv(temp, 1, MPI_INT, partner, 0, 
						comm, MPI_STATUS_IGNORE);
				}
			}
			// for(int i = 0; i < local_col_size; i ++){
			// 		printf("temp:  %d\n", temp[i]);
			// 	}
		}
	}
//this is to count the neighbors and call alive or dead to change 
//the 0s or 1s accordingly
	for(int i = 0; i < local_row_size; i ++){
		for(int j = 0; j < local_col_size; j ++){
			for(int k = -1; k <= 1; k ++){
				for(int l = -1; l <= 1; l ++){
					int row = i + l;
					int col = j + k;
					if(row < 0 || col < 0 || row > new1 || col > new2){
						//youre out of bounds of the temp array
						continue;
					}  
					if(temp[row * local_col_size + col] == 1){
						count ++;
					}
				}
			}
			if (local_array[i * local_col_size + j] == 1){
				if (count < 2 || count > 3){
					local_array[i * local_col_size + j] = 0;
				}
				else{
					local_array[i * local_col_size + j] = 1;
				}
			}
			else{
				if (count == 3){
					local_array[i * local_col_size + j] = 1;
				}
				else{
					local_array[i * local_col_size + j] = 0;
				}
			}
		} 
	}
}  /* Neighbors */



