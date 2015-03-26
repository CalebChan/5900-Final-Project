// single_window.c : Defines the entry point for the console application.
//

//
//  //
//  Created by nuss on 2014-07-29.
//  Copyright (c) 2014 Dorn Nussbaum . All rights reserved.
//
//  	This program is for education purposes


/*
	LICENSE AGREEMENT 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

	Copyright and License agreement must remain if software is used.

    This program is distributed	AS IS in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


/************************************************************************************************/
// INCLUDE FILES 

#include <stdio.h>   
#include <stdlib.h>

#include "windows.h"  // windows header requires by opengl, glut etc.
#include "GL/glew.h" 
#include <GL/gl.h>    // gl header for gl commands and function calls
#include <GL/glu.h>   // glu header for glu helper functions
#include <GL/glut.h>  // glut header for basic window management and callback functions


#include "gameApp.h"


/************************************************************************************************/
// DEFINES and MACROS
#define WIN_POS_X	30
#define WIN_POS_Y	10
#define WIN_WIDTH	500
#define WIN_HEIGHT	400




/************************************************************************************************/
// DATA STRUCTURES



/************************************************************************************************/
// GLOBAL VARIABLES

/************************************************************************************************/

// FUNCTION PROTOTYPES 






/***********************************************************************************************/


 
// Main program
int main(int argc, char** argv)
{

	gameApp *myGame = new gameApp();


	// initialize OpenGL 
	myGame->initGraphics(argc, argv, WIN_WIDTH, WIN_HEIGHT, WIN_POS_X, WIN_POS_Y);

	myGame->initGame();

	myGame->gameLoop();



	return 0;
}




