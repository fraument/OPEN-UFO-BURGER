#include "Bread.h"
CBread::CBread() {
	cout << "���ο� �� ����!" << endl;
	modelTransform=glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);
	modelRotate = glm::mat4(1.0f);
}
CBread::~CBread() {
	cout << "�� ����!" << endl;
}
void CBread::Draw(GLuint programid) {
	GetModelTransform();
	GLuint modelTransformLocation = glGetUniformLocation(programid, "g_modelTransform");
	glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(GetModelTransform()));
}