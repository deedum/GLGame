#pragma once
#include "MeshObj.h"

class CPlayer :	public CMeshObj
{
private:
	MESH	m_mesh;

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