///gl 3d �ǽ��� �� �ڵ�
//�򰥸��� �����ص� ��.

#include <iostream>
using namespace std;

#include "gl/glew.h" // �ʿ��� ������� include
#include "gl/freeglut.h"
#include "gl/freeglut_ext.h"
#include "filetobuf.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <random>
#include <math.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* VertexSource, * FragmentSource; // �ҽ��ڵ� ���� ����
GLuint VertexShader, FragmentShader; // ���̴� ��ü
GLuint s_program;
GLuint VAO_s, VAO_p,VAO_g, VBOcV, VBOcC , VBOcV2, VBOcC2,VBO_gV,VB0_gC, EBO1, EBO2 ;
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void DrawSquare();
void DrawPyra();
void DrawLines();
void Reset();
glm::mat4 DetransformMatrixSq(1.0f);
glm::mat4 DetransformMatrixTp(1.0f);
glm::mat4 transformMatrixSq(1.0f);
glm::mat4 transformMatrixTp(1.0f);
glm::mat4 transformMatrixGl(1.0f);
glm::mat4 translateMatrixSq(1.0f);
glm::mat4 translateMatrixTp(1.0f);
glm::mat4 rotateMatrixSq(1.0f);
glm::mat4 rotateMatrixTp(1.0f);

struct UIs {
	bool bTurnX = false;
	bool bTurnx = false;
	bool bTurnY = false;
	bool bTurny = false;
	int iSq = 0;
	int iPy = 1;
	int iCull = 1;
	int iLine = 0;
	bool bStartS = false;
	bool bStartP = false;
	bool bTimer = true;
};

UIs ui;
float Guideline[] = {
	-1.0f, 0.0f,0.0f, 1.0f,0.0f,0.0f,
	0.0f,-1.0f,0.0f,0.0,1.0,0.0
};

float SqPoses[] = {
	//����ü
	0.3,0.3,0.3, 0.3,0.3,-0.3,
	-0.3,0.3,-0.3,-0.3,0.3,0.3,
	0.3,-0.3,0.3,0.3,-0.3,-0.3,
	-0.3,-0.3,-0.3,-0.3,-0.3,0.3
};

float PyPoses[] = {
	0,0.3,0, 0.3,-0.3,0.3,
	0.3,-0.3,-0.3, -0.3,-0.3,-0.3,
	-0.3,-0.3,0.3
};

unsigned int SqIdx[] = {
0,1,2,
3,0,2,
3,2,7,
2,6,7,
6,2,1,
6,1,5,
5,1,0,
5,0,4,
4,0,7,
7,0,3,
5,4,7,
5,7,6
};

unsigned int PyIdx[] = {
	0,1,2,
	0,2,3,
	0,3,4,
	0,4,1,
	1,4,3,
	1,3,2
};

float ShapeCol[] = {

	0.35f,0.35f,0.35f, 0.1f,0.3f,0.55f , 0.3f,0.4f,0.1f ,
	0.56f,0.15f,0.22f, 0.5f,0.1f,0.1f , 0.12f,0.16f,0.44f ,
	0.15f,0.88f,0.44f, 0.1f,0.18f,0.1f , 0.0f,0.6f,0.14f ,
	0.05f,0.39f,0.15f, 0.4f,0.15f,0.0f , 0.15f,0.4f,0.1f ,
	0.05f,0.39f,0.15f, 0.4f,0.65f,0.22f , 0.10f,0.66f,0.2f ,
	0.15f,0.39f,0.25f, 0.1f,0.15f,0.15f , 0.4f,0.21f,0.1f ,
	0.05f,0.39f,0.15f, 0.4f,0.15f,0.0f , 0.33f,0.1f,0.19f,
	0.35f,0.05f,0.45f, 0.4f,0.25f,0.10f , 0.15f,0.30f,0.5f ,

};

float ShapeCol2[] = {

	0.05f,0.35f,0.35f, 0.1f,0.3f,0.55f , 0.33f,0.1f,0.1f ,
	0.4f,0.2f,0.5f, 0.5f,0.2f,0.1f

};

