
//=============================================================================
// gameApp.cpp 
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
// Description:
//--------------
// A simple application that demonstrates how to use directx to surface and how to use the camea.  
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


#include "gameApp.h"
#include "House.h"
#include "time.h"


#include "Constant.h"
#include "PointOfView.h"
/********************************************************************************/


// DEFINE
#define FRAMES_PER_SECOND 30
#define MS_PER_FRAME (1000.0/30)

#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0

// event types for processing game state
#define GAME_UPDATE_FRAME_STATE_EVENT 1
#define GAME_COLLISION_EVENT 2

#define NEAR_PLANE 0.1
#define FAR_PLANE 100.0

/*******************************************************************************/
// class static variables
PointOfView *gameApp::cam = NULL;
meshSurface *gameApp::drawSurface = NULL;
Chaser *gameApp::chaserYellowKia = NULL;
Prey   *gameApp::preyRedKia = NULL;
gameApp * gameApp::myApp;
House *gameApp::targetHouse = NULL;
std::vector<gameObject *> gameApp::gameStaticEntities;	// these entities do not change with time
std::vector<gameObject *> gameApp::gameDynamicEntities; // these entities' attributes change with time
std::vector<House *> gameApp::staticHouses;

bool gameApp::mode = false;
bool gameApp::runningGame = true;


Shader *gameApp::defaultShader = NULL;
TerrainShader *gameApp::terrainShader = NULL;
Shader *gameApp::redHouseShader = NULL;
Shader *gameApp::defaultShadowShader = NULL;
Shader *gameApp::redHouseShadowShader = NULL;
Shader *gameApp::blackWhiteShadowShader = NULL;
Shader *gameApp::blackWhiteTerrainShadowShader = NULL;
StencilShader *gameApp::stencilShader = NULL;

LightingShader *gameApp::lightingShader = NULL;

camera *gameApp::overheadCam = NULL;
camera *gameApp::lightSource = NULL;
camera *gameApp::depthTextureCam = NULL;

GLuint gameApp::terrainFrameBuf;
GLuint gameApp::terrainTexId;

DepthMeshSurface *gameApp::depthSurface;

int gameApp::renderMode = 0;

VisibleTexture *gameApp::visText = NULL;

std::vector<Silhouette *> gameApp::silhouettes;
std::vector<gameObject *> gameApp::filledObjects;

/******************************************************************/
/*
Purpose: constructor of the game application


Descripton: 

Return:


*/

gameApp::gameApp(void) 
{
	myApp = this;
}





/******************************************************************/
/*
Purpose: destructor of the game application


Descripton: 

Return:
1 - if failed
0 - if successful

*/

gameApp::~gameApp(void)
{
}





/******************************************************************/
/*
Purpose: initializes the graphics device

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::initGraphics(int argc, char** argv, int winWidth, int winHeight, int winPosx, int winPosy)
{
	int rc = 0;

	GLenum err;

	//  initialize GLUT and pass it the command variables
	glutInit(&argc, argv);
	
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	// Set the window
	// instruct openGL where to put the window on the screen
	glutInitWindowPosition(winPosx, winPosy);

	// instuct openGL what window size ot use
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// no magic numbers

	// careate the fist window and obtain a handle to it 
	wId = glutCreateWindow("My First Window");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set the colour buffer to black   
	glEnable(GL_DEPTH_TEST);				// Enable depth ubffer testing 
	glClearDepth(1.0f);						// Set depth buffer 
	glDepthFunc(GL_LEQUAL);					// depth test operation (this is the default)
	glShadeModel(GL_SMOOTH);				//  smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // highest quality perspective corrections

	// set the call back functions
	glutDisplayFunc(renderFrame);
	glutReshapeFunc(reshapeFun);
	glutKeyboardFunc(keyboardFun);
	glutSpecialFunc(specialKeyboardFun);
	glutTimerFunc(10, timerFun, 1);


	// initialize GLEW
	err = glewInit();
	//GLenum err = glewInit();
	if (err != GLEW_OK)  fprintf(stderr, " Error initializing GLEW! \n");
	else fprintf(stderr, "Initializing GLEW succeeded!\n");

	createTerrainBuffer();

	visText = new VisibleTexture();

	return 0;


	return(0);
}


/******************************************************************/
/*
Purpose: executes the message loop

Descripton: 
This function is a virtual function and can be replaced by a similar function in the derived class.



Return:
1 - if failed
0 - if successful

*/


