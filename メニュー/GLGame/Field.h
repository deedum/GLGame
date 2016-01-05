#pragma once
#include "MeshObj.h"

class CField :	public CMeshObj
{
private:
	MESH	*m_mesh;

public:
	CField(CScene* pScene);
	virtual ~CField(void);

	virtual void Init(void);
	virtual void PostInit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	virtual void Fin(void);

};