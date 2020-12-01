////카메라 클래스(기본세팅, 죽었을때 떨어짐..)
//#pragma once
//#include "Headers.h"
//class CCam 
//{
//public:
//	explicit CCam();
//	virtual ~CCam();
//
//public:
//	//메인에서 카메라 좌표 불렀을때 단순히 리턴만 하는 함수 신경 ㄴㄴ
//	glm::vec3 GetCamPos() { return cameraPos; }; 
//	glm::vec3 GetCamDir() { return cameraDirection; };
//	glm::vec3 GetCamUp() { return cameraUp; };
//	void UpdateCamInfo();//카메라 위치 조정한거 재계산하는함su
//	void SetCamPOS(int n);//
//	public:
//		glm::vec3 cameraPos = glm::vec3(0.1f, 0.15f, 0.99f);
//		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라가 바라보는 곳
//		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //--- 카메라 방향 벡터: z축 양의 방향 
//		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//eoqnqns up
//		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
//};