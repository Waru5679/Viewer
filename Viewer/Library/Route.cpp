#include "Route.h"

CRoute g_Route;

//������
void CRoute::Init(int Size_x, int Size_y)
{
	m_Size = { Size_x,Size_y };

	//�T���p�z��̃������m��
	m_map = new Node*[m_Size.y];
	for (int y = 0; y < m_Size.y; y++)
	{
		m_map[y] = new Node[m_Size.x];
	}

	//��Q�����X�g
	int failue[] =
	{
		1
	};

	//��Q�����X�g�̃T�C�Y
	int failue_size = sizeof(failue) / sizeof(int);

	for (int i = 0; i < failue_size; i++)
	{
		m_failue.push_back(failue[i]);
	}

	//�T������(���㍶�E)
	m_SearchVec[0] = MY_Pos{ 1,0 };
	m_SearchVec[1] = MY_Pos{ -1,0 };
	m_SearchVec[2] = MY_Pos{ 0,-1 };
	m_SearchVec[3] = MY_Pos{ 0,1 };
}

//�_�C�N�X�g���@�i����j
MY_Pos CRoute::RouteSearch(int Start_x, int Start_y, int Goal_x, int Goal_y, int** Array)
{
	MY_Pos Start = { Start_x,Start_y };
	MY_Pos Goal = { Goal_x,Goal_y };

	MY_Pos Out = RouteSearch(Start, Goal, Array);
	return Out;
}

//�o�H�T���i�_�C�N�X�g���@�j
//�ŒZ���[�g�̕�����Ԃ�
MY_Pos CRoute::RouteSearch(MY_Pos Start, MY_Pos Goal, int** Array)
{
	int x, y;

	//������
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
	
	//�}�b�v�O�Ȃ�ύX
	Goal=NearInMap(Goal);
	Start = NearInMap(Start);

	//�T���J�n�ʒu����Q���Ȃ����̈ʒu��T��
	Goal=SerchSubsPos(Goal);

	//�T���I���ʒu����Q���Ȃ����̈ʒu��T��
	Start = SerchSubsPos(Start);

	//�T���J�n�ʒu(�ړI�n)
	m_map[Goal.y][Goal.x].cost = 0;
	m_map[Goal.y][Goal.x].done = true;

	//�m��J�E���g
	int DoneCount = 1;

	while (m_map[Start.y][Start.x].done != true)
	{
		for (y = 0; y < m_Size.y; y++)
		{
			for (x = 0; x < m_Size.x; x++)
			{
				//true�̈ʒu
				if (m_map[y][x].done == true)
				{
					Node now = m_map[y][x];

					//4�������ׂ�
					for (int i = 0; i < 4; i++)
					{
						if((y + m_SearchVec[i].y )<0 ||( y + m_SearchVec[i].y)>=m_Size.y||
							(x + m_SearchVec[i].x) <0 || (x + m_SearchVec[i].x)>=m_Size.x)
						{
							continue;//map�͈̔͊O
						}
						else
						{
							//map�͈͓̔�
							Node next = m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x];

							//�ڑ���̃R�X�g�����̃R�X�g+1���傫���Ƃ�
							if (next.cost > now.cost + 1)
							{
								//�ڑ���̃`�F�b�N
								MY_Pos p = { x + m_SearchVec[i].x ,y + m_SearchVec[i].y };
																
								//��Q������Ȃ����
								if (PosCheck(p) == true)
								{
									//�R�X�g�ǉ�
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].cost = now.cost++;

									//���̈ʒu�ւ̕���������
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].next.x = m_SearchVec[i].x * (-1);
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].next.y = m_SearchVec[i].y * (-1);
						
									//�t���O�����Ă�
									m_map[y + m_SearchVec[i].y][x + m_SearchVec[i].x].done = true;
								}
							}
						}
					}
					if (y == Start.y&&x == Start.x)
					{
						//�T���̏I���ʒu
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


//����̒n�_��T��
MY_Pos CRoute::SerchSubsPos(MY_Pos Pos)
{
	int Range = 1;	//�T���͈͊g���p

	do
	{
		//�X�^�[�g�ʒu����Q���Ȃ����̈ʒu��T��
		if (PosCheck(Pos) == false)
		{
			MY_Pos Copy = Pos;

			//����̈ʒu�������Ă��邩
			bool Sub_f = false;

			for (int k = 0; k < 4; k++)
			{
				//���ɑ���̃X�^�[�g�ʒu���������Ă����
				if (Sub_f == true)
					break;

				//�ڑ��悪�}�b�v������Ȃ��Ȃ�
				if (Pos.y + m_SearchVec[k].y * Range > m_Size.y - 1 || Pos.y + m_SearchVec[k].y * Range < 0 ||
					Pos.x + m_SearchVec[k].x * Range > m_Size.x - 1 || Pos.x + m_SearchVec[k].x * Range < 0)
				{
					continue;
				}
				else
				{
					//�}�b�v��
					MY_Pos Check = { Pos.x + m_SearchVec[k].x * Range ,Pos.y + m_SearchVec[k].y * Range };
					if (PosCheck(Check) == true)
					{
						Pos.y += m_SearchVec[k].y * Range;
						Pos.x += m_SearchVec[k].x * Range;
						Sub_f = true;
					}
				}
			}
			//��ŕύX�ł��Ȃ����
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

//�w�肵���n�_�̃`�F�b�N
bool CRoute::PosCheck(int Pos_x, int Pos_y)
{
	MY_Pos Pos = {Pos_x,Pos_y};
	return PosCheck(Pos);
}

//�w�肵���n�_�̃`�F�b�N
bool CRoute::PosCheck(MY_Pos Pos)
{
	bool out = true;

	//�w�肵���ʒu����Q�������ׂ�
	for (unsigned int k = 0; k < m_failue.size(); k++)
	{
		if (m_map[Pos.y][Pos.x].num == m_failue[k])
		{
			//��Q���Ȃ�t���O��false��
			out = false;
		}
	}

	return out;
}

//��ԋ߂��}�b�v�̈ʒu�����߂�(����)
MY_Pos CRoute::NearInMap(int Pos_x, int Pos_y)
{
	MY_Pos out;
	MY_Pos Pos = { Pos_x,Pos_y };

	out =NearInMap(Pos);

	return out;
}

//��ԋ߂��}�b�v�̈ʒu�����߂�
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
