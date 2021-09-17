#include <stdio.h>
#include <unistd.h> //For usleep
#include <stdlib.h> //For system clear
#include <string.h> //For memcopy

#define ROWS 63
#define COLS 63


//will write a 2d array to console
void writeToConsole(void *g, int h, int w)
{
	unsigned (*grid)[h] = g;
	int x,y;

	for (int x = 0; x < h; x++)
	{
		for (int y = 0; y < h; y++)
		{
			if (grid[x][y] == 1)
			{
				printf("col(x): %d, row(y): %d\n", x, y);
			}
		}
	}
}
//Count Neighbors
int cNeighbors(void *g, int x, int y) {
	unsigned (*grid)[ROWS] = g;
	int aliveAroundMe = 0;

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			//convert to coordinates?? rolling problem with modulus

			int row = (x + i + ROWS) % ROWS;
			int col = (y + j + COLS) % COLS;

			aliveAroundMe += grid[row][col];
		}
	}

	aliveAroundMe -= grid[x][y];

	return aliveAroundMe;
}
void liveOrDie(void *g, void *g1)
{
	unsigned (*grid)[ROWS] = g;
	//unsigned new[ROWS][COLS];

	unsigned (*grid1)[ROWS] = g1;
	//unsigned new[ROWS][COLS];

    int x, y;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			int aliveOrDead = grid[i][j];
			int neighbors = cNeighbors(grid, i, j);

			if (aliveOrDead == 0 && neighbors == 3) {
				grid1[i][j] = 1;
			} else if
			 (aliveOrDead == 1 && (neighbors < 2 || neighbors > 3)) {
				grid1[i][j] = 0;
			} else {
				grid1[i][j] = grid[i][j];
			}
			
		}
	}
}

void show(void *g, int h, int w)
{
	unsigned (*grid)[w] = g;
	//unsigned new[h][w];
    int x, y;
 
	for (int x = 0; x < h; x++)
	{
		for (int y = 0; y < h; y++)
		{
			if (grid[x][y] == 1)
			{
				//printf("Found live one@ ");
				//printf("col(x): %d, row(y): %d\n", x, y);
				printf("\033[0;32m"); 
				printf("@");
				printf("\033[0m");
			}
			else {
				printf("\033[0;36m"); 
				printf(".");
				printf("\033[0m");
			}
		}
		printf("...\n");
	}
	//usleep(100000);
}



int main( int argc, char *argv[])
{
	//Exit if no Arguments
	 if(argc <2){
        printf("No argument entered. Correct format is:\n");
		printf(" ./prog <path-to-input-configuration> <path-to-output-dir> <number-of-generations-to-run-before-saving>+\n");
      return 0;
	 }


	//Grid Size
	//int w = ROWS, h = 63;
	int grid[ROWS][COLS] = {0};
	int grid1[ROWS][COLS] = {0};
	int generations = 100;

	//import txt file
	FILE *inputFile = fopen(argv[1], "r");
	int x, y;
	while (fscanf(inputFile, "%d,%d", &x, &y) == 2){
		grid[x][y] = 1;
	}
    writeToConsole(grid, ROWS, COLS);
    int i = 2;//start out at second arg
    while(argv[i] != NULL){
		char *a = argv[i]; // how many generations should run
		generations = atoi(a); 
		while (generations > 0) {
			int swap[ROWS][COLS] = {0};
			system("clear");
			liveOrDie(grid,grid1);
    		show(grid,ROWS, COLS);
			//Swap
			memcpy(grid, grid1, sizeof(grid));
			memcpy(grid1, swap, sizeof(grid));
			generations--;

			char txt [12] =".txt";
			char output [12] ="";
			strcat(output,a);
			strcat(output,txt);

			FILE *fp;
  			fp = fopen(output, "w+");
			for (int x = 0; x < ROWS; x++)
			{
				for (int y = 0; y < COLS; y++)
				{
					if (grid[x][y] == 1)
					{
					printf("col(x): %d, row(y): %d\n", x, y);
   					fprintf(fp, "%d,%d\n",x,y);
					}
				}
			}
			fputs("End of file...\n", fp);
   			fclose(fp);
		}
		i++;
    }
   


//   //show(grid,ROWS, COLS);
//   while (generations) {
//   	int swap[ROWS][COLS] = {0};
//   	system("clear");
//   	liveOrDie(grid,grid1);
//       show(grid,ROWS, COLS);
//   
//   	//Swap
//   	memcpy(grid, grid1, sizeof(grid));
//   	memcpy(grid1, swap, sizeof(grid));
//   
//   }
	//liveOrDie(grid,grid1);
	//printf("\n");
	//writeToConsole(grid, ROWS, COLS);
	return 0;
}