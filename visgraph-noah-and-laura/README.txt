Motion planning with visibility graph!

# pqueue

A simple priority queue implemented as a binary heap. 

It  supports insert and deleteMin. 

To use this pqueue you need to modify the elemType in {basicType.h, basicType.c} to contain the data that you need.  By default elemType contains a distance (float) and a point (point2D).  ElemType does not store a field for the priority. The priority of an element is found by calling function getPriority(elemType). You have to implement this function in basicType.c to suit your needs. Right now  getPriority(elemType x) returns x.dist. 



Checkout file pqtest.c to see how to use it. 



README



Geom.cpp mostly deals with the construction of the visibility graph. The general flow of this 
construction is first, collecting all vertices from the start, end, and all objects. These 
vertices are placed in the visabilityGraph and will be pointed to in the adjacency list to 
represent adjacency. So, for every vertex if another vertex is visible, a pointer is connected 
to the index of the visible pointer. Is Visible checks if two points are visible from each other. 
It goes through all the edges of all the objects, and if the line connecting ab intersects with 
any of these tested edges, it is not visible. Ab represents the tested line segment. In 
isVisible, it calls inPoly too because an error occurred where vertices which were internally 
visible were considered visible. To account for this, we generate a midpoint on that sudo visible 
line, and if it’s inside the polygon the line is considered not visible. 
	After the visibility graph is constructed visgraph_driver.cpp contains main and runs all 
visuals and Dijkstra’s. To start main, an initial scene is generated, the visibility graph is
determined, and Dijkstra’s is run. The result is shown immediately. The scene can be regenerated
by pressing I. To place objects, select ‘o’ between each object, and either place in 
counterclockwise order or clockwise order. Toggling ‘s’ and ‘e’ allow the start and end points 
to be placed. After a scene is constructed ‘d’ re-runs the calculation. 	
	The only bug we have deals with the construction of the visibility graph. If two points on an 
object are inline another two points form a different object, and the inPolyg function is run,
the midpoint on that line might fall outside of the object. This allows a visible edge to 
travel through two objects. This, however, is a rare bug, because when placing objects, this 
alignment never happens.


Laura Friel
Noah Gans
December 14th, 2021
