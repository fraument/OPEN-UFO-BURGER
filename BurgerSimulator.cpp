#include <vector>
#include "Camera.h"
#include "ReadObj.h"
#include "Obj.h"
#include "filetobuf.h"
#include "RhyInfo.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define BGM "Summerlove"

struct GameSystem {
    int idx = 1;
    bool isDropped = false;
    bool isTurn = true;
    bool bDir = true;
    float ObjSpeed = 1.2f;
    float BgColR = 0.25;
    float BgColB = 0.75f;

    int RhyIdx = 1;
    //clock_t clock
};
GameSystem sys;

void textcolor(int foreground, int background)
{
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#define LIGHT_AMBIENT   glm::vec3(0.5, 0.5, 0.5)
#define LIGHT_POS      glm::vec3(15.0, 10.0, 0.0)
#define LIGHT_COLOR      glm::vec3(1.0, 1.0, 1.0)
#define NUMOFOBJ 100

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* VertexSource, * FragmentSource; // 소스코드 저장 변수
GLuint VertexShader, FragmentShader; // 세이더 객체
GLuint s_program;

GLuint vao[3];
GLuint vbo[NUMOFOBJ];
GLuint ebo[NUMOFOBJ];
glm::mat4 Objmvp[NUMOFOBJ];
glm::vec3 ObjCol[NUMOFOBJ];

vector<float*>vPosData;
vector<float*>vNormalData;
vector<float*>vTextureCoordinateData;
vector<int*> indexData;
vector<int>vertexCount;

int indexCount;
int badCount = 0;
int pressButton = 0;

GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void InitializeOBJs();
void DrawObject();
void CompareRhythm(char key, int sec);
void UpdatesysInfo();
void InfoList(int num);
Camera* maincam = nullptr;
Obj* gameobj[NUMOFOBJ];

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

    for (int i = 0; i < 100; i++) {
        glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 3, vPosData[i], GL_STATIC_DRAW);
        int posLocation1 = glGetAttribLocation(s_program, "in_position");
        glVertexAttribPointer(posLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
        glEnableVertexAttribArray(posLocation1);
    }

    glGenBuffers(1, &vbo[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    for (int i = 0; i < 100; i++) {
        glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 3, vNormalData[i], GL_STATIC_DRAW);
        int normalLocation1 = glGetAttribLocation(s_program, "in_normal");
        glVertexAttribPointer(normalLocation1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
        glEnableVertexAttribArray(normalLocation1);
    }

    glGenBuffers(1, &vbo[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    for (int i = 0; i < 100; i++) {
        glBufferData(GL_ARRAY_BUFFER, vertexCount[i] * sizeof(float) * 2, vTextureCoordinateData[i], GL_STATIC_DRAW);
        int uvLocation1 = glGetAttribLocation(s_program, "in_uv");
        glVertexAttribPointer(uvLocation1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
        glEnableVertexAttribArray(uvLocation1);
    }

    glGenBuffers(100, &ebo[0]);
    for (int i = 0; i < 100; i++) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData[i], GL_STATIC_DRAW);
    }
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
    vPosData.assign(100, NULL);
    vNormalData.assign(100, NULL);
    vTextureCoordinateData.assign(100, NULL);
    indexData.assign(100, NULL);
    vertexCount.assign(100, 0);

    for (int i = 0; i < 100; i++) {
        gameobj[i] = new Obj(i);
    }

    srand(unsigned(time(NULL)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 800);
    glutCreateWindow("OPEN!UFO BURGER");
    glewExperimental = GL_TRUE;
    glewInit();

    ReadObj("st.obj", vPosData[0], vNormalData[0], vTextureCoordinateData[0], indexData[0], vertexCount[0], indexCount);

    for (int i = 0; i < 100; i++) {
        vPosData[i] = vPosData[0];
        vNormalData[i] = vNormalData[0];
        vTextureCoordinateData[i] = vTextureCoordinateData[0];
        indexData[i] = indexData[0];
        vertexCount[i] = vertexCount[0];
    }
    SetVecData();
    InitShader();
    InitBuffer();
    InitializeOBJs();

    
    if (pressButton == 0) {
        for (int i = 0; i < 5; i++) {
            cout << RhythmInfo[i].key1 << " ";
        }
        cout << '\n';
    }
    

    if (glewInit() != GLEW_OK) // glew 초기화
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        //textcolor(RED, WHITE);
        std::cout << "GLEW Initialized\n";
    }
    PlaySound(TEXT(BGM), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);



    glutTimerFunc(50, Timerfunction, 1);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
    //glutKeyboardUpFunc(KeyboardUp);
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
    glClearColor(sys.BgColR, 0.0f, sys.BgColB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //카메라 클래스에서 카메라값매번 받아와서그림
    glUseProgram(s_program);
    glCullFace(GL_BACK);
    //카메라 부분

    glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(maincam->camDegree), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(maincam->cameraPos, 1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800 / (float)600, 0.001f, 1000.f);

    //뷰&투영
    GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
    glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

    //상자들을 그림
    DrawObject();

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

    /*if (pressButton == 10) {
        system("cls");
        pressButton = 0;
    }*/

    //GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
    //glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

    glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key) {

    case 'a':
        maincam->camDegree -= 10.0f;
        break;

    case 'd':
        pressButton++;
        InfoList(pressButton);
        CompareRhythm('d', clock());
        UpdatesysInfo();
        break;

    case 'f':
        pressButton++;
        InfoList(pressButton);
        CompareRhythm('f', clock());
        UpdatesysInfo();
        break;

    case 'j':
        pressButton++;
        InfoList(pressButton);
        CompareRhythm('j', clock());
        UpdatesysInfo();
        break;

    case 'k':
        pressButton++;
        InfoList(pressButton);
        CompareRhythm('k', clock());
        UpdatesysInfo();
        break;

    case 'q':
    case 'Q':
        delete maincam;
        maincam = nullptr;
        delete* gameobj;
        *gameobj = nullptr;
        PlaySound(NULL, 0, 0);
        exit(0);
        break;

    case 'v':
        if ((GetAsyncKeyState('a') & 0x8000)) {
            cout << "동시입력중!" << endl;
        }
        break;

    }
}

void UpdatesysInfo() {
    if (sys.idx % 2 == 1) {
        sys.bDir = true;//음-->약(++)
    }
    else if (sys.idx % 2 == 0) {
        sys.bDir = false;//양-->음(--)
    }
    sys.BgColR -= 0.003f;
    sys.BgColB -= 0.005f;
    sys.isDropped = true;
    sys.isTurn = false;
    maincam->cameraPos.x += 1.5f;
    maincam->cameraPos.y += 1.0f;
    sys.RhyIdx++;
}

void DrawObject() {
    //0부터 내려온애까지 오브젝트 그림
    for (int i = 0; i <= sys.idx; i++) {
        Objmvp[i] = gameobj[i]->GetTransform_Matrix();
        GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
        glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[i]));

        glm::vec3 objColor = gameobj[i]->GetModelCol();
        ObjCol[i] = gameobj[i]->GetModelCol();
        GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
        glUniform3fv(objColorLocation, 1, glm::value_ptr(ObjCol[i]));

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
}
void CompareRhythm(char key, int sec) {
    int fabsGapoftime = fabs(RhythmInfo[sys.RhyIdx - 1].milisec - sec);
    cout << "절댓값:" << fabsGapoftime << endl;

    if (RhythmInfo[sys.RhyIdx - 1].key1 == key) {
        if (fabsGapoftime <= 250) cout << "Great!" << endl;
    }

    if (fabsGapoftime > 250 && fabsGapoftime <= 350) cout << "Good" << endl;


    if (fabsGapoftime > 350 && fabsGapoftime <= 450) {
        cout << "Bad" << endl;

            if (RhythmInfo[sys.RhyIdx - 1].milisec > 26000) { // 연습시간 이후
                badCount++;
                cout << "Bad 횟수 : " << badCount << endl;
            }
            if (badCount == 3) exit(0);
        }


    if (RhythmInfo[sys.RhyIdx - 1].key1 != key) {
        if (RhythmInfo[sys.RhyIdx - 1].milisec > 26000) {
            badCount++;
            cout << "Bad 횟수 : " << badCount << endl;
            if (badCount == 3) exit(0);
        }
        
    }


}

void Timerfunction(int value) {
    if (sys.isDropped == true) {
        if (gameobj[sys.idx]->ReturnPos(1) <= 1.0f * (sys.idx - 1)) {
            sys.isDropped = false;
            sys.isTurn = true;//하강 끝! 다음 애가 좌우 움직임 시작
            sys.idx++;
        }

        if (gameobj[sys.idx]->ReturnPos(1) > 1.0f * (sys.idx - 1)) {
            glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(0.0f, -0.1f, 0.0f));
            gameobj[sys.idx]->SetTrans_Matrix(Temp, 0);
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }
    }

    if (sys.isTurn) { //이제 쌓을 애가 양옆으로 움직이는 코드
        if (sys.idx % 2 == 0) {//양수.음수쪽으로 더많이 가야함
            if (sys.bDir == true) {
                if (gameobj[sys.idx]->ReturnPos(0) >= 12.0f) {
                    sys.bDir = false;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp, 0);
            }

            if (sys.bDir == false) {
                if (gameobj[sys.idx]->ReturnPos(0) <= -12.0f) {
                    sys.bDir = true;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(-sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp, 0);

            }
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }

        if (sys.idx % 2 == 1) {//음수.양수쪽으로 더!
            if (sys.bDir == true) {
                if (gameobj[sys.idx]->ReturnPos(0) >= 10.0f) {
                    sys.bDir = false;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp, 0);

            }

            if (sys.bDir == false) {
                if (gameobj[sys.idx]->ReturnPos(0) <= -10.0f) {
                    sys.bDir = true;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(-sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp, 0);

            }
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }
    }


    if (sys.idx > 20) { //난이도 조절용
        maincam->camDegree -= 10.0f;
    }
    glutPostRedisplay(); // 화면 재 출력
    glutTimerFunc(50, Timerfunction, 1);

}
void InfoList(int num)
{
    /*for (int i = 0; i < RhythmInfo.size(); i++) {
        cout << RhythmInfo[i].key1 << " ";
    }
    cout << '\n'*/;


    if (num == 5) {
        for (int i = 5; i < 13; i++) {
            cout << RhythmInfo[i].key1 << " ";
        }
        cout << '\n';
    }

    if (num == 13) {
        for (int i = 13; i < 21; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 21) {
        for (int i = 21; i < 29; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 29) {
        for (int i = 29; i < 37; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 37) {
        for (int i = 37; i < 45; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 45) {
        for (int i = 45; i < 53; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 53) {
        for (int i = 53; i < 61; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 61) {
        for (int i = 61; i < 69; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 69) {
        for (int i = 69; i < 77; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }

    if (num == 77) {
        for (int i = 77; i < 80; i++) {
            cout << RhythmInfo[i].key1 << "  ";
        }
        cout << '\n';
    }
        

}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    }
    glutPostRedisplay(); // 화면 재 출력
}

void SpecialKeyboard(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_DOWN:
        cout << "들어옴";
        break;
    }
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}
