////메인게임. 여기서 다 돌리고 클래스 멤버함수 변수만 가져옴
////얘도 클래스화하고싶엇는데 안되더라..리무 깃 참고했는데 얘네도 메인부분은 클래스화 안했길래 일단냅둠
//
//#include <iostream>
//using namespace std;
//
//#include "gl/glew.h" // 필요한 헤더파일 include
//#include "gl/freeglut.h"
//#include "gl/freeglut_ext.h"
//#include "filetobuf.h"
//#include <glm/glm.hpp>
//#include <glm/ext.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <ctime>
//#include <random>
//#include <math.h>
//#include "Camera.h"
//#include "Obj.h"
//
//GLvoid drawScene(GLvoid);
//GLvoid Reshape(int w, int h);
//GLchar* VertexSource, * FragmentSource; // 소스코드 저장 변수
//GLuint VertexShader, FragmentShader; // 세이더 객체
//GLuint s_program;
//GLuint VAO[4], VBO[4];
////vao와 vbo 배열 사용, 각 오브젝트마다 vao 하나와 vbo 두개 사용
////VAO[0]-->플레인 (vbo[0]: 플레인 좌표, vbo[1]:플레인 색깔)
//GLvoid Keyboard(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
//void SpecialKeyboard(int key, int x, int y);
//void Timerfunction(int value);
//void InitializeOBJs();
//
////한 객체를 이동하기 위해서는 다음 세 종류의 행렬이 필요하다.
////플레인의 경우 변환이 필요없으나 기본형으로 써둠
//
//
////클래스 포인터 하나 부르고 널ptr로 초기화
//CCam *gamecam = nullptr;
//
//float floorPos[] = {
//	//바닥 플레인부분
//	   0.8, -0.5, 1.0, -0.8, -0.5, 1.0,
//   -0.8, -0.5, -1.0, 0.8, -0.5, -1.0
//
//};
//
////바닥컬러
//float floorCol[] = {
//	0.0,0.0,1.0, 1.0,1.0,1.0,
//	1.0,0.0,0.0, 0.0,1.0,0.0
//};
//
//void make_vertexShader()
//{
//	VertexSource = filetobuf((char*)"vertex.txt");
//	//--- 버텍스 세이더 객체 만들기
//	VertexShader = glCreateShader(GL_VERTEX_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(VertexShader, 1, (const GLchar**)&VertexSource, 0);
//	//--- 버텍스 세이더 컴파일하기
//	glCompileShader(VertexShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);
//
//	if (!result) {
//		glGetShaderInfoLog(VertexShader, 512, NULL, errorLog);
//		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
//		//return false;
//	}
//}
//
//void make_fragmentShader()
//{
//	FragmentSource = filetobuf((char*)"fragment.txt");
//	//--- 프래그먼트 세이더 객체 만들기
//	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	//--- 세이더 코드를 세이더 객체에 넣기
//	glShaderSource(FragmentShader, 1, (const GLchar**)&FragmentSource, 0);
//	//--- 프래그먼트 세이더 컴파일
//	glCompileShader(FragmentShader);
//	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(FragmentShader, 512, NULL, errorLog);
//		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
//		//return false;
//	}
//}
//
//void InitBuffer() {
//	glGenVertexArrays(1, &VAO[0]);
//	glBindVertexArray(VAO[0]);
//
//	glGenBuffers(1, &VBO[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(floorPos), floorPos, GL_STATIC_DRAW);
//
//	GLint pAttribute1 = glGetAttribLocation(s_program, "in_Position");
//	glVertexAttribPointer(pAttribute1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(pAttribute1);
//
//	glGenBuffers(1, &VBO[1]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(floorCol), floorCol, GL_STATIC_DRAW);
//
//	GLint cAttribute2 = glGetAttribLocation(s_program, "in_Color");
//	glVertexAttribPointer(cAttribute2, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glEnableVertexAttribArray(cAttribute2);
//
//}
//
//void InitShader()
//{
//	make_vertexShader();
//	make_fragmentShader();
//	//-- shader Program
//	s_program = glCreateProgram();
//	glAttachShader(s_program, VertexShader);
//	glAttachShader(s_program, FragmentShader);
//	glLinkProgram(s_program);
//	//checkCompileErrors(s_program, "PROGRAM");
//	//--- 세이더 삭제하기
//	glDeleteShader(VertexShader);
//	glDeleteShader(FragmentShader);
//	//--- Shader Program 사용하기
//	glUseProgram(s_program);
//
//}
//
//void main(int argc, char** argv) {
//	srand(unsigned(time(NULL)));
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("OPEN!UFO BURGER");
//	glewExperimental = GL_TRUE;
//	glewInit();
//	InitShader();
//	InitBuffer();
//	InitializeOBJs();
//
//	if (glewInit() != GLEW_OK) // glew 초기화
//	{
//		std::cerr << "Unable to initialize GLEW" << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	else
//		std::cout << "GLEW Initialized\n";
//
//	glutTimerFunc(300, Timerfunction, 1);
//	glutMouseFunc(Mouse);
//	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
//	glutSpecialFunc(SpecialKeyboard);
//	glutDisplayFunc(drawScene); // 출력 함수의 지정
//	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
//	glutMainLoop(); // 이벤트 처리 시작 
//
//}
//void InitializeOBJs() {
//	//nullptr일때 새로 생성(안하면터짐 새클래스쓸때마다 무조건!)
//	if (gamecam == nullptr)
//		gamecam = new CCam;
//}
//GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//카메라 클래스에서 카메라값매번 받아와서그림
//	glUseProgram(s_program);
//	glm::mat4 view = glm::mat4(1.0f);
//	view = glm::lookAt(gamecam->GetCamPos(), gamecam->GetCamDir(), gamecam->GetCamUp());
//	unsigned int viewLocation = glGetUniformLocation(s_program, "viewtransform"); //--- 버텍스 세이더에서 viewTransform 변수위치
//	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
//
//	//임시로 그린 플레인 그리기. 나중에 한번만그리도록수정예정
//	glBindVertexArray(VAO[0]);
//	glDrawArrays(GL_QUADS, 0, 12);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	glutSwapBuffers(); // 화면에 출력하기
//}
//
//GLvoid Keyboard(unsigned char key, int x, int y)
//{
//	switch (key) {
//
//		//카메라수정하는거,, 굳ㅇ이신경쓸필요 ㄴ
//	case 'w':
//		gamecam->SetCamPOS(1);
//		break;
//
//	case 's':
//		gamecam->SetCamPOS(2);
//		break;
//
//	case 'a':
//		gamecam->SetCamPOS(3);
//		break;
//
//	case 'd':
//		gamecam->SetCamPOS(4);
//		break;
//
//	case 'q':
//		gamecam->SetCamPOS(5);
//		break;
//
//	case 'e':
//		gamecam->SetCamPOS(6);
//		break;
//
//	case 'Q':
//		delete gamecam;
//		gamecam = nullptr;
//		exit(0);
//		break;
//
//	}
//
//	glutPostRedisplay(); //--- 배경색이 바뀔때마다 출력 콜백함수를 호출하여 화면을 refresh 한다
//}
//
//void Timerfunction(int value) {
//
//
//	glutPostRedisplay(); // 화면 재 출력
//	glutTimerFunc(100, Timerfunction, 1); // 타이머함수 재 설정
//
//}
//
//void Mouse(int button, int state, int x, int y)
//{
//	glutPostRedisplay(); // 화면 재 출력
//}
//
//void SpecialKeyboard(int key, int x, int y)
//{
//	
//}
//
//GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
//{
//	glViewport(0, 0, w, h);
//}
//
//void ReadObj(FILE* objFile)
//{
//	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
//	char count[100];
//	int vertexNum = 0;
//	int faceNum = 0;
//	while (!feof(objFile)) {
//		fscanf(objFile, "%s", count);
//		if (count[0] == 'v' && count[1] == '\0')
//			vertexNum += 1;
//		else if (count[0] == 'f' && count[1] == '\0')
//			faceNum += 1;
//		memset(count, '\0', sizeof(count)); // 배열 초기화
//	}
//	//--- 2. 메모리 할당
//	vec4 * vertex;
//	vec4* face;
//	int vertIndex = 0;
//	int faceIndex = 0;
//	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
//	face = (vec4*)malloc(sizeof(vec4) * faceNum);
//
//	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
//	while (!feof(objFile)) {
//		fscanf(objFile, "%s", bind);
//		if (bind[0] == 'v' && bind[1] == '\0') {
//			fscanf(objFile, "%f %f %f",
//				&vertex[vertIndex].x, &vertex[vertIndex].y,
//				&vertex[vertIndex].z);
//			vertIndex++;
//		}
//		else if (bind[0] == 'f' && bind[1] == '\0') {
//			fscanf(objFile, "%f %f %f",
//				&face[faceIndex].x, &face[faceIndex].y, &face[faceIndex].z);
//			faceIndex++;
//		}
//	}
//}