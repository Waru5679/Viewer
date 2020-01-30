#include "Audio.h"
#include "Release.h"

//インスタンス
CAudio* CAudio::m_pInstance = nullptr;

//初期化
void CAudio::Init()
{
	m_pXAudio2 = nullptr;
	m_pMasteringVoice = nullptr;
	m_pSubmixVoice = nullptr;

	//XAudioの作成
	XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスの作成
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);

	if (m_pMasteringVoice != nullptr)
	{
		//ミックスボイス作成
		m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
	}

	//マスターボリューム設定
	MasterVolume(1.0f);
}

//音楽の読み込み
void CAudio::Load(int Id,const char* Name,bool Loop)
{
	WAVEFORMATEX WaveFormatEx;
	
	//Wave情報を読み込む
	ChunkInfo Chunk;
	unsigned char* pResourceData = LoadWave(&Chunk, &WaveFormatEx, Name);

	if (m_pMasteringVoice != nullptr)
	{
		//再生のためのインターフェースの作成
		IXAudio2SourceVoice* pSourceVoice;
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };

		m_pXAudio2->CreateSourceVoice(&pSourceVoice, &WaveFormatEx, 0U, 2.0f, NULL, &SendList);
	
		//サウンド情報を登録
		SoundData data(Chunk, pResourceData, pSourceVoice, Loop);
		m_SoundData.insert(m_SoundData.begin() + Id, data);
	}
}

//再生
void CAudio::StartMusic(int Id)
{
	if(m_pMasteringVoice!=nullptr)
	{
		//サウンドバッファをソースボイスに送信
		XAUDIO2_BUFFER SoundBuffer={ 0 };
		SoundBuffer.AudioBytes = m_SoundData[Id].m_Chunk.Size;
		SoundBuffer.pAudioData = (BYTE*)m_SoundData[Id].m_Chunk.pData;
		SoundBuffer.Flags = XAUDIO2_END_OF_STREAM;
		SoundBuffer.LoopCount = 0;
	
		//サウンドバッファセット
		m_SoundData[Id].m_pSourceVoice->SubmitSourceBuffer(&SoundBuffer);

		//サウンドスタート
		m_SoundData[Id].m_pSourceVoice->Start();
		m_SoundData[Id].m_bStart = true;
	}
}

//更新
void CAudio::Update()
{
	 XAUDIO2_VOICE_STATE state;
	 for (unsigned int i = 0; i < m_SoundData.size(); i++)
	 {
		 //再生中かどうかをチェックする
		 m_SoundData[i].m_pSourceVoice->GetState(&state);

		 //再生が終わった時
		 if (state.BuffersQueued == 0)
		 {
			 StopMusic(i);	//停止

			 //ループで再生されていたらもう一度再生
			 if (m_SoundData[i].m_bStart == true && m_SoundData[i].m_bLoop == true)
			 {  
				 StartMusic(i);	
			 }
		 } 
	 }
}

//停止
void CAudio::StopMusic(int Id)
{
	m_SoundData[Id].m_pSourceVoice->Stop();					//音楽を止める
	m_SoundData[Id].m_pSourceVoice->FlushSourceBuffers();	//サウンドボイスに保留中のバッファを破棄
	m_SoundData[Id].m_bStart = false;
}

//Wave読み込み
unsigned char* CAudio::LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name)
{
	unsigned char* WaveData;

	//Waveファイルのオープン
	FILE* fp;
	fopen_s(&fp, Name, "rb");

	//ファイルサイズ取得
	unsigned int Size = 0;
	fseek(fp, 0, SEEK_END);
	Size = ftell(fp);
	WaveData = new unsigned char[Size];

	//ファイルデータをメモリに移す
	fseek(fp, 0, SEEK_SET);
	fread(WaveData, Size, 1, fp);
	fclose(fp);

	//RIFFファイルの解析
	//RIFFデータの先頭アドレスとRIFFデータのサイズを渡す
	ChunkInfo WaveChunk = FindChunk(WaveData, (char*)"fmt");
	unsigned char*p = WaveChunk.pData;

	//Wave情報の取得
	pWave->wFormatTag = GetWord(p);
	p += sizeof(WORD);

	pWave->nChannels = GetWord(p);
	p += sizeof(WORD);

	pWave->nSamplesPerSec = GetDWord(p);
	p += sizeof(DWORD);

	pWave->nAvgBytesPerSec = GetDWord(p);
	p += sizeof(DWORD);

	pWave->nBlockAlign = GetWord(p);
	p += sizeof(WORD);

	pWave->wBitsPerSample = GetWord(p);
	p += sizeof(WORD);

	pWave->cbSize = GetWord(p);
	p += sizeof(WORD);

	//波形データの先頭アドレスと波形データのサイズを渡す
	ChunkInfo info= FindChunk(WaveData, (char*)"data");
	pChunkInfo->Size = info.Size;
	pChunkInfo->pData = info.pData;
	
	return WaveData;
}

//マスターボリュームの調整
void CAudio::MasterVolume(float t)
{
	if(m_pMasteringVoice!=nullptr)
		m_pMasteringVoice->SetVolume(t, 0);
}

//char * を　WORD　に変換
WORD CAudio::GetWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8;
	return value;
}

//char * を　DWORD　に変換
DWORD CAudio::GetDWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
	return value;
}

//チャンクを探す関数
ChunkInfo CAudio::FindChunk(unsigned char* pData, char* pChunkName)
{
	unsigned CHUNKNAME_LENGTH = strlen(pChunkName);

	while (1)
	{
		bool bFind = true;
		for (unsigned int i = 0; i < CHUNKNAME_LENGTH; ++i)
		{
			if (pData[i] != pChunkName[i])
			{
				bFind = false;
				break;
			}
		}
		if (bFind == true)
		{
			ChunkInfo Info;
			Info.Size = pData[4 + 0] | pData[4 + 1] << 8 | pData[4 + 2] << 16 | pData[4 + 3] << 24;
			Info.pData = (unsigned char*)(pData + 8);
			return Info;
		}
		pData++;
	}
	return ChunkInfo();
}

//音楽情報の破棄
void CAudio::DeleteMusic()
{
	//ソースボイス破棄
	for (unsigned int i = 0; i < m_SoundData.size(); i++)
	{
		if (m_SoundData[i].m_pSourceVoice != nullptr)
		{
			m_SoundData[i].m_pSourceVoice->Stop();
			m_SoundData[i].m_pSourceVoice->FlushSourceBuffers();
			m_SoundData[i].m_pSourceVoice->DestroyVoice();
		}
	}
	//サウンド情報破棄
	VectorRelease(m_SoundData);
}

//解放
void CAudio::Release()
{	
	//ミックスサウンドの破棄
	if (m_pSubmixVoice != nullptr)
	{
		m_pSubmixVoice->DestroyVoice();
	}

	//マスターボイスの破棄
	if (m_pMasteringVoice != nullptr)
	{
		m_pMasteringVoice->DestroyVoice();
	}

	//XAudio2オブジェクト破棄
	m_pXAudio2->Release();

	//インスタンス破棄
	PointerRelease(m_pInstance);
}