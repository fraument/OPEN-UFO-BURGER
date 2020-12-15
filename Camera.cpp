#include "Camera.h"
//rotorrlemfdk!!!!!!!
Camera::Camera() {

}
Camera::~Camera() {

}

void Camera::SetCamPOS() {//재료 쌓이면 카메라 올라가도록
	//어케해야 같은각도로 올라가지ㅠ
	cameraPos.y += 0.3f;
	cameraPos.x += 0.3f;
}