void make_vertexShader()
{
	VertexSource = filetobuf((char*)"vertex12.txt");
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
	FragmentSource = filetobuf((char*)"fragment12.txt");
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

void InitBuffer(){
	glGenVertexArrays(1, &VAO_g);
	glBindVertexArray(VAO_g);

	//--- vertex data ������ ���� VBO ���� �� ���ε�.
	glGenBuffers(1, &VBO_gV);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_gV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Guideline), Guideline, GL_STATIC_DRAW);

	GLint pAttribute = glGetAttribLocation(s_program, "in_Position");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pAttribute);


	glGenBuffers(1, &VB0_gC);
	glBindBuffer(GL_ARRAY_BUFFER, VB0_gC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeCol), ShapeCol, GL_STATIC_DRAW);

	GLint cAttribute = glGetAttribLocation(s_program, "in_Color");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(cAttribute);


	//����ü////
	glGenVertexArrays(1, &VAO_s);
	glBindVertexArray(VAO_s);

	glGenBuffers(1, &VBOcV);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SqPoses), SqPoses, GL_STATIC_DRAW);

	GLint pAttribute1 = glGetAttribLocation(s_program, "in_Position");
	glVertexAttribPointer(pAttribute1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pAttribute1);

	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SqIdx), SqIdx, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBOcC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeCol), ShapeCol, GL_STATIC_DRAW);

	GLint cAttribute1 = glGetAttribLocation(s_program, "in_Color");
	glVertexAttribPointer(cAttribute1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(cAttribute1);


	// �ﰢ��//
	glGenVertexArrays(1, &VAO_p);
	glBindVertexArray(VAO_p);
	glGenBuffers(1, &VBOcV2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcV2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PyPoses), PyPoses, GL_STATIC_DRAW);

	GLint pAttribute2 = glGetAttribLocation(s_program, "in_Position");
	glVertexAttribPointer(pAttribute2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pAttribute2);

	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2); // GL_ELEMENT_ARRAY_BUFFER ���� �������� ���ε�
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PyIdx), PyIdx, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBOcC2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcC2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeCol2), ShapeCol2, GL_STATIC_DRAW);

	GLint cAttribute2 = glGetAttribLocation(s_program, "in_Color");
	glVertexAttribPointer(cAttribute2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(cAttribute2);
	
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
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("12");
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();

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

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	// ����� VAO �ҷ�����
	DrawLines();

	if (ui.iSq == 0) {
		DrawSquare();
	}
	if (ui.iPy==0) {
		DrawPyra();
	}

	if (ui.iCull % 2 == 0) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	if (ui.iCull % 2 == 1) {
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

	if (ui.iLine % 2 == 0) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}

	if (ui.iLine % 2 == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 'Q':
		exit(0);
		break;

	case 'c':
		ui.iSq=0;
		ui.iPy = 1;
		break;

	case 'p':
		ui.iSq = 1;
		ui.iPy = 0;
		break;

	case 'x':
		ui.bTimer = true;
		ui.bTurnx = false;
		ui.bTurnY = false;
		ui.bTurny = false;
		ui.bTurnX = true;
		break;

	case 'X':
		ui.bTimer = true;
		ui.bTurnX = false;
		ui.bTurny = false;
		ui.bTurnY = false;
		ui.bTurnx = true;
		break;

	case 'y':
		ui.bTimer = true;
		ui.bTurnx = false;
		ui.bTurnX = false;
		ui.bTurny = false;
		ui.bTurnY = true;
		break;

	case 'Y':
		ui.bTimer = true;
		ui.bTurnx = false;
		ui.bTurnY = false;
		ui.bTurnX = false;
		ui.bTurny = true;
		break;

	case 'h':
		ui.iCull++;
		break;

	case 'w':
		ui.iLine++;
		break;

	case 's':
		ui.bTimer = false;
		Reset();
		break;

	}
	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void Timerfunction(int value) {

	if (ui.bTimer == true) {
		if (ui.iSq == 0) {

			if (ui.bTurnX == true) {
				rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			}
			if (ui.bTurnx == true) {
				rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
			}
			if (ui.bTurnY == true) {
				rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
			}
			if (ui.bTurny == true) {
				rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
			}

			transformMatrixSq = translateMatrixSq * rotateMatrixSq;
			unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixSq));
		}

		if (ui.iPy == 0) {
			if (ui.bTurnX == true) {
				rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			}
			if (ui.bTurnx == true) {
				rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
			}
			if (ui.bTurnY == true) {
				rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
			}
			if (ui.bTurny == true) {
				rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
			}

			transformMatrixTp = translateMatrixTp * rotateMatrixTp;
			unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixTp));
		}
	}
	
	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(100, Timerfunction, 1); // Ÿ�̸��Լ� �� ����

}

