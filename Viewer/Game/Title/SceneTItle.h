#pragma once

//���C�u�����t�@�C��
#include "../../Library/Scene.h"

//�V�[�����C��
class CSceneTitle :public CSceneBase
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