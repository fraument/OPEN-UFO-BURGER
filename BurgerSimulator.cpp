//���ΰ���. ���⼭ �� ������ Ŭ���� ����Լ� ������ ������
//�굵 Ŭ����ȭ�ϰ�;��µ� �ȵǴ���..���� �� �����ߴµ� ��׵� ���κκ��� Ŭ����ȭ ���߱淡 �ϴܳ���

#include <iostream>
using namespace std;

#include <vector>
#include "Camera.h"
#include "ReadObj.h"
#include "Obj.h"
#include "filetobuf.h"
#include <Windows.h>

#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15
float xp = 0.0f;
float yp = 0.0f;
int idx = 0;

//��ư ����!�ϰ�!!�� Ʈ������?
bool isDropped = false;
//�ϳ� �����߰� �����ְ� �Դٸ����ٸ��ϴ°�?Ʈ������.
bool isTurn = false;
int times = 0;

void textcolor(int foreground, int background)
{
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
float Uipos[] = {
1.0,-0.7f,1.0f, 1.0f,-1.0f,1.0f,
-1.0f,-1.0f,1.0f, -1.0f, -0.7f,1.0f
};
float Uicols[] = {
    0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
};

#define OBJECT_COLOR   glm::vec3(1.0, 0.7, 1.0)
#define LIGHT_AMBIENT   glm::vec3(0.1, 0.1, 0.1)
#define LIGHT_POS      glm::vec3(15.0, 8.0, 0.0)
#define LIGHT_COLOR      glm::vec3(1.0, 1.0, 1.0)
#define OBJSPEED 0.06f

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

int indexCount;
float degree_lightPos = 0.0f;

int i = 0;
//vao�� vbo �迭 ���, �� ������Ʈ���� vao �ϳ��� vbo �ΰ� ���
//VAO[0]-->�÷��� (vbo[0]: �÷��� ��ǥ, vbo[1]:�÷��� ����)
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timerfunction(int value);
void InitializeOBJs();
void DrawObject();
void CheckPos();


//�� ��ü�� �̵��ϱ� ���ؼ��� ���� �� ������ ����� �ʿ��ϴ�.
//�÷����� ��� ��ȯ�� �ʿ������ �⺻������ ���

glm::mat4 modelTransform(1.0f);
//Ŭ���� ������ �ϳ� �θ��� ��ptr�� �ʱ�ȭ
//CCam* gamecam = nullptr;
Camera* maincam = nullptr;
Obj* gameobj[20];
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
    system("mode con cols=40 lines=20 | title GUI");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHTBLUE);
 
    vPosData.assign(20, NULL);
    vNormalData.assign(20, NULL);
    vTextureCoordinateData.assign(20, NULL);
    indexData.assign(20, NULL);
    vertexCount.assign(20, 0);

    for (int i = 0; i < 20; i++) {
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
    else {
        textcolor(RED, WHITE);
        std::cout << "GLEW Initialized\n";
        cout << "1. ���󺯰� 2.������? 3.���� ��¼��" << endl;
    }
        
    /*for (int i = 0; i < 10; i++) {
        textcolor(RED, WHITE);
        cout << "                                                       " << endl;
    }*/
    
    glutTimerFunc(100, Timerfunction, 1);
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ī�޶� Ŭ�������� ī�޶󰪸Ź� �޾ƿͼ��׸�
    glUseProgram(s_program);

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

    glm::vec3 objColor = OBJECT_COLOR;
    GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation, 1, (float*)&objColor);

    //GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
    //glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
void CheckPos() {
    for (int i = 0; i <= idx; i++) {
        cout << i << ": " << endl;
        gameobj[i]->PrintMatrix();//���� ��Ʈ������ ��� �κп� y��ǥ ����Ǵ��� ����� �� 
    }
   
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key) {

        //ī�޶�����ϴ°�,, �����̽Ű澵�ʿ� ��
    case 'w':
        //i++;
        //gameobj[3]->modelTranslate
        CheckPos();
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
        //��ư ������ �� ���� ���ڰ�(idx) �ϰ��ϵ���
        isDropped = true;
        isTurn = false;
        idx++;
        break;

    case 'Q':
        delete maincam;
        maincam = nullptr;
        delete* gameobj;
        *gameobj = nullptr;
        exit(0);
        break;

    }
}
void DrawObject() {
    //0���� �����¾ֱ��� ������Ʈ �׸�. �Ȱǵ���� ��
    for (int i = 0; i <= idx+1; i++) {
        Objmvp[i] = gameobj[i]->GetTransform_Matrix();
        GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
        glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[i]));
        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
}
void Timerfunction(int value) {
    if (isDropped == true) {
        if (times>=10) {
            isDropped = false;
            isTurn = true;//�ϰ� ��! ���� �ְ� �¿� ������ ����
            times = 0;
        }
        
        //��ǥ ��ǥ�� ����ΰ�?-->���� ���� y��ǥ
        //��ǥ ��ǥ�� ������ 0.1f�� �ϰ�
        //�Ƹ� [3][1]���� �ٲ�°Ű�����!!!!!!!!�Ӹ���!!�ȵ��ư�
        //gameobj[idx]->ReturnPos() < gameobj[idx - 1]->ReturnPos()

        if (times<10) {
            glm::mat4 Temp = glm::translate(gameobj[idx]->GetTransform_Matrix(), glm::vec3(0.0f, -0.1f, 0.0f));
            gameobj[idx]->SetTrans_Matrix(Temp);
            Objmvp[idx] = gameobj[idx]->GetTransform_Matrix();
            GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
            glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[idx]));
            times++;
        }
    }

    if (isTurn) { //���� ���� �ְ� �翷���� �����̴� �ڵ�
        //���� ���� matrix�� x�� 0.1��ŭ �̵���Ų �� temp�� ����
        glm::mat4 Temp = glm::translate(gameobj[idx + 1]->GetTransform_Matrix(), glm::vec3(0.1f, 0.0f, 1.0f));
        //���⼭���� �Ȱǵ���� ��
        gameobj[idx+1]->SetTrans_Matrix(Temp);
        Objmvp[idx + 1] = gameobj[idx + 1]->GetTransform_Matrix();
        GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
        glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(Objmvp[idx + 1]));
    }
    glutPostRedisplay(); // ȭ�� �� ���
    glutTimerFunc(100, Timerfunction, 1);

}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
       xp= ((float)x / 800.0 * 2) - 1;
       yp= -(((float)y / 600.0f * 2) - 1);
    }
    cout << xp << " " << yp << endl;
   glutPostRedisplay(); // ȭ�� �� ���
}

void SpecialKeyboard(int key, int x, int y)
{

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
    glViewport(0, 0, w, h);
}