void Mouse(int button, int state, int x, int y)
{
	glutPostRedisplay(); // ȭ�� �� ���
}

void DrawLines() {
	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixGl));
	glBindVertexArray(VAO_g);
	glDrawArrays(GL_LINES, 0, 4);
}

void DrawSquare() {
	if (ui.bStartS == false) {
		rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(30.0f), glm::vec3(1.0, -1.0, 0.0));
		ui.bStartS = true;
	}
	DetransformMatrixSq = translateMatrixSq;
	transformMatrixSq = translateMatrixSq * rotateMatrixSq;
	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixSq));
	glBindVertexArray(VAO_s);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void DrawPyra() {
	if (ui.bStartP == false) {
		rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(30.0f), glm::vec3(1.0, -1.0, 0.0));
		ui.bStartP = true;
	}
	
	transformMatrixTp = translateMatrixTp * rotateMatrixTp;
	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixTp));
	glBindVertexArray(VAO_p);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}

void Reset() {
	rotateMatrixSq= glm::mat4(1.0f);
	rotateMatrixTp = glm::mat4(1.0f);
	rotateMatrixTp = glm::rotate(rotateMatrixTp, glm::radians(30.0f), glm::vec3(1.0, -1.0, 0.0));
	rotateMatrixSq = glm::rotate(rotateMatrixSq, glm::radians(30.0f), glm::vec3(1.0, -1.0, 0.0));

	translateMatrixTp= glm::mat4(1.0f);
	translateMatrixSq = glm::mat4(1.0f);
	transformMatrixTp = translateMatrixTp * rotateMatrixTp;
	transformMatrixSq = translateMatrixSq * rotateMatrixSq;

	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixTp));
}

void SpecialKeyboard(int key, int x, int y)
{
	if (ui.iSq % 2 == 0) {
		if (key == GLUT_KEY_UP) {
			translateMatrixSq = glm::translate(translateMatrixSq, glm::vec3(0.0, 0.1, 0.0));
		}
		if (key == GLUT_KEY_DOWN) {
			translateMatrixSq = glm::translate(translateMatrixSq, glm::vec3(0.0, -0.1, 0.0));
		}
		if (key == GLUT_KEY_LEFT) {
			translateMatrixSq = glm::translate(translateMatrixSq, glm::vec3(-0.1, 0.0, 0.0));
		}
		if (key == GLUT_KEY_RIGHT) {
			translateMatrixSq = glm::translate(translateMatrixSq, glm::vec3(0.1, 0.0, 0.0));
		}
		transformMatrixSq = translateMatrixSq * rotateMatrixSq;
	}

	if (ui.iSq % 2 == 1) {
		if (key == GLUT_KEY_UP) {
			translateMatrixTp = glm::translate(translateMatrixTp, glm::vec3(0.0, 0.1, 0.0));
		}
		if (key == GLUT_KEY_DOWN) {
			translateMatrixTp = glm::translate(translateMatrixTp, glm::vec3(0.0, -0.1, 0.0));
		}
		if (key == GLUT_KEY_LEFT) {
			translateMatrixTp = glm::translate(translateMatrixTp, glm::vec3(-0.1, 0.0, 0.0));
		}
		if (key == GLUT_KEY_RIGHT) {
			translateMatrixTp = glm::translate(translateMatrixTp, glm::vec3(0.1, 0.0, 0.0));
		}
		transformMatrixTp = translateMatrixTp * rotateMatrixTp;
	}

	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrixSq));
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}