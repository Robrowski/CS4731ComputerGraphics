// Any function in this file is suitable to be called by a "callback function" like display or keys


#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%*//*%%%%%%%%%%%%%%%%%%%%%%%%%*/
/** These functions have the potential to be made into a "framework"
class that gets extended and such with two functions to fill in
- init variables
- calculate points

*/
float a[] = {	0.0f,	0.2f,	-0.15f,	0.85f};
float b[] = {	0.0f,	0.23f,	0.26f ,	-0.04f };
float c[] = {	0.0f,	-0.26f,	0.28f ,	0.04f };
float d[] = {	0.16f,	0.22f,	0.24f ,	0.85f };
float tx[] = {  0.0f,	0.0f,	0.0f ,	0.0f };
float ty[] = {  0.0f,	1.6f,	0.44f ,	1.6f };


/** Take the number of iterations and draw the fern for that many points */
void drawFern(int iterations){
	// Going to make a picture out of it!
	MyPicture* fernPic = generateEmptyPicture(1);
	fernPic->pl = generateEmptyPolyline(iterations);
	MyPoint* pt = fernPic->pl->pt; // array of points in MyPolyline
	fernPic->f = newFrame(0,0,0,0);// Initially...

	// Initial point
	pt->x = 0.0;
	pt->y = 0.0;

	// Generate the points
	int iter, chosenIndex;
	for (iter = 1; iter < iterations; iter++){
		int rNum = (rand()%100) + 1; // Random number from 1 to 100
		if (rNum <= 85) chosenIndex = 3;      // 85%
		else if (rNum <= 92) chosenIndex = 2; //  7%
		else if (rNum <= 99) chosenIndex = 1; //  7%
		else				 chosenIndex = 0; //  1%

		pt[iter].x =  a[chosenIndex] * pt[iter-1].x + c[chosenIndex] * pt[iter-1].y + tx[chosenIndex];
		pt[iter].y =  b[chosenIndex] * pt[iter-1].x + d[chosenIndex] * pt[iter-1].y + ty[chosenIndex];
	
		// Check for edges of world space
		if (pt[iter].x > fernPic->f.R) fernPic->f.R = pt[iter].x;
		if (pt[iter].x < fernPic->f.L) fernPic->f.L = pt[iter].x;
		if (pt[iter].y > fernPic->f.T) fernPic->f.T = pt[iter].y;
	}
	
	setLineColor(GREEN_VEC);
	sendOrthoToShader(fernPic->f);
	setViewPort(0, 0 , WINDOW_WIDTH, WINDOW_HEIGHT, fernPic->f);
	drawMyPicture(fernPic, GL_POINTS);
	free(fernPic);
}

/** Take the number of iterations and draw the fern for that many points */
void drawGingerBreadMan(int iterations){
	// Going to make a picture out of it!
	MyPicture* gingerPic = generateEmptyPicture(1);
	gingerPic->pl = generateEmptyPolyline(iterations);
	MyPoint* pt = gingerPic->pl->pt; // array of points in MyPolyline
	gingerPic->f = newFrame(0,0,0,0);// Initially...

	// Initial point
	pt->x = 0;
	pt->y = 0;

	//Constants
	const int M = 40;
	const int L = 3;

	// Generate the points
	int iter, chosenIndex;
	for (iter = 1; iter < iterations; iter++){
		pt[iter].x =  M*(1 + 2L) - pt[iter - 1].y + abs(pt[iter-1].x - L*M);
		pt[iter].y =  pt[iter - 1].x;


		// Check for edges of world space
		if (pt[iter].x > gingerPic->f.R) gingerPic->f.R = pt[iter].x;
		if (pt[iter].x < gingerPic->f.L) gingerPic->f.L = pt[iter].x;
		if (pt[iter].y > gingerPic->f.T) gingerPic->f.T = pt[iter].y;
		if (pt[iter].y < gingerPic->f.T) gingerPic->f.B = pt[iter].y;

	}
	 
	setLineColor(BLUE_VEC);
	sendOrthoToShader(gingerPic->f);
	setViewPort(0, 0 , WINDOW_WIDTH, WINDOW_HEIGHT, gingerPic->f);
	drawMyPicture(gingerPic, GL_POINTS);
	free(gingerPic);
}

// Draw the Sierpinski gasket
void drawSierpinski(void ){
	const int NumPoints = 10000;
	MyPoint points[NumPoints];
	// Specifiy the vertices for a triangle
	MyPoint vertices[3] = {MyPoint( -1.0, -1.0,0,1 ), MyPoint( 0.0, 1.0,0,1 ), MyPoint( 1.0, -1.0,0,1 )};
	// An arbitrary initial point inside the triangle
	points[0] = MyPoint(0.25, 0.50, 0,1);
	// compute and store N-1 new points
	for ( int i = 1; i < NumPoints; ++i ) {
		int j = rand() % 3; // pick a vertex at random
		// Compute the point halfway between the selected vertex
		// and the previous point
		points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
	}

	setLineColor(RED_VEC);
	setGLViewport(DEFAULT_WORLD_FRAME);
	sendOrthoToShader(newFrame(-1,1, -1,1  ));
	// Send off data and draw it
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points , GL_STATIC_DRAW );
	glDrawArrays( GL_POINTS, 0, NumPoints);    // draw
}
