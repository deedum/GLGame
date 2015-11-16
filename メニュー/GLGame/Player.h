#pragma once
class CPlayer;

#include "C3DObj.h"
#include "Billboard.h"

class CPlayer :	public C3DObj
{
private:
	GLuint		m_tex;
	VECTOR2		m_size;
	CBillboard*	m_billboard;

public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer(void);

	virtual void Init(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	virtual void Fin(void);
};