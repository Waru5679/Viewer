#pragma once

//���C�u�����t�@�C��
#include "../../Library/CObj.h"
#include "../../Library/LibraryHeader.h"

//TitleObj
class CTitleObj :public CObj2DBase
{
public:
	void Init();		//������
	void Update();		//�X�V
	void Draw();		//�`��
	void Release() {};	//�J��
private:

	wchar_t m_wPos_x[100];
	wchar_t m_wPos_y[100];
};