//���ΰ���. ���⼭ �� ������ Ŭ���� ����Լ� ������ ������
//�굵 Ŭ����ȭ�ϰ�;��µ� �ȵǴ���..���� �� �����ߴµ� ��׵� ���κκ��� Ŭ����ȭ ���߱淡 �ϴܳ���

#include <iostream>
using namespace std;

#include "gl/glew.h" // �ʿ��� ������� include
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
GLchar* VertexSource, * FragmentSource; // �ҽ��ڵ� ���� ����
GLuint VertexShader, FragmentShader; // ���̴� ��ü
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
//vao�� vbo �迭 ���, �� ������Ʈ���� vao �ϳ��� vbo �ΰ� ���
//VAO[0]-->�÷��� (vbo[0]: �÷��� ��ǥ, vbo[1]:�÷��� ����)
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void InitializeOBJs();
void DrawObject();

//�� ��ü�� �̵��ϱ� ���ؼ��� ���� �� ������ ����� �ʿ��ϴ�.
//�÷����� ��� ��ȯ�� �ʿ������ �⺻������ ���

glm::mat4 modelTransform(1.0f);
//Ŭ���� ������ �ϳ� �θ��� ��ptr�� �ʱ�ȭ
//CCam* gamecam = nullptr;
Camera* maincam = nullptr;
Obj *gameobj[20];
void make_vertexShader()
{
	VertexSource = filetobuf((char*)"vertex.txt");
	//--- ���ؽ� ���̴� ��ü �����
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(VertexShader, 1, (const GLchar**)&VertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(VertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(VertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		//return false;
	}
}

void make_fragmentShader()
{
	FragmentSource = filetobuf((char*)"fragment.txt");
	//--- �����׸�Ʈ ���̴� ��ü �����
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(FragmentShader, 1, (const GLchar**)&FragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(FragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
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

	cout << "���� �Ϸ�!" << endl;
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
	//--- ���̴� �����ϱ�
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	//--- Shader Program ����ϱ�
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

	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutTimerFunc(300, Timerfunction, 1);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutSpecialFunc(SpecialKeyboard);
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ���� 

}
void InitializeOBJs() {
	//nullptr�϶� ���� ����(���ϸ����� ��Ŭ������������ ������!)
	if (maincam == nullptr)
		maincam = new Camera;
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ī�޶� Ŭ�������� ī�޶󰪸Ź� �޾ƿͼ��׸�
	glUseProgram(s_program);

	//ī�޶� �κ�

	glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(camDegree), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(camerapos, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800 / (float)600, 0.001f, 1000.f);
	
	//��&����
	GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

	
	//�Ź� obj �׸��°�!
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
	////����Ʈ ������ ���߿� ��

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

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

		//ī�޶�����ϴ°�,, �����̽Ű澵�ʿ� ��
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

	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
void DrawObject() {
	
	
}
void Timerfunction(int value) {
	
	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(300, Timerfunction, 1);

}

void Mouse(int button, int state, int x, int y)
{
	glutPostRedisplay(); // ȭ�� �� ���
}

void SpecialKeyboard(int key, int x, int y)
{

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

