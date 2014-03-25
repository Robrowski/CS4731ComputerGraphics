#include "Angel.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


#define BUFFER_SIZE 100 // 100 characters
char buf[BUFFER_SIZE];
#define READ_NEXT_LINE fgets( buf, BUFFER_SIZE,f)

// Kepp track of the previous incase we have to iterate
MyPolyline* prev = NULL;

/** Create a struct to hold "size" points  */
MyPolyline* generateEmptyPolyline(int size){
	// New struct to hold all data
	MyPolyline* newPoints = (MyPolyline*) malloc(sizeof(MyPolyline));
	newPoints->NumPoints = size;
	newPoints->pt =  (MyPoint*) calloc(size,sizeof(MyPoint));
	
	// Record the current and previous incase we have to iterate
	newPoints->prev = prev;
	prev = newPoints;

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
	dst->prev = src->prev;
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


/*****************************************************
	FUNCTIONS based on given textFileRead function
  *****************************************/
/** Return index of first if asteric is present, else 0
*/
int findLastAst(char* text){
	unsigned long long length = strlen(text);
	unsigned long long lastA = 0; // default is 0
	int i;
	// Check every single character....
	for (i = 0; i < length; i++){
		// Indicates end of file and no asterics...
		if (text[i] == '\0') return 0;
		
		// Indicates last asterisc
		if (text[i] != '*' && lastA == i -1 && lastA > 0){
			//printf("Found lastA: %d\n",i);
			return i + 1;		
		}
		
		// Indicates AN asterisc
		if (text[i] == '*' ) lastA = i;
	}
	return 0; // default
}



/*****************************************************
	FUNCTIONS FOR READING GRS FILES
  *****************************************/

/** Read and return the next Polyline in the given file
* @param f file descriptor - file must be open and a poly line must be next
* @return the polyline just read in
*/
MyPolyline* readNextPolyline(FILE* f){
	// First line is the number of vertices
	unsigned int numVertices;
	if (sscanf(READ_NEXT_LINE, "%u", &numVertices) != 1) return NULL;	

	// Have a place ready to put the vertices
	MyPolyline* newPoly = generateEmptyPolyline(numVertices);
	
	// the next "numVertices" will be vertices
	unsigned int Vnum;
	for (Vnum = 0; Vnum < numVertices; Vnum++){
		float x,y;
		if (sscanf(READ_NEXT_LINE, "%f %f", &x, &y) != 2) return NULL;	
		
		// Put data in!
		newPoly->pt[Vnum].x = x;
		newPoly->pt[Vnum].y = y;
	}	

	return newPoly;
}

/**  Read in a picture's worth of polylines - file must be read right up to 'extent
* @param f file descriptor - must be open
* @param GIVENnumPolyLines 0 if 'extent' not found
* @return the entire picture including all polylines
*/
MyPicture* readPicture(FILE* f,unsigned int  GIVENnumPolyLines){
	unsigned int numPolyLines = GIVENnumPolyLines;
	// The first number after the 'extent' is the number of polylines.. but we might
	// have read it already for cases like dino.dat
	if (numPolyLines == 0){
		if (sscanf(READ_NEXT_LINE, "%u", &numPolyLines) != 1) return NULL; // error
	}
	//printf("Num Poly Lines: %d\n", numPolyLines);
	
	// A place to put all the poly lines
	MyPicture* newPic = generateEmptyPicture(numPolyLines);
	
	// This part checks out all the individual polylines 
	unsigned int polyNum;
	MyPolyline* prev = NULL;
	for (polyNum = 0; polyNum < numPolyLines; polyNum++){
		MyPolyline* now = readNextPolyline(f);
		now->prev = prev;// save ref to previous
		prev = now; // record current
		
		copyPolyline(now, &newPic->pl[polyNum]);
	}

	return newPic;
}

/** Read the picture out of the given file */
MyPicture* parseGRS( char *file ){
	printf("Now Reading: %s\n", file);
				
	// Check for comments at the top
	char* text = textFileRead(file); // read text in 
	int lastA = findLastAst(text) ;

	// Open file
	FILE* f = fopen(file, "r");		
	
	// If comments, scan until the end of them so the rest
	// of the scans will be of content.
	if (lastA > 0){
		while (strstr((const char *)buf, "*") == NULL){
			if (READ_NEXT_LINE == NULL) return NULL;
		}	
	}
		
	// Look for 'extent'
	char* extentHopefully = READ_NEXT_LINE;
	float L,T,R,B;
	unsigned int numPolyLines = 0;
	if (sscanf(extentHopefully, "%f %f %f %f", &L,&T,&R,&B) != 4){
		//printf("Default: ");
		numPolyLines = (unsigned int)L; // assume first line was actually number of Polylines
		L = DEFAULT_L;
		T = DEFAULT_T;
		R = DEFAULT_R;
     	B = DEFAULT_B;
	}
	//printf("L T R B:  %f %f %f %f \n",L, T, R , B);	
		
	MyPicture* newPic = readPicture(f,numPolyLines);
	newPic->f = newFrame(L, R, B, T);

	return newPic;
}



