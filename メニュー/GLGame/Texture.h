//=======================================================================================
//
//	  �e�N�X�`���֘A��`
//
//=======================================================================================
#ifndef _H_TEXTURE
#define _H_TEXTURE

#include <windows.h>		// Windows �֘A�̃w�b�_
#include <GL/gl.h>			// OpenGL �֘A�̃w�b�_
#include <GL/glu.h>			// OpenGL �֘A�̃w�b�_

GLuint LoadTexture(const char *pszFName);
void ReleaseTexture(GLuint uTexID);

//-------- ���C�u�����̃����N
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#endif	// _H_TEXTURE

//=======================================================================================
//	End of File
//=======================================================================================