
//=============================================================================
// gameApp.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to use directx input 
// b. demonstrates how to use directx fonts
// c. demonstrates how to initialize direct3D
// d. provide a framework for creating games
// e. demonstrates how to create an egnine frame and provide a skeleton function of one.
// f. demonstarte the usage of a camera
// g. demonstrates the creation of a surface and of vertex and index buffers
// h. demonstartes how to set the differnt matrices. 
//
//
// Description:
//--------------
// A simple application that demonstrates how to use directx to draw a font on the screen.
// It uses a framework for a general purpose simple game engine
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#pragma once







#include "stdio.h"
#include "stdlib.h"
#include <vector>


#include "glew.h"
#include "glut.h"
#include "gl/gl.h"
#include "camera.h"
#include "shader.h"
#include "SOIL.h"
#include "SkyBox.h"

#include "nuss_matrix.h"

#include "gameObject.h"
#include "meshSurface.h"
#include "camera.h"
#include "chaser.h"
#include "Prey.h"
#include "Shader.h"
#include "House.h"

#include "TerrainShader.h"
#include "DepthMeshSurface.h"
#include "VisibleTexture.h"
#include "PointOfView.h"
#include "StencilShader.h"
#include "LightingShader.h"

#include "Silhouette.h"

// DEFINES

#define GAME_TITLE_SISE 128



class gameApp
{
// Default functions
public:
	//gameApp(void);
	~gameApp(void);
	gameApp();
	int initGraphics(int argc, char** argv, int winWidth, int winHeight, int winPosx, int winPosy);
	virtual int updateGameState(long time);  // update the game state
	virtual int gameLoop(void);					// start the game loop
	virtual int initGame(void);				// initialize the game
	static gameApp *myApp;
	static Prey    *preyRedKia;
	static House *targetHouse;
	static PointOfView *cam;
private:
	int wId;		// the window handle 
	static meshSurface *drawSurface;
	static meshSurface *drawVolumeSurface;
	static Chaser  *chaserYellowKia;
			// one general camera
	static std::vector<gameObject *> gameStaticEntities;	// these entities do not change with time
	static std::vector<gameObject *> gameDynamicEntities; // these entities' attributes change with time
protected:
	// static functions  - Call Back functions
	static void keyboardFun(unsigned char key, int x, int y);
	static void specialKeyboardFun(int key, int x, int y);
	static void reshapeFun(int w, int h);
	static void timerFun(int timerEvent);
	static void renderFrame();		// render the frame



//FUNCTIONS
public:
	static std::vector<House *> staticHouses;
	static GLuint terrainFrameBuf;
	static GLuint terrainTexId;

	static StencilShader *stencilShader;

	static std::vector<gameObject *> filledObjects;
	static std::vector<Silhouette *> silhouettes;
public:
	static void updateFirstPersonCamera(void);


private:
	static camera *overheadCam;
	static camera *depthTextureCam;
	static camera *lightSource;
	static bool mode;
	static bool runningGame;
	static DepthMeshSurface *depthSurface;

	static int renderMode;

	static VisibleTexture *visText;

private:
	static void scissorViewport(int x, int y, int w, int h);
	House* createHouse(Vector3f pos, Vector3f orientation, Shader *shader);
	void createShaders();
	void createTerrainBuffer();

protected:
	static Shader *defaultShader;
	static Shader *redHouseShader;
	static Shader *defaultShadowShader;
	static Shader *redHouseShadowShader;
	static Shader *blackWhiteShadowShader;
	static Shader *VolumehadowShader;
	static Shader *blackWhiteTerrainShadowShader;

	static LightingShader *lightingShader;

	static TerrainShader *terrainShader;
protected:
	static void renderShadowMappingScene(RENDER_MAT_TYPE type, camera *viewPoint, bool renderTerrain = true);	
	static void renderShadowMappingSceneWShader(RENDER_MAT_TYPE type, camera *view, Shader *s, bool terrain = true);
	
	static void renderDepthPass();
	static void renderShadowScene(Shader *shader, camera *lightSource, camera *viewCam, Shader *terrainShader = NULL);
	static void renderVisibilityPass();

	static void renderShadowVolumeScene(int type, camera *view, bool renderTerrain = true);
	static void renderShadowVolumeSceneWShader(int renderType, RENDER_MAT_TYPE type, camera *view, Shader *s, bool terrain = true, bool useBWTerrainShader = false);

	static void renderSceneIntoDepth();
	static void renderIntoStencil();
	static void renderStencilShadow();

	static void renderDepthMap();

	Shader *createShader(char* vertex, char* frag);
};
