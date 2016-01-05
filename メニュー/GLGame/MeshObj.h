#pragma once
#include "C3DObj.h"
#include "Mesh.h"

#include "OBB.h"

class CMeshObj : public C3DObj
{
protected:
	MESH*	m_pMesh;

	COBB	m_OBB;
	float	m_fRadius;

public:
	CMeshObj(CScene* pScene);
	virtual ~CMeshObj(void);
	virtual void PreUpdate(void);
	virtual void PostUpdate(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	void SetMesh(MESH* pMesh);
	
	float GetRadius(void) {return m_fRadius;}
	MESH* GetMesh(void) {return m_pMesh;}
	COBB& GetOBB(void) {return m_OBB;}
	bool IntersectOBB(COBB obb, MATRIX world);
	bool IntersectAABB(COBB obb, MATRIX world, int type);
};