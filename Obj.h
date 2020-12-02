//�츮�� ����� ��� obj(�ܹ��� ���, ufo��) �� ���⿡!
//�θ� �� ���̴�.
#pragma once
#include "Headers.h"

class CObj
{
public:
	CObj();
	~CObj();
	glm::mat4 GetModelTransform();
	void MoveBread();
	void DrawObject();

protected:
	//�� obj�� �������� ��
	glm::mat4 modelTransform = glm::mat4(1.0f);
	glm::mat4 modelTranslate = glm::mat4(1.0f);
	glm::mat4 modelRotate = glm::mat4(1.0f);
};

