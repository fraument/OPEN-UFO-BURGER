#include "Camera.h"
//rotorrlemfdk!!!!!!!
Camera::Camera() {

}
Camera::~Camera() {

}

void Camera::SetCamPOS() {//��� ���̸� ī�޶� �ö󰡵���
	//�����ؾ� ���������� �ö�����
	cameraPos.y += 0.3f;
	cameraPos.x += 0.3f;
}

