#include "../../Library/LibraryHeader.h"
#include "../GameHeader.h"

//������
void CSceneTitle::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();

	//�J����
	CTitleCamera* pCamera = new CTitleCamera();
	TASK->InsertObj(pCamera, ObjName::ObjTitleCamera);

	//�^�C�g���I�u�W�F�N�g
	CTitleObj* pObj = new CTitleObj();
	TASK->InsertObj(pObj, ObjName::ObjTitle);

	//�V�F�[�_�[�ɃJ�����Z�b�g
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

}

//�X�V
void CSceneTitle::Update()
{
	TASK->Update();
	AUDIO->Update();
}

//�`��
void CSceneTitle::Draw()
{
	TASK->Draw();
}

//�J��
void CSceneTitle::Release()
{
	TASK->ClearTask();
	AUDIO->DeleteMusic();
}

//���y
void CSceneTitle::LoadAudio()
{
}

//�e�N�X�`��
void CSceneTitle::LoadTexture()
{
	TEXTURE->Load(TexName::TexTitle, "Texture/Title.png", 640, 480);
}

//���b�V��
void CSceneTitle::LoadMesh()
{

}