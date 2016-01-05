#pragma once
#include "MeshObj.h"

class CBlock :	public CMeshObj
{
private:
	MESH	*m_mesh;

public:
	CBlock(CScene* pScene);
	virtual ~CBlock(void);

	virtual void Init(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	virtual void Fin(void);

};