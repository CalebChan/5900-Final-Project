#pragma once

#include "car.h"
#include "SilhouetteExtra.h"

#include <vector>
#include <map>

class Silhouette : public Car{
public:
	Silhouette();
	int createGraphicsBuffers(Shader *shader);
	void FindAdjacent(const struct carVertex *vBuff, GLuint *Indics, GLuint numVert);
	int render(Matrix4f *mvp, Matrix4f *obj, camera *cam, RENDER_MAT_TYPE type = NORMAL);
	void computeOtherStuff(const struct carVertex *vBuff, GLuint *Indics, GLuint numVert);

private:
	std::vector<GLuint> indicies;
	std::map<Vector4f, GLuint, CompareVectors> posMap;
	std::map<Edge, Neighbour, CompareEdges> indexMap;
	std::vector<Face> uniqueFaces;

	std::vector<struct carVertex> vertexs;
};