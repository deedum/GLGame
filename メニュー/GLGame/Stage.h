#pragma once
#include "MeshObj.h"

class CStage :	public CMeshObj
{
private:
	MESH	m_mesh;

public:
	CStage(CScene* pScene);
	virtual ~CStage(void);

	virtual void Init(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	virtual void Fin(void);
};