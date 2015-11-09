#pragma once

#include <windows.h>
#include "audiere.h"

#include "C3DObj.h"

enum eBGM {
	BGM_MARIO = 0,	// ���C���e�[�}
	BGM_BATTLE,		// �퓬�V�[��
	BGM_TITLE,		// �^�C�g��

	MAX_BGM
};

enum eSE {
	SE_JUMP = 0,	// �W�����v
	SE_COIN,		// �A�C�e���擾
	SE_FIRE,		// ����
	SE_GAMEOVER,	// �Q�[���I�[�o�[
	SE_EXP_S,		// ������(��)
	SE_EXP_L,		// ������(��)

	MAX_SE
};

class CSound : public C3DObj
{
private:
	static audiere::AudioDevicePtr	m_device;
	static audiere::OutputStreamPtr	m_bgm[MAX_BGM];
	static audiere::SoundEffectPtr	m_se[MAX_SE];

public:
	CSound(CScene* pScene);
	virtual ~CSound(void);

	virtual void Init(void);
	static void Play(eBGM bgm);
	static void Play(eSE se);
	static bool IsPlaying(eBGM bgm);
	static void Stop(eBGM bgm);
	static void Stop(eSE se);
};