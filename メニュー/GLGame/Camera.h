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
	VECTOR3			m_newPos;
	VECTOR3			m_at;		// 注視点
	VECTOR3			m_up;		// 上方向
static	int			m_camtype;	// 1:東　2:北　3:西　4:南
	CSky*			m_pSky;		// スカイドーム
	CCameraInfo*	m_pCamInfo;	// カメラ情報
	bool			m_bMove;	// 移動中

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
	void SetSky(CSky* pSky) {m_pSky = pSky;}
	void SetType(int nType) {m_camtype = nType;}
	static int GetCameraType(){return m_camtype;}
};