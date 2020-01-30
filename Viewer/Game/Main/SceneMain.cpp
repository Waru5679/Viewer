#include "../../Library/LibraryHeader.h"
#include "../GameHeader.h"

#include "Hand.h"

//初期化
void CSceneMain::Init()
{
	//素材読み込み
	LoadTexture();
	LoadAudio();
	LoadMesh();
	
	//カメラ
	CMainCamera* pCamera = new CMainCamera();
	TASK->InsertObj(pCamera, ObjName::ObjMainCamera);

	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAngle(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vScale(2.0f, 2.0f, 2.0f);

	//手
	CHand* pHand = new CHand(vPos, vAngle, vScale);
	TASK->InsertObj(pHand, ObjName::ObjHand);

	//シェーダーにカメラセット
	SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	SKIN_SHADER->SetCamera(dynamic_cast<CCameraBase*>(pCamera));
	DRAW->SetCamera(dynamic_cast<CCameraBase*>(pCamera));

}

//更新
void CSceneMain::Update()
{
	TASK->Update();
	AUDIO->Update();
}

//描画
void CSceneMain::Draw()
{
	TASK->Draw();
}

//開放
void CSceneMain::Release()
{
	TASK->ClearTask();
	AUDIO->DeleteMusic();
}

//音楽
void CSceneMain::LoadAudio()
{
}

//テクスチャ
void CSceneMain::LoadTexture()
{
}

//メッシュ
void CSceneMain::LoadMesh()
{
	//X読み込み用
	MODEL->LoadModel(ModelName::ModelHand,"Model/Hand/hand_tex.x");
}