#include "Obj.h"

Obj::Obj(int idx) {
	int randcol = rand() % 5;
	modelTransform = glm::mat4(1.0f);

	if (idx == 0) {
		modelTransform[3][1] = 0.0f;
		modelColor = glm::vec3(0.9, 0.7, 0.5);
	}
	if (idx > 0&&idx%2==1) {
		modelTransform[3][1] = 1.0f*idx;
		modelTransform[3][0] = -3.0f;
	}

	if (idx > 0 && idx % 2 == 0) {
		modelTransform[3][1] = 1.0f * idx;
		modelTransform[3][0] = 3.0f;
	}

	if (randcol == 0) {//��!
		modelColor = glm::vec3(0.9,0.7,0.5 );
	}
	if (randcol == 1) {//ġ��
		modelColor = glm::vec3(0.95, 0.8, 0.1);
	}
	if (randcol == 2) {//�丶��
		modelColor = glm::vec3(0.9, 0.0, 0.0);
	}
	if (randcol == 3) {//��ä
		modelColor = glm::vec3(0.0, 0.7, 0.2);
	}
	if (randcol == 4) {//��Ƽ
		modelColor = glm::vec3(0.6, 0.3, 0.1);
	}
}
Obj::~Obj()
{

}
void Obj::PrintMatrix() {
	cout << modelTransform[3][0] << endl;
}

float Obj::ReturnPos(int idx) {
	//x��
	if (idx == 0) {
		return modelTransform[3][0];
	}
	//y��
	if (idx == 1) {
		return modelTransform[3][1];
	}
	if (idx == 2) {
		return modelTransform[3][2];
	}
}
