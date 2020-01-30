#pragma once

#include <vector>
using namespace std;

//位置
struct MY_Pos
{
	int x;
	int y;
};

//配列探査用
struct Node
{
	MY_Pos next;//次の位置
	int cost;	//回数
	int num;	//マップの数値
	bool done;	//確定かどうか
	
};

//経路探索クラス
class CRoute
{
public:
	void Init(int Size_x, int Size_y);

	//経路探索（ダイクストラ法）
	MY_Pos RouteSearch(int Start_x, int Start_y, int Goal_x, int Goal_y, int** Array);
	MY_Pos RouteSearch(MY_Pos Start, MY_Pos Goal, int** Array);

	//一番近いマップの位置を求める
	MY_Pos NearInMap(int Pos_x,int Pos_y);
	MY_Pos NearInMap(MY_Pos Pos);

	//代わりの地点を探す
	MY_Pos SerchSubsPos(MY_Pos Pos);
	
	//指定した地点のチェック
	bool PosCheck(int Pos_x,int Pos_y);
	bool PosCheck(MY_Pos Pos);
private:
	Node** m_map;
	vector<int> m_failue;//障害物リスト

	MY_Pos m_SearchVec[4];//探索方向(下上左右)

	MY_Pos m_Size;//マップサイズ
};

extern CRoute g_Route;