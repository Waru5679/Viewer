#pragma once
#include <stdio.h>
#include <Windows.h>
#include <XAudio2.h>
#include <vector>
#include <memory>

#include "Singleton.h"
#include "Release.h"
using namespace std;

//チャンク情報
struct ChunkInfo
{
	ChunkInfo() { Size = 0, pData = nullptr; }
	~ChunkInfo() { pData = nullptr; }
	unsigned int Size;		//チャンク部のサイズ
	unsigned char* pData;	//チャンクデータの先頭ポインタ
};

//サウンド情報
struct SoundData
{
	//コンストラクタ
	SoundData() {}
	SoundData(ChunkInfo Chunk, unsigned char* pResourceData, IXAudio2SourceVoice* pSourceVoice, bool Loop)
	{
		m_Chunk = Chunk;
		m_pResourceData = pResourceData;
		m_pSourceVoice = pSourceVoice;
		m_bLoop = Loop;
		m_bStart = false;
	}
	//デストラクタ
	~SoundData()
	{
	}

	ChunkInfo m_Chunk;					//チャンク情報
	unsigned char* m_pResourceData;		//サウンドファイルのポインタ
	IXAudio2SourceVoice* m_pSourceVoice;//サウンドボイスのインターフェース
	bool m_bLoop;	//ループ再生
	bool m_bStart;	//再生確認
};

//オーディオクラス
class CAudio:public CSingleton<CAudio>
{
private:
	//シングルトン
	friend CSingleton<CAudio>;
	CAudio() {};	//コンストラクタ
	~CAudio() {};	//デストラクタ
public:
	void Init();	//初期化
	void Update();	//解放
	void Release();	//解放

	//音楽情報の破棄
	void DeleteMusic();	

	//音楽の読み込み
	void Load(int Id ,const char* Name,bool Loop);
	
	//マスターボリュームの調整
	void MasterVolume(float t);

	//スタート
	void StartMusic(int Id);

	//ストップ
	void StopMusic(int Id);
	
private:	
	//Wave読み込み
	unsigned char* LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name);

	//char * を　WORD　に変換
	WORD GetWord(unsigned char* pData);

	//char * を　DWORD　に変換
	DWORD GetDWord(unsigned char* pData);

	//チャンクを探す関数
	ChunkInfo FindChunk(unsigned char* pData, char* pChunkName);

	IXAudio2* m_pXAudio2;						//XAudio2オブジェクト
	IXAudio2MasteringVoice* m_pMasteringVoice;	//マスターボイス
	IXAudio2SubmixVoice* m_pSubmixVoice;		//サブミクスインターフェース
	vector<SoundData> m_SoundData;				//サウンド情報
};