#include "Audio.h"
#include "Release.h"

//�C���X�^���X
CAudio* CAudio::m_pInstance = nullptr;

//������
void CAudio::Init()
{
	m_pXAudio2 = nullptr;
	m_pMasteringVoice = nullptr;
	m_pSubmixVoice = nullptr;

	//XAudio�̍쐬
	XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�̍쐬
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);

	if (m_pMasteringVoice != nullptr)
	{
		//�~�b�N�X�{�C�X�쐬
		m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
	}

	//�}�X�^�[�{�����[���ݒ�
	MasterVolume(1.0f);
}

//���y�̓ǂݍ���
void CAudio::Load(int Id,const char* Name,bool Loop)
{
	WAVEFORMATEX WaveFormatEx;
	
	//Wave����ǂݍ���
	ChunkInfo Chunk;
	unsigned char* pResourceData = LoadWave(&Chunk, &WaveFormatEx, Name);

	if (m_pMasteringVoice != nullptr)
	{
		//�Đ��̂��߂̃C���^�[�t�F�[�X�̍쐬
		IXAudio2SourceVoice* pSourceVoice;
		XAUDIO2_SEND_DESCRIPTOR Send = { 0,m_pSubmixVoice };
		XAUDIO2_VOICE_SENDS SendList = { 1,&Send };

		m_pXAudio2->CreateSourceVoice(&pSourceVoice, &WaveFormatEx, 0U, 2.0f, NULL, &SendList);
	
		//�T�E���h����o�^
		SoundData data(Chunk, pResourceData, pSourceVoice, Loop);
		m_SoundData.insert(m_SoundData.begin() + Id, data);
	}
}

//�Đ�
void CAudio::StartMusic(int Id)
{
	if(m_pMasteringVoice!=nullptr)
	{
		//�T�E���h�o�b�t�@���\�[�X�{�C�X�ɑ��M
		XAUDIO2_BUFFER SoundBuffer={ 0 };
		SoundBuffer.AudioBytes = m_SoundData[Id].m_Chunk.Size;
		SoundBuffer.pAudioData = (BYTE*)m_SoundData[Id].m_Chunk.pData;
		SoundBuffer.Flags = XAUDIO2_END_OF_STREAM;
		SoundBuffer.LoopCount = 0;
	
		//�T�E���h�o�b�t�@�Z�b�g
		m_SoundData[Id].m_pSourceVoice->SubmitSourceBuffer(&SoundBuffer);

		//�T�E���h�X�^�[�g
		m_SoundData[Id].m_pSourceVoice->Start();
		m_SoundData[Id].m_bStart = true;
	}
}

//�X�V
void CAudio::Update()
{
	 XAUDIO2_VOICE_STATE state;
	 for (unsigned int i = 0; i < m_SoundData.size(); i++)
	 {
		 //�Đ������ǂ������`�F�b�N����
		 m_SoundData[i].m_pSourceVoice->GetState(&state);

		 //�Đ����I�������
		 if (state.BuffersQueued == 0)
		 {
			 StopMusic(i);	//��~

			 //���[�v�ōĐ�����Ă����������x�Đ�
			 if (m_SoundData[i].m_bStart == true && m_SoundData[i].m_bLoop == true)
			 {  
				 StartMusic(i);	
			 }
		 } 
	 }
}

//��~
void CAudio::StopMusic(int Id)
{
	m_SoundData[Id].m_pSourceVoice->Stop();					//���y���~�߂�
	m_SoundData[Id].m_pSourceVoice->FlushSourceBuffers();	//�T�E���h�{�C�X�ɕۗ����̃o�b�t�@��j��
	m_SoundData[Id].m_bStart = false;
}

//Wave�ǂݍ���
unsigned char* CAudio::LoadWave(ChunkInfo* pChunkInfo, WAVEFORMATEX* pWave, const char* Name)
{
	unsigned char* WaveData;

	//Wave�t�@�C���̃I�[�v��
	FILE* fp;
	fopen_s(&fp, Name, "rb");

	//�t�@�C���T�C�Y�擾
	unsigned int Size = 0;
	fseek(fp, 0, SEEK_END);
	Size = ftell(fp);
	WaveData = new unsigned char[Size];

	//�t�@�C���f�[�^���������Ɉڂ�
	fseek(fp, 0, SEEK_SET);
	fread(WaveData, Size, 1, fp);
	fclose(fp);

	//RIFF�t�@�C���̉��
	//RIFF�f�[�^�̐擪�A�h���X��RIFF�f�[�^�̃T�C�Y��n��
	ChunkInfo WaveChunk = FindChunk(WaveData, (char*)"fmt");
	unsigned char*p = WaveChunk.pData;

	//Wave���̎擾
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

	//�g�`�f�[�^�̐擪�A�h���X�Ɣg�`�f�[�^�̃T�C�Y��n��
	ChunkInfo info= FindChunk(WaveData, (char*)"data");
	pChunkInfo->Size = info.Size;
	pChunkInfo->pData = info.pData;
	
	return WaveData;
}

//�}�X�^�[�{�����[���̒���
void CAudio::MasterVolume(float t)
{
	if(m_pMasteringVoice!=nullptr)
		m_pMasteringVoice->SetVolume(t, 0);
}

//char * ���@WORD�@�ɕϊ�
WORD CAudio::GetWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8;
	return value;
}

//char * ���@DWORD�@�ɕϊ�
DWORD CAudio::GetDWord(unsigned char* pData)
{
	WORD value = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
	return value;
}

//�`�����N��T���֐�
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

//���y���̔j��
void CAudio::DeleteMusic()
{
	//�\�[�X�{�C�X�j��
	for (unsigned int i = 0; i < m_SoundData.size(); i++)
	{
		if (m_SoundData[i].m_pSourceVoice != nullptr)
		{
			m_SoundData[i].m_pSourceVoice->Stop();
			m_SoundData[i].m_pSourceVoice->FlushSourceBuffers();
			m_SoundData[i].m_pSourceVoice->DestroyVoice();
		}
	}
	//�T�E���h���j��
	VectorRelease(m_SoundData);
}

//���
void CAudio::Release()
{	
	//�~�b�N�X�T�E���h�̔j��
	if (m_pSubmixVoice != nullptr)
	{
		m_pSubmixVoice->DestroyVoice();
	}

	//�}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice != nullptr)
	{
		m_pMasteringVoice->DestroyVoice();
	}

	//XAudio2�I�u�W�F�N�g�j��
	m_pXAudio2->Release();

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}