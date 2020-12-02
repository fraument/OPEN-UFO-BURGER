//카메라 클래스(기본세팅, 죽었을때 떨어짐..)
#pragma once
#include "Headers.h"
class CCam 
{
public:
	explicit CCam();
	virtual ~CCam();

public:
	//메인에서 카메라 좌표 불렀을때 단순히 리턴만 하는 함수 신경 ㄴㄴ
	glm::vec3 GetCamPos() { return cameraPos; }; 
	void SetCamPOS(int n);

public:
	glm::vec3 cameraPos = glm::vec3(20.0f, 5.0f, 1.0f);
	float camDegree = 45.0f;
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라가 바라보는 곳
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //--- 카메라 방향 벡터: z축 양의 방향 
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//eoqnqns up
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
};