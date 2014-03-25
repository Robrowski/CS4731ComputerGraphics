/** geometries.cpp
*
*   The functions in this file are all for generating a series
*   of vertices and storing them into given arrays. 
*
*
*/

#include "Angel.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


/** Generate the Polyline for a triangle */
MyPolyline* generateTriangle(void){
	MyPolyline* newPoints = generateEmptyPolyline(NUM_VERTICES_TRI);
	
	// Pre defined points
	newPoints->pt[0] =  MyPoint( -0.5, -0.5 );
	newPoints->pt[1] =  MyPoint( 0.0, 0.5 );
	newPoints->pt[2] =	MyPoint( 0.5, -1.0 );
	
	return newPoints;
}


/** Generate the Polyline for a triangle */
MyPolyline* generatePentagon(void){
	MyPolyline* newPoints = generateEmptyPolyline(NUM_VERTICES_PENT);
	
	// Pre defined points
	newPoints->pt[0] =  MyPoint( -0.3, -0.5 );
	newPoints->pt[1] =	MyPoint( -0.56, 0.13 );
	newPoints->pt[2] =  MyPoint( 0.0, 0.5 );
	newPoints->pt[3] =	MyPoint( 0.56, 0.13);
	newPoints->pt[4] =	MyPoint( 0.3, -0.5 );
	
	

	return newPoints;
}



