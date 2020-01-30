#include "X_Loader.h"

CX_Loader* CX_Loader::m_pInstance = nullptr;


//templateを飛ばした読み込み開始位置を取得する
long CX_Loader::GetTemplateSkipStartPos(FILE* fp)
{
	//ファイルの先頭にセット
	fseek(fp, 0, SEEK_SET);

	long pos;//ファイルの現在位置

	//キーワード読み込み
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//ファイルの現在位置保存
		pos = ftell(fp);

		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//Frameが出るまで飛ばす
		if (strcmp(str, "Frame") == 0)
		{
			return pos;
		}
	}
}

//メッシュ情報の読み込み
bool CX_Loader::LoadMesh(FILE* fp, X_MESH* pMesh, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//読み込みの開始位置にセット
	fseek(fp, lStartPos, SEEK_SET);

	int verNum	= 0;//頂点数
	int faceNum = 0;//面の数
	int normNum = 0;//法線数
	int uvNum	= 0;//UV数
	int mateNum = 0;//マテリアルの数

	//読み込み用（読み込み後破棄）
	D3DXVECTOR3* pvPos		= nullptr;//頂点座標
	D3DXVECTOR3* pvNormal	= nullptr;//法線
	D3DXVECTOR2* pvTex		= nullptr;//テクスチャ座標

	//読み込み後そのまま使うもの
	X_FACE*		pFace		= nullptr;//面のリスト
	X_MATERIAL* pMaterial	= nullptr;//マテリアルのリスト
	X_VERTEX*	pVertex		= nullptr;//頂点リスト

	//読み込み用
	float x, y, z, w;
	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	//キーワード読み込み
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//頂点
		if (strcmp(str, "Mesh") == 0)
		{
			//メッシュ名
			fgets(str, sizeof(str), fp);

			//頂点数
			fgets(str, sizeof(str), fp);
			verNum = atoi(str);

			//頂点座標メモリ確保
			pvPos = new D3DXVECTOR3[verNum];

			//頂点座標読み込み
			for (int i = 0; i < verNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvPos[i].x = x;
				pvPos[i].y = y;
				pvPos[i].z = z;
			}

			//;除去
			fgets(str, sizeof(str), fp);

			//面の数
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//面のメモリ確保
			pFace = new X_FACE[faceNum];

			int faceOfVer = 0;//面を構成する頂点数

			//面読み込み
			for (int i = 0; i < faceNum; i++)
			{
				//面を構成する頂点の数
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace[i].m_FaceOfVer = faceOfVer;

				//面構成のインデックスリストのメモリ確保
				pFace[i].m_pIndex = new int[faceOfVer];

				//三角ポリゴン
				if (faceOfVer == TRIANGLE_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
				}
				//四角ポリゴン
				if (faceOfVer == QUAD_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d,%d;,", &v1, &v2, &v3, &v4);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
					pFace[i].m_pIndex[3] = v4;
				}
			}
		}
		//法線
		if (strcmp(str, "MeshNormals") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//法線数
			fgets(str, sizeof(str), fp);
			normNum = atoi(str);

			//法線メモリ確保
			pvNormal = new D3DXVECTOR3[normNum];

			//法線読み込み
			for (int i = 0; i < normNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//テクスチャー座標 読み込み
		if (strcmp(str, "MeshTextureCoords") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//UVの数
			fgets(str, sizeof(str), fp);
			uvNum = atoi(str);

			//UVメモリ確保
			pvTex = new D3DXVECTOR2[uvNum];

			//UV読み込み
			for (int i = 0; i < uvNum; i++)
			{
				fscanf_s(fp, "%f;%f;,", &x, &y);
				pvTex[i].x = x;
				pvTex[i].y = y;
			}
		}

		//マテリアルリスト読み込み
		if (strcmp(str, "MeshMaterialList") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//マテリアルの数
			fgets(str, sizeof(str), fp);
			mateNum = atoi(str);

			//マテリアルメモリ確保
			pMaterial = new X_MATERIAL[mateNum];

			//面の数
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//面に何番のマテリアルを使うか
			for (int i = 0; i < faceNum; i++)
			{
				//読み込み
				fscanf_s(fp, "%d", &pFace[i].m_UseMaterial);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}
		}
	}

	//読み込みの開始位置に戻る
	fseek(fp, lStartPos, SEEK_SET);

	while (!feof(fp))
	{
		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//マテリアル読み込み
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				//{除去
				fgets(str, sizeof(str), fp);

				//面の色
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMaterial[i].m_vFaceColor.x = x;
				pMaterial[i].m_vFaceColor.y = y;
				pMaterial[i].m_vFaceColor.z = z;
				pMaterial[i].m_vFaceColor.w = w;

				//スペキュラーのパワー
				fscanf_s(fp, "%f;", &pMaterial[i].m_fPower);

				//スペキュラー
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKs.x = x;
				pMaterial[i].m_vKs.y = y;
				pMaterial[i].m_vKs.z = z;

				//エミッシブ
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKe.x = x;
				pMaterial[i].m_vKe.y = y;
				pMaterial[i].m_vKe.z = z;

				//テクスチャ
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{除去

					//読み込み最大サイズ
					int size = sizeof(pMaterial[i].m_TexName);

					//テクスチャ名
					fscanf_s(fp, "%s", pMaterial[i].m_TexName, size);

					//"と;を除去する
					ErasCharFromString(pMaterial->m_TexName, size, '\"');
					ErasCharFromString(pMaterial->m_TexName, size, ';');

					//テクスチャーを作成
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMaterial[i].m_TexName, NULL, NULL, &pMaterial[i].m_pTexture, NULL)))
					{
						//テクスチャ作成失敗
						return false;
					}
				}
			}
		}
	}

	//頂点構造体メモリ確保
	pVertex = new X_VERTEX[verNum];

	//頂点構造体にまとめる
	for (int i = 0; i < verNum; i++)
	{
		pVertex[i].m_vPos = pvPos[i];
		pVertex[i].m_vNorm = pvNormal[i];
		pVertex[i].m_vTex = pvTex[i];
	}

	//マテリアル毎に情報を整理する
	for (int i = 0; i < mateNum; i++)
	{
		//そのマテリアルを使用する面の数を数える
		int UseFaceNum = 0;
		for (int j = 0; j < faceNum; j++)
		{
			if (pFace[j].m_UseMaterial == i)
			{
				UseFaceNum++;
			}
		}

		//面の数
		pMaterial[i].m_FaceNum = UseFaceNum;

		//面のリストメモリ確保
		pMaterial[i].m_pFaceIndex = new int[UseFaceNum];

		//面のインデックスを保存
		int face_count = 0;
		for (int j = 0; j < faceNum; j++)
		{
			//このマテリアルを使用する面のみ
			if (pFace[j].m_UseMaterial == i)
			{
				pMaterial[i].m_pFaceIndex[face_count] = j;
				face_count++;
			}
		}

		//面の数だけメモリ確保
		pMaterial[i].m_ppIndexBuffer = new ID3D10Buffer * [pMaterial[i].m_FaceNum];

		//面の数だけインデックスバッファ作成
		for (int j = 0; j < pMaterial[i].m_FaceNum; j++)
		{
			X_FACE face = pFace[pMaterial[i].m_pFaceIndex[j]];

			//インデックスバッファ作成
			pMaterial[i].m_ppIndexBuffer[j] = DRAW->BufferCreate(face.m_pIndex, sizeof(int) * face.m_FaceOfVer, D3D10_BIND_INDEX_BUFFER);
		}
	}

	//一時保存からメッシュポインタへデータを移す
	pMesh->m_MaterialNum = mateNum;
	pMesh->m_pMaterial	= pMaterial;
	pMesh->m_FaceNum	= faceNum;
	pMesh->m_pFace		= pFace;
	pMesh->m_pVertex	= pVertex;
	pMesh->m_VerNum		= verNum;

	//一時保存は破棄
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);

	return true;
}

