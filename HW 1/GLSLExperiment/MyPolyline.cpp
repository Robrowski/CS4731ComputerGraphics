#include "Angel.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


/** Create a struct to hold "size" points  */
MyPolyline* generateEmptyPolyline(int size){
	// New struct to hold all data
	MyPolyline* newPoints = (MyPolyline*) malloc(sizeof(MyPolyline));
	newPoints->NumPoints = size;
	newPoints->pt =  (MyPoint*) calloc(size,sizeof(MyPoint));
	
	return newPoints;
}

/** Create a struct to hold "numPolyLines"   */
MyPicture* generateEmptyPicture(int numPolyLines){
	// New struct to hold all data
	MyPicture* newPic = (MyPicture*) malloc(sizeof(MyPicture));
	newPic->numLines = numPolyLines;
	newPic->pl =  (MyPolyline*) calloc(numPolyLines,sizeof(MyPolyline));

	return newPic;
}

/** Copy src into dst Polyline
*@param src the place to copy from
*@param dst the place to get overwritten
*/
void copyPolyline(MyPolyline* src, MyPolyline* dst){
	dst->NumPoints = src->NumPoints;
	dst->pt = src->pt;
}

/** Print the given Polyline's attributes
* @param toPrint the polyline to print
*/
void printPolyline(MyPolyline* toPrint){
	printf("numVertices: %d\n", toPrint->NumPoints);
	int numPts;
	for (numPts = 0; numPts < toPrint->NumPoints; numPts++){
		printf("   X: %2.4f   Y: %2.4f\n",(toPrint->pt[numPts]).x, (toPrint->pt[numPts]).y);
	}
}


// Takes a polyline and draws it for the given line type
void drawMyPolyline(MyPolyline ln, GLint lineType){
	// Because of what the pointer is in the MyPolyline Struct, can't actually get the size 
	// of it because the compiler doesn't know that its a pointer to an array. The compiler
	// thinks it is just a pointer to one polyline. 
	glBufferData( GL_ARRAY_BUFFER, ln.NumPoints*sizeof(MyPoint), ln.pt , GL_STATIC_DRAW );
	
	// All drawing happens in display function
	glDrawArrays( lineType, 0, ln.NumPoints );    // draw the points
}
