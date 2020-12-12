#pragma once
#include "Headers.h"
class Obj
{
public:
	Obj(int idx);
	~Obj();
	glm::mat4 GetTransform_Matrix() { return modelTransform; };
	glm::vec3 GetModelCol() { return modelColor; }
	void SetTrans_Matrix(glm::mat4 Temp) { modelTransform = Temp; };
	void PrintMatrix();//y��ǥ
	float ReturnPos(int idx);
private:
	glm::mat4 modelTransform;
	glm::vec3 modelColor=glm::vec3(0.0f,0.0f,0.0f);
};

