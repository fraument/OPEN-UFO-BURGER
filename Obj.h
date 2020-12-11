#pragma once
#include "Headers.h"
class Obj
{
public:
	Obj(int idx);
	~Obj();
	glm::mat4 GetTransform_Matrix() { return modelTransform; };
	void SetTrans_Matrix(glm::mat4 Temp) { modelTransform = Temp; };
	void PrintMatrix();//yÁÂÇ¥
	float ReturnPos() { return modelTransform[3][1]; }
private:
	glm::mat4 modelTransform;
	glm::vec3 modelColor;
};

