#pragma once
#include <vector>
#include <iostream>
using namespace std;
struct Rhythms {
	int milisec = 0;
	char key1 = '\0';
	char key2 = '\0';
};
vector<Rhythms> RhythmInfo(100);

void SetVecData() {
	//RhythmInfo.push_back(Rhythms(0,'b','b'));
}