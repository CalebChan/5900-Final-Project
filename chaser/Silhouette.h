#pragma once

#include "car.h"

class Silhouette : public Car{
	int loadModelOBJ(char *fileName, struct carVertex **vtxBuf, GLuint *numVtx, GLuint **indBuf, GLuint *numInd);

};

class Face{
public:
	void addVertex(Vector4f point, int i){
		if (i < 2 && i >= 0){
			points[i] = Vector4f(point);
		}
	}

	void addIndex(GLuint point, int i){
		if (i < 2 && i >= 0){
			index[i] = point;
		}
	}

	GLuint getIndex(int i){
		return index[i];
	}

private:
	Vector4f points[3];
	GLuint index[3];
};

class Edge{
public:
	Edge(GLuint e1, GLuint e2) : vect1(e1), vect2(e2){
	}

private:
	GLuint vect1;
	GLuint vect2;

};

class Neighbour{
public:
	void addNeighbour(GLuint index){
		neighbour = index;
	}

	GLuint getOther(int i){
		return neighbour;
	}

private:
	GLuint neighbour;
};