//指定文字を文字列から消す
void CX_Loader::ErasCharFromString(char* pSource, int Size, char Erace)
{
	int count = 0;//除去カウント

	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{
			//除去数をカウント
			count++;
		}
		else
		{
			//カウント分を詰めてコピー
			pSource[i - count] = pSource[i];
		}
	}
}

//スキンメッシュヘッダー読み込み
void CX_Loader::LoadSkinMeshHeader(FILE* fp, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//スキンメッシュヘッダー読み込み
		if (strcmp(str, "XSkinMeshHeader") == 0)
		{
			//{除去
			fscanf_s(fp, "%s", str, sizeof(str));

			//頂点の最大ウェイト数
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxVertex);

			//面の最大ウェイト数
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxFace);

			//ボーン数
			fscanf_s(fp, "%d;", &pSkinHeader->m_BoneNum);

			//}除去
			fscanf_s(fp, "%s", str, sizeof(str));
		}
	}
}

//ボーン数の取得
int CX_Loader::GetBoneNum(FILE* fp, long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//ボーンの数

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//ボーンの数を数える
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "Frame") == 0)
		{
			boneNum++;
		}
	}

	return boneNum;
}

//ボーン読み込み
bool CX_Loader::LoadBone(FILE* fp, X_BONE* pBone, long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//ボーンの数

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	int start_count = 0;//{を数える
	int end_count	= 0;//}を数える

	int boneIndex = 0;//インデックスカウンター

	//ボーン読み込み
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//ボーンをリストに保存
			X_BONE bone = LoadBoneInfo(fp, &boneIndex, pBone);
			pBone[bone.m_index] = bone;
		}
	}
	return true;
}

