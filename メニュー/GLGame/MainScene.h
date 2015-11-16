#pragma once
#include "Scene.h"
#include "Player.h"
#include "Sound.h"
#include "Camera.h"
#include "Sky.h"
#include "Billboard.h"
#include "Stage.h"

class CMainScene : public CScene
{
protected:
	//-------- ゲーム用オブジェクト
	CPlayer*	m_pPlayer;
	//CSound*		m_pSound;
	CStage*		m_pStage;
	CCamera*	m_pCamera;
	CSky*		m_pSky;

public:
	CMainScene(void);
	virtual ~CMainScene(void);

	virtual bool Initialize();
	virtual void Finalize();
	virtual void Update();
	virtual void Draw();
};