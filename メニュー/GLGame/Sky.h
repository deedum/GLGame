#pragma once
#include "MeshObj.h"

class CSky : public CMeshObj
{
protected:
	MESH	m_mesh;

public:
	CSky(CScene* pScene);
	virtual ~CSky(void);

	virtual void Init(void);
	virtual void Fin(void);
	virtual void PreDraw(void) {}
	virtual void Draw(void) {}
	virtual void PostDraw(void) {}
	void RawDraw(void);
};