//ボーン情報の読み込み
X_BONE CX_Loader::LoadBoneInfo(FILE* fp, int* pBoneIndex, X_BONE* pBone)
{
	//関数呼び出し時のファイルの位置を保存
	long ReadStartPos = ftell(fp);

	int start_count = 0;//{カウント
	int end_count	= 0;//}カウント
	int childNum	= 0;//子ボーンの数

	//読み込み用
	char str[READ_ARRAY_SIZE];

	//ボーン読み込み用
	X_BONE bone;

	//自身のインデックス
	bone.m_index = *(pBoneIndex);

	char test[READ_ARRAY_SIZE];

	fscanf_s(fp, "%s ", test, sizeof(test));

	//先に子ボーンの数を数える
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}カウント
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}
		//子ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//このボーンの直属の子
			if (start_count - end_count == 1)
			{
				childNum++;
			}
			else
			{
				;//孫ボーンなどはカウントしない
			}
		}
	}

	//この関数が呼ばれたと時の位置に戻す
	fseek(fp, ReadStartPos, SEEK_SET);

	//子の数を保存
	bone.m_ChildNum = childNum;

	//子ボーンのインデックスリストメモリ確保
	bone.m_pChildIndex = new int[childNum];

	//カウンタを初期化
	start_count = 0;
	end_count	= 0;
	childNum	= 0;

	fscanf_s(fp, "%s", bone.m_Name, sizeof(bone.m_Name));

	//本読み込み
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}カウント
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}

		//初期ポーズ
		if (strcmp(str, "FrameTransformMatrix") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//行列読み込み
			D3DXMATRIX mat;
			fgets(str, sizeof(str), fp);
			sscanf_s(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);

			//保存
			bone.m_matBindPose = mat;

			//}除去
			fgets(str, sizeof(str), fp);
		}

		//子ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//ボーンのインデックス更新
			*pBoneIndex = *(pBoneIndex)+1;

			//子ボーンのインデックスを保存
			bone.m_pChildIndex[childNum++] = *pBoneIndex;

			//ボーンをリストに保存
			X_BONE read = LoadBoneInfo(fp, pBoneIndex, pBone);
			pBone[read.m_index] = read;
		}
	}

	return bone;
}


//スキンウェイトの数を取得
int CX_Loader::GetSkinWeightNum(FILE* fp, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//スキンウェイトの数
	int SkinWeightNum = 0;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//スキンウェイトの数をカウントする
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "SkinWeights") == 0)
		{
			SkinWeightNum++;
		}
	}

	return SkinWeightNum;
}

//スキンウェイトの読み込み
bool CX_Loader::LoadSkinWeight(FILE* fp, X_SKIN_WEIGHT* pSkinWeight, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//読み込み用
	char boneName[NAME_ARRAY_SIZE];	//ボーン名
	int count		= 0;			//カウンター		
	int weightNum	= 0;			//ウェイト数
	D3DXMATRIX mat;					//行列

	//読み込み
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//スキンウェイト
		if (strcmp(str, "SkinWeights") == 0)
		{
			fgets(str, sizeof(str), fp);//{除去

			//ボーン名
			fscanf_s(fp, "%s", boneName, sizeof(boneName));

			//"と;を除去する
			ErasCharFromString(boneName, sizeof(boneName), '\"');
			ErasCharFromString(boneName, sizeof(boneName), ';');

			//保存
			strcpy_s(pSkinWeight[count].m_BoneName, boneName);

			//ウェイトの数
			fscanf_s(fp, "%d;", &weightNum);
			pSkinWeight[count].m_WeightNum = weightNum;

			//インデックスとウェイトのメモリ確保
			pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinWeight[count].m_pWeight = new float[weightNum];

			//インデックス読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinWeight[count].m_pIndex[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}

			//ウェイト読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f", &pSkinWeight[count].m_pWeight[i]);
				//,または;の除去
				fgets(str, sizeof(str), fp);
			}
			//オフセット行列
			fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinWeight[count].m_matOffset = mat;

			//カウンター更新
			count++;
		}
	}
	return true;
}

