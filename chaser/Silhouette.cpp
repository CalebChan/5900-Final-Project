#include "Silhouette.h"

#include <vector>
#include <map>

using namespace std;

int Silhouette::loadModelOBJ(char *fileName, struct carVertex **vtxBuf, GLuint *numVtx, GLuint **indBuf, GLuint *numInd){

}

void FindAdjacent(const struct carVertex *vBuff, GLuint *Indics, GLuint numVert){

	map<Vector4f, GLuint> posMap;
	map<Edge, Neighbour> indexMap;
	vector<Face> uniqueFaces;

	for (int i = 0; i < numVert; i += 3){

		Face face;

		for (int j = 0; j < 3; j++){
			GLuint index = Indics[i + j];
			Vector4f pos = vBuff[i + j].pos;

			if (posMap.find(pos) == posMap.end()){
				posMap[pos] = index;
			}
			else{
				index = posMap[pos];
			}

			face.addIndex(index, i);
		}
		uniqueFaces.push_back(face);

		Edge e1(face.getIndex(0), face.getIndex(1));
		Edge e2(face.getIndex(1), face.getIndex(2));
		Edge e3(face.getIndex(2), face.getIndex(0));

		indexMap[e1].addNeighbour(i / 3);
		indexMap[e2].addNeighbour(i / 3);
		indexMap[e3].addNeighbour(i / 3);
	}

	for (int i = 0; i < numVert; i += 3){
		Face f = uniqueFaces[i / 3];

		for (int j = 0; j < 3; j++){
			Edge e(f.getIndex(i), f.getIndex((j + 1) % 3));
			assert(indexMap.find(e) != indexMap.end());
			Neighbour n = indexMap[e];
			GLuint otherTri = n.getOther(i / 3);

			assert(otherTri != -1);
		}
	}
}