//#include "Obj.h"
//CObj::CObj() {
//
//	
//
//}
//CObj::~CObj() {
//
//}
//
////void ReadObj(FILE* objFile)
////{
////	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
////	char count[100];
////	int vertexNum = 0;
////	int faceNum = 0;
////	while (!feof(objFile)) {
////		fscanf(objFile, "%s", count);
////		if (count[0] == 'v' && count[1] == '\0')
////			vertexNum += 1;
////		else if (count[0] == 'f' && count[1] == '\0')
////			faceNum += 1;
////		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
////	}
////	//--- 2. �޸� �Ҵ�
////	vec4* vertex;
////	vec4* face;
////	int vertIndex = 0;
////	int faceIndex = 0;
////	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
////	face = (vec4*)malloc(sizeof(vec4) * faceNum);
////
////	//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
////	while (!feof(objFile)) {
////		fscanf(objFile, "%s", bind);
////		if (bind[0] == 'v' && bind[1] == '\0') {
////			fscanf(objFile, "%f %f %f",
////				&vertex[vertIndex].x, &vertex[vertIndex].y,
////				&vertex[vertIndex].z);
////			vertIndex++;
////		}
////		else if (bind[0] == 'f' && bind[1] == '\0') {
////			fscanf(objFile, "%f %f %f",
////				&face[faceIndex].x, &face[faceIndex].y, &face[faceIndex].z);
////			faceIndex++;
////		}
////	}
////}
