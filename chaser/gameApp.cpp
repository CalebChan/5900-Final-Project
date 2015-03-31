
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
camera *gameApp::cam = NULL;
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
camera *gameApp::overheadCam = NULL;
camera *gameApp::lightSource = NULL;

GLuint gameApp::terrainFrameBuf;
GLuint gameApp::terrainTexId;

DepthMeshSurface *gameApp::depthSurface;

std::vector<DepthMeshSurface *> gameApp::depthEntities;

unsigned char gameApp::texture[3 * SCREEN_HEIGHT * SCREEN_WIDTH];

/******************************************************************/
/*
Purpose: constructor of the game application


Descripton: 

Return:


*/

gameApp::gameApp(void) 
{
	ZeroMemory(texture, sizeof(texture));
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

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



void gameApp::renderFrame(void)
{
	static int dir = 1;
	unsigned int i;
	static time_t oldTime = 0;
	time_t currentTime, deltaTime;
	glClearColor(0, 0, 0, 0.0);

	//glEnable(GL_SCISSOR_TEST);
	renderTerrainFrame();

	//scissorViewport(SCREEN_WIDTH * 2, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//renderRegularFrame();

	//scissorViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//renderLightFrame(true);

	glutSwapBuffers();

	currentTime = clock();
	deltaTime = currentTime - oldTime;
	if (deltaTime < MS_PER_FRAME) {
		Sleep(MS_PER_FRAME - deltaTime);  // the Sleep function has to be replaced with a draw event 
	}
	oldTime = clock();
}

void gameApp::renderRegularFrame(){

	Matrix4f model = Matrix4f::identity();
	Matrix4f view = gameApp::cam->getViewMatrix(NULL);
	Matrix4f proj = gameApp::cam->getProjectionMatrix(NULL);

	Matrix4f depthMat = proj * view * model;

	Matrix4f bias(	Vector4f(0.5, 0, 0, 0),
					Vector4f(0, 0.5, 0, 0),
					Vector4f(0, 0, 0.5, 0),
					Vector4f(0.5, 0.5, 0.5, 1));
	Matrix4f biasDepth = bias * depthMat;

	unsigned int i;
	for (i = 0; i < gameDynamicEntities.size(); i++) {
		gameDynamicEntities.at(i)->setShader(defaultShadowShader);
		gameDynamicEntities.at(i)->render(NULL, gameApp::lightSource, &biasDepth, LIGHT);
		gameDynamicEntities.at(i)->setShader(defaultShader);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		gameStaticEntities.at(i)->setShader(defaultShadowShader);
		gameStaticEntities.at(i)->render(NULL, gameApp::lightSource, &biasDepth, LIGHT);
		gameStaticEntities.at(i)->setShader(defaultShader);
	}
}

void gameApp::renderLightFrame(bool depthPass){
	unsigned int i;
	for (i = 0; i < gameDynamicEntities.size(); i++) {
		gameDynamicEntities.at(i)->render(NULL, gameApp::cam, NULL, (depthPass == true) ? DEPTH : NORMAL);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		gameStaticEntities.at(i)->render(NULL, gameApp::cam, NULL, (depthPass == true) ? DEPTH : NORMAL);
	}
}

void gameApp::renderTerrainFrame(){
	// First pass
	//scissorViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, terrainFrameBuf);
	glClear(GL_DEPTH_BUFFER_BIT);

	renderLightFrame(true);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


	depthSurface->setMeshTexture(terrainTexId);

	// Second pass
	//scissorViewport(SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	depthSurface->render(NULL, gameApp::overheadCam);

	depthSurface->revertTexture();
	
}

void gameApp::scissorViewport(int x, int y, int w, int h){
	glScissor(x, y, w, h);
	glViewport(x, y, w, h);
}

void gameApp::createShaders(){
	int rc;
	GLuint shaderId;

	printf("Build Default Shader\n");
	defaultShader = createShader("Shader\\general.vert", "Shader\\general.frag");

	printf("Build Red House Shader\n");
	redHouseShader = createShader("Shader\\generalRed.vert", "Shader\\generalRed.frag");

	printf("Build Default Shadow Shader\n");
	defaultShadowShader = createShader("Shader\\generalShadow.vert", "Shader\\generalShadow.frag");

	printf("Build Red House Shadow Shader\n");
	redHouseShadowShader = createShader("Shader\\generalRedShadow.vert", "Shader\\generalRedShadow.frag");

	printf("Build Terrain Shader\n");
	terrainShader = new TerrainShader();		/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (terrainShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = terrainShader->createShaderProgram("Shader\\terrainShader.vert", "Shader\\terrainShader.frag", &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", "general.vert", "general.frag");
		delete terrainShader;
		terrainShader = NULL;
		assert(0);
	}
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
	glGenFramebuffersEXT(1, &terrainFrameBuf);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, terrainFrameBuf);

	glGenTextures(1, &terrainTexId);
	glBindTexture(GL_TEXTURE_2D, terrainTexId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, terrainTexId, 0);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	GLenum frameStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frameStatus != GL_FRAMEBUFFER_COMPLETE){
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
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

	createShaders();

	chaserYellowKia->setShader(defaultShader);
	house1->setShader(defaultShader);
	drawSurface->setShader(defaultShader);
	depthSurface->setShader(terrainShader);
	targetHouse->setShader(redHouseShader);


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

	// set the global camera
	cam = new camera();
	cam->setCamera(Vector3f(0, 10, 20), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	cam->setPerspectiveView(DEFAULT_FOV, 1, (float) NEAR_PLANE, FAR_PLANE);

	lightSource = new camera();
	lightSource->setCamera(Vector3f(0, 10, 20), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	lightSource->setPerspectiveView(DEFAULT_FOV, 1, (float)NEAR_PLANE, FAR_PLANE);

	overheadCam = new camera();
	overheadCam->setCamera(Vector3f(0, -100, 0), Vector3f(0, 0, 0), Vector3f(0, 0, 1));
	overheadCam->setPerspectiveView(DEFAULT_FOV, 1, (float)NEAR_PLANE, FAR_PLANE);


	// set the surface
	drawSurface->createSurface(20, 20, 60, 60);
	drawSurface->loadTexture("surface\\grass_texture_256.tga");

	depthSurface->createSurface(10, 10, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10);
	depthSurface->loadTexture("surface\\grass_texture_256.tga");
	depthSurface->setOverheadCamera(overheadCam);

	// add the objects to the list of game objects
	gameDynamicEntities.push_back(chaserYellowKia);

	// add the objecst to the list of game objects
	gameStaticEntities.push_back(drawSurface);
	gameStaticEntities.push_back(house1);
	gameStaticEntities.push_back(targetHouse);

	depthEntities.push_back(depthSurface);

	

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
			cam->yaw((float) .2);
			break;
		case 'A':
			cam->yaw(1);
			break;
		case 'D':
			cam->yaw(-1);
			break;
		case 'd':
			cam->yaw((float)-.2);
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
			cam->roll((float) .2);
			break;
		case GLUT_KEY_UP:
			cam->pitch((float) .2);
			break;
		case GLUT_KEY_RIGHT:
			cam->roll((float) -.2);
			break;
		case GLUT_KEY_DOWN:
			cam->pitch((float) -.2);
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


	// Set the state to accept projection information
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
	cam->setPerspectiveView(cam->fieldOfView,aspect,cam->nearPlane,cam->farPlane);
	lightSource->setPerspectiveView(lightSource->fieldOfView, aspect, lightSource->nearPlane, lightSource->farPlane);

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



