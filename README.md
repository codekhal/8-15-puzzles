# 8-15-puzzles

A puzzle solver using A* Algorithm in C++


## To run on your local machine

Step 1: `git clone https://github.com/codekhal/8-15-puzzles.git`
<br> <br>
Step 2: `cd 8-15-puzzles`
<br> <br>
Step 3: Open project in any of the Text editors. <br> Go to main.cpp then run build `g++ -o main.cpp`
<br> <br>
Step 4: Finally compile the code through `./a.out`
<br> <br>


### Objective
It is a sliding puzzle game that consists of a frame of numbered square tiles in random order with one tile missing. <br> 
The objective of the puzzle is to place the tiles in order by making sliding moves that use the empty space. <br>
We have to arrange the numbers in the matrix in 2 possible ways: <br>
1). Sorted matrix with multiple blank spaces at the end. <br>
2). Arrange the rows of the matrix in the form odd numbers in one row and even numbers in other rows.<br> <br>


 ![alt State Space tree](puzzle-1.jpg)


### Data Structure Used:
The data structure used to efficiently solve the A* algorithm is a modified heap which is able to allow the user to update the priority in O(ln(n)) time.

### Project Synopsis:
[Link](https://drive.google.com/open?id=1VEaKrzQb2B0msJPAobsep54-GW27N-Tf)

