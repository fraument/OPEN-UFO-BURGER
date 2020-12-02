//우리가 사용할 모든 obj(햄버거 재료, ufo등) 는 여기에!
//부모가 될 것이다.
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
	//각 obj는 움직여야 함
	glm::mat4 modelTransform = glm::mat4(1.0f);
	glm::mat4 modelTranslate = glm::mat4(1.0f);
	glm::mat4 modelRotate = glm::mat4(1.0f);
};

