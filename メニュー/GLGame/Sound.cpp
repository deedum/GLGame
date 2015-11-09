#include "Sound.h"

#pragma comment(lib, "audiere")

using namespace audiere;

// �O���[�o���ϐ�
static const char* g_pszPathBGM[MAX_BGM] = {
	"../data/sound/bgm/S.M.Bros.xm",
	"../data/sound/bgm/battle01.mp3",
	"../data/sound/bgm/theme05.mp3",
};
static const char* g_pszPathSE[MAX_SE] = {
	"../data/sound/se/se_maoudamashii_system31.ogg",
	"../data/sound/se/se_maoudamashii_system46.ogg",
	"../data/sound/se/bomb0.wav",
	"../data/sound/se/jingle03.mp3",
	"../data/sound/se/sexplode.wav",
	"../data/sound/se/lexplode.wav",
};

// �ÓI�����o
AudioDevicePtr	CSound::m_device = NULL;
OutputStreamPtr	CSound::m_bgm[MAX_BGM];
SoundEffectPtr	CSound::m_se[MAX_SE];

// �R���X�g���N�^
CSound::CSound(CScene* pScene) : C3DObj(pScene)
{
}

// �f�X�g���N�^
CSound::~CSound(void)
{
}

// ������
void CSound::Init(void)
{
	if (!m_device) {
		m_device = OpenDevice();
	}
	if (m_device) {
		for (int i = 0; i < MAX_BGM; ++i) {
			m_bgm[i] = OpenSound(m_device.get(), g_pszPathBGM[i]);
			if (m_bgm[i]) {
				m_bgm[i]->setRepeat(true);
			}
		}
		for (int i = 0; i < MAX_SE; ++i) {
			m_se[i] = OpenSoundEffect(m_device.get(), g_pszPathSE[i], MULTIPLE);
		}
	}
}

// BGM�Đ�
void CSound::Play(eBGM bgm)
{
	if (bgm < 0 || bgm >= MAX_BGM) return;
	if (m_bgm[bgm]) {
		m_bgm[bgm]->play();
	}
}

// SE�Đ�
void CSound::Play(eSE se)
{
	if (se < 0 || se >= MAX_SE) return;
	if (m_se[se]) {
		m_se[se]->play();
	}
}

// BGM�Đ����H
bool CSound::IsPlaying(eBGM bgm)
{
	if (bgm < 0 || bgm >= MAX_BGM) return false;
	if (m_bgm[bgm]) {
		return m_bgm[bgm]->isPlaying();
	}
	return false;
}

// BGM�Đ���~
void CSound::Stop(eBGM bgm)
{
	if (bgm < 0 || bgm >= MAX_BGM) return;
	if (m_bgm[bgm]) {
		m_bgm[bgm]->stop();
	}
}

// SE�Đ���~
void CSound::Stop(eSE se)
{
	if (se < 0 || se >= MAX_SE) return;
	if (m_se[se]) {
		m_se[se]->stop();
	}
}