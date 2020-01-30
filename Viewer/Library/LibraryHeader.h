#pragma once

//���C�u�����Ŏg�p���鎩��w�b�_�t�@�C��
#include "Singleton.h"
#include "SceneInclude.h"
#include "DirectX.h"
#include "Font.h"
#include "Audio.h"
#include "Shader.h"
#include "FrameRate.h"
#include "Scene.h"
#include "Release.h"
#include "Input.h"
#include "Polygon.h"
#include "SkinShader.h"
#include "X_Loader.h"
#include "XLoader_Skin.h"
#include "Task.h"

//�ς��邩������Ȃ����ǂ����ŃC���N���[�h�������
#include "Struct.h"
#include "Math.h"
#include "Task.h"
#include "TextureManager.h"
#include "ModelManager.h"

//���C�u�����Ŏg�p����萔�}�N��
#define DX CDirectX::GetInstance()
#define SHADER CShader::GetInstance()
#define FRAME CFrameRate::GetInstance()
#define SCENE CSceneManager::GetInstance()
#define TASK CTask::GetInstance()
#define AUDIO CAudio::GetInstance()
#define FONT CFont::GetInstance()
#define DRAW CDraw::GetInstance()
#define SKIN_SHADER CSkinShader::GetInstance()
#define X_LOADER CX_Loader::GetInstance()
#define INPUT CInput::GetInstance()
#define TEXTURE CTextureManager::GetInstance()
#define SKIN CX_Skin::GetInstance()
#define MODEL CModelManager::GetInstance()