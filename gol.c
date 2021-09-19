#include <stdio.h>
#include <unistd.h> //For usleep
#include <stdlib.h> //For system clear
#include <string.h> //For memcopy
#include <assert.h> //For tests
#define ROWS 63
#define COLS 63


//will write a 2d array to console
void writeToConsole(void *g)
{
	unsigned (*grid)[ROWS] = g;
	int x,y;

	for (int x = 0; x < ROWS; x++)
	{
		for (int y = 0; y <COLS; y++)
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

			//rolling problem with modulus?
			int row = (x + i + ROWS) % ROWS;
			int col = (y + j + COLS) % COLS;

			//add all 1's and 0's up
			aliveAroundMe += grid[row][col];
		}
	}

	aliveAroundMe -= grid[x][y];
	return aliveAroundMe;
}

//Will complete on generation of evolution 
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


//Show grid on terminal, very slow
void show(void *g, int h, int w)
{
	unsigned (*grid)[ROWS] = g;
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
	usleep(200000);
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

void writeTxt(char * a,void *g){
	unsigned (*grid)[ROWS] = g;
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
	//fputs("End of file...\n", fp);
   	fclose(fp);

}
void testwrite(char*name,void *g)
{
	unsigned (*grid)[ROWS] = g;
	load(name, grid);

	char a[4] = "test";
	char *b = a;
	writeTxt(b,grid);

	grid[1][0]=0;
	grid[2][1]=0;
	grid[0][2]=0;
	grid[1][2]=0;
	grid[2][2]=0;


	load("test.txt", grid);

	assert(grid[1][0]==1);
	assert(grid[2][1]==1);
	assert(grid[0][2]==1);
	assert(grid[1][2]==1);
	assert(grid[2][2]==1);
	assert(grid[0][0]!=1);
	assert(grid[0][1]!=1);
}


void testLoad(char*name,void *g){
	unsigned (*grid)[ROWS] = g;
	load(name, grid);
	assert(grid[1][0]==1);
	assert(grid[2][1]==1);
	assert(grid[0][2]==1);
	assert(grid[1][2]==1);
	assert(grid[2][2]==1);
	assert(grid[0][0]!=1);
	assert(grid[0][1]!=1);
}

void testOutput(){
	int grid[ROWS][COLS] = {0};
	int grid1[ROWS][COLS] = {0};
	char *a = "5";
	grid[1][0]=0;
	grid[2][1]=0;
	grid[0][2]=0;
	grid[1][2]=0;
	grid[2][2]=0;

	assert(grid[1][0]==0);
	assert(grid[2][1]==0);
	assert(grid[0][2]==0);
	assert(grid[1][2]==0);
	assert(grid[2][2]==0);

	load( "in1.txt", grid);
	int i =1;
	i++;
	int generations =5;

	while (generations> 0) {
		int swap[ROWS][COLS] = {0};
		liveOrDie(grid,grid1);
		//Swap
		memcpy(grid, grid1, sizeof(grid));
		memcpy(grid1, swap, sizeof(grid));
		--generations;
	}
	//Reversed x and y compared to out_5.txt
	//Mine matches https://playgameoflife.com/
	assert(grid[1][2]==1);
	assert(grid[2][3]==1);
	assert(grid[2][4]==1);
	assert(grid[3][2]==1);
	assert(grid[3][3]==1);

}

void runTests()
{
	int gridTest[ROWS][COLS] = {0};
	testLoad("in1.txt",gridTest);
	testwrite("in1.txt",gridTest);
	testOutput();
}


int main( int argc, char *argv[])
{
	//Grid Size
	int grid[ROWS][COLS] = {0};
	int grid1[ROWS][COLS] = {0};
	int generations = 100;
	int showArg =0;

	runTests();


	//Exit if no Arguments
	 if(argc <2){
        printf("No argument entered. Correct format is:\n");
		printf(" ./prog <path-to-input-configuration> <path-to-output-dir> <number-of-generations-to-run-before-saving>+\n");
      return 0;
	 }

    int i = 1;//For Arg count
	//check they want to show board Use s
	if (*argv[i]== 's'){
		showArg =1;
		i =2;
	}
	
	load( argv[i], grid);
    i++;
    while(argv[i] != NULL){
		char *a = argv[i]; // how many generations should run
		generations = atoi(a); 
		while (generations> 0) {
			int swap[ROWS][COLS] = {0};

			//send to grow or die
			liveOrDie(grid,grid1);

			//Swap
			memcpy(grid, grid1, sizeof(grid));
			memcpy(grid1, swap, sizeof(grid));		
			--generations;

			//system("clear");
			//writeToConsole(grid);
			if(showArg ==1){
			system("clear");
    		show(grid,ROWS, COLS);			
			}
		}
		writeTxt(argv[i],grid);
		i++;
    }
  
	return 0;
}