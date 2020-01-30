#include "LibraryHeader.h"
#include "XLoader_Skin.h"
#include "Release.h"
#include "Math.h"

#include <math.h>

//インスタンス
CX_Skin* CX_Skin::m_pInstance = nullptr;

//スキンメッシュの読み込み
bool CX_Skin::LoadSkinMesh(const char* FileName, X_SKIN_MESH* pSkinMesh)
{
	//ファイル読み込み
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");
	
	if (fp == nullptr)
	{
		//ファイルオープン失敗
		return false;
	}

	//templateを省いたファイルの読み込み開始位置を保存
	long ReadStartPos = X_LOADER->GetTemplateSkipStartPos(fp);

	X_MESH	Mesh;				//メッシュ
	int		BoneNum			= 0;//ボーン数
	int		SkinWeightNum	= 0;//スキンウェイトの数
	int		AnimeNum		= 0;//アニメーションの数

	X_BONE*			pBone		= nullptr;	//ボーンリスト
	X_SKIN_WEIGHT*	pSkinWeight	= nullptr;	//スキンウェイトリスト
	X_ANIMATION*	pAnime		= nullptr;	//アニメションリスト

	X_SKIN_MESH_HEADER SkinHeader;	//スキンメッシュヘッダー

	//スキンメッシュヘッダー読み込み
	X_LOADER->LoadSkinMeshHeader(fp, &SkinHeader, ReadStartPos);

	//メッシュの読み込み
	if (X_LOADER->LoadMesh(fp, &Mesh, &SkinHeader,ReadStartPos) == false)
	{
		//メッシュ読み込み失敗
		return false;
	}
	
	//ボーン数取得
	BoneNum = X_LOADER->GetBoneNum(fp, ReadStartPos);
	
	//ボーンがあるときのみ
	if (BoneNum != 0)
	{
		//ボーンリストメモリ確保
		pBone = new X_BONE[BoneNum];

		//ボーン読み込み
		if (X_LOADER->LoadBone(fp, pBone, ReadStartPos) == false)
		{
			//ボーン読み込み失敗
			return false;
		}
	}

	//スキンウェイトの数を数える
	SkinWeightNum = X_LOADER->GetSkinWeightNum(fp, ReadStartPos);

	//スキンウェイトがあるときのみ
	if (SkinWeightNum != 0)
	{
		//スキンウェイトメモリ確保
		pSkinWeight = new X_SKIN_WEIGHT[SkinWeightNum];
		
		//スキン情報の読み込み
		if (X_LOADER->LoadSkinWeight(fp, pSkinWeight, ReadStartPos) == false)
		{
			//スキン情報読み込み失敗
			return false;
		}
	}

	//アニメーションの数取得
	AnimeNum = X_LOADER->GetAnimeNum(fp, ReadStartPos);

	//アニメーションがあるときのみ
	if (AnimeNum != 0)
	{
		//アニメーションメモリ確保
		pAnime = new X_ANIMATION[AnimeNum];

		//アニメーション読み込み
		if (X_LOADER->LoadAnimation(fp, pAnime, ReadStartPos) == false)
		{
			//アニメーション読み込み失敗
			return false;
		}
	}

	//スキンメッシュにまとめる
	X_LOADER->SkinMeshPutTogether(Mesh, pBone, BoneNum,pSkinWeight, SkinWeightNum, pAnime,AnimeNum, pSkinMesh,SkinHeader);

	//スキンウェイトの情報をもとに各頂点に対応ボーンとウェイトの情報を持たせる
	X_LOADER->VertexMatchBone(pSkinMesh);

	//ウェイト順に各頂点のボーンインデックスをソートする
	WeightSort(pSkinMesh);

	//バーテックスバッファーを作成
	pSkinMesh->m_Mesh.m_pVertexBuffer = DRAW->BufferCreate(pSkinMesh->m_Mesh.m_pVertex, sizeof(X_VERTEX) * pSkinMesh->m_Mesh.m_VerNum, D3D10_BIND_VERTEX_BUFFER);

	return true;
}

