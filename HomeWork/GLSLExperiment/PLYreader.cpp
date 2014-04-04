#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"





// File reading usefulness
char plybuf[BUFFER_SIZE];
#define READ_LINE fgets( plybuf, BUFFER_SIZE,f)


PLYPicture* readPLYFile(char* file){
	// Start reading	
	printf("Now reading %s\n", file);
	FILE* f = fopen(file, "r");		// Open file
	char* ply = READ_LINE;
	if (ply[0] != 'p' || ply[1] != 'l' || ply[2] != 'y'){
		printf("Not a PLY file\n");
		return generateEmptyPLYPicture(0,0);
	}

	READ_LINE; // skip line = ascii version	

	// Header Stuff
	int numVertices, numTriangles,i;


	char* txt = READ_LINE;
	if (sscanf(&txt[15], "%u", &numVertices) != 1) return NULL;
	
	READ_LINE; READ_LINE; READ_LINE; // skip three lines

	char* txt2 = READ_LINE;
	if (sscanf(&txt2[13], "%u", &numTriangles) != 1) return NULL;

	READ_LINE; READ_LINE; // skip two more lines

	PLYPicture* newPLY = generateEmptyPLYPicture( numVertices,  numTriangles);
//	printf("Verts: %d      Tri: %d\n", numVertices, numTriangles);
	

	// Points
	MyPoint* pts = newPLY->points->pt; // shortcut
	
	// Reading first line
	float x , y , z;
	if (sscanf(READ_LINE, "%f %f %f", &x, &y, &z) != 3) return NULL;
	vec3 maxPt = vec3(x,y,z); // Best place to start is with first point
	vec3 minPt = vec3(x,y,z);
	pts[0] = MyPoint(x,y,z, 1.0f);
	for (i = 1; i < numVertices; i++){
		if (sscanf(READ_LINE, "%f %f %f", &x, &y, &z) != 3) return NULL;
		pts[i] = MyPoint(x,y,z, 1.0f);
	
		// Boundaries too
		maxPt.x = max(maxPt.x,x);
		maxPt.y = max(maxPt.y,y);
		maxPt.z = max(maxPt.z,z);
		
		minPt.x = min(minPt.x,x);
		minPt.y = min(minPt.y,y);
		minPt.z = min(minPt.z,z);
		//	printf("X: %f, Y: %f, Z: %f\n",x,y,z);
	}

	newPLY->max = maxPt;
	newPLY->min = minPt;
//	printf("Max: ");	printv(maxPt); printf("\nMin: ");printv(minPt); printf("\n");


	// Triangles
	for (i = 0; i < numTriangles; i++){
		unsigned int n, x , y , z;
		if (sscanf(READ_LINE, "%u %u %u %u", &n,&x, &y, &z) != 4) return NULL;
		newPLY->triangles[i] = vec3(x,y,z);
		//printf("1: %u, 2: %u, 3: %u\n",x,y,z);
	}

	return newPLY;
}


char* plyAvailable[43] = { "PLYFiles/airplane.ply",
"PLYFiles/ant.ply",
"PLYFiles/apple.ply",
"PLYFiles/balance.ply",
"PLYFiles/beethoven.ply",
"PLYFiles/big_atc.ply",
"PLYFiles/big_dodge.ply",
"PLYFiles/big_porsche.ply",
"PLYFiles/big_spider.ply",
"PLYFiles/canstick.ply",
"PLYFiles/chopper.ply",
"PLYFiles/cow.ply",
"PLYFiles/dolphins.ply",
"PLYFiles/egret.ply",
"PLYFiles/f16.ply",
"PLYFiles/footbones.ply",
"PLYFiles/fracttree.ply",
"PLYFiles/galleon.ply",
"PLYFiles/hammerhead.ply",
"PLYFiles/helix.ply",
"PLYFiles/hind.ply",
"PLYFiles/kerolamp.ply",
"PLYFiles/ketchup.ply",
"PLYFiles/mug.ply",
"PLYFiles/part.ply",
"PLYFiles/pickup_big.ply",
"PLYFiles/pump.ply",
"PLYFiles/pumpa_tb.ply",
"PLYFiles/sandal.ply",
"PLYFiles/saratoga.ply",
"PLYFiles/scissors.ply",
"PLYFiles/shark.ply",
"PLYFiles/steeringweel.ply",
"PLYFiles/stratocaster.ply",
"PLYFiles/street_lamp.ply",
"PLYFiles/teapot.ply",
"PLYFiles/tennis_shoe.ply",
"PLYFiles/tommygun.ply",
"PLYFiles/trashcan.ply",
"PLYFiles/turbine.ply",
"PLYFiles/urn2.ply",
"PLYFiles/walkman.ply",
"PLYFiles/weathervane.ply" };


int currentFile = -1;
char* nextFile(void){
	currentFile++;
	if (currentFile >= 43) currentFile = 0;
	return plyAvailable[currentFile];
}


char* prevFile(void){
	currentFile--;
	if (currentFile < 0) currentFile = 42;
	return plyAvailable[currentFile];
}





