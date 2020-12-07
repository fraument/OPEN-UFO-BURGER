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
        std::cout << objFileName << " ������ ���� ���Ͽ����ϴ�." << std::endl;
        return false;
    }

    std::vector<float> rawPositionBuffer; //obj ����ǥ
    std::vector<float> rawNormalBuffer;//���
    std::vector<float> rawTextureCoordinateBuffer;//�ؽ��� ������ ��ǥ

    std::vector<int> rawPosIndexBuffer;//�����ڵ忡�� ��ǥ�� �ε����� ���?
    std::vector<int> rawNormalIndexBuffer;//��ֵ�
    std::vector<int> rawTextureCoordinateIndexBuffer;

    float tempFloat;
    std::string tempString;

    while (!in.eof())//������ �����ִ� ����
    {
        in >> tempString;//���ϳ��� �о tempstring�� ����
        //obj�� ��ǥ ������ .obj�� txt�� �ٲٸ� �� �� ����.
        //��, obj�� �޸������� �� �� �ű⿡ ����ִ� ������ ���⿡ �����ϰڴٴ� ��.
    //http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/

        if (tempString.size() == 1 && tempString[0] == 'v')//
        {//obj�� ���� ��ǥ�� �о���� �ڵ�
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawPositionBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 2 && tempString[0] == 'v' && tempString[1] == 'n')
        {//obj ������ ��� ��ǥ �д� �ڵ�
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawNormalBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 2 && tempString[0] == 'v' && tempString[1] == 't')
        {//obj ������ �ؽ��� ��ǥ �д� �ڵ�
            in >> tempFloat;
            rawTextureCoordinateBuffer.push_back(tempFloat);
            in >> tempFloat;
            rawTextureCoordinateBuffer.push_back(tempFloat);
        }
        else if (tempString.size() == 1 && tempString[0] == 'f')
        {
            //�� �о���� �ڵ�
            //a/b/c �� �������� ����
            //a: ������ �ε���(���� v��� �����Ǿ� �ִ� ���� ������� �о�� ������)
            //b: �ؽ��� ��ǥ�� �ε���(���� vt��� �����Ǿ��ִ� ���� ���������)
            //c: ��� ��ǥ�� �ε���(n�̶�� �����Ǿ��մ¾ֵ���...)

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

    std::vector<float> dupPosBuffer; // Allowing Duplicating  ��ø�� ���
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