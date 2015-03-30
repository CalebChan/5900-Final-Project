#include "stdio.h"
#include "stdlib.h"
#include "Shader.h"


Shader::Shader(void)
{
	shaderProgramid = 0;
	vertShaderid = 0;
	fragShaderid = 0;
}


Shader::~Shader(void)
{
}


char * Shader::readCode(char * fileName)
{
	char * shaderCode = NULL;
	int codeLength = 0;
	FILE *fp = NULL;

    // check for error in file name

    fp = fopen(fileName, "r");	// open file and check for errors
    if ( fp == NULL ) { return NULL; }

     // fine the length of code
    fseek(fp, 0L, SEEK_END); 
    codeLength = ftell(fp);
    rewind(fp);		// could use fseek(fp, 0L, SEEK_SET)

	if (codeLength > 0) {
		// allocated space for code and check for errors
		shaderCode = (char *) malloc(codeLength+1);	// add a space for the '\0'
		if (shaderCode == NULL) return(NULL);
		memset((void *) shaderCode, 0, codeLength+1);
		fread((void *) shaderCode, sizeof(char), codeLength, fp);
		shaderCode[codeLength] = 0;
	}

	if (fp != NULL) fclose(fp);

	return shaderCode;
}


/******************************************************************************/
/* create the shaders

 input:
 fileName - file name of shadre.  Path is assume to be the project.  
		Otherwise, pass in the directory as well

shaderType - can either be vertex shader or fragment shader.  The type
		must correspond to the OpenGL shader types.

Output:
shaderid - a handle to the shader.

return:
0 - if successfull
-1 if error

*/

int Shader::createShaderObj(char* fileName , int shaderType, GLuint *shaderid)
{
	char *code = NULL;
	int rc = 0;


	// create a shader handle
	*shaderid = glCreateShader(shaderType);

	// read the code
	code = readCode(fileName);
	if (code == NULL) return(-1);

	// attach the source to the shaders
	glShaderSource(*shaderid, 1, &code, NULL);

	// compile the code
	glCompileShader(*shaderid);;

	// check for errors
	glGetShaderiv(*shaderid, GL_COMPILE_STATUS, &rc);
	if (rc != GL_TRUE) {
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr,"Error when creating a shader \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetShaderiv(*shaderid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *) malloc(bufSize*sizeof(char)+1);
		if (error != NULL) {
			glGetShaderInfoLog(*shaderid, bufSize, &length, error);
			fprintf(stderr,"%s \n", error);
		}
		if (error != NULL) free(error);
		rc = -1;
	} else rc = 0;

	if (code != NULL) free(code);	// free the source code of the shader

	return(rc);
		
}


/***************************************************************************************************************/

/* functions creates a shader program.  The two shader programs (vertex and fragment) were already compiled.
 input:
vertShaderid - a handle to the vertex shader

fragShaderid - a handle to the fragment shader

Output:
shhderProgId - a handle to the shader program.

return:
0 - if successfull
-1 if error
*/

GLint Shader::ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint *shaderProgId)
{

	int rc = 0;
	// get a handle to the shader program
	shaderProgramid = glCreateProgram();
	// connect the shaders subprogram to the "main" program
	glAttachShader(shaderProgramid, vertShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr,"error in attach shaders \n");
		if (rc == GL_INVALID_VALUE) printf("error is GL_INVALID_VALUE \n");
		rc = -1;
		goto err;
	} 

	glAttachShader(shaderProgramid, fragShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr,"error in attach shaders \n");
		rc = -1;
		goto err;
	} 

	// bind a varialbe before linking 
	//glBindAttribLocation(shaderProgramid, 7, "testNormal");
	glLinkProgram(shaderProgramid);

	// check for errors
	glGetProgramiv(shaderProgramid, GL_LINK_STATUS, &rc);

	if (rc != GL_TRUE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr,"Error when creating a shader program \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *) malloc(bufSize*sizeof(char)+1);
		if (error != NULL) {
			glGetProgramInfoLog(shaderProgramid, bufSize, &length, error);
			printf("%s \n", error);
		} 
		if (error != NULL) free(error);
	} else rc = 0;

	shaderStatus();  // may want to remove it.

	if (shaderProgId != NULL) *shaderProgId = shaderProgramid;
	glDetachShader(shaderProgramid, vertShaderid);
	glDetachShader(shaderProgramid, fragShaderid);
	//glDeleteShader(vertShaderid);
	//glDeleteShader(fragShaderid);
