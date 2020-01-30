#pragma once

//���߂̃V�[��
#define START_SCENE SceneName::SceneMain


//�Q�[���Ŏg���I�u�W�F�N�g�t�@�C��
#include "../Game/Main/MainCamera.h"
#include "../Game/Title/TitleCamera.h"
#include "../Game/Title/TitleObj.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/Main/SceneMain.h"
#include "../Game/Title/SceneTitle.h"

//�V�[��
enum SceneName
{
	SceneMain,	//���C��
	SceneTitle,	//�^�C�g��
};

//�I�u�W�F�N�gID(������)
enum ObjName
{
	ObjMainCamera,	//���C���J����
	ObjDraw2D,		//2D�`��
	ObjTitleCamera,	//�^�C�g���J����
	ObjTitle,		//�^�C�g��
	ObjHand,		//��
};

//���f��
enum ModelName
{
	ModelHand,	//��
};

//�e�N�X�`��
enum TexName
{
	TexTitle,	//�^�C�g���e�N�X�`��
};