//ウェイトが大きい順にソートする
void CX_Skin::WeightSort(X_SKIN_MESH* pSkin)
{
	//頂点分回す
	for (int i=0;i<pSkin->m_Mesh.m_VerNum; i++)
	{
		//ウェイトが大きい順にソート
		for (int j = 0; j<MAX_VER_WEIGH-1; j++)
		{
			for (int k = j + 1; k < MAX_VER_WEIGH; k++)
			{
				if (pSkin->m_Mesh.m_pVertex[i].m_fWeight[j] < pSkin->m_Mesh.m_pVertex[i].m_fWeight[k])
				{
					float fTmp = pSkin->m_Mesh.m_pVertex[i].m_fWeight[j];
					int iTmp = pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[j];

					pSkin->m_Mesh.m_pVertex[i].m_fWeight[j]= pSkin->m_Mesh.m_pVertex[i].m_fWeight[k];
					pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[j] = pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[k];

					pSkin->m_Mesh.m_pVertex[i].m_fWeight[k] = fTmp;
					pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[k] = iTmp;
				}
			}
		}
	}
}

//フレーム補完
X_KEY CX_Skin::FrameComplement(int NowFrame, X_ANIMATOIN_KEY AnimKey)
{
	X_KEY out;

	int	 keyNum		= AnimKey.m_KeyNum;
	int* pFrameDiff = new int[keyNum];
	bool bKey = false;

	//フレームの間隔差を保存
	for (int i = 0; i <keyNum; i++)
	{
		pFrameDiff[i] = AnimKey.m_pKey[i].m_Time - NowFrame;

		//現在のフレームがキーフレームの場合
		if (pFrameDiff[i] == 0)
		{
			bKey = true;
			out = AnimKey.m_pKey[i];
		}
	}

	//キーフレーム以外なら補完する
	if(bKey==false)
	{
		X_KEY before= AnimKey.m_pKey[0];
		X_KEY after= AnimKey.m_pKey[keyNum-1];
		
		//前フレームを探す
		for (int i = 0; i < keyNum; i++)
		{
			//差がマイナスの場合前フレーム
			if (pFrameDiff[i] < 0)
				before = AnimKey.m_pKey[i];
		}

		//後フレームを探す
		for (int i = keyNum - 1; i >= 0; i--)
		{
			//差がプラスなら後フレーム
			if (pFrameDiff[i] > 0)
				after = AnimKey.m_pKey[i];
		}

		//フレーム差を求める
		int FrameDiff = after.m_Time - before.m_Time;

		//変化の割合を求める
		float fPercent=	(float)(NowFrame - before.m_Time) / FrameDiff;

		//差分ポーズのメモリ確保
		float* pfPoseDiff = new float[before.m_ValueNum];

		//前後フレームでのポーズ差(前後フレームのデータ数は同じと仮定する)
		for (int i = 0; i < before.m_ValueNum; i++)
		{			
			//ポーズの差分を求める
			pfPoseDiff[i] = after.m_pfValue[i] - before.m_pfValue[i];
		}

		//値の数保存
		out.m_ValueNum = before.m_ValueNum;

		//メモリの確保
		out.m_pfValue = new float[out.m_ValueNum];

		//現在のポーズを求める
		for (int i = 0; i < out.m_ValueNum ; i++)
		{
			out.m_pfValue[i] = before.m_pfValue[i] + (pfPoseDiff[i] * fPercent);
		}

		//ポーズの差分破棄
		delete[] pfPoseDiff;
	}

	//現在フレーム保存
	out.m_Time = NowFrame;

	//フレーム差分破棄
	delete[] pFrameDiff;
	pFrameDiff = nullptr;

	return out;
}

//ボーンの更新
void CX_Skin::BoneUpdate(X_SKIN_MESH* pSkin, int AnimeId, int NowFrame)
{
	//実行するアニメーションデータ
	X_ANIMATION anime = pSkin->m_pAnimation[AnimeId];
	
	//ボーン毎に更新する
	bool bBoneFind = false;
	for (int i = 0; i < pSkin->m_BoneNum; i++)
	{
		bBoneFind = false;
		pSkin->m_pBone[i].m_matNewPose=GetPose(&bBoneFind,pSkin, pSkin->m_pRoot, anime, NowFrame,i);
	}
}

