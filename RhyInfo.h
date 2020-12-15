#pragma once
#include <vector>
#include <iostream>
using namespace std;

typedef struct Rhythms {
	int milisec;
	char key1;
	char key2;
	int LongNoteEndIdx; 
	//롱프레스 시작 끝 알려주는 지표들
	//0-->long note start, -1=short note& middle of long note, n=long note end idx(+n)
	Rhythms()
	:milisec(0),key1('\n'),key2('\0')
	{
	}

	Rhythms(int sec, char k1, char k2)
		:milisec(sec), key1(k1),key2(k2)
	{
	}

};
vector<Rhythms> RhythmInfo;

void SetVecData() {
	RhythmInfo.reserve(100);
	//키 df jk
	//key2 nonset-->/
	//전주 st 느린박자~ 단타 롱타 연습
	RhythmInfo.push_back(Rhythms(3028, 'd', '/'));
	RhythmInfo.push_back(Rhythms(4905, 'f', '/'));
	RhythmInfo.push_back(Rhythms(6734, 'j', '/'));
	RhythmInfo.push_back(Rhythms(8584, 'j', '/'));
	RhythmInfo.push_back(Rhythms(10480, 'k', '/'));

	RhythmInfo.push_back(Rhythms(12306, 'd', '/'));
	RhythmInfo.push_back(Rhythms(14230, 'd', '/'));
	RhythmInfo.push_back(Rhythms(16088, 'd', '/'));
	RhythmInfo.push_back(Rhythms(17972, 'd', 'f'));
	RhythmInfo.push_back(Rhythms(19815, 'j', '/'));
	RhythmInfo.push_back(Rhythms(21720, 'j', '/'));
	RhythmInfo.push_back(Rhythms(23542, 'j', '/'));
	RhythmInfo.push_back(Rhythms(25482, 'j', 'k'));

	//본격스테이지 들어감
	RhythmInfo.push_back(Rhythms(28394, 'j', '/'));//long press
	RhythmInfo.push_back(Rhythms(30265, 'j', 'k'));
	RhythmInfo.push_back(Rhythms(31213, 'j', 'k'));
	RhythmInfo.push_back(Rhythms(32161, 'j', 'k'));
	RhythmInfo.push_back(Rhythms(33089, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(34037, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(34985, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(35909, 'd', 'k'));

	//한번더들어감 
	RhythmInfo.push_back(Rhythms(36860, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(37786, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(38724, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(39647, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(40617, 'd', '/'));
	RhythmInfo.push_back(Rhythms(41540, 'f', '/'));
	RhythmInfo.push_back(Rhythms(42486, 'k', '/'));
	RhythmInfo.push_back(Rhythms(43433, 'j', '/'));

	RhythmInfo.push_back(Rhythms(44366, 'f', '/'));//long press
	RhythmInfo.push_back(Rhythms(45284, 'f', 'd'));
	RhythmInfo.push_back(Rhythms(46209, 'k', '/'));//long press
	RhythmInfo.push_back(Rhythms(47170, 'k', 'j'));
	RhythmInfo.push_back(Rhythms(48123, 'd', '/'));
	RhythmInfo.push_back(Rhythms(49031, 'k', '/'));//long press
	RhythmInfo.push_back(Rhythms(49942, 'k', 'f'));
	RhythmInfo.push_back(Rhythms(50905, 'k', 'f'));

	RhythmInfo.push_back(Rhythms(51878, 'd', '/'));
	RhythmInfo.push_back(Rhythms(52796, 'f', '/'));//long press
	RhythmInfo.push_back(Rhythms(53719, 'f', 'k'));
	RhythmInfo.push_back(Rhythms(54651, 'j', '/'));
	RhythmInfo.push_back(Rhythms(55602, 'j', '/'));
	RhythmInfo.push_back(Rhythms(56512, 'k', '/'));//long press
	RhythmInfo.push_back(Rhythms(57497, 'k', 'f'));
	RhythmInfo.push_back(Rhythms(58422, 'k', 'f'));

	RhythmInfo.push_back(Rhythms(59391, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(60298, 'd', 'f'));
	RhythmInfo.push_back(Rhythms(61227, 'j', '/'));//long press
	RhythmInfo.push_back(Rhythms(62164, 'j', 'd'));
	RhythmInfo.push_back(Rhythms(63112, 'f', '/'));
	RhythmInfo.push_back(Rhythms(64040, 'k', '/'));
	RhythmInfo.push_back(Rhythms(64952, 'j', '/'));//long press
	RhythmInfo.push_back(Rhythms(65911, 'j', 'd'));

	RhythmInfo.push_back(Rhythms(66847, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(67781, 'd', 'j'));
	RhythmInfo.push_back(Rhythms(68728, 'd', 'f'));
	RhythmInfo.push_back(Rhythms(69672, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(70620, 'j', '/'));//long press
	RhythmInfo.push_back(Rhythms(71541, 'j', 'f'));
	RhythmInfo.push_back(Rhythms(72475, 'f', '/'));//long press
	RhythmInfo.push_back(Rhythms(73411, 'f', 'd'));

	RhythmInfo.push_back(Rhythms(74379, 'd', '/'));
	RhythmInfo.push_back(Rhythms(75286, 'f', '/'));
	RhythmInfo.push_back(Rhythms(76214, 'k', '/'));//long press
	RhythmInfo.push_back(Rhythms(77172, 'k', 'd'));
	RhythmInfo.push_back(Rhythms(78104, 'k', 'f'));
	RhythmInfo.push_back(Rhythms(79038, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(79966, 'd', 'j'));
	RhythmInfo.push_back(Rhythms(80958, 'd', 'k'));

	RhythmInfo.push_back(Rhythms(81870, 'k', '/'));
	RhythmInfo.push_back(Rhythms(82803, 'j', '/'));//long press
	RhythmInfo.push_back(Rhythms(83751, 'j', 'd'));
	RhythmInfo.push_back(Rhythms(84696, 'j', 'k'));
	RhythmInfo.push_back(Rhythms(85646, 'f', '/'));
	RhythmInfo.push_back(Rhythms(86535, 'd', '/'));//long press
	RhythmInfo.push_back(Rhythms(87471, 'd', 'k'));
	RhythmInfo.push_back(Rhythms(88415, 'j', '/'));

	RhythmInfo.push_back(Rhythms(89377, 'f', '/'));//long press
	RhythmInfo.push_back(Rhythms(90341, 'f', 'k'));
	RhythmInfo.push_back(Rhythms(91330, 'f', '/'));

}