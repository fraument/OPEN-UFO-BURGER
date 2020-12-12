#include "Obj.h"

Obj::Obj(int idx) {
	int randcol = rand() % 5;
	modelTransform = glm::mat4(1.0f);

	if (idx == 0) {
		modelTransform[3][1] = 0.0f;
		modelColor = glm::vec3(0.9, 0.7, 0.5);
	}
	if (idx > 0) {
		modelTransform[3][1] = 1.0f*idx;
	}
	if (randcol == 0) {//빵!
		modelColor = glm::vec3(0.9,0.7,0.5 );
	}
	if (randcol == 1) {//치즈
		modelColor = glm::vec3(0.95, 0.8, 0.1);
	}
	if (randcol == 2) {//토마토
		modelColor = glm::vec3(0.9, 0.0, 0.0);
	}
	if (randcol == 3) {//야채
		modelColor = glm::vec3(0.0, 0.7, 0.2);
	}
	if (randcol == 4) {//패티
		modelColor = glm::vec3(0.6, 0.3, 0.1);
	}
	cout << modelTransform[3][1] << endl;
}
Obj::~Obj()
{

}
void Obj::PrintMatrix() {
	cout << modelTransform[3][1] << endl;
}
