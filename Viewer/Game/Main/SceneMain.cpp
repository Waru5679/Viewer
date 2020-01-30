#include "../../Library/LibraryHeader.h"
#include "../GameHeader.h"

#include "Hand.h"

//������
void CSceneMain::Init()
{
	//�f�ޓǂݍ���
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//�J����
	CMainCamera* pCamera = new CMainCamera();
	TASK->InsertObj(pCamera, ObjName::ObjMainCamera);

	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(2.0f, 2.0f, 2.0f);

	//��
	CHand* pHand = new CHand(vPos, vAngle, vScale);
	TASK->InsertObj(pHand, ObjName::ObjHand);

	//�V�F�[�_�[�ɃJ�����Z�b�g
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

}

//�X�V
void CSceneMain::Update()
{
	TASK->Update();
	AUDIO->Update();
}

//�`��
void CSceneMain::Draw()
{
	TASK->Draw();
}

//�J��
void CSceneMain::Release()
{
	TASK->ClearTask();
	AUDIO->DeleteMusic();
}

//���y
void CSceneMain::LoadAudio()
{
}

//�e�N�X�`��
void CSceneMain::LoadTexture()
{
}

//���b�V��
void CSceneMain::LoadMesh()
{
	//X�ǂݍ��ݗp
	MODEL->LoadModel(ModelName::ModelHand,"Model/Hand/hand_tex.x");
}