err:
	return (rc);
}

/***************************************************************************************************************/

/* creates a shader program from files vsFileName and fsFileName

input:
vsFileName - name of vertex shader

fsFileName - name of fragment shader

output:
shaderProgramId - a handle to the shader program.  Note that it is also stored
internally.  

return:
0 if succssfull
!=0 - if error

*/


int Shader::createShaderProgram(char * vsFileName, char * fsFileName, GLuint *shaderProgramid)
{

	int rc = 0;
		
	rc = createShaderObj(vsFileName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsFileName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = ceateShaderProgram(vertShaderid, fragShaderid,shaderProgramid);
	}
	this->shaderProgramid = *shaderProgramid;
	return(rc);
}




/*********************************************************************/
// print the status and some information about the linked shader program

int Shader::shaderStatus(void)
{
	int rc;

	glGetProgramiv(shaderProgramid, GL_VALIDATE_STATUS , &rc);
	if (rc == 0) {
		fprintf(stderr,"shader is valid status = %d \n", rc);
	} else {
		fprintf(stderr,"shader is not valid status = %d \n", rc);
	}

	rc = 0;	
	glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &rc); 
	if (rc == 0) {
		fprintf(stderr,"shader program info log length= %d \n", rc);
	} else {
		char s[1024];
		int length = 0;
		s[0]=0;
		glGetProgramInfoLog(shaderProgramid, 1024, &length, s);
		fprintf(stderr,"shader info log = %s \n", s);
	}

	glGetProgramiv(shaderProgramid, GL_ATTACHED_SHADERS, &rc); 
	fprintf(stderr,"shader number ofattached shaders= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTES, &rc); 
	fprintf(stderr,"shader number of active attributes= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH , &rc);
	fprintf(stderr,"shader maximum name length of an active attriute= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORMS, &rc);
	fprintf(stderr,"shader number of active uniforms= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &rc);
	fprintf(stderr,"shader maximum name length of an active uniform= %d \n", rc);

	int total = -1;
	glGetProgramiv(this->getProgId(), GL_ACTIVE_UNIFORMS, &total);
	for (int i = 0; i<total; ++i)  {
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(this->getProgId(), GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
		name[name_len] = 0;
		GLuint location = glGetUniformLocation(this->getProgId(), name);
		printf("\t Name %s : Local %d\n", name, location);
	}


	return 0;
}

/*******************************************************************************************/


GLuint Shader::getProgId()
{

	return (this->shaderProgramid);
}


int Shader::copyMatrixToShader(Matrix4f mat, char *matName)

{
	int loc = glGetUniformLocation(this->getProgId(), matName);
	if (loc == -1){
		printf("ERROR HAS OCCURED : \n");
		int total = -1;
		glGetProgramiv(this->getProgId(), GL_ACTIVE_UNIFORMS, &total);
		for (int i = 0; i<total; ++i)  {
			int name_len = -1, num = -1;
			GLenum type = GL_ZERO;
			char name[100];
			glGetActiveUniform(this->getProgId(), GLuint(i), sizeof(name) - 1, &name_len, &num, &type, name);
			name[name_len] = 0;
			GLuint location = glGetUniformLocation(this->getProgId(), name);
			printf("\t Name %s : Local %d\n", name, location);
		}

		assert(loc != -1);
	}
	glUniformMatrix4fv(loc, 1, true, (float *) mat.vm);
	return 0;
}
