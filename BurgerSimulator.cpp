//메인게임. 여기서 다 돌리고 클래스 멤버함수 변수만 가져옴
//얘도 클래스화하고싶엇는데 안되더라..리무 깃 참고했는데 얘네도 메인부분은 클래스화 안했길래 일단냅둠

#include <iostream>
using namespace std;

#include "gl/glew.h" // 필요한 헤더파일 include
#include "gl/freeglut.h"
#include "gl/freeglut_ext.h"
#include "filetobuf.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <vector>
#include "Camera.h"
#include "ReadObj.h"
#include "Obj.h"

#define OBJECT_COLOR	glm::vec3(0.1, 0.7, 1.0)
#define LIGHT_AMBIENT	glm::vec3(0.1, 0.1, 0.1)
#define LIGHT_POS		glm::vec3(50.0, 50.0, 0.0)
#define LIGHT_COLOR		glm::vec3(1.0, 1.0, 1.0)
#define OBJSPEED 0.05f
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* VertexSource, * FragmentSource; // 소스코드 저장 변수
GLuint VertexShader, FragmentShader; // 세이더 객체
GLuint s_program;

GLuint vao[3];
GLuint vbo[20];
GLuint ebo[20];

glm::mat4 Objmvp[20];

vector<float*>vPosData;
vector<float*>vNormalData;
vector<float*>vTextureCoordinateData;
vector<int*> indexData;
vector<int>vertexCount;

//float* vPosData[20];
//float* vNormalData[20];
//float* vTextureCoordinateData[20];
//int* indexData[20];
//int vertexCount[20];


int indexCount;
float degree_lightPos = 0.0f;

glm::vec3 camerapos = glm::vec3(20.0f, 5.0f, 1.0f);
float camDegree = 45.0f;

int i = 0;
//vao와 vbo 배열 사용, 각 오브젝트마다 vao 하나와 vbo 두개 사용
//VAO[0]-->플레인 (vbo[0]: 플레인 좌표, vbo[1]:플레인 색깔)
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void InitializeOBJs();
void DrawObject();

//한 객체를 이동하기 위해서는 다음 세 종류의 행렬이 필요하다.
//플레인의 경우 변환이 필요없으나 기본형으로 써둠

glm::mat4 modelTransform(1.0f);
//클래스 포인터 하나 부르고 널ptr로 초기화
//CCam* gamecam = nullptr;
Camera* maincam = nullptr;
Obj *gameobj[20];
void make_vertexShader()
{
	VertexSource = filetobuf((char*)"vertex.txt");
	//--- 버텍스 세이더 객체 만들기
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(VertexShader, 1, (const GLchar**)&VertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(VertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(VertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		//return false;
	}
}

void make_fragmentShader()
{
	FragmentSource = filetobuf((char*)"fragment.txt");
	//--- 프래그먼트 세이더 객체 만들기
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(FragmentShader, 1, (const GLchar**)&FragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(FragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		//return false;
	}
}

void InitBuffer() {
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	for (int i = 0; i < 20; i++) {
		glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 3, vPosData[i], GL_STATIC_DRAW);
		int posLocation1 = glGetAttribLocation(s_program, "in_position");
		glVertexAttribPointer(posLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(posLocation1);
	}

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	for (int i = 0; i < 20; i++) {
		glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 3, vNormalData[i], GL_STATIC_DRAW);
		int normalLocation1 = glGetAttribLocation(s_program, "in_normal");
		glVertexAttribPointer(normalLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
		glEnableVertexAttribArray(normalLocation1);
	}

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	for (int i = 0; i < 20; i++) {
		glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 2, vTextureCoordinateData[i], GL_STATIC_DRAW);
		int uvLocation1 = glGetAttribLocation(s_program, "in_uv");
		glVertexAttribPointer(uvLocation1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
		glEnableVertexAttribArray(uvLocation1);
	}
	
	glGenBuffers(20, &ebo[0]);
	for (int i = 0; i < 20; i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData[i], GL_STATIC_DRAW);
	}

	cout << "세팅 완료!" << endl;
}

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, VertexShader);
	glAttachShader(s_program, FragmentShader);
	glLinkProgram(s_program);
	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);

}