//ポーズを取得する
D3DXMATRIX CX_Skin::GetPose(bool* bBoneFind, X_SKIN_MESH* pSkin, X_BONE* pBone, X_ANIMATION Anime, int NowFrame, int BoneID)
{
	//ポーズ
	D3DXMATRIX matOut;
	D3DXMatrixIdentity(&matOut);

	//新しいポーズ
	D3DXMATRIX matNewPose;
	D3DXMatrixIdentity(&matNewPose);

	//ボーン名と一致するアニメーションデータを探す
	bool bAnimFind = false;
	for (int i = 0; i < Anime.m_BoneKeyNum && bAnimFind ==false; i++)
	{
		//ボーン名と一致するアニメーションデータ発見
		if (strcmp(pBone->m_Name, Anime.m_pBoneKey[i].m_AffectBoneName) == 0)
		{
			bAnimFind = true;

			//フレーム補完用
			D3DXMATRIX matRot, matTrans, matScale,matMatrix;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matScale);
			D3DXMatrixIdentity(&matMatrix);

			//そのボーンのポーズを更新する　
			for (int j = 0; j < Anime.m_pBoneKey[i].m_AniKeyNum; j++)
			{
				//回転
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType==KEY_TYPE::ROT)
				{
					//フレーム補完
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//補完したデータをベクトルにする
					D3DXVECTOR3 vRot(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//行列に変換
					matRot=MakeMatRot(vRot);
				}
				//スケール
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType == KEY_TYPE::SCALE)
				{
					//フレーム補完
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//補完したデータをベクトルにする
					D3DXVECTOR3 vScale(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//行列に変換
					matScale = MakeMatScale(vScale);
				}
				//移動
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType == KEY_TYPE::TRANS)
				{
					//フレーム補完
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//補完したデータをベクトルにする
					D3DXVECTOR3 vTrans(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//行列に変換
					matTrans = MakeMatTrans(vTrans);
				}
				//行列
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType >= KEY_TYPE::MATRIX)
				{
					//フレーム補完
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//行列に変換
					matMatrix = D3DXMATRIX(newPose.m_pfValue);
				}
			}

			//新しいポーズ
			matNewPose = matScale * matRot * matTrans * matMatrix;

			//ほしいボーンIDのボーンなら
			if (pBone->m_index == BoneID)
			{
				//ボーンのポーズ更新
				*bBoneFind = true;
				matOut = pBone->m_matOffset * matNewPose;
				return matOut;
			}
		}
	}

	//子ボーンのポーズを求める
	bool bFind = false;
	for (int i = 0; i < pBone->m_ChildNum && bFind == false; i++)
	{
		matOut=GetPose(&bFind,pSkin, &pSkin->m_pBone[pBone->m_pChildIndex[i]], Anime, NowFrame,BoneID)*matNewPose;
		
		//目当てのボーンが見つかっていれば
		if (bFind == true)
		{
			*bBoneFind = true;
			return matOut;
		}
	}

	D3DXMatrixIdentity(&matOut);
	return matOut;
}

//アニメーション
void CX_Skin::Animation(int AnimeId,int NowFrame, X_SKIN_MESH* pSkinMesh)
{
	//ボーン更新
	BoneUpdate(pSkinMesh, AnimeId, NowFrame);
}

//メッシュ描画
void CX_Skin::DrawMesh(D3DMATRIX matWorld, X_SKIN_MESH* pSkinMesh, CColorData* pColor)
{
	//バーテックスバッファーをセット）
	UINT stride = sizeof(X_VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pSkinMesh->m_Mesh.m_pVertexBuffer, &stride, &offset);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i < pSkinMesh->m_Mesh.m_MaterialNum; i++)
	{
		//面ごとに描画
		for (int j = 0; j < pSkinMesh->m_Mesh.m_pMaterial[i].m_FaceNum; j++)
		{
			X_FACE face = pSkinMesh->m_Mesh.m_pFace[pSkinMesh->m_Mesh.m_pMaterial[i].m_pFaceIndex[j]];

			//インデックスバッファーをセット
			stride = sizeof(int);
			offset = 0;
			DX->GetDevice()->IASetIndexBuffer(pSkinMesh->m_Mesh.m_pMaterial[i].m_ppIndexBuffer[j], DXGI_FORMAT_R32_UINT, 0);

			//プリミティブ・トポロジーをセット
			if (face.m_FaceOfVer == TRIANGLE_POLYGON)
			{
				//三角ポリゴン
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			if (face.m_FaceOfVer == QUAD_POLYGON)
			{
				//四角ポリゴン
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			//シェーダセット
			SKIN_SHADER->SetShader(pSkinMesh->m_Mesh.m_pMaterial[i].m_pTexture, NULL, pColor, matWorld,pSkinMesh);

			D3D10_TECHNIQUE_DESC dc;
			SKIN_SHADER->GetTechnique()->GetDesc(&dc);
			for (UINT p = 0; p < dc.Passes; ++p)
			{
				SKIN_SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
				DX->GetDevice()->DrawIndexed(face.m_FaceOfVer, 0, 0);
			}
		}
	}
}

//開放
void CX_Skin::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}