//アニメーションの数を取得
int CX_Loader::GetAnimeNum(FILE* fp, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//アニメーションの数
	int animeNum = 0;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//アニメーションの数を数える
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "AnimationSet") == 0)
		{
			animeNum++;
		}
	}
	return animeNum;
}

//スキンウェイトの情報をもとに各頂点に対応ボーンとウェイトの情報を持たせる
void CX_Loader::VertexMatchBone(X_SKIN_MESH* pSkin)
{
	//対応ボーン名
	char boneName[NAME_ARRAY_SIZE];

	for (int i = 0; i < pSkin->m_WeightNum; i++)
	{
		//対応ボーン名
		strcpy_s(boneName, pSkin->m_pWeight[i].m_BoneName);

		//対応ボーンを探す
		bool bFind = false;
		int boneID = -1;
		for (int j = 0; j < pSkin->m_BoneNum && bFind == false; j++)
		{
			//対応ボーン発見
			if (strcmp(pSkin->m_pBone[j].m_Name, boneName) == 0)
			{
				bFind = true;
				boneID = j;
			}
		}

		//対応ボーンにオフセット行列を渡す
		pSkin->m_pBone[boneID].m_matOffset = pSkin->m_pWeight[i].m_matOffset;

		//対応頂点にボーンIDとウェイトを渡す
		for (int j = 0; j < pSkin->m_pWeight[i].m_WeightNum; j++)
		{
			X_VERTEX ver = pSkin->m_Mesh.m_pVertex[pSkin->m_pWeight[i].m_pIndex[j]];

			//ボーンID
			ver.m_BoneIndex[ver.m_WeightNum] = boneID;

			//ウェイト
			ver.m_fWeight[ver.m_WeightNum] = pSkin->m_pWeight[i].m_pWeight[j];

			//ウェイト数更新
			ver.m_WeightNum++;

			//保存
			pSkin->m_Mesh.m_pVertex[pSkin->m_pWeight[i].m_pIndex[j]] = ver;
		}
	}
}

//ボーン毎のキー情報の読み込み
X_BONE_KEY CX_Loader::LoadBoneKey(FILE* fp)
{
	X_BONE_KEY Out;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//対応ボーン名
	char boneName[NAME_ARRAY_SIZE];

	//読み込み用初期化
	int keyNum = 0;
	int keyCount = 0;
	int valueNum = 0;
	int count = 0;//カウンター

	int start_count = 0;	//{カウント
	int end_count = 0;		//}カウント

	//関数呼び出し時のファイルの位置を保存
	long ReadStartPos = ftell(fp);

	//アニメーションキーの数を取得
	int AnimeKeyNum = GetAnimeKeyNum(fp);

	//アニメーションキーのメモリ確保
	Out.m_pAniKey = new X_ANIMATOIN_KEY[AnimeKeyNum];

	//アニメーションキーの数保存
	Out.m_AniKeyNum = AnimeKeyNum;

	//この関数が呼ばれたと時の位置に戻す
	fseek(fp, ReadStartPos, SEEK_SET);

	//アニメーション名
	fscanf_s(fp, "%s ", str, sizeof(str));

	//アニメーション名がない場合
	if (strcmp(str, "{") == 0)
	{
		start_count++;
	}

	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
		{
			start_count++;

			//対応ボーン名
			if (start_count - end_count >= 2)
			{
				fscanf_s(fp, "%s ", boneName, sizeof(boneName));

				//保存
				strcpy_s(Out.m_AffectBoneName, boneName);
			}						
		}
		
		//}カウント
		if (strcmp(str, "}") == 0)
			end_count++;


		//アニメーションキー読み込み
		if (strcmp(str, "AnimationKey") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//読み込み
			Out.m_pAniKey[count++]=LoadAnimationKey(fp);

			//}除去
			fgets(str, sizeof(str), fp);
		}
	}
	return Out;
}

//アニメーションキーの数を取得する
int CX_Loader::GetAnimeKeyNum(FILE* fp)
{
	int Out = 0;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	int start_count = 0;	//{カウント
	int end_count = 0;		//}カウント

	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
			start_count++;
		
		//}カウント
		if (strcmp(str, "}") == 0)
			end_count++;
		
		//アニメーションキーをカウント
		if (strcmp(str, "AnimationKey") == 0)
			Out++;
	}
	return Out;
}

