//���C�u�����t�@�C��
#include "LibraryHeader.h"

//�Q�[���Ŏg���V�[���t�@�C��
#include "../Game/GameHeader.h"

//�V�[���̍쐬
void CSceneInclude::LoadScene()
{
	//���C��
	CSceneMain* pMain = new CSceneMain();
	SCENE->Insert(pMain, SceneName::SceneMain);

	//�^�C�g��
	CSceneTitle* pTitle = new CSceneTitle();
	SCENE->Insert(pTitle, SceneName::SceneTitle);

}