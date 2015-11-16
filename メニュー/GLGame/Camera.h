#pragma once
class CCamera;

#include "C3DObj.h"
#include "Player.h"
#include "Sky.h"

enum ECameraType {
	CT_NONE = 0,
	CT_FIRSTPERSON,
	CT_FIXED,
	CT_THIRDPERSON,
	CT_VIEWPLAYER,

	MAX_CAMERATYPE
};

class CCameraInfo
{
public:
	VECTOR3		m_eye;		// 視点
	VECTOR3		m_at;		// 注視点
	VECTOR3		m_up;		// 上方ベクトル
};

class CCamera :	public C3DObj
{
protected:
	VECTOR3			m_at;		// 注視点
	VECTOR3			m_up;		// 上方向
	int				m_camtype;	// 1:1人称、2:固定、3:3人称
	CPlayer*		m_pPlayer;	// プレイヤー
	CSky*			m_pSky;		// スカイドーム
	CCameraInfo*	m_pCamInfo;	// カメラ情報

public:
	CCamera(CScene* pScene);
	virtual ~CCamera(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void PostUpdate2(void);
	void InitDraw(void);
	void LookAt(VECTOR3* pAt);
	void SetUp(VECTOR3* pUp);
	void SetPlayer(CPlayer* pPlayer) {m_pPlayer = pPlayer;}
	void SetSky(CSky* pSky) {m_pSky = pSky;}
	void SetType(int nType) {m_camtype = nType;}
};