int gameApp::gameLoop(void)
{
	// enter glut main loop
	glutMainLoop();

	return 0;
}


/******************************************************************/
/*
Purpose: updates the game state

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::updateGameState(long event)
{

	static int frameCount = 0;
	static int dir = 1;
	unsigned int i;
	int rc;

	switch (event) {
	case GAME_UPDATE_FRAME_STATE_EVENT:
		if (!runningGame){
			break;
		}
		frameCount++;	// increament frame count

		// this section should have the game logic with respect to time.
		// Then it calls each object with updateStateFunction

		for (i = 0; i < gameDynamicEntities.size(); i++) {
			rc = gameDynamicEntities.at(i)->updateState(frameCount);
			if (rc == GAME_OBJECT_DELETE_ENTITY) {
				// swap current object with the last one
				gameObject *temp = gameDynamicEntities.at(i);
				gameDynamicEntities.at(i) = gameDynamicEntities.back();
				delete temp;
				gameDynamicEntities.pop_back();
			}
		}
		gameApp::cam->updatePOVCamera();
		break;
	case GAME_COLLISION_EVENT:
		break;

	case GAME_OVER:
		runningGame = false;
		printf("Game Over\n");

		break;
	case GAME_WON:
		runningGame = false;
		printf("You Win\n");
		break;
	default:
		printf("another event occured \n");
		break;
	}



	return 0;
}


/******************************************************************/
/*
Purpose: renders one frame

Descripton: 

Return:
1 - if failed
0 - if successful

*/

enum RENDER_SWITCH_TYPE{
	RENDER_SHADOW_VOLUMN_VIEW,

	LAST_POSITION, // Everyting after this enum is not in the page up page down rotation

	RENDER_FULL_VIEW, // Renders everything up until current

	RENDER_UPTO_SHADOW_PASS,
	DISPLAY_DEPTH_MAP, // Display shadow draw scene
	RENDER_UPTO_VISIBLITY_PASS,
	
	DISPLAY_DEFAULT_FRAME,
	
};
enum SHADOW_VOLUME_RENDER_TYPE{
	FILLED_OBJECTS = 1,
	SILHOUTTES = 2,
};



