#include "Obj.h"

Obj::Obj(int idx) {
	//idx=0 , badak sangja
	if (idx == 0) {
		modelTransform[3][1] = 0.0f;
	}
	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, glm::vec3(0.0f, 0.8f, 0.0f));
}
Obj::~Obj()
{

}
void Obj::PrintMatrix() {
	double dArray[16] = { 0.0 };
	const float* pSource = (const float*)glm::value_ptr(modelTransform);
	for (int i = 0; i < 16; ++i) {
		dArray[i] = pSource[i];
		cout << dArray[i] << " ";
	}
		
}
