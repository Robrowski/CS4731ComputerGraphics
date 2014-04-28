#include "Angel.h"
#include "utils.h"
#include "bmpread.h"

bmpread_t bitmap;
static  GLuint  texture = 0;
#define GRASS 1
#define STONE 2
GLint groundPlaneState = GRASS;


GLfloat inc = 10;
GLfloat vertLocation = -1.3;
 
MyPoint groundPlane[6] = {
	MyPoint( -inc, vertLocation,  inc, 1.0 ),
	MyPoint( -inc, vertLocation, -inc, 1.0 ),
	MyPoint(  inc, vertLocation, -inc, 1.0 ),
	MyPoint(  inc, vertLocation, -inc, 1.0 ),
	MyPoint(  inc, vertLocation,  inc, 1.0 ),
	MyPoint( -inc, vertLocation,  inc, 1.0 )
    };

// Sends teh given color vector to the fragment shader for colorizing
void setTextureStatus(int toSet){  
	sendIntToShader( "useTexture", toSet );
}

void toggleGroundPlane(void){
	if ( groundPlaneState == GRASS){
		initTexture("textures/stones.bmp");
		groundPlaneState = STONE;
	} else {
		initTexture("textures/grass.bmp");
		groundPlaneState = GRASS;
	}
}

// Assumes ONLY camera is in CTM
void drawGroundPlane(void){
	setTextureStatus(TRUE);

    glBufferData( GL_ARRAY_BUFFER, sizeof(groundPlane), groundPlane, GL_STATIC_DRAW );

	// set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0) );

	glDrawArrays( GL_TRIANGLES, 0, 6 );
	
	setTextureStatus(FALSE);
}




void initTexture(char* file){
	printf("Loading the %s texture\n", file);

	if(!bmpread(file, 0, &bitmap))
	{
		fprintf(stderr, "%s:error loading bitmap file\n", file);
		exit(1);
	}

	// I think this stuff loads and activates the texture
	glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	bmpread_free(&bitmap);


	// Set our texture samples to the active texture unit
    glUniform1i( glGetUniformLocation(program, "texture"), 0 );
    glBindTexture(GL_TEXTURE_2D, texture);
}



















