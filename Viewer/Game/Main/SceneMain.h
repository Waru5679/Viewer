#pragma once

//���C�u�����t�@�C��
#include "../../Library/Scene.h"

#include "MainCamera.h"

//�V�[�����C��
class CSceneMain :public CSceneBase
{
public:
	void Init();	//������
	void Update();	//�X�V	
	void Draw();	//�`��
	void Release();	//���

private:
	void LoadAudio();		//���y
	void LoadTexture();		//�e�N�X�`��
	void LoadMesh();		//���b�V��

};