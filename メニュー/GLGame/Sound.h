#pragma once

#include <windows.h>
#include "audiere.h"

#include "C3DObj.h"

enum eBGM {
	BGM_MARIO = 0,	// メインテーマ
	BGM_BATTLE,		// 戦闘シーン
	BGM_TITLE,		// タイトル

	MAX_BGM
};

enum eSE {
	SE_JUMP = 0,	// ジャンプ
	SE_COIN,		// アイテム取得
	SE_FIRE,		// 発射
	SE_GAMEOVER,	// ゲームオーバー
	SE_EXP_S,		// 爆発音(小)
	SE_EXP_L,		// 爆発音(大)

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