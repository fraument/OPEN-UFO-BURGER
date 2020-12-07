#include "Obj.h"
Obj::Obj() {
	modelTransform = glm::mat4(1.0f);
	modelRotate = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);
	fSpeed = (rand() % 10) / 10.0f;
}
Obj::~Obj()
{

}
void Obj::Translate() {
	//객체이동
	modelTranslate= glm::translate(glm::mat4(0.1f), glm::vec3(0.1f, 0, 0));
	modelTransform = modelTranslate * modelRotate;
}
//void Obj::Rotate() {
//	//객체회전
//	
//}
glm::mat4 Obj::GetTrans_Matrix() {
	//현재 매트릭스 정보 가져옴
	//modelRotate = glm::rotate(modelTranslate, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	//modelTransform = modelTranslate * modelRotate;
	return modelTransform;
	
}
void Obj::SetMatrix(glm::mat4 Temp,int idx) {
	if (idx == 2) {
		modelTranslate = Temp;
	}
	modelTransform = modelTranslate * modelRotate;
}

