////#pragma comment (lib, "glew32.lib")
////#pragma comment (lib, "freeglut.lib")
//
////reupload!
//#include <GL/glew.h>
//#include <GL/freeglut.h> 
//#include <GL/freeglut_ext.h>
//#include <glm/glm.hpp>
//#include <glm/ext.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//
//#include "ReadObj.h"
//
//using namespace std;
//
//#define WINDOW_WITDH	800
//#define WINDOW_HEIGHT	600
//
//
//
//#define FILE_NAME		"tomato.obj"
//
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLvoid Keyboard(unsigned char key, int x, int y);
//
//void make_vertexShaders();
//void make_fragmentShaders();
//void InitBuffer();
//void InitShader();
//
//GLuint fragmentShader;
//GLuint vertexShader;
//GLuint s_program;
//
//
//
//int i = 0;
//
//
//bool draw_fill = true;
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(WINDOW_WITDH, WINDOW_HEIGHT);
//	glutCreateWindow("Example1");
//
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		std::cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		std::cout << "GLEW Initialized\n";
//
//	ReadObj("bread1.obj", vPosData[0], vNormalData[0], vTextureCoordinateData[0], indexData[0], vertexCount[0], indexCount);
//	ReadObj("bread2.obj", vPosData[1], vNormalData[1], vTextureCoordinateData[1], indexData[1], vertexCount[1], indexCount);
//	InitShader();
//	InitBuffer();
//
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutKeyboardFunc(Keyboard);
//	glutMainLoop();
//}
//
//GLvoid drawScene()
//{
//	glClearColor(0, 0, 0, 1);
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	if (draw_fill)
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	else
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	glFrontFace(GL_CCW);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//
//	glUseProgram(s_program);
//
//	glm::mat4 modelTransform = glm::mat4(1.0f);
//	modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//
//	glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(degree_cameraRotate), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(CAMERA_POS, 1.0f);
//	glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 proj = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);
//
//	GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
//	glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));
//
//	GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
//	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
//
//	GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
//	glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));
//
//	glm::vec3 lightAmbient = LIGHT_AMBIENT;
//	GLuint lightAmbientLocation = glGetUniformLocation(s_program, "g_lightAmbient");
//	glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
//
//	glm::vec3 lightPos = LIGHT_POS;
//	GLuint lightPosLocation = glGetUniformLocation(s_program, "g_lightPos");
//	glUniform3fv(lightPosLocation, 1, (float*)&lightPos);
//
//	glm::vec3 lightColor = LIGHT_COLOR;
//	GLuint lightColorLocation = glGetUniformLocation(s_program, "g_lightColor");
//	glUniform3fv(lightColorLocation, 1, (float*)&lightColor);
//
//	glm::vec3 objColor = OBJECT_COLOR;
//	GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
//	glUniform3fv(objColorLocation, 1, (float*)&objColor);
//
//	GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
//	glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);
//
//	
//
//	glutSwapBuffers();
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'a':
//		degree_cameraRotate -= 20.f;
//		break;
//	case 'd':
//		degree_cameraRotate += 20.f;
//		break;
//	case 'r':
//		draw_fill = !draw_fill;
//		break;
//		
//	case 'i':
//		i++;
//		break;
//
//	default:
//		break;
//	}
//
//	glutPostRedisplay();
//}
//
//char* filetobuf(const char* file)
//{
//	FILE* fptr;
//	long length;
//	char* buf;
//	fopen_s(&fptr, file, "rb"); // Open file for reading
//	if (!fptr) // Return NULL on failure
//		return NULL;
//	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
//	length = ftell(fptr); // Find out how many bytes into the file we are
//	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
//	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
//	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
//	fclose(fptr); // Close the file
//	buf[length] = 0; // Null terminator
//	return buf; // Return the buffer
//}
//
//
//void make_vertexShaders()
//{
//	char* vertexsource = filetobuf("vertex.glsl");
//
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
//	glCompileShader(vertexShader);
//
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		cerr << "ERROR: vertex shader error\n" << errorLog << endl;
//	}
//}
//
//void make_fragmentShaders()
//{
//	char* fragmentsource = filetobuf("fragment.glsl");
//
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
//	glCompileShader(fragmentShader);
//
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		cerr << "ERROR: fragment shader error\n" << errorLog << endl;
//	}
//}
//
//void InitBuffer()
//{
//	
//}
//
//void InitShader()
//{
//	make_vertexShaders();
//	make_fragmentShaders();
//
//	s_program = glCreateProgram();
//
//	glAttachShader(s_program, vertexShader);
//	glAttachShader(s_program, fragmentShader);
//	glLinkProgram(s_program);
//
//	//checkCompileErrors(s_program, "PROGRAM");
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	glUseProgram(s_program);
//}
//
//
