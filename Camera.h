#pragma once
#include "Headers.h"
//dlrjfdksqhsosdo
class Camera
{
public:
	explicit Camera();
	virtual ~Camera();

public:
	//메인에서 카메라 좌표 불렀을때 단순히 리턴만 하는 함수 신경 ㄴㄴ
	glm::vec3 GetCamPos() { return cameraPos; };
	void SetCamPOS();

public:
	glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 1.0f);
	float camDegree = -25.0f;
};

