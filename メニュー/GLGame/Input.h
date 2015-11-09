//
// �L�[/�Q�[���p�b�h/�}�E�X���̓N���X��`
//
#pragma once

#include <windows.h>
#include <tchar.h>

//-------- �L�[�{�[�h�֌W
#define MAX_KEY			256			// �L�[��

//-------- �W���C�X�e�B�b�N�֌W
#define JOY_X			0			// �w��
#define JOY_Y			1			// �x��

//-------- �}�E�X�֌W
#define MOUSE_X			0			// �w��
#define MOUSE_Y			1			// �x��
#define MOUSE_Z			2			// �y��

//-------- �Q�[���p�b�h�֌W
#define MAX_JOY			4			// �ő�F���ڑ���
#define JOY_MIN_X		-32768		// �w���ŏ��l
#define JOY_MAX_X		32767		// �w���ő�l
#define JOY_MIN_Y		-32768		// �x���ŏ��l
#define JOY_MAX_Y		32767		// �x���ő�l

typedef struct _MOUSESTATE {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    BYTE    rgbButtons[4];
} MOUSESTATE, *LPMOUSESTATE;

class CInput
{
private:
	static BYTE					m_diKeyState[MAX_KEY];		// �L�[�{�[�h���擾�o�b�t�@
	static BYTE					m_diPrevKeyState[MAX_KEY];	// ���O�̃L�[�{�[�h���
	static MOUSESTATE			m_diMouseState;				// �}�E�X���擾�o�b�t�@
	static MOUSESTATE			m_diPrevMouseState;			// ���O�̃}�E�X���
	static JOYINFOEX 			m_diJoyState[MAX_JOY];		// �W���C�X�e�B�b�N���擾�o�b�t�@
	static JOYINFOEX 			m_diPrevJoyState[MAX_JOY];	// ���O�̃W���C�X�e�B�b�N���
	static int					m_nJoy;						// �W���C�X�e�B�b�N�ڑ���
	static HWND					m_hWnd;

public:
	CInput(void);
	virtual ~CInput(void);

	static void Init(HWND hWnd);
	static void Fin(void);
	static void Update(void);

	static bool GetKeyPress(int nKey);
	static bool GetKeyTrigger(int nKey);
	static bool GetKeyRelease(int nKey);
	static int GetNumJoystick(void);
	static bool GetJoyPress(int nJoy, int nButton);
	static bool GetJoyTrigger(int nJoy, int nButton);
	static bool GetJoyRelease(int nJoy, int nButton);
	static float GetJoyAxis(int nJoy, int nAxis);
	static bool GetMousePress(int nButton);
	static bool GetMouseTrigger(int nButton);
	static bool GetMouseRelease(int nButton);
	static long GetMouseAxis(int nAxis);
	static void SetMouseAxis(int nAxis, long lValue);
};