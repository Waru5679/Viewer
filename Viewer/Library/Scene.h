#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
using namespace std;

//�V�[���N���X(�p���p)
class CSceneBase
{
public:
	virtual void Init() = 0;	//������
	virtual void Update() = 0;	//�X�V
	virtual void Draw()=0;		//�`��
	virtual void Release()=0;	//���
private:
	virtual void LoadAudio() = 0;		//���y
	virtual void LoadTexture() = 0;		//�e�N�X�`��
	virtual void LoadMesh() = 0;		//���b�V��
};

//�V�[���Ǘ��p�\����
struct MY_SCENE
{
	//�R���X�g���N�^
	MY_SCENE() {}
	MY_SCENE(CSceneBase* pScene,int Id)
	{
		m_pScene = pScene;
		m_Id = Id;
	}
	
	CSceneBase* m_pScene;	//�V�[���|�C���^
	int m_Id;				//�Ǘ��pID
};

using pMyScene=unique_ptr<MY_SCENE>;

//�V�[���}�l�[�W���[
class CSceneManager:public CSingleton<CSceneManager>
{
private:
	//�V���O���g��
	friend CSingleton<CSceneManager>;
	CSceneManager() {};		//�R���X�g���N�^
	~CSceneManager() {};	//�f�X�g���N�^
public:
	void Update();							//�X�V
	void Draw();							//�`��
	void Release();							//���
	void Insert(CSceneBase* pScene, int Id);//�o�^
	void SetScene(int n);					//�V�[���̃Z�b�g
	CSceneBase* GetScene(int Id);			//�V�[���擾
private:
	CSceneBase* m_pNowScene;				//���݂̃V�[���̃|�C���^
	vector<pMyScene> m_Scene;				//�V�[�����X�g
};