#include "pch.h"

void test(Mesh* mesh) {

	for (int i = 0; i < 10; i++) {
		mesh->vertices[i].Position[0]+= controlMove_0.x;
		mesh->vertices[i].Position[1] += controlMove_0.y;
		mesh->vertices[i].Position[2] += controlMove_0.z;
	}
	mesh->updateVertex();
	cout << "1";
}