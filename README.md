# AETG Algorithm

## Description

aetg.cpp is a test suite generator using combinatorics to minimize
test cases. It generates pairwise coverage of all possible options
 and outputs the best results to 'results'

Input is the number of components and options

## Input
Enter the number of components (columns): **4**
Enter the number of options (rows): **3**

### Example Table represented by input
||||||
|--|--|--|--|--|
||0|3|6|9
||1|4|10|7 
||2|5|8|11

## Output
Output is the number of unique combinations using pairwise coverage of all possible options.

### Example Output
9

2 4 6 10 

2 3 7 11 

1 5 6 11 

0 3 6 9 

0 4 8 11 

0 5 7 10 

1 4 7 9 

1 3 8 10 

2 5 8 9 

# Running the Code
TO COMPILE:
`make` to compile the AETG program

TO RUN:
`make run` to run test suite generator

ETC:
`make clean` clears out the object files, a.out, and results
