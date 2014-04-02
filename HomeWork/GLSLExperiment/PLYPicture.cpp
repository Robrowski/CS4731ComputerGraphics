#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"





// Generates an empty PLY Picture with the given num vertices and triangles - initialized to zero
PLYPicture* generateEmptyPLYPicture(GLint numVertices, GLint numTriangles){
	PLYPicture* ply = (PLYPicture*) malloc(sizeof(PLYPicture));
	ply->numVertices = numVertices;
	ply->points = generateEmptyPolyline(numVertices);
	ply->triangles = (vec3 *) calloc(numTriangles, sizeof(vec3)); // Data is zeroed
	return ply;
}