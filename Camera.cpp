//#include "Camera.h"
//#include <iostream>
//CCam::CCam() {
//
//}
//CCam::~CCam() {
//
//}
//
//void CCam::SetCamPOS(int n) {
//	//카메라 고정위치찾을 함수들
//	if (n == 1) {
//		cameraPos.y -= 0.01f;
//	}
//		
//	if (n == 2) {
//		cameraPos.y += 0.01f;
//	}
//	if (n == 3) {
//		cameraPos.z -= 0.01f;
//	}
//
//	if (n == 4) {
//		cameraPos.z += 0.01f;
//	}
//	if (n == 5) {
//		cameraPos.x -= 0.01f;
//	}
//
//	if (n == 6) {
//		cameraPos.x += 0.01f;
//	}
//	UpdateCamInfo();
//	std::cout << "x좌표: "<<cameraPos.x <<" ";
//	std::cout << "y좌표: " << cameraPos.y <<" ";
//	std::cout << "z좌표: " << cameraPos.z << std::endl;
//
//	
//}
//void CCam::UpdateCamInfo() {
//	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //--- 카메라 방향 벡터: z축 양의 방향 
//	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
//}
//
