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
	VECTOR3		m_eye;		// ���_
	VECTOR3		m_at;		// �����_
	VECTOR3		m_up;		// ����x�N�g��
};

class CCamera :	public C3DObj
{
protected:
	VECTOR3			m_at;		// �����_
	VECTOR3			m_up;		// �����
	int				m_camtype;	// 1:1�l�́A2:�Œ�A3:3�l��
	CPlayer*		m_pPlayer;	// �v���C���[
	CSky*			m_pSky;		// �X�J�C�h�[��
	CCameraInfo*	m_pCamInfo;	// �J�������

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