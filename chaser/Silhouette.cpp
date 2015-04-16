#include "Silhouette.h"

Silhouette::Silhouette():Car(){
}

int Silhouette::createGraphicsBuffers(Shader *shader)
{

	int rc = 0;
	struct carVertex *v = NULL;
	long *ind = NULL;


	//create vertex array object
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//create vertex buffer object
	glGenBuffers(1, &mVtxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVtxVbo);
	//glBufferData(GL_ARRAY_BUFFER, mNumVtx * sizeof(struct carVertex), mVtxBuf, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(struct carVertex), &vertexs[0], GL_STATIC_DRAW);

	//create vertex buffer object
	glGenBuffers(1, &mIndVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies[0]) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

	configureShader(shader);

	//end creation
	glBindVertexArray(0);

	return(rc);
}

int Silhouette::render(Matrix4f *mvp, Matrix4f *obj, camera *cam, RENDER_MAT_TYPE type)
{
	Matrix4f modelMat = Matrix4f::rotateY(this->mYaw, 1)*Matrix4f::scale(this->mScaleX, this->mScaleY, this->mScaleZ);
	Matrix4f modelWorldMat = Matrix4f::objectMatrix(this->mPosition, this->mPosition + this->lookAtVector, this->upVector)*modelMat;

	renderShaderSetup(modelWorldMat, cam->getViewMatrix(NULL), cam->getProjectionMatrix(NULL), Matrix4f::identity(), NULL, type);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
	glUniform1i(texLoc, 4);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// redner the triangles
	glBindVertexArray(mVao);

	glDrawElements(GL_TRIANGLES_ADJACENCY, indicies.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}

void Silhouette::computeOtherStuff(const struct carVertex *vBuff, GLuint *Indics, GLuint numVert){
	FindAdjacent(vBuff, Indics, numVert);
}

void Silhouette::FindAdjacent(const struct carVertex *vBuff, GLuint *Indics, GLuint numVert){
	std::map<GLuint, struct carVertex> indexPosMap;
	for (int i = 0; i < numVert; i += 3){

		Face face;

		for (int j = 0; j < 3; j++) {
			GLuint Index = Indics[i + j];
			Vector4f v = vBuff[i + j].pos;

			if (indexPosMap.find(Index) == indexPosMap.end()){
				indexPosMap[Index] = vBuff[i + j];
			}

			if (posMap.find(v) == posMap.end()) {
				posMap[v] = Index;
			}
			else {
				Index = posMap[v];
			}

			face.addIndex(Index, j);
		}

		uniqueFaces.push_back(face);

		Edge e1(face.getIndex(0), face.getIndex(1));
		Edge e2(face.getIndex(1), face.getIndex(2));
		Edge e3(face.getIndex(2), face.getIndex(0));

		indexMap[e1].addNeighbour(i / 3);
		indexMap[e2].addNeighbour(i / 3);
		indexMap[e3].addNeighbour(i / 3);
	}

	for (int i = 0; i < uniqueFaces.size(); i++){
		Face f = uniqueFaces[i];

		for (int j = 0; j < 3; j++){
			Edge e(f.getIndex(j), f.getIndex((j + 1) % 3));
			assert(indexMap.find(e) != indexMap.end());
			Neighbour n = indexMap[e];
			GLuint otherTri = n.getOther(i);

			assert(otherTri != (GLuint)-1);

			Face otherFace = uniqueFaces[otherTri];
			GLuint oppInd = otherFace.getOppositeIndex(e);
			indicies.push_back(f.getIndex(j));
			indicies.push_back(oppInd);
		}
	}
	printf("IND :\n");
	for (int i = 0; i < indicies.size();){
		printf("Face :\n");
		for (int j = 0; j < 6; j++, i++){
			printf("\t : %d %d\n", i, indicies.at(i));
		}
	}

	for (int i = 0; i < indexPosMap.size(); i++){
		vertexs.push_back(indexPosMap[i]);
	}
	
}