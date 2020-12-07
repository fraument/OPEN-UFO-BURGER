#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <map>
#include <unordered_map>
#include "Headers.h"

template<class T>
bool CheckSame(T& container, int index_0, int index_1, int count)
{
    for (int i = 0; i < count; ++i)
    {
        if (container[index_0 * count + i] != container[index_1 * count + i])
            return false;
    }

    return true;
}

bool ReadObj(const char* objFileName, float*& vPosOut, float*& vNormalOut, float*& vTextureCoordinateOut, int*& indexOut, int& vertexCount, int& indexCount)
{
    std::ifstream in(objFileName);

    if (!in.is_open())
    {
        std::cout << "ReadObj(const char*, float*&, float*& float*&, int*&)" << std::endl;
        std::cout << objFileName << " 파일을 읽지 못하였습니다." << std::endl;
        return false;
    }

    std::vector<float> rawPositionBuffer; //obj 행좌표
    std::vector<float> rawNormalBuffer;//노멀
    std::vector<float> rawTextureCoordinateBuffer;//텍스쳐 매핑할 좌표

    std::vector<int> rawPosIndexBuffer;//실제코드에서 좌표를 인덱스로 사용?
    std::vector<int> rawNormalIndexBuffer;//노멀도
    std::vector<int> rawTextureCoordinateIndexBuffer;

    float tempFloat;
    std::string tempString;

    while (!in.eof())//파일이 열려있는 동안
    {
        in >> tempString;//파일내용 읽어서 tempstring에 복사
        //obj의 좌표 정보는 .obj를 txt로 바꾸면 알 수 있음.
        //즉, obj를 메모장으로 연 후 거기에 담겨있는 정보를 여기에 복사하겠다는 뜻.
    //http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/

        if (tempString.size() == 1 && tempString[0] == 'v')//
        {//obj의 정점 좌표를 읽어오는 코드
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 2 && tempString[0] == 'v' && tempString[1] == 'n')
        {//obj 정점의 노멀 좌표 읽는 코드
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 2 && tempString[0] == 'v' && tempString[1] == 't')
        {//obj 정점의 텍스쳐 좌표 읽는 코드
            in >> tempFloat;
            rawTextureCoordinateBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawTextureCoordinateBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 1 && tempString[0] == 'f')
        {
            //면 읽어오는 코드
            //a/b/c 로 나누어져 있음
            //a: 정점의 인덱스(위에 v라고 나열되어 있는 애중 몇번쨰줄 읽어올 것인지)
            //b: 텍스쳐 좌표의 인덱스(위에 vt라고 나열되어있는 애중 몇번쨰인지)
            //c: 노멀 좌표의 인덱스(n이라고 나열되어잇는애들중...)

            for (int i = 0; i < 3; ++i)
            {
                in >> tempString;

                std::istringstream iss(tempString);

                std::getline(iss, tempString, '/');
                rawPosIndexBuffer.push_back(atoi(tempString.c_str()) - 1);

                std::getline(iss, tempString, '/');
                rawTextureCoordinateIndexBuffer.push_back(atoi(tempString.c_str()) - 1);

                std::getline(iss, tempString, '/');
                rawNormalIndexBuffer.push_back(atoi(tempString.c_str()) - 1);
            }
        }
    }

    std::vector<float> dupPosBuffer; // Allowing Duplicating  중첩을 허용
    std::vector<float> dupNormalBuffer;
    std::vector<float> dupTextureCoordinateBuffer;
    std::vector<int> dupIndexBuffer;

    int rawIndexCount = rawPosIndexBuffer.size();

    for (int i = 0; i < rawIndexCount; ++i)
    {
        dupPosBuffer.push_back(rawPositionBuffer[rawPosIndexBuffer[i] * 3 + 0]);
        dupPosBuffer.push_back(rawPositionBuffer[rawPosIndexBuffer[i] * 3 + 1]);
        dupPosBuffer.push_back(rawPositionBuffer[rawPosIndexBuffer[i] * 3 + 2]);

        dupNormalBuffer.push_back(rawNormalBuffer[rawNormalIndexBuffer[i] * 3 + 0]);
        dupNormalBuffer.push_back(rawNormalBuffer[rawNormalIndexBuffer[i] * 3 + 1]);
        dupNormalBuffer.push_back(rawNormalBuffer[rawNormalIndexBuffer[i] * 3 + 2]);

        dupTextureCoordinateBuffer.push_back(rawTextureCoordinateBuffer[rawTextureCoordinateIndexBuffer[i] * 2 + 0]);
        dupTextureCoordinateBuffer.push_back(rawTextureCoordinateBuffer[rawTextureCoordinateIndexBuffer[i] * 2 + 1]);

        dupIndexBuffer.push_back(i);
    }

    std::vector<int> noDupLocations;
    noDupLocations.resize(dupIndexBuffer.size());
    std::iota(noDupLocations.begin(), noDupLocations.end(), 0);

    std::map<int, int> oldIndexToNewIndexDict;

    int checkIndex = 0;
    do
    {
        for (int i = checkIndex + 1; i < (int)noDupLocations.size(); ++i)
        {
            if (!CheckSame(dupPosBuffer, noDupLocations[checkIndex], noDupLocations[i], 3))
                continue;

            if (!CheckSame(dupNormalBuffer, noDupLocations[checkIndex], noDupLocations[i], 3))
                continue;

            if (!CheckSame(dupTextureCoordinateBuffer, noDupLocations[checkIndex], noDupLocations[i], 2))
                continue;

            oldIndexToNewIndexDict.emplace(noDupLocations[i], checkIndex);
            noDupLocations.erase(noDupLocations.begin() + i);

            --i;
        }

        ++checkIndex;
    } while (checkIndex < (int)noDupLocations.size());

    for (int i = 0; i < (int)noDupLocations.size(); ++i)
        oldIndexToNewIndexDict.emplace(noDupLocations[i], i);

    vertexCount = noDupLocations.size();
    indexCount = rawIndexCount;
    vPosOut = new float[vertexCount * 3];
    vNormalOut = new float[vertexCount * 3];
    vTextureCoordinateOut = new float[vertexCount * 2];
    indexOut = new int[indexCount];

    for (int i = 0; i < vertexCount; ++i)
    {
        vPosOut[i * 3 + 0] = dupPosBuffer[noDupLocations[i] * 3 + 0];
        vPosOut[i * 3 + 1] = dupPosBuffer[noDupLocations[i] * 3 + 1];
        vPosOut[i * 3 + 2] = dupPosBuffer[noDupLocations[i] * 3 + 2];

        vNormalOut[i * 3 + 0] = dupNormalBuffer[noDupLocations[i] * 3 + 0];
        vNormalOut[i * 3 + 1] = dupNormalBuffer[noDupLocations[i] * 3 + 1];
        vNormalOut[i * 3 + 2] = dupNormalBuffer[noDupLocations[i] * 3 + 2];

        vTextureCoordinateOut[i * 2 + 0] = dupTextureCoordinateBuffer[noDupLocations[i] * 2 + 0];
        vTextureCoordinateOut[i * 2 + 1] = dupTextureCoordinateBuffer[noDupLocations[i] * 2 + 1];
    }


    for (int i = 0; i < indexCount; ++i)
    {
        indexOut[i] = oldIndexToNewIndexDict[dupIndexBuffer[i]];
    }

    cout << "readobj" << endl;
    return true;
}