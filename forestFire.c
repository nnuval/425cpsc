/* Nate Nuval
 * Finlayson
 * CPSC 425
 * Forest Fire Simulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

/* Checks if the cell is a tree */
int isTree(char cell){
	if(cell == 'T'){
		return 1;
	}else{
		return 0;
	}
}

/* Checks if the cell is a burning tree */
int isBurningTree(char cell){
	if(cell=='X'){
		return 1;
	}else{
		return 0;
	}
}

/* Checks if the cell is a newline character */
int isNewline(char cell){
	if(cell=='\n'){
		return 1;
	}else{
		return 0;
	}
}

/* Tests the surrounding cells and returns the correct character for the next
 * generation */
char testTree(int i, int row, int col,unsigned char * arr, double I, double
		G,int RANK,int SIZE){	
	int TL = arr[i-82]; // Top-left neighbor     [TL][ T ][TR]
	int T = arr[i-81];  // Top neighbor          [L ][You][ R]
	int TR = arr[i-80]; // Top-right neighbor    [BL][ B ][BR]
	int R = arr[i+1];   // Right neighbor        
	int BR = arr[i+82]; // Bottom-right neighbor
	int B = arr[i+81];  // Bottom neighbor
	int BL = arr[i+80]; // Bottom-left neighbor
	int L = arr[i-1];   // Left neighbor
	int trees = 0;
	int burning = 0;
	
	/* if the cell is a burning tree */
	if (isBurningTree(arr[i])){
		return ' '; /* it will be an empty space in the next generation */
	}
	
	/* Top row. Only applies to the first process */
	if(row == 0 && RANK == 0){
		if(col == 0){ //top left corner
			if(isTree(R)){                // [cell][test]->
				trees++;                  // [    ][    ]->
			}else if (isBurningTree(R)){  //    |     |
				burning++;                //    V     V
			}

			if(isTree(BR)){               // [cell][    ]->
				trees++;                  // [    ][test]->
			}else if (isBurningTree(BR)){ //    |     |
				burning++;                //    V     V
			}
														
			if(isTree(B)){                // [cell][    ]->
				trees++;                  // [test][    ]->
			}else if (isBurningTree(B)){  //    |     |
				burning++;                //    V     V
			}

		}else if(col == 79){ //top right corner
			if(isTree(L)){                // <-[test][cell] 
				trees++;                  // <-[    ][    ]
			}else if (isBurningTree(L)){  //      |     |
				burning++;                //      V     V
			}
			if(isTree(BL)){               // <-[    ][cell] 
				trees++;                  // <-[test][    ]
			}else if (isBurningTree(BL)){ //      |     |
				burning++;                //      V     V
			}
			if(isTree(B)){                // <-[    ][cell] 
				trees++;                  // <-[    ][test]
			}else if (isBurningTree(B)){  //      |     |
				burning++;                //      V     V
			}

		}else{
			if(isTree(L)){                // <-[test][cell][    ]-> 
				trees++;                  // <-[    ][    ][    ]->
			}else if(isBurningTree(L)){   //     |     |     | 
				burning++;                //     V     V     V
			}	
			if(isTree(BL)){               // <-[    ][cell][    ]-> 
				trees++;                  // <-[test][    ][    ]->
			}else if(isBurningTree(BL)){  //      |     |     | 
				burning++;                //      V     V     V
			}	
			if(isTree(B)){                // <-[    ][cell][    ]-> 
				trees++;                  // <-[    ][test][    ]->
			}else if(isBurningTree(B)){   //      |     |     | 
				burning++;                //      V     V     V
			}	
			if(isTree(BR)){               // <-[    ][cell][    ]-> 
				trees++;                  // <-[    ][    ][test]->
			}else if(isBurningTree(BR)){  //      |     |     | 
				burning++;                //      V     V     V
			}	
			if(isTree(R)){                // <-[    ][cell][test]-> 
				trees++;                  // <-[    ][    ][    ]->
			}else if(isBurningTree(R)){   //      |     |     | 
				burning++;                //      V     V     V
			}	
		}

	/* Bottom row. Only applies to the last process */
	}else if(row == (((3240/SIZE)/81)-1) && RANK == (SIZE-1)){
		if(col == 0){ //bottom left corner
			if(isTree(T)){                //   ^     ^
				trees++;                  //   |     |
			}else if(isBurningTree(T)){   // [test][    ]->
				burning++;                // [cell][    ]->
			}
			if(isTree(TR)){               //   ^     ^
				trees++;                  //   |     |
			}else if(isBurningTree(TR)){  // [    ][test]->
				burning++;                // [cell][    ]->
			}
			if(isTree(R)){                //   ^     ^
				trees++;                  //   |     |
			}else if(isBurningTree(R)){   // [    ][    ]->
				burning++;                // [cell][test]->
			}
		
		}else if(col == 79){ //bottom right corner
			if(isTree(T)){                //     ^     ^
				trees++;                  //     |     |
			}else if(isBurningTree(T)){   // <-[    ][test]
				burning++;                // <-[    ][cell]
			}
			if(isTree(TL)){               //     ^     ^
				trees++;                  //     |     |
			}else if(isBurningTree(TL)){  // <-[test][    ]
				burning++;                // <-[    ][cell]
			}
			if(isTree(L)){                //     ^     ^
				trees++;                  //     |     |
			}else if(isBurningTree(L)){   // <-[    ][    ]
				burning++;                // <-[test][cell]
			}
		}else{
			if(isTree(L)){                //     ^     ^     ^
				trees++;                  //     |     |     |
			}else if(isBurningTree(L)){   // <-[    ][    ][    ]->
				burning++;                // <-[test][cell][    ]->
			}	
			if(isTree(TL)){               //     ^     ^     ^
				trees++;                  //     |     |     |
			}else if(isBurningTree(TL)){  // <-[test][    ][    ]->
				burning++;                // <-[    ][cell][    ]->
			}	
			if(isTree(T)){                //     ^     ^     ^
				trees++;                  //     |     |     |
			}else if(isBurningTree(T)){   // <-[    ][test][    ]->
				burning++;                // <-[    ][cell][    ]->
			}	
			if(isTree(TR)){               //     ^     ^     ^
				trees++;                  //     |     |     |
			}else if(isBurningTree(TR)){  // <-[    ][    ][test]->
				burning++;                // <-[    ][cell][    ]->
			}	
			if(isTree(R)){                //     ^     ^     ^
				trees++;                  //     |     |     |
			}else if(isBurningTree(R)){   // <-[    ][    ][    ]->
				burning++;                // <-[    ][cell][test]->
			}	
		}
	}else{

		/* Left side */
		if(col == 0){
			if(isTree(T)){               //   ^     ^
				trees++;                 // [test][    ]>
			}else if(isBurningTree(T)){  // [cell][    ]>
				burning++;               // [    ][    ]>
			}                            //    V     V

			if(isTree(TR)){              //   ^     ^
				trees++;                 // [    ][test]>
			}else if(isBurningTree(TR)){ // [cell][    ]>
				burning++;               // [    ][    ]>
			}                            //    V     V

			if(isTree(R)){               //   ^     ^
				trees++;                 // [    ][    ]>
			}else if(isBurningTree(R)){  // [cell][test]>
				burning++;               // [    ][    ]>
			}                            //    V     V

			if(isTree(BR)){              //   ^     ^
				trees++;                 // [    ][    ]>
			}else if(isBurningTree(BR)){ // [cell][    ]>
				burning++;               // [    ][test]>
			}                            //    V     V

			if(isTree(B)){               //   ^     ^
				trees++;                 // [    ][    ]>
			}else if(isBurningTree(B)){  // [cell][    ]>
				burning++;               // [test][    ]>
			}                            //    V     V
		
		/* Right side */
		}else if(col == 79){
			if(isTree(T)){               //     ^     ^
				trees++;                 // <[    ][test]
			}else if(isBurningTree(T)){  // <[    ][cell]
				burning++;               // <[    ][    ]
			}                            //     V     V

			if(isTree(TL)){              //     ^     ^
				trees++;                 // <[test][    ]
			}else if(isBurningTree(TL)){ // <[    ][cell]
				burning++;               // <[    ][    ]
			}                            //     V     V

			if(isTree(L)){               //     ^     ^
				trees++;                 // <[    ][    ]
			}else if(isBurningTree(L)){  // <[test][cell]
				burning++;               // <[    ][    ]
			}                            //     V     V

			if(isTree(BL)){              //     ^     ^
				trees++;                 // <[    ][    ]
			}else if(isBurningTree(BL)){ // <[    ][cell]
				burning++;               // <[test][    ]
			}                            //     V     V

			if(isTree(B)){               //     ^     ^
				trees++;                 // <[    ][    ]
			}else if(isBurningTree(B)){  // <[    ][cell]
				burning++;               // <[    ][test]
			}                            //     V     V
		
		/* All other cell */
		}else{
			if(isTree(L)){               //    ^     ^     ^  
				trees++;                 // <[    ][    ][    ]>
			}else if(isBurningTree(L)){  // <[test][cell][    ]>
				burning++;               // <[    ][    ][    ]>
			}	                         //     V     V     V

			if(isTree(TL)){              //    ^     ^     ^  
				trees++;                 // <[test][    ][    ]>
			}else if(isBurningTree(TL)){ // <[    ][cell][    ]>
				burning++;               // <[    ][    ][    ]>
			}	                         //     V     V     V

			if(isTree(T)){               //    ^     ^     ^  
				trees++;                 // <[    ][test][    ]>
			}else if(isBurningTree(T)){  // <[    ][cell][    ]>
				burning++;               // <[    ][    ][    ]>
			}	                         //     V     V     V

			if(isTree(TR)){              //    ^     ^     ^  
				trees++;                 // <[    ][    ][test]>
			}else if(isBurningTree(TR)){ // <[    ][cell][    ]>
				burning++;               // <[    ][    ][    ]>
			}	                         //     V     V     V

			if(isTree(R)){               //    ^     ^     ^  
				trees++;                 // <[    ][    ][    ]>
			}else if(isBurningTree(R)){  // <[    ][cell][test]>
				burning++;               // <[    ][    ][    ]>
			}	                         //     V     V     V

			if(isTree(BR)){              //    ^     ^     ^  
				trees++;                 // <[    ][    ][    ]>
			}else if(isBurningTree(BR)){ // <[    ][cell][    ]>
				burning++;               // <[    ][    ][test]>
			}	                         //     V     V     V

			if(isTree(B)){               //    ^     ^     ^  
				trees++;                 // <[    ][    ][    ]>
			}else if(isBurningTree(B)){  // <[    ][cell][    ]>
				burning++;               // <[    ][test][    ]>
			}	                         //     V     V     V

			if(isTree(BL)){              //    ^     ^     ^  
				trees++;                 // <[    ][    ][    ]>
			}else if(isBurningTree(BL)){ // <[    ][cell][    ]>
				burning++;               // <[test][    ][    ]>
			}	                         //     V     V     V

		}
	}

	/* Probability from a random number */
	double prob = (double)rand()/(double)RAND_MAX;
	
	/* If the cell is a tree... */
	if(isTree(arr[i])){
		/* ...and has a neighbor burning */
		if(burning){
			return 'X'; /* the tree will burn in the next generation */
		}else{
			/* has no burning neighbors, apply ignition probability */
			if(prob <= I){
				return 'X'; /* the tree will burn in the next generation */
			}
		}
		return 'T';
	}else{
		/* An empty space will turn into a tree based on the growth probability */
		if(prob<=(G*(trees+1))){
		    return 'T';
		}
		return ' ';
	}
}

