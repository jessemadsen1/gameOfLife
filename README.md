# gameOfLife
Use C to write your own implementation of Conway's Game of Life.

Usage: ./prog <path-to-input-configuration> <path-to-output-dir> <number-of-generations-to-run-before-saving>+

Example:

./a.out in1.txt out 5
With in1.txt containing:

1,0
2,1
0,2
1,2
2,2
Should result in a file called "out/5.txt" containing:

2,1
2,3
3,2
3,3
4,2
The goal is to make your code as fast as possible with no constraints on the configurations that can be represented and computed other than the start x,y pairs and the number of generations need only allow x and y between 0 and 63 inclusive.

You may use any representation that you want, but please also provide a tool that converts to a text file with "x, y" pairs.

20 points Extra Credit: port your implementation to Go and demonstrates that it still works.
