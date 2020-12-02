#include "Obj.h"
CObj::CObj() {
	modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}
CObj::~CObj() {

}
glm::mat4 CObj :: GetModelTransform() {
	//UpdateMatrix();
	modelTransform = modelTranslate * modelRotate;
	return modelTransform;
}
void CObj::MoveBread() {
	modelTranslate = glm::translate(modelTranslate, glm::vec3(0.0, 0.1, 0.0));
}
void CObj::DrawObject() {

}
