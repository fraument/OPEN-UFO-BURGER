#pragma once
#include "Headers.h"
class Camera
{
public:
	explicit Camera();
	virtual ~Camera();

public:
	//���ο��� ī�޶� ��ǥ �ҷ����� �ܼ��� ���ϸ� �ϴ� �Լ� �Ű� ����
	glm::vec3 GetCamPos() { return cameraPos; };
	void SetCamPOS();

public:
	glm::vec3 cameraPos = glm::vec3(8.0f, 8.0f, 1.0f);
	float camDegree = 45.0f;
};

