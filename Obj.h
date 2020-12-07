#pragma once
#include "Headers.h"
class Obj
{
public:

	Obj();
	~Obj();
	glm::mat4 GetTrans_Matrix();
	void Translate();
	void SetMatrix(glm::mat4 Temp,int idx);

public:
	glm::mat4 modelTransform;
	glm::mat4 modelTranslate;
	glm::mat4 modelRotate = glm::mat4(1.0f);
	float fSpeed;
};