//アニメーションキーの読み込み
X_ANIMATOIN_KEY CX_Loader::LoadAnimationKey(FILE* fp)
{
	X_ANIMATOIN_KEY Out;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	int keyNum = -1;//キーの数
	int valueNum = -1;//値の数
	int keyCount = 0;//キーカウンター

	//キータイプ
	fscanf_s(fp, "%d;", &Out.m_KeyType);

	//キーの数
	fscanf_s(fp, "%d;", &keyNum);
	Out.m_KeyNum = keyNum;

	//キーのメモリ確保
	Out.m_pKey = new X_KEY[keyNum];

	//キーの読み込み
	for (int i = 0; i < keyNum; i++)
	{
		//コマ
		fscanf_s(fp, "%d;", &Out.m_pKey[i].m_Time);

		//データの数
		fscanf_s(fp, "%d;", &valueNum);
		Out.m_pKey[i].m_ValueNum = valueNum;

		//データのメモリ確保
		Out.m_pKey[i].m_pfValue = new float[valueNum];

		//カウンター初期化
		keyCount = 0;

		//最初の一つ
		fscanf_s(fp, "%f", &Out.m_pKey[i].m_pfValue[keyCount++]);

		//2つ目以降は,も含めて読む
		while (keyCount < Out.m_pKey[i].m_ValueNum)
		{
			fscanf_s(fp, ",%f", &Out.m_pKey[i].m_pfValue[keyCount++]);
		}

		//;;,を除去
		fgets(str, sizeof(str), fp);
	}

	return Out;
}


//アニメーション読み込み
bool CX_Loader::LoadAnimation(FILE* fp, X_ANIMATION* pAnime, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//読み込み用
	char animeName[NAME_ARRAY_SIZE];	//アニメーション名
	int animeCount = 0;					//アニメーションカウンター

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//アニメーション
		if (strcmp(str, "AnimationSet") == 0)
		{
			//アニメーション名
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pAnime[animeCount].m_Name, animeName);

			//ファイルの位置を保存
			long ReadStartPos = ftell(fp);

			int boneNum = 0;
			int start_count = 0;
			int end_count = 0;

			//先に対応ボーンの数を数える
			while (start_count != end_count || start_count == 0 || end_count == 0)
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				//{カウント
				if (strcmp(str, "{") == 0)
				{
					start_count++;
				}

				//}カウント
				if (strcmp(str, "}") == 0)
				{
					end_count++;
				}

				//対応ボーン
				if (strcmp(str, "Animation") == 0)
				{
					boneNum++;
				}
			}

			//対応ボーン数保存
			pAnime[animeCount].m_BoneKeyNum = boneNum;

			//メモリ確保
			pAnime[animeCount].m_pBoneKey = new X_BONE_KEY[boneNum];

			//アニメーション読み込み位置にセットする
			fseek(fp, ReadStartPos, SEEK_SET);

			//対応ボーン分読み込み
			int boneCount = 0;
			while (!feof(fp) && (boneCount < boneNum))
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				if (strcmp(str, "Animation") == 0)
				{
					//ボーン毎のキー読み込み
					pAnime[animeCount].m_pBoneKey[boneCount] = LoadBoneKey(fp);
					boneCount++;
				}
			}
			//カウンター更新
			animeCount++;
		}
	}
	return true;
}

//スキンメッシュにまとめる
void CX_Loader::SkinMeshPutTogether(X_MESH Mesh, X_BONE* pBone, int BoneNum, X_SKIN_WEIGHT* pSkinWeight, int WeightNum, X_ANIMATION* pAnimation, int AnimeNum, X_SKIN_MESH* pSkinMesh, X_SKIN_MESH_HEADER SkinHeader)
{
	pSkinMesh->m_Mesh = Mesh;						//メッシュ
	pSkinMesh->m_BoneNum = BoneNum;					//ボーン数
	pSkinMesh->m_pBone = pBone;						//ボーン
	pSkinMesh->m_WeightNum = WeightNum;				//ウェイト数	
	pSkinMesh->m_pWeight = pSkinWeight;				//ウェイトリスト
	pSkinMesh->m_AnimeNum = AnimeNum;				//アニメーションの数
	pSkinMesh->m_pAnimation = pAnimation;			//アニメーション
	pSkinMesh->m_SkinHeader = SkinHeader;			//スキンヘッダー
	pSkinMesh->m_pRoot = &pSkinMesh->m_pBone[0];	//ルートボーン
}