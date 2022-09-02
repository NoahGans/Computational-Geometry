closePoints is an implementation of the closet point problem useing the brute force and grid methods. 
It takes two arguments, the first being the number of points, and the second being the range of points.
The grid algoritm funtions by making a grid of size k by k where k is the root(n/2). This value for k
is slected to hopefully make for two points in each cell of the grid. After the grid is made, each 2D 
point is placed into the grid that corospods to the area the point resides in. After this each cell in the
grid is iterated throuhg, and for each cell the shortest distance is found. This is done by finding the shortest
distance within the observed cell, and then shortest distance into adjacent cells. Distances into adjacent
cells is calculated so that if two points that bride two cells are very close, they are not missed in the 
calculation. Becase the cells are iterated through only the adjacent bottom right, middle right, top right
and top middle cells are cacluated for distance. The others have already been caculated when previous cells
did their adjacent calculation. This results in n run time. Although at small values of n, the grid algoritm
takes longer probably becasse there is more inital overhead funtions to run.


Results:
n = 10
Range = 1000000

Brute Force Approch
Distance: 6.73638e+08
Point One: 82425 219388
Point Two: 70127 196532
Brute Time: 17 Microseconds

Grid Approch
Distance: 6.73638e+08
Point One: 82425 219388
Point Two: 70127 196532
Grid Time: 42 Microseconds



n = 100
Range = 1000000

Brute Force Approch
Distance: 1.19583e+08
Point One: 111972 446891
Point Two: 122800 448420
Brute Time: 732 Microseconds

Grid Approch
Distance: 1.19583e+08
Point One: 111972 446891
Point Two: 122800 448420
Grid Time: 154 Microseconds



n = 1000
Range = 1000000

Brute Force Approch
Distance: 143284
Point One: 909567 173283
Point Two: 909195 173213
Brute Time: 66135 Microseconds

Grid Approch
Distance: 143284
Point One: 909567 173283
Point Two: 909195 173213
Grid Time: 1604 Microseconds


n = 10000
Range = 1000000

Brute Force Approch
Distance: 18826
Point One: 496249 506075
Point Two: 496150 506170
Brute Time: 6452011 Microseconds

Grid Approch
Distance: 18826
Point One: 496249 506075
Point Two: 496150 506170
Grid Time: 14192 Microseconds

for n = 100000 and 1000000
it took to long. 