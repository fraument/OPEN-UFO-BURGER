//ī�޶� Ŭ����(�⺻����, �׾����� ������..)
#pragma once
#include "Headers.h"
class CCam 
{
public:
	explicit CCam();
	virtual ~CCam();

public:
	//���ο��� ī�޶� ��ǥ �ҷ����� �ܼ��� ���ϸ� �ϴ� �Լ� �Ű� ����
	glm::vec3 GetCamPos() { return cameraPos; }; 
	void SetCamPOS(int n);

public:
	glm::vec3 cameraPos = glm::vec3(20.0f, 5.0f, 1.0f);
	float camDegree = 45.0f;
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ��
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //--- ī�޶� ���� ����: z�� ���� ���� 
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//eoqnqns up
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
};