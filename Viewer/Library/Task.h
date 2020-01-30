#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Singleton.h"
using namespace std;

//�Ǘ��p�@�N���X
class CTask:public CSingleton<CTask>
{
private:
	//�V���O���g��
	friend CSingleton<CTask>;
	CTask() {};
	~CTask() {};
public:
	void Update();		//�X�V
	void Draw();		//�`��
	void Release();		//���
	void ClearTask();	//�^�X�N���N���A
		
	//�o�^
	void InsertObj(CObjBase* pObj, int Id);
	
	//�擾	
	CObjBase* GetObj(int Id);
	
private:
	//�\�[�g
	void Sort();

	vector<CObjBase*> m_Obj;			//�I�u�W�F�N�g
};