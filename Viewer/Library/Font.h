#pragma once 

#include <locale.h>
#include <wchar.h>
#include <memory>
#include <list>
using namespace std;

//�������ʃN���X
class CCharClass
{
public:
	//�R���X�g���N�^
	CCharClass()
	{
		m_pc.reset();
	}
	//�f�X�g���N�^
	~CCharClass()
	{
		m_pc.reset();
		m_pTexResView->Release();
		m_pTexResView = nullptr;
	}

	//�����e�N�X�`���쐬
	void CreateCharTex(wchar_t c, HDC hdc, TEXTMETRIC TM);
	
	//���ʕ����擾
	wchar_t* GetChar() { return m_pc.get(); }

	//�e�N�X�`���擾
	ID3D10ShaderResourceView* GetTex() { return m_pTexResView; }

private:
	//��e�N�X�`���쐬
	ID3D10Texture2D* CreateTexture();

	//ShaderResourceView�쐬
	ID3D10ShaderResourceView* CreateResourceView(ID3D10Texture2D* pTex);
	
	unique_ptr<wchar_t>			m_pc;			//���ʗp����
	ID3D10ShaderResourceView*	m_pTexResView;	//�e�N�X�`��
};

//Font�N���X
class CFont:public CSingleton<CFont> 
{
private:
	//�V���O���g��
	friend CSingleton<CFont>;
	CFont() {};		//�R���X�g���N�^
	~CFont() {};	//�f�X�g���N�^
public:
	void Init();	//������
	void Release();	//���

	//������`��
	void DrawStr(const wchar_t* Str,float Pos_x,float Pos_y,float FontSize,float Rad);

	//����������ɕ����e�N�X�`�����쐬
	void CreateStrTex(const wchar_t* str);
private:	
	HDC m_hdc;			//�f�B�X�v���C�f�o�C�X�R���e�L�X�g�̃n���h��
	TEXTMETRIC m_TM;	//�t�H���g���i�[�p

	list< unique_ptr<CCharClass>>* m_Char; //�������X�g
};