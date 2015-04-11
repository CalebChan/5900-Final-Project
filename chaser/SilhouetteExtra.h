#pragma once

#include "glew.h"
#include "glut.h"
#include "gl/gl.h"


class Edge{
public:
	Edge(GLuint e1, GLuint e2) {

		assert(e1 != e2);

		if (e1 < e2){
			vect1 = e1;
			vect2 = e2;
		}
		else{
			vect1 = e2;
			vect2 = e1;
		}
	}

	GLuint getPoint1(){
		return vect1;
	}

	GLuint getPoint2(){
		return vect2;
	}

	GLuint vect1;
	GLuint vect2;

};

class Face{
public:
	void addIndex(GLuint point, int i){
		index[i] = point;
	}

	GLuint getIndex(int i){
		return index[i];
	}

	GLuint getOppositeIndex(Edge e){

		for (int i = 0; i < 3; i++){
			GLuint ind = index[i];
			if (ind != e.getPoint1() && ind != e.getPoint2()){
				return ind;
			}
		}
		assert(0);
		return 0;
	}

private:
	GLuint index[3];
};



class Neighbour{
public:
	Neighbour(){
		n1 = n2 = (GLuint)-1;
	}

	void addNeighbour(GLuint index){
		if (n1 == (GLuint)-1){
			n1 = index;
		}
		else if (n2 == (GLuint)-1){
			n2 = index;
		}
		else{
			assert(0);
		}

	}

	GLuint getOther(int i){
		if (i == n1){
			return n2;
		}
		else if (i == n2){
			return n1;
		}
		else{
			assert(0);
		}
		return 0;
	}

private:
	GLuint n2;
	GLuint n1;
};

struct CompareEdges
{
	bool operator()(const Edge& Edge1, const Edge& Edge2)
	{
		if (Edge1.vect1 < Edge2.vect1) {
			return true;
		}
		else if (Edge1.vect1 == Edge2.vect1) {
			return (Edge1.vect2 < Edge2.vect2);
		}
		else {
			return false;
		}
	}
};


struct CompareVectors
{
	bool operator()(const Vector4f& a, const Vector4f& b)
	{
		if (a.x < b.x) {
			return true;
		}
		else if (a.x == b.x) {
			if (a.y < b.y) {
				return true;
			}
			else if (a.y == b.y) {
				if (a.z < b.z) {
					return true;
				}				
			}
		}

		return false;
	}
};