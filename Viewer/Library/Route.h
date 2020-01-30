#pragma once

#include <vector>
using namespace std;

//�ʒu
struct MY_Pos
{
	int x;
	int y;
};

//�z��T���p
struct Node
{
	MY_Pos next;//���̈ʒu
	int cost;	//��
	int num;	//�}�b�v�̐��l
	bool done;	//�m�肩�ǂ���
	
};

//�o�H�T���N���X
class CRoute
{
public:
	void Init(int Size_x, int Size_y);

	//�o�H�T���i�_�C�N�X�g���@�j
	MY_Pos RouteSearch(int Start_x, int Start_y, int Goal_x, int Goal_y, int** Array);
	MY_Pos RouteSearch(MY_Pos Start, MY_Pos Goal, int** Array);

	//��ԋ߂��}�b�v�̈ʒu�����߂�
	MY_Pos NearInMap(int Pos_x,int Pos_y);
	MY_Pos NearInMap(MY_Pos Pos);

	//����̒n�_��T��
	MY_Pos SerchSubsPos(MY_Pos Pos);
	
	//�w�肵���n�_�̃`�F�b�N
	bool PosCheck(int Pos_x,int Pos_y);
	bool PosCheck(MY_Pos Pos);
private:
	Node** m_map;
	vector<int> m_failue;//��Q�����X�g

	MY_Pos m_SearchVec[4];//�T������(���㍶�E)

	MY_Pos m_Size;//�}�b�v�T�C�Y
};

extern CRoute g_Route;