void main(int argc, char** argv) {
	//
	float* temp = NULL;
	int* itemp = NULL;
	vPosData.assign(20, temp);
	vNormalData.assign(20, temp);
	vTextureCoordinateData.assign(20, temp);
	indexData.assign(20, itemp);
	vertexCount.assign(20, 0);

	for (int i = 0; i < 20; i++) {
		gameobj[i] = new Obj();
	}
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OPEN!UFO BURGER");
	glewExperimental = GL_TRUE;
	glewInit();
	
	ReadObj("bread2.obj", vPosData[0], vNormalData[0], vTextureCoordinateData[0], indexData[0], vertexCount[0], indexCount);
	
	for (int i = 0; i < 20; i++) {
		vPosData[i] = vPosData[0];
		vNormalData[i] = vNormalData[0];
		vTextureCoordinateData[i] = vTextureCoordinateData[0];
		indexData[i] = indexData[0];
		vertexCount[i] = vertexCount[0];
	}
	InitShader();
	InitBuffer();
	InitializeOBJs();

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutTimerFunc(300, Timerfunction, 1);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
	glutSpecialFunc(SpecialKeyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMainLoop(); // 이벤트 처리 시작 

}
void InitializeOBJs() {
	//nullptr일때 새로 생성(안하면터짐 새클래스쓸때마다 무조건!)
	if (maincam == nullptr)
		maincam = new Camera;
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//카메라 클래스에서 카메라값매번 받아와서그림
	glUseProgram(s_program);

	//카메라 부분

	glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(camDegree), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camerapos, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800 / (float)600, 0.001f, 1000.f);
	
	//뷰&투영
	GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

	
	//매번 obj 그리는곳!
	for (int i = 0; i < 20; i++) {
		float randfx = (rand()%20)*OBJSPEED;
		float randfy = (rand() % 10) * OBJSPEED;
		glm::mat4 Temp = glm::translate(gameobj[i]->modelTranslate, glm::vec3(randfx, randfy, 0.0f));
		gameobj[i]->SetMatrix(Temp, 2);
		Objmvp[i] = gameobj[i]->modelTransform;
		GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
		glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[i]));
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	////라이트 관리는 나중엥 ㅠ

	glm::vec3 lightAmbient = LIGHT_AMBIENT;
	GLuint lightAmbientLocation = glGetUniformLocation(s_program, "g_lightAmbient");
	glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);

	glm::vec3 lightPos = LIGHT_POS;
	GLuint lightPosLocation = glGetUniformLocation(s_program, "g_lightPos");
	glUniform3fv(lightPosLocation, 1, (float*)&lightPos);

	glm::vec3 lightColor = LIGHT_COLOR;
	GLuint lightColorLocation = glGetUniformLocation(s_program, "g_lightColor");
	glUniform3fv(lightColorLocation, 1, (float*)&lightColor);

	glm::vec3 objColor = OBJECT_COLOR;
	GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
	glUniform3fv(objColorLocation, 1, (float*)&objColor);

	//GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
	//glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

		//카메라수정하는거,, 굳ㅇ이신경쓸필요 ㄴ
	case 'w':
		//i++;
		//gameobj[3]->modelTranslate
		break;

	case 's':
		degree_lightPos += 5.0f;
		//gamecam->SetCamPOS(2);
		break;

	case 'a':
		maincam->camDegree -= 10.0f;
		//gamecam->camDegree -= 10.0f;
		//gamecam->SetCamPOS(3);
		break;

	case 'd':
		maincam->camDegree += 10.0f;
		//gamecam->SetCamPOS(4);
		break;

	case 'q':
		//gamecam->SetCamPOS(5);
		break;

	case 'e':
		//gamecam->SetCamPOS(6);
		break;

	case 'b':
		break;

	case 'Q':
		delete maincam;
		maincam = nullptr;
		delete * gameobj;
		*gameobj = nullptr;
		exit(0);
		break;

	}

	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
}
void DrawObject() {
	
	
}
void Timerfunction(int value) {
	
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(300, Timerfunction, 1);

}

void Mouse(int button, int state, int x, int y)
{
	glutPostRedisplay(); // 화면 재 출력
}

void SpecialKeyboard(int key, int x, int y)
{

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

