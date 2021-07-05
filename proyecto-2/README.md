Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0
# Proyect 2: Enchanted forest

#### Integrants

B90807 David Atias
B94560 Gilbert Marquez
B80626 Kevin Arguedas

### Proyect description

The purpose of the proyecto is to work with a matrix representing a forest which could have three characters representing three types of objects: 'a' representing a tree, 'l' representing a piece of lake and '-' representing a piece of meadow. With each passing day the forest will change cell per cell acording to a set of rules regarding the up to eight cells surrounding it:
1. If the cell is a tree and it has at least four neighbors that are lake, then it will become lake.
2. If the cell is a lake and it has less than tree neighbors that are lake, then it will become meadow.
3. If the cell is a meadow and it has at least tree neighbors that are tree, then it will become a tree.
4. If the cell is a tre and it has more than four neighbors that are tree, then it will become a meadow.
5. If anything else happends the cell will remain the same.
The point is to receive a file job0xx.txt with the name of files containing a map to work with and a number of days it wants to simulate. If the number given is positive then it produces the result of each forest for each day, if it is negative only creates the forest for the last day. The maps are a file map0xx.txt where in the first line it says the number of rows and columns, and the following lines are the characters arrange to form the matrix(according to the rows and columns given). The program produces a file named map0xx-n.txt where the 'x' is the number of the original map and the 'n' is the result passed n nights.
	Examples:
1. job001.txt:
map001.txt 2
map002.txt -100
map003.txt -20000

2. map001.txt:
7 7
-------
-l--l--
-ll----
-l-----
---laa-
-aa-al-
a-a----

3. map001-1.txt:
-------
-------
-ll----
-------
----aa-
-aaaa--
aaaa---

### Compiling

This proyect comes with a Makefile that has everything you need to compile it just use the command make in the proyect folder on a terminal terminal.

### User manual

After compiling execute forest in a terminal followed by a space and the path to the job0xx.txt and the program will create a folder named "output" where the results of the requested maps through the job file are saved.
Example: ./bin/forest test_set_1/input/job001.txt
Its important to remember that the file job needs to follow a format explained in the proyect description with the respective maps following their own format in the same route.

