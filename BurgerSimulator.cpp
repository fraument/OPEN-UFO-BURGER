#include <vector>
#include "Camera.h"
#include "ReadObj.h"
#include "Obj.h"
#include "filetobuf.h"
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
GLchar* VertexSource, * FragmentSource; // �ҽ��ڵ� ���� ����
GLuint VertexShader, FragmentShader; // ���̴� ��ü
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

GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void InitializeOBJs();
void DrawObject();

Camera* maincam = nullptr;
Obj* gameobj[NUMOFOBJ];

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
    //--- ���̴� �����ϱ�
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    //--- Shader Program ����ϱ�
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

    InitShader();
    InitBuffer();
    InitializeOBJs();

    if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
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
    glClearColor(sys.BgColR, 0.0f,sys.BgColB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ī�޶� Ŭ�������� ī�޶󰪸Ź� �޾ƿͼ��׸�
    glUseProgram(s_program);
    glCullFace(GL_BACK);
    //ī�޶� �κ�
    
    glm::vec4 cameraPos = glm::rotate(glm::mat4(1.0f), glm::radians(maincam->camDegree), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(maincam->cameraPos, 1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(cameraPos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 800 / (float)600, 0.001f, 1000.f);

    //��&����
    GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
    glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(proj));

    //���ڵ��� �׸�
    DrawObject();

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

    //GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
    //glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key) {

    case 'w':
        gameobj[sys.idx]->PrintMatrix();
        break;

    case 's':
        break;

    case 'a':
        maincam->camDegree -= 10.0f;
        break;

    case 'd':
        maincam->camDegree += 10.0f;
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

    case 'b':
    case 'B':
        cout<<clock()<<endl;
        if (sys.idx % 2 == 1) {
            sys.bDir = true;//��-->��(++)
        }
        else if (sys.idx % 2 == 0) {
            sys.bDir = false;//��-->��(--)
        }
        sys.BgColR -= 0.003f;
        sys.BgColB -= 0.005f;
        sys.isDropped = true;
        sys.isTurn = false;
        maincam->cameraPos.x += 1.5f;
        maincam->cameraPos.y += 1.0f;
        break;

    case 'v':
        if ((GetAsyncKeyState(VK_LEFT) & 0x8000)) {
            cout<<"�����Է���!"<<endl;
        }
        break;

    }
}
void DrawObject() {
    //0���� �����¾ֱ��� ������Ʈ �׸�
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

void Timerfunction(int value) {
    if (sys.isDropped == true) {
        if (gameobj[sys.idx]->ReturnPos(1) <= 1.0f * (sys.idx - 1)) {
            sys.isDropped = false;
            sys.isTurn = true;//�ϰ� ��! ���� �ְ� �¿� ������ ����
            sys.idx++;
        }

        if (gameobj[sys.idx]->ReturnPos(1) > 1.0f*(sys.idx-1)) {
            glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(0.0f, -0.1f, 0.0f));
            gameobj[sys.idx]->SetTrans_Matrix(Temp,0);
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }
    }

    if (sys.isTurn) { //���� ���� �ְ� �翷���� �����̴� �ڵ�
        if (sys.idx % 2 == 0) {//���.���������� ������ ������
            if (sys.bDir == true) {
                if (gameobj[sys.idx]->ReturnPos(0) >= 12.0f) {
                    sys.bDir = false;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp,0);
            }

            if (sys.bDir == false) {
                if (gameobj[sys.idx]->ReturnPos(0) <= -12.0f) {
                    sys.bDir = true;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(-sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp,0);

            }
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }

        if (sys.idx % 2 == 1) {//����.��������� ��!
            if (sys.bDir == true) {
                if (gameobj[sys.idx]->ReturnPos(0) >= 10.0f) {
                    sys.bDir = false;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp,0);

            }

            if (sys.bDir == false) {
                if (gameobj[sys.idx]->ReturnPos(0)<=-10.0f) {
                    sys.bDir = true;
                }
                glm::mat4 Temp = glm::translate(gameobj[sys.idx]->GetTransform_Matrix(), glm::vec3(-sys.ObjSpeed, 0.0f, 0.0f));
                gameobj[sys.idx]->SetTrans_Matrix(Temp,0);

            }
            Objmvp[sys.idx] = gameobj[sys.idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[sys.idx]));
        }
    }
        

    if (sys.idx > 20) { //���̵� ������
        maincam->camDegree -= 10.0f;
    }
    glutPostRedisplay(); // ȭ�� �� ���
    glutTimerFunc(50, Timerfunction, 1);

}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    }
   glutPostRedisplay(); // ȭ�� �� ���
}

void SpecialKeyboard(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_DOWN:
        cout << "����";
        break;
    }
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
    glViewport(0, 0, w, h);
}
