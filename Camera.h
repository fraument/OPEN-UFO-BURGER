////ī�޶� Ŭ����(�⺻����, �׾����� ������..)
//#pragma once
//#include "Headers.h"
//class CCam 
//{
//public:
//	explicit CCam();
//	virtual ~CCam();
//
//public:
//	//���ο��� ī�޶� ��ǥ �ҷ����� �ܼ��� ���ϸ� �ϴ� �Լ� �Ű� ����
//	glm::vec3 GetCamPos() { return cameraPos; }; 
//	glm::vec3 GetCamDir() { return cameraDirection; };
//	glm::vec3 GetCamUp() { return cameraUp; };
//	void UpdateCamInfo();//ī�޶� ��ġ �����Ѱ� �����ϴ���su
//	void SetCamPOS(int n);//
//	public:
//		glm::vec3 cameraPos = glm::vec3(0.1f, 0.15f, 0.99f);
//		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ��
//		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //--- ī�޶� ���� ����: z�� ���� ���� 
//		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//eoqnqns up
//		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
//};