void gameApp::renderFrame(void)
{
	static int dir = 1;
	unsigned int i;
	static time_t oldTime = 0;
	time_t currentTime, deltaTime;
	glClearColor(0, 0, 0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (LAST_POSITION == 1){
		renderMode == 0;
	}

	switch (renderMode % LAST_POSITION){
	case RENDER_UPTO_SHADOW_PASS:
		renderDepthPass();
		renderShadowScene(defaultShadowShader, gameApp::cam, gameApp::overheadCam);
		break;
	case RENDER_FULL_VIEW:
		// NOTHING Just fall into next case statement as current full view
	case RENDER_UPTO_VISIBLITY_PASS:
		renderDepthPass();
		renderShadowScene(defaultShadowShader, gameApp::cam, gameApp::overheadCam);
		renderVisibilityPass();
		break;
	case DISPLAY_DEPTH_MAP:
		renderDepthMap();
		break;
	case DISPLAY_DEFAULT_FRAME:
		renderShadowMappingScene(NORMAL, gameApp::overheadCam, true);
		break;
	
	case RENDER_SHADOW_VOLUMN_VIEW:
		renderSceneIntoDepth();
		glEnable(GL_STENCIL_TEST);
		renderIntoStencil();
		renderStencilShadow();
		glDisable(GL_STENCIL_TEST);
		//renderShadowVolumeSceneWShader(SILHOUTTES, STENCIL, gameApp::overheadCam, lightingShader, true);
		break;
	}

	glutSwapBuffers();

	currentTime = clock();
	deltaTime = currentTime - oldTime;
	if (deltaTime < MS_PER_FRAME) {
		Sleep(MS_PER_FRAME - deltaTime);  // the Sleep function has to be replaced with a draw event 
	}
	oldTime = clock();
}

void gameApp::renderShadowScene(Shader *shader, camera *lightSource, camera *viewCam, Shader *terrainShader){
	glClearColor(0, 0, 0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4f depthMat = lightSource->getProjectionMatrix(NULL) * lightSource->getViewMatrix(NULL);

	Shader *tmp = drawSurface->getShader();
	drawSurface->setShader((terrainShader == NULL) ? shader : terrainShader);
	drawSurface->render(NULL, viewCam, &depthMat, SHADOW);
	drawSurface->setShader(tmp);

	unsigned int i;
	for (i = 0; i < gameDynamicEntities.size(); i++) {
		Shader *tmp = gameDynamicEntities.at(i)->getShader();
		gameDynamicEntities.at(i)->setShader(shader);
		gameDynamicEntities.at(i)->render(NULL, viewCam, &depthMat, SHADOW);
		gameDynamicEntities.at(i)->setShader(tmp);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		Shader *tmp = gameStaticEntities.at(i)->getShader();
		gameStaticEntities.at(i)->setShader(shader);
		gameStaticEntities.at(i)->render(NULL, viewCam, &depthMat, SHADOW);
		gameStaticEntities.at(i)->setShader(tmp);
	}
}

void gameApp::renderShadowMappingScene(RENDER_MAT_TYPE type, camera *viewPoint, bool renderTerrain){
	
	unsigned int i;
	for (i = 0; i < gameDynamicEntities.size(); i++) {
		gameDynamicEntities.at(i)->render(NULL, viewPoint, NULL, type);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		gameStaticEntities.at(i)->render(NULL, viewPoint, NULL, type);
	}
	
	if (renderTerrain){
		drawSurface->render(NULL, viewPoint, NULL, type);
	}
}

void gameApp::renderShadowMappingSceneWShader(RENDER_MAT_TYPE type, camera *view, Shader *s, bool terrain){
	unsigned int i;
	for (i = 0; i < gameDynamicEntities.size(); i++) {
		Shader *tmp = gameDynamicEntities.at(i)->getShader();
		gameDynamicEntities.at(i)->setShader(s);
		gameDynamicEntities.at(i)->render(NULL, view, NULL, type);
		gameDynamicEntities.at(i)->setShader(tmp);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		Shader *tmp = gameStaticEntities.at(i)->getShader();
		gameStaticEntities.at(i)->setShader(s);
		gameStaticEntities.at(i)->render(NULL, view, NULL, type);
		gameStaticEntities.at(i)->setShader(tmp);
	}

	if (terrain){
		printf("Render : TRUE\n");
		drawSurface->render(NULL, view, NULL, type);
	}
}

void gameApp::renderDepthPass(){
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFrameBuf);
	glClearColor(0, 0, 0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	renderShadowMappingScene(DEPTH, gameApp::cam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gameApp::renderVisibilityPass(){
	glClearColor(0, 0, 0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	visText->bindTexture();
	renderShadowScene(blackWhiteShadowShader, gameApp::cam, gameApp::overheadCam, blackWhiteTerrainShadowShader);
	visText->unbindTexture();

	Matrix4f mat = overheadCam->getProjectionMatrix(NULL) * overheadCam->getViewMatrix(NULL);

	if (visText->checkIfValidPointOnTexture(cam->position + cam->lookAtVector * 10, mat)){
		//printf("Check\n");
		Vector3f pos = visText->generateValidDirections(cam->position, cam->lookAtVector, mat, 45, 30, 1);
		gameApp::cam->setNextLocation(pos);
	}
	

	lightSource->setCamera(cam->getPosition(), cam->getLookAtPoint(), cam->getUpVector());
	lightSource->changePositionDelta(0, -5, 0);

	//renderShadowScene(defaultShadowShader, gameApp::cam, gameApp::overheadCam, defaultShadowShader);
	renderShadowScene(blackWhiteShadowShader, gameApp::cam, gameApp::overheadCam, blackWhiteTerrainShadowShader);
	//Matrix4f depthMat = overheadCam->getProjectionMatrix(NULL) * overheadCam->getViewMatrix(NULL);
}

void gameApp::renderDepthMap(){
	glClearColor(0, 0, 0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	depthSurface->setMeshTexture(gameApp::terrainTexId);
	depthSurface->render(NULL, depthTextureCam);
}





void gameApp::renderShadowVolumeScene(int type, camera *view, bool renderTerrain){
	for (int i = 0; i < silhouettes.size(); i++){
		silhouettes.at(i)->render(NULL, NULL, view);
	}

	if (renderTerrain){
		drawSurface->render(NULL, view, NULL, NORMAL);
	}
}



void gameApp::renderShadowVolumeSceneWShader(int renderType, RENDER_MAT_TYPE type, camera *view, Shader *s, bool terrain){

	for (int i = 0; i < silhouettes.size(); i++){
		Shader *tmp = silhouettes.at(i)->getShader();
		silhouettes.at(i)->setShader(s);
		silhouettes.at(i)->render(NULL, NULL, view, type);
		silhouettes.at(i)->setShader(tmp);
	}

	if (terrain){
		drawSurface->render(NULL, view, NULL, NORMAL);
	}
}

void gameApp::renderSceneIntoDepth(){
	glDepthMask(GL_TRUE);
	glClearColor(0, 0, 0, 0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glDrawBuffer(GL_NONE);
	
	renderShadowVolumeScene(SILHOUTTES, gameApp::cam, true);
	//renderShadowVolumeSceneWShader(SILHOUTTES, STENCIL, gameApp::cam, defaultShader, true);
}

void gameApp::renderIntoStencil(){
	
	glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP); 
    glDisable(GL_CULL_FACE);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0xff);

	// Set the stencil test per the depth fail algorithm
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	stencilShader->setLightPosition(overheadCam->getPosition());
	renderShadowVolumeSceneWShader(SILHOUTTES, STENCIL, gameApp::cam, stencilShader, false);

	// Restore local stuff
	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
}

void gameApp::renderStencilShadow(){

	glDrawBuffer(GL_BACK);

	// Draw only if the corresponding stencil value is zero
	glStencilFunc(GL_EQUAL, 0x00, 0xFF);

	// prevent update to the stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// prevent update to the stencil buffer
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);

	lightingShader->setAmbientIntensity(1.0);
	lightingShader->setDiffiuseIntensity(0.8);
	lightingShader->setLightPosition(overheadCam->getPosition());

	glDepthMask(GL_TRUE);

	glDisable(GL_STENCIL_TEST);
	renderShadowVolumeSceneWShader(SILHOUTTES, NORMAL, gameApp::cam, lightingShader, true);
	
	

	//renderScene(NORMAL, gameApp::overheadCam, true);
}



void gameApp::scissorViewport(int x, int y, int w, int h){
	glScissor(x, y, w, h);
	glViewport(x, y, w, h);
}

void gameApp::createShaders(){
	int rc;
	GLuint shaderId;

	printf("Build Default Shader\n");
	defaultShader = createShader("Shader\\Default_Shader\\general.vert", "Shader\\Default_Shader\\general.frag");
	printf("\n");

	printf("Build Red House Shader\n");
	redHouseShader = createShader("Shader\\Default_Shader\\general.vert", "Shader\\Default_Shader\\generalRed.frag");
	printf("\n");

	printf("Build Default Shadow Shader\n");
	defaultShadowShader = createShader("Shader\\Shadow_Shader\\general.vert", "Shader\\Shadow_Shader\\general.frag");
	printf("\n");

	printf("Build Red House Shadow Shader\n");
	redHouseShadowShader = createShader("Shader\\Shadow_Shader\\general.vert", "Shader\\Shadow_Shader\\Red.frag");
	printf("\n");

	printf("Build Black White Shadow Shader\n");
	blackWhiteShadowShader = createShader("Shader\\Shadow_Shader\\general.vert", "Shader\\Shadow_Shader\\BWObject.frag");
	printf("\n");

	printf("Build Black White Terrain Shadow Shader\n");
	blackWhiteTerrainShadowShader = createShader("Shader\\Shadow_Shader\\general.vert", "Shader\\Shadow_Shader\\BWTerrain.frag");
	printf("\n");

	printf("Build Stencil Shader\n");
	stencilShader = new StencilShader();		/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (stencilShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = stencilShader->createShaderProgram("Shader\\Stencil_Shader\\general.vert", "Shader\\Stencil_Shader\\general.frag", "Shader\\Stencil_Shader\\general.geom", &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", "general.vert", "general.frag");
		delete stencilShader;
		stencilShader = NULL;
		assert(0);
	}
	printf("\n");

	printf("Build Terrain Shader\n");
	terrainShader = new TerrainShader();		/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (terrainShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = terrainShader->createShaderProgram("Shader\\Depth_Map_Shader\\depthMap.vert", "Shader\\Depth_Map_Shader\\depthMap.frag", &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", "general.vert", "general.frag");
		delete terrainShader;
		terrainShader = NULL;
		assert(0);
	}

	printf("Build Lighting Shader\n");
	lightingShader = new LightingShader();		/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (lightingShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = lightingShader->createShaderProgram("Shader\\Lighting_Shader\\general.vert", "Shader\\Lighting_Shader\\general.frag", &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", "general.vert", "general.frag");
		delete lightingShader;
		lightingShader = NULL;
		assert(0);
	}
	lightingShader->init();
	printf("\n");
}

Shader *gameApp::createShader(char *vertex, char* frag){
	int rc;
	GLuint shaderId;
	Shader *shader = new Shader();		/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (shader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = shader->createShaderProgram(vertex, frag, &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", vertex, frag);
		delete shader;
		shader = NULL;
		assert(0);
	}
	return shader;
}

void gameApp::createTerrainBuffer(){

	terrainFrameBuf = 0;
	glGenFramebuffers(1, &terrainFrameBuf);
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFrameBuf);

	glGenTextures(1, &terrainTexId);
	glBindTexture(GL_TEXTURE_2D, terrainTexId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, terrainTexId, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum frameStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frameStatus != GL_FRAMEBUFFER_COMPLETE){
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/******************************************************************/
/*
Purpose: initialize the game

Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameApp::initGame(void)
{
	int rc;
		// create game objects
	House *house1 = new House();
	targetHouse = new House();
	chaserYellowKia = new Chaser();
	drawSurface = new meshSurface();
	depthSurface = new DepthMeshSurface();

	Silhouette *sil = new Silhouette();
	House *ball = new House();

	createShaders();

	chaserYellowKia->setShader(defaultShader);
	house1->setShader(defaultShader);
	drawSurface->setShader(defaultShader);
	depthSurface->setShader(terrainShader);
	//targetHouse->setShader(redHouseShader);
	targetHouse->setShader(defaultShader);
	ball->setShader(defaultShader);


	// set the target house
	targetHouse->loadModelOBJ("targethouse\\target.obj", &targetHouse->mVtxBuf, &targetHouse->mNumVtx, &targetHouse->mIndBuf, &targetHouse->mNumInd);
	// load the textures
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	targetHouse->setScale((float) 0.015, (float)  0.015, (float) 0.02);
	targetHouse->mYaw = 180;
	targetHouse->setPositionOrientation(Vector3f(-3, 2.13, (float) -10.2), Vector3f(1, 0, 1), Vector3f(0, 1, 0));

	// set the chaser object
	// load the gemoetry model
	chaserYellowKia->loadModelOBJ("kia\\kia_rio.obj",&chaserYellowKia->mVtxBuf, &chaserYellowKia->mNumVtx,&chaserYellowKia->mIndBuf, &chaserYellowKia->mNumInd);
	// load the textures
	chaserYellowKia->loadTexture("kia\\rio_yellow.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed
	chaserYellowKia->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	chaserYellowKia->mYaw=90;		// orient it so that it faces that +zaxis
	chaserYellowKia->setPositionOrientation(Vector3f(-10,0,10), Vector3f(0,0,1),Vector3f(0,1,0));

	// set the house object
	house1->loadModelOBJ("house_obj\\house_obj.obj",&house1->mVtxBuf, &house1->mNumVtx,&house1->mIndBuf, &house1->mNumInd);
	// load the textures
	house1->loadTexture("house_obj\\house_diffuse_256.tga");
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	house1->setScale((float) 0.006, (float)  0.006, (float) 0.006);
	house1->setPositionOrientation(Vector3f(6, 0, (float)1), Vector3f(1, 0, 0), Vector3f(0, 1, 0));
	staticHouses.push_back(house1);
	// set the house object

	sil->setShader(stencilShader);
	sil->loadModelOBJ("ball\\ball.obj", &sil->mVtxBuf, &sil->mNumVtx, &sil->mIndBuf, &sil->mNumInd);
	sil->loadTexture("ball\\ball_tex.png");

	sil->setScale((float)1, (float)1, (float)1);
	sil->setPositionOrientation(Vector3f(0, 5, (float)10), Vector3f(1, 0, 0), Vector3f(0, 1, 0));
	// load the textures
	silhouettes.push_back(sil);

	

	// set the global camera
	cam = new PointOfView();
	cam->setCamera(Vector3f(0, 7, 20), Vector3f(0, 7, 0), Vector3f(0, 1, 0));
	//cam->setCamera(Vector3f(0, 30, 0), Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	cam->setPerspectiveView(DEFAULT_FOV, 1, (float) NEAR_PLANE, FAR_PLANE);
	cam->setNextLocation(cam->position + cam->lookAtVector);

	lightSource = new camera();
	lightSource->setCamera(Vector3f(0, 5, 60), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	lightSource->setPerspectiveView(DEFAULT_FOV, 1, (float)NEAR_PLANE, FAR_PLANE);

	overheadCam = new camera();
	overheadCam->setCamera(Vector3f(0, 50, 0), Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	overheadCam->setPerspectiveView(DEFAULT_FOV, 1, (float)NEAR_PLANE, FAR_PLANE);

	depthTextureCam = new camera();
	depthTextureCam->setCamera(Vector3f(0, -30, 0), Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	depthTextureCam->setPerspectiveView(DEFAULT_FOV, 1, (float)NEAR_PLANE, FAR_PLANE);


	// set the surface
	drawSurface->createSurface(1, 1, 40, 40);
	drawSurface->loadTexture("surface\\grass_texture_256.tga");
	drawSurface->setShader(defaultShader);

	//depthSurface->createSurface(10, 10, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10);
	depthSurface->createSurface(1, 1, 40, 40);
	depthSurface->loadTexture("surface\\grass_texture_256.tga");
	depthSurface->setOverheadCamera(overheadCam);

	// add the objects to the list of game objects
	gameDynamicEntities.push_back(chaserYellowKia);

	// add the objecst to the list of game objects
	//gameStaticEntities.push_back(drawSurface);
	gameStaticEntities.push_back(house1);
	gameStaticEntities.push_back(targetHouse);


	return 0;
}


void gameApp::updateFirstPersonCamera(void){
	if (!mode){
		return;
	}
	Vector3f newPos(preyRedKia->mPosition.x, preyRedKia->mPosition.y + 1.5, preyRedKia->mPosition.z);
	Vector3f newLook = newPos + DEFAULT_PREY_LOOK_AT_VECTOR;
	cam->setCamera(newPos, newLook, Vector3f(0, 1, 0));
	cam->yaw(preyRedKia->totalYaw);
}


/****************************************************************************/



House* gameApp::createHouse(Vector3f pos, Vector3f orientation, Shader *shader){
	House *h = new House();
	h->setShader(shader);

	h->loadModelOBJ("house_obj\\house_obj.obj", &h->mVtxBuf, &h->mNumVtx, &h->mIndBuf, &h->mNumInd);
	// load the textures
	h->loadTexture("house_obj\\house_diffuse_256.tga");
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	h->setScale(0.006, 0.006, 0.006);
	h->setPositionOrientation(pos, orientation, Vector3f(0, 1, 0));
	return h;
}

void gameApp::keyboardFun(unsigned char key, int x, int y) 
{

	if (!runningGame){
		switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		}

		return;
	}

	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;

		case 'c':
		{
			mode = true;
			updateFirstPersonCamera();
			cam->setPerspectiveView(DEFAULT_FOV, 1, (float) 0.2, 1000);
			
			break;
		}
		case 'C':
			cam->setCamera(Vector3f(0, 20, 40), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
			cam->setPerspectiveView(DEFAULT_FOV, 1, (float) 0.2, 1000);
			mode = false;
			break;
		
		case 'i':
		case 'I':
			preyRedKia->moveForward(MAX_CAR_MOVE_SPEED);
			preyRedKia->updatePrey(MAX_CAR_MOVE_SPEED, 0);
			if (mode){
				updateFirstPersonCamera();
			}
			break;

		case 'j':
		case 'J':
			preyRedKia->yaw(MAX_CAR_TURN_SPEED);
			preyRedKia->updatePrey(0, MAX_CAR_TURN_SPEED);
			if (mode){
				updateFirstPersonCamera();
			}
			break;

		case 'k':
		case 'K':
			preyRedKia->moveForward(-MAX_CAR_MOVE_SPEED);
			preyRedKia->updatePrey(-MAX_CAR_MOVE_SPEED, 0);
			if (mode){
				updateFirstPersonCamera();
			}
			break;

		case 'l':
		case 'L':
			preyRedKia->yaw(-MAX_CAR_TURN_SPEED);
			preyRedKia->updatePrey(0, -MAX_CAR_TURN_SPEED);
			if (mode){
				updateFirstPersonCamera();
			}
			break;

		case 'w':
			cam->moveForward(MAX_CAM_MOVE_SPEED);
			break;
		case 'W':
			cam->moveForward(MAX_CAM_TURN_SPEED);
			break;
		case 's':
			cam->moveForward(-MAX_CAM_MOVE_SPEED);
			break;
		case 'S':
			cam->moveForward(-MAX_CAM_TURN_SPEED);
			break;
		case 'a':
			cam->yaw((float)CAMERA_MOVE_SPEED);
			break;
		case 'A':
			cam->yaw(FAST_CAMERA_MOVE_SPEED);
			break;
		case 'D':
			cam->yaw(-FAST_CAMERA_MOVE_SPEED);
			break;
		case 'd':
			cam->yaw((float)-CAMERA_MOVE_SPEED);
			break;
		case 'z':
			cam->zoomIn();
			break;
		case 'Z':
			cam->zoomOut();
			break;
	}
}

/***************************************************************************************/

void gameApp::specialKeyboardFun(int key, int x, int y) 
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case GLUT_KEY_LEFT:
			silhouettes.at(0)->roll((float)CAMERA_MOVE_SPEED);
			break;
		case GLUT_KEY_UP:
			silhouettes.at(0)->pitch((float)CAMERA_MOVE_SPEED);
			break;
		case GLUT_KEY_RIGHT:
			silhouettes.at(0)->roll((float)-CAMERA_MOVE_SPEED);
			break;
		case GLUT_KEY_DOWN:
			silhouettes.at(0)->pitch((float)-CAMERA_MOVE_SPEED);
			break;
		case GLUT_KEY_PAGE_UP:
			renderMode++;
			printf("Render Mode : %d\n", renderMode);
			break;
		case GLUT_KEY_PAGE_DOWN:
			renderMode--;
			if (renderMode < 0){
				renderMode = 0;
			}
			printf("Render Mode : %d\n", renderMode);
			break;
	}
}



/************************************************************************************************/
/*
This functions in invoked when ther
e is a change to the window by the user
 Here a new setting of the rendering paramters are set. In most cases it consists of two things:
a. changeing the view port dimensions
b. changing the aspect ratio

input 
w, h - the width and height of the new window in the pixel coordinates

*/

void gameApp::reshapeFun(int w, int h)
{

	static GLfloat aspect;			// aspect ratio of height and width

	// change the view port
	glViewport (0 , 0, (GLsizei) w, (GLsizei) h);

	// Compute aspect ratio of the new window
	aspect = (float)w / (float)h;

	printf("Resize window W : %d  H : %d\n", w, h);
	// Set the state to accept projection information
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(DEFAULT_FOV, aspect, 0.1f, 100.0f);
	cam->setPerspectiveView(cam->fieldOfView,aspect,cam->nearPlane,cam->farPlane);
	lightSource->setPerspectiveView(lightSource->fieldOfView, aspect, lightSource->nearPlane, lightSource->farPlane);
	overheadCam->setPerspectiveView(overheadCam->fieldOfView, aspect, overheadCam->nearPlane, overheadCam->farPlane);

	glBindTexture(GL_TEXTURE_2D, terrainTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
 


/***************************************************************************************************/

void gameApp::timerFun(int timerEvent)
{
	static int frameNumber = 1;
	frameNumber;

	//	renderSceneFun();
	glutPostRedisplay();								// should be moved to update game state as a redraw state
	// get next event from Queue  (future implementation) 
	// the queue should have return an event and the event timer

	myApp->updateGameState((long) GAME_UPDATE_FRAME_STATE_EVENT);
	glutTimerFunc(30, timerFun, frameNumber);					// move to the updateGameState()

}



