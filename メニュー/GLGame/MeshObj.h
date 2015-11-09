#pragma once
#include "C3DObj.h"
#include "Mesh.h"

class CMeshObj : public C3DObj
{
protected:
	MESH*	m_pMesh;

	VECTOR3	m_vBBox;
	VECTOR3	m_vCenter;
	float	m_fRadius;

public:
	CMeshObj(CScene* pScene);
	virtual ~CMeshObj(void);
	virtual void PreUpdate(void);
	virtual void PostUpdate(void);
	virtual void Draw(void);
	virtual void PostDraw(void);
	void SetMesh(MESH* pMesh);
	VECTOR3& GetCenter(void) {return m_vCenter;}
	float GetRadius(void) {return m_fRadius;}
	MESH* GetMesh(void) {return m_pMesh;}
	VECTOR3& GetBBox(void) {return m_vBBox;}
};