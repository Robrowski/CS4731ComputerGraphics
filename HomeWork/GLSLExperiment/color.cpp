#include "Angel.h"
#include "utils.h"
#include <stdlib.h>


// RGBA olors
#define NUM_AVAILABLE_COLORS 12
color4 colorsAvailable[NUM_AVAILABLE_COLORS] = {
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
	color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue	
	color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
	color4( 0.5, 0.2, 0.7, 1.0 ), //shark
	color4( 0.8, 0.4, 0.2, 1.0 ),
	color4( 0.7, 0.7, 0.0, 1.0 ),
	color4( 0.2, 0.7, 0.2, 1.0 ),
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan      # 8
};


// Sends teh given color vector to the fragment shader for colorizing
void setLineColor(vec4 toSet){
	glUniform4fv( glGetUniformLocation(program, "fragmentShaderLineColor"),1, toSet );
}


void setColor(GLint toSet){
	setLineColor(colorsAvailable[toSet]);
}

// Generates an array of random colors from 0 to num - 1
color4* randomColors(GLint num){
	color4* newColors = (color4*) calloc(num, sizeof(color4));

	int i;
	for (i = 0; i < num; i++){
		newColors[i] = colorsAvailable[rand()%NUM_AVAILABLE_COLORS];
	}

	return newColors;
}

// Generates an array of red
color4* redArray(GLint num){
	color4* colors = (color4*) calloc(num, sizeof(color4));

	int i;
	for (i = 0; i < num; i++){
		colors[i] = colorsAvailable[RED];
	}

	return colors;
}


