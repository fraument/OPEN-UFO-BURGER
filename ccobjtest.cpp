//#pragma comment (lib, "glew32.lib")
//#pragma comment (lib, "freeglut.lib")

#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ReadObj.h"

using namespace std;

#define WINDOW_WITDH	800
#define WINDOW_HEIGHT	600

#define CAMERA_POS		glm::vec3(0.0, 0.0, 50.0)
#define OBJECT_COLOR	glm::vec3(1.0, 1.0, 1.0)
#define LIGHT_AMBIENT	glm::vec3(0.1, 0.1, 0.1)
#define LIGHT_POS		glm::vec3(100.0, 100.0, 0.0)
#define LIGHT_COLOR		glm::vec3(1.0, 1.0, 1.0)

#define FILE_NAME		"tomato.obj"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

GLuint fragmentShader;
GLuint vertexShader;
GLuint s_program;

GLuint vao[2];
GLuint vbo[6];
GLuint ebo[2];

float* vPosData[2];
float* vNormalData[2];
float* vTextureCoordinateData[2];
int* indexData[2];
int vertexCount[2];
int indexCount;

int i = 0;

float degree_cameraRotate = 0.0f;
float degree_lightPos = 0.0f;
bool draw_fill = true;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WITDH, WINDOW_HEIGHT);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	ReadObj("tomato.obj", vPosData[0], vNormalData[0], vTextureCoordinateData[0], indexData[0], vertexCount[0], indexCount);
	ReadObj("untitled.obj", vPosData[1], vNormalData[1], vTextureCoordinateData[1], indexData[1], vertexCount[1], indexCount);
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (draw_fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(s_program);

	glm::mat4 modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(degree_cameraRotate), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(CAMERA_POS, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);

	GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
	glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));

	GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

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

	GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
	glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

	if (i % 2 == 0) {
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else if (i % 2 == 1) {
		glBindVertexArray(vao[1]);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		degree_cameraRotate -= 20.f;
		break;
	case 'd':
		degree_cameraRotate += 20.f;
		break;
	case 'r':
		draw_fill = !draw_fill;
		break;
		
	case 'i':
		i++;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fopen_s(&fptr, file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}


void make_vertexShaders()
{
	char* vertexsource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader error\n" << errorLog << endl;
	}
}

void make_fragmentShaders()
{
	char* fragmentsource = filetobuf("fragment.glsl");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader error\n" << errorLog << endl;
	}
}

void InitBuffer()
{
	//테스트큐브 버퍼
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[0] * sizeof(float) * 3, vPosData[0], GL_STATIC_DRAW);

	int posLocation1= glGetAttribLocation(s_program, "in_position");
	glVertexAttribPointer(posLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(posLocation1);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[0] * sizeof(float) * 3, vNormalData[0], GL_STATIC_DRAW);
	int normalLocation1 = glGetAttribLocation(s_program, "in_normal");
	glVertexAttribPointer(normalLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(normalLocation1);

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[0] * sizeof(float) * 2, vTextureCoordinateData[0], GL_STATIC_DRAW);
	int uvLocation1 = glGetAttribLocation(s_program, "in_uv");
	glVertexAttribPointer(uvLocation1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(uvLocation1);

	glGenBuffers(1, &ebo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData[0], GL_STATIC_DRAW);

	//토마토 모델링 버퍼
	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[1] * sizeof(float) * 3, vPosData[1], GL_STATIC_DRAW);
	int posLocation2 = glGetAttribLocation(s_program, "in_position");
	glVertexAttribPointer(posLocation2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(posLocation2);

	glGenBuffers(1, &vbo[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[1] * sizeof(float) * 3, vNormalData[1], GL_STATIC_DRAW);
	int normalLocation2 = glGetAttribLocation(s_program, "in_normal");
	glVertexAttribPointer(normalLocation2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(normalLocation2);

	glGenBuffers(1, &vbo[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount[1] * sizeof(float) * 2, vTextureCoordinateData[1], GL_STATIC_DRAW);
	int uvLocation2 = glGetAttribLocation(s_program, "in_uv");
	glVertexAttribPointer(uvLocation2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
	glEnableVertexAttribArray(uvLocation2);

	glGenBuffers(1, &ebo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData[1], GL_STATIC_DRAW);
}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(s_program);
}


