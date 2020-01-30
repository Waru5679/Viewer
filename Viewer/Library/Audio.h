#pragma once
#include <stdio.h>
#include <Windows.h>
#include <XAudio2.h>
#include <vector>
#include <memory>

#include "Singleton.h"
#include "Release.h"
using namespace std;

//�`�����N���
struct ChunkInfo
{
	ChunkInfo() { Size = 0, pData = nullptr; }
	~ChunkInfo() { pData = nullptr; }
	unsigned int Size;		//�`�����N���̃T�C�Y
	unsigned char* pData;	//�`�����N�f�[�^�̐擪�|�C���^
};

//�T�E���h���
struct SoundData
{
	//�R���X�g���N�^
	SoundData() {}
	SoundData(ChunkInfo Chunk, unsigned char* pResourceData, IXAudio2SourceVoice* pSourceVoice, bool Loop)
	{
		m_Chunk = Chunk;
		m_pResourceData = pResourceData;
		m_pSourceVoice = pSourceVoice;
		m_bLoop = Loop;
		m_bStart = false;
	}
	//�f�X�g���N�^
	~SoundData()
	{
	}

	ChunkInfo m_Chunk;					//�`�����N���
	unsigned char* m_pResourceData;		//�T�E���h�t�@�C���̃|�C���^
	IXAudio2SourceVoice* m_pSourceVoice;//�T�E���h�{�C�X�̃C���^�[�t�F�[�X
	bool m_bLoop;	//���[�v�Đ�
	bool m_bStart;	//�Đ��m�F
};

//�I�[�f�B�I�N���X
class CAudio:public CSingleton<CAudio>
{
private:
	//�V���O���g��
	friend CSingleton<CAudio>;
	CAudio() {};	//�R���X�g���N�^
	~CAudio() {};	//�f�X�g���N�^
public:
	void Init();	//������
	void Update();	//���
	void Release();	//���

	//���y���̔j��
	void DeleteMusic();	

	//���y�̓ǂݍ���
	void Load(int Id ,const char* Name,bool Loop);
	
	//�}�X�^�[�{�����[���̒���
	void MasterVolume(float t);

	//�X�^�[�g
	void StartMusic(int Id);

	//�X�g�b�v
	void StopMusic(int Id);
	
private:	
	//Wave�ǂݍ���
	unsigned char* LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name);

	//char * ���@WORD�@�ɕϊ�
	WORD GetWord(unsigned char* pData);

	//char * ���@DWORD�@�ɕϊ�
	DWORD GetDWord(unsigned char* pData);

	//�`�����N��T���֐�
	ChunkInfo FindChunk(unsigned char* pData, char* pChunkName);

	IXAudio2* m_pXAudio2;						//XAudio2�I�u�W�F�N�g
	IXAudio2MasteringVoice* m_pMasteringVoice;	//�}�X�^�[�{�C�X
	IXAudio2SubmixVoice* m_pSubmixVoice;		//�T�u�~�N�X�C���^�[�t�F�[�X
	vector<SoundData> m_SoundData;				//�T�E���h���
};