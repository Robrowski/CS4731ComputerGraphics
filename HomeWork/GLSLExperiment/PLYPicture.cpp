#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"





// Generates an empty PLY Picture with the given num vertices and triangles - initialized to zero
PLYPicture* generateEmptyPLYPicture(GLint numVertices, GLint numTriangles){
	PLYPicture* ply = (PLYPicture*) malloc(sizeof(PLYPicture));
	ply->numTriangles = numTriangles;
	ply->points = generateEmptyPolyline(numVertices);
	ply->numPointsInPicture = numTriangles*3;
	ply->triangles = (vec3 *) calloc(numTriangles, sizeof(vec3)); // Data is zeroed
	return ply;
}


// Generates a PLYPicture of a cube
PLYPicture* generatePLYCube(void){
	PLYPicture* pic = generateEmptyPLYPicture(8, 12);
	MyPoint* pts = pic->points->pt;

	MyPoint vertices[8] = {
		MyPoint( -0.5, -0.5,  0.5, 1.0 ),
		MyPoint( -0.5,  0.5,  0.5, 1.0 ),
		MyPoint(  0.5,  0.5,  0.5, 1.0 ),
		MyPoint(  0.5, -0.5,  0.5, 1.0 ),
		MyPoint( -0.5, -0.5, -0.5, 1.0 ),
		MyPoint( -0.5,  0.5, -0.5, 1.0 ),
		MyPoint(  0.5,  0.5, -0.5, 1.0 ),
		MyPoint(  0.5, -0.5, -0.5, 1.0 )
	};

	// Save in the vertices
	int i;
	for (i = 0; i < 8; i++){
		pts[i] = vertices[i];
	}

	// save in the triangles
	pic->triangles[0] = vec3(1, 0, 3);
	pic->triangles[1] = vec3(2, 3, 7);
	pic->triangles[2] = vec3(3, 0, 4);
	pic->triangles[3] = vec3(6, 5, 1);
	pic->triangles[4] = vec3(4, 5, 6);
	pic->triangles[5] = vec3(5, 4, 0);

	pic->triangles[6]  = vec3(1, 3, 2);
	pic->triangles[7]  = vec3(2, 7, 6);
	pic->triangles[8]  = vec3(3, 4, 7);
	pic->triangles[9]  = vec3(6, 1, 2);
	pic->triangles[10] = vec3(4, 6, 7);
	pic->triangles[11] = vec3(5, 0, 1);

	return pic;
}



// Draw the given PLY picture
void drawPLYPicture(PLYPicture* p){
	// Local variables for ezness
	int numPoints = p->numTriangles*3;
	int sizePoints = numPoints*sizeof(MyPoint);
	int sizeColors = numPoints*sizeof(color4);
	MyPoint* verts = p->points->pt;
	vec3* triVerts = p->triangles;

	// Prepare buffer
    glBufferData( GL_ARRAY_BUFFER, sizePoints + sizeColors,  NULL, GL_STATIC_DRAW );
	
	// Preparing Data
	MyPoint* points = (MyPoint *) calloc(numPoints, sizeof(MyPoint)); // holds data
	int t; int i = 0; 
	// for each triangle, copy th three points in
	for (t = 0; t < p->numTriangles; t++, i+=3 ){
		// Convert to ints first
		int one = triVerts[t].x;
	    int two = triVerts[t].y;
		int three = triVerts[t].z;

		// Copy vertices
		points[i]     = verts[one];
		points[i + 1] = verts[two];
		points[i + 2] = verts[three];                 
	}

	// Buffering data  // ACTUALLY SENDS DATA
	glBufferSubData( GL_ARRAY_BUFFER, 0,          sizePoints, points );
	glBufferSubData( GL_ARRAY_BUFFER, sizePoints, sizeColors, randomColors(numPoints) );

	// set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(sizePoints) );


	// Free allocated data
	free(points); 
}