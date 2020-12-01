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
////	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
////	char count[100];
////	int vertexNum = 0;
////	int faceNum = 0;
////	while (!feof(objFile)) {
////		fscanf(objFile, "%s", count);
////		if (count[0] == 'v' && count[1] == '\0')
////			vertexNum += 1;
////		else if (count[0] == 'f' && count[1] == '\0')
////			faceNum += 1;
////		memset(count, '\0', sizeof(count)); // 배열 초기화
////	}
////	//--- 2. 메모리 할당
////	vec4* vertex;
////	vec4* face;
////	int vertIndex = 0;
////	int faceIndex = 0;
////	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
////	face = (vec4*)malloc(sizeof(vec4) * faceNum);
////
////	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
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
