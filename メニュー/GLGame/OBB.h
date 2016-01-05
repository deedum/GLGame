#pragma once

#include "Matrix.h"

class COBB
{
private:
	VECTOR3 m_vCenter;
	VECTOR3 m_vRadius;

public:
	COBB(void);
	virtual ~COBB(void);
	
	VECTOR3& GetCenter(void) {return m_vCenter;}
	VECTOR3& GetRadius(void) {return m_vRadius;}
	void SetCenter(VECTOR3 vCenter) {m_vCenter = vCenter;}
	void SetRadius(VECTOR3 vRadius) {m_vRadius = vRadius;}

	bool IntersectOBB(COBB obb1, MATRIX world1,COBB obb2, MATRIX world2);

	void Draw(MATRIX *matWorld);
};