int main(int argc, char** argv){
	int rank, size;

	/* initialize MPI */
	MPI_Init(&argc, &argv);

	/*get the rank (process id) and size (number of processes) */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* calculate the start and end points by evenly dividing the range */
	int start = (3240/ size) * rank;
	int end = start + (3240/size) - 1;
	
	/* The last process needs to do all the remaining ones */
	if (rank == (size - 1)){
		end = 3239;
	}
	
	/* The partial array holds the row that needs to be passed between 
	 * processes. The chunk array holds the entire section each process
	 * works on */
	unsigned char partial[81], chunk[3240/size];
	MPI_Status status;


	int N; // Number of generations
	double I, G; // probability of ignition & growth
	FILE * file;
	unsigned char forest[3240], nextGen[3240]; // array holding the forest and
	                                           // the next generation of the
											   // forest
	srand((unsigned)time(NULL)+rank); // seed for random

	/* makes sure user enters all the necessary values */
	if(argc < 5){
		printf("Error: not enough arguments passed.\n");
		return 0;
	}

	file = fopen(argv[1], "r"); // opens file for reading
	N = atoi(argv[2]); 
	I = atof(argv[3]);
	G = atof(argv[4]);

	/* copies file into the forest array */
	if(fread(forest, sizeof(unsigned char), 3240, file) != 3240){
		printf("Error\n");
	}else{
//		printf("All read\n");
	}
	
	int i,j,k, temp, curGen;
	/* runs the simulation based on the number of generations */
	for(curGen = 0; curGen < N; curGen++){	
		int row = 0; // row index
		int col = 0; // column index

		/* runs through each cell in the array */
		for(i = start; i <= end; i++){
			
			/* if the cell is a newline char */
			if(isNewline(forest[i])){
				row++; // increment the row index
				col = 0; // set column index to 0
				nextGen[i] = forest[i]; // copy the char to the nextGen array
				continue; // go back to top of the for loop
			}

			/* use testTree() to determine what the cell will be in the next
			 * generation */
			nextGen[i] = testTree(i,row,col,forest,I,G,rank,size);
			col++;
		}
		
		/* Pass rows to neighboring processes */
		temp = 0;
		for(k = 0; k < (size-1); k++){
			if(rank == temp){
				
				/* Store the proc's last row into the partial array */
				for(i = 0; i <= 80; i++){
					partial[i] = nextGen[(end-80) + i];
				}

				/* Send the last row to the neighbor below */
				MPI_Send(partial, 81, MPI_CHAR, (temp+1), 0, MPI_COMM_WORLD);
				
				/* Receive the row from the neighbor below */
				MPI_Recv(partial, 81, MPI_CHAR, (temp+1), 0, MPI_COMM_WORLD,
						&status);

				/* Stores the received row into this proc's nextGen 
				 * array */
				for(i = 0; i <= 80; i++){
					nextGen[(end+1)+i]=partial[i]; 
				}	
			}else if(rank == (temp+1)){			
				/* Receive the row from the neighbor above */
				MPI_Recv(partial, 81, MPI_CHAR, temp, 0, MPI_COMM_WORLD, 
						&status);
				
				for(i = 0; i <= 161; i++){
					if(i >= 81){
						/* Stores this proc's first row into the partial 
						 * array */
						partial[i-81] = nextGen[start+(i-81)];
					}else{
						/* Stores the received row into this proc's nextGen
						 * array */	
						nextGen[(start-81) + i] = partial[i];
					}
				}
				
				/* Sends the newly refilled partial array to the neighbor 
				 * above */	
				MPI_Send(partial, 81, MPI_CHAR, temp, 0, MPI_COMM_WORLD);
			}
			temp++; //Increment to move onto the next processes
		}
	
		/* If this NOT the last generation */	
		if(curGen != (N-1)){
			/* and this is process 0 */	
			if(rank == 0){
				temp = start; //The starting cell will be the same
				k = end+81;	  //The ending cell will be the last cell of the
							  // first row below.
			/* All other procs */
			}else{ 
				/* The last proc's end will remain the same */
				if (rank == (size-1)){
					k = end;
				
				/* All other procs will start 1 row earlier and end 1 row 
				 * later */
				}else{
					k = end+81;
				}
				temp = start-81;
			}
		
		/* If this IS the last generation use the normal start and end */
		}else{
			temp = start;
			k = end;
		}

		/* Store the next generation into the forest array */
		for(i = temp; i <= k; i++){
			forest[i] = nextGen[i];
		}
	}

	/* Sends final generation to process 0 */	
	if(rank == 0){
		
		/* Iterates through all the procs */
		for (i = 1; i < size; i++){

			/* Receives each procs final forest */
			MPI_Recv(chunk,(3240/size), MPI_CHAR, i, 0, MPI_COMM_WORLD,
					&status);
			temp = 0;
			/* Stores the chunk into proc 0 forest array */
			for(j = (i*(3240/size)); j <= (((i*(3240/size))+(3240/size))-1); 
					j++){
				forest[j] = chunk[temp];
				temp++;
			}
		}
	}else{
		temp = 0;
		/* Stores the proc's final forest into chunk */
		for(i = start; i <= end; i++){
			chunk[temp] = forest[i];
			temp++;	
		}

		/* Sends chunk to proc 0 */	
		MPI_Send(chunk, (3240/size), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}

	/* print the final generation to the screen */
	if(rank == 0){
		for(i = 0; i < 3240; i++){
			printf("%c",forest[i]);
		}	
	}

    /* quit MPI */
	MPI_Finalize();
	return 0;
}
