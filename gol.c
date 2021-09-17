#include <stdio.h>
#include <unistd.h> //For usleep
#include <stdlib.h> //For system clear
#include <string.h> //For memcopy
#include <assert.h> //For tests
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

//Make them live or die
void liveOrDie(void *g, void *g1)
{
	unsigned (*grid)[ROWS] = g;

	unsigned (*grid1)[ROWS] = g1;


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
//Show them on terminal
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

//import txt file
void load(char*name,void *g){
	unsigned (*grid)[ROWS] = g;

	//import txt file
	const char *In = name;
	FILE *inputFile = fopen(In, "r");
	int x, y;
	while (fscanf(inputFile, "%d,%d", &x, &y) == 2){
		grid[x][y] = 1;
	}
}

void testGeneration()
{

}

void testLoad(){
	int grid[ROWS][COLS] = {0};
	FILE *inputFile = fopen("in1.txt", "r");
	int x, y;
	while (fscanf(inputFile, "%d,%d", &x, &y) == 2){
		grid[x][y] = 1;
			}
	assert(grid[1][0]==1);
	assert(grid[2][1]==1);
	assert(grid[0][2]==1);
	assert(grid[1][2]==1);
	assert(grid[2][2]==1);
	assert(grid[0][0]!=1);
	assert(grid[0][1]!=1);
}

void testOuput(){

}

void runTests()
{
	testLoad();
}


int main( int argc, char *argv[])
{
	runTests();


	//Exit if no Arguments
	 if(argc <2){
        printf("No argument entered. Correct format is:\n");
		printf(" ./prog <path-to-input-configuration> <path-to-output-dir> <number-of-generations-to-run-before-saving>+\n");
      return 0;
	 }

	



	//Grid Size
	int grid[ROWS][COLS] = {0};
	int grid1[ROWS][COLS] = {0};
	int generations = 100;
	int showArg =0;

    int i = 1;//start out at second arg
	if (*argv[i]== 's'){
		showArg =1;
		i =2;
	}
	
	load( argv[i], grid);

    //writeToConsole(grid, ROWS, COLS);
    while(argv[i] != NULL){
		char *a = argv[i]; // how many generations should run
		generations = atoi(a); 
		while (generations > 0) {
			int swap[ROWS][COLS] = {0};
			system("clear");
			liveOrDie(grid,grid1);

    		//to show -s must be first arg
			if(showArg ==1){
    		show(grid,ROWS, COLS);
			}

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
   					fprintf(fp, "%d,%d\n",x,y);
					}
				}
			}
			fputs("End of file...\n", fp);
   			fclose(fp);
		}
		i++;
    }
  
	return 0;
}