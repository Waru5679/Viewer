#include "Route.h"

CRoute g_Route;

//初期化
void CRoute::Init(int Size_x, int Size_y)
{
	m_Size = { Size_x,Size_y };

	//探索用配列のメモリ確保
	m_map = new Node*[m_Size.y];
	for (int y = 0; y < m_Size.y; y++)
	{
		m_map[y] = new Node[m_Size.x];
	}

	//障害物リスト
	int failue[] =
	{
		1
	};

	//障害物リストのサイズ
	int failue_size = sizeof(failue) / sizeof(int);

	for (int i = 0; i < failue_size; i++)
	{
		m_failue.push_back(failue[i]);
	}

	//探索方向(下上左右)
	m_SearchVec[0] = MY_Pos{ 1,0 };
	m_SearchVec[1] = MY_Pos{ -1,0 };
	m_SearchVec[2] = MY_Pos{ 0,-1 };
	m_SearchVec[3] = MY_Pos{ 0,1 };
}

//ダイクストラ法（仲介）
MY_Pos CRoute::RouteSearch(int Start_x, int Start_y, int Goal_x, int Goal_y, int** Array)
{
	MY_Pos Start = { Start_x,Start_y };
	MY_Pos Goal = { Goal_x,Goal_y };

	MY_Pos Out = RouteSearch(Start, Goal, Array);
	return Out;
}

//経路探索（ダイクストラ法）
//最短ルートの方向を返す
MY_Pos CRoute::RouteSearch(MY_Pos Start, MY_Pos Goal, int** Array)
{
	int x, y;

	//初期化
	for (y = 0; y < m_Size.y; y++)
	{
		for (x = 0; x < m_Size.x; x++)
		{
			m_map[y][x].next.x = 0;
			m_map[y][x].next.y = 0;
			m_map[y][x].cost = 999;
			m_map[y][x].done = false;

			m_map[y][x].num = Array[y][x];
		}
	}
	
	//マップ外なら変更
	Goal=NearInMap(Goal);
	Start = NearInMap(Start);

	//探索開始位置が障害物なら代わりの位置を探す
	Goal=SerchSubsPos(Goal);

	//探索終了位置が障害物なら代わりの位置を探す
	Start = SerchSubsPos(Start);

	//探索開始位置(目的地)
	m_map[Goal.y][Goal.x].cost = 0;
	m_map[Goal.y][Goal.x].done = true;

	//確定カウント
	int DoneCount = 1;

	while (m_map[Start.y][Start.x].done != true)
	{
		for (y = 0; y < m_Size.y; y++)
		{
			for (x = 0; x < m_Size.x; x++)
			{
				//trueの位置
				if (m_map[y][x].done == true)
				{
					Node now = m_map[y][x];

					//4方向調べる
					for (int i = 0; i < 4; i++)
					{
						if((y + m_SearchVec[i].y )<0 ||( y + m_SearchVec[i].y)>=m_Size.y||
							(x + m_SearchVec[i].x) <0 || (x + m_SearchVec[i].x)>=m_Size.x)
						{
							continue;//mapの範囲外
						}
						else
						{
							//mapの範囲内
							Node next = m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x];

							//接続先のコストが今のコスト+1より大きいとき
							if (next.cost > now.cost + 1)
							{
								//接続先のチェック
								MY_Pos p = { x + m_SearchVec[i].x ,y + m_SearchVec[i].y };
																
								//障害物じゃなければ
								if (PosCheck(p) == true)
								{
									//コスト追加
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].cost = now.cost++;

									//今の位置への方向を入れる
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].next.x = m_SearchVec[i].x * (-1);
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].next.y = m_SearchVec[i].y * (-1);
						
									//フラグを建てる
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].done = true;
								}
							}
						}
					}
					if (y == Start.y&&x == Start.x)
					{
						//探索の終了位置
					}
					else
					{
						m_map[y][x].done = false;
					}
				}
			}
		}

		DoneCount = 0;
		for (y = 0; y < m_Size.y; y++)
		{
			for (x = 0; x < m_Size.x; x++)
			{
				if (m_map[y][x].done == true)
				{
					DoneCount++;
				}
			}
		}

		if (DoneCount == 0)
			break;

	}
	return m_map[Start.y][Start.x].next;
}


//代わりの地点を探す
MY_Pos CRoute::SerchSubsPos(MY_Pos Pos)
{
	int Range = 1;	//探索範囲拡張用

	do
	{
		//スタート位置が障害物なら代わりの位置を探す
		if (PosCheck(Pos) == false)
		{
			MY_Pos Copy = Pos;

			//代わりの位置が見つかているか
			bool Sub_f = false;

			for (int k = 0; k < 4; k++)
			{
				//既に代わりのスタート位置が見つかっていれば
				if (Sub_f == true)
					break;

				//接続先がマップ内じゃないなら
				if (Pos.y + m_SearchVec[k].y * Range > m_Size.y - 1 || Pos.y + m_SearchVec[k].y * Range < 0 ||
					Pos.x + m_SearchVec[k].x * Range > m_Size.x - 1 || Pos.x + m_SearchVec[k].x * Range < 0)
				{
					continue;
				}
				else
				{
					//マップ内
					MY_Pos Check = { Pos.x + m_SearchVec[k].x * Range ,Pos.y + m_SearchVec[k].y * Range };
					if (PosCheck(Check) == true)
					{
						Pos.y += m_SearchVec[k].y * Range;
						Pos.x += m_SearchVec[k].x * Range;
						Sub_f = true;
					}
				}
			}
			//上で変更できなければ
			if (Pos.x == Copy.x && Pos.y == Copy.y)
			{
				Range++;
				if (Range > 10)
					Range = 0;
			}
		}
	} while ( PosCheck(Pos) == false);

	return Pos;

}

//指定した地点のチェック
bool CRoute::PosCheck(int Pos_x, int Pos_y)
{
	MY_Pos Pos = {Pos_x,Pos_y};
	return PosCheck(Pos);
}

//指定した地点のチェック
bool CRoute::PosCheck(MY_Pos Pos)
{
	bool out = true;

	//指定した位置が障害物か調べる
	for (unsigned int k = 0; k < m_failue.size(); k++)
	{
		if (m_map[Pos.y][Pos.x].num == m_failue[k])
		{
			//障害物ならフラグをfalseに
			out = false;
		}
	}

	return out;
}

//一番近いマップの位置を求める(仲介)
MY_Pos CRoute::NearInMap(int Pos_x, int Pos_y)
{
	MY_Pos out;
	MY_Pos Pos = { Pos_x,Pos_y };

	out =NearInMap(Pos);

	return out;
}

//一番近いマップの位置を求める
MY_Pos CRoute::NearInMap(MY_Pos Pos)
{
	MY_Pos out = Pos;
	if (Pos.y < 0)
		out.y = 0;
	if (Pos.y > m_Size.y - 1)
		out.y = m_Size.y - 1;
	if (Pos.x < 0)
		out.x = 0;
	if (Pos.x > m_Size.x - 1)
		out.x = m_Size.x - 1;

	return out;
}
