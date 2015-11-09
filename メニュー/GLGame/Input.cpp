//
// �L�[/�Q�[���p�b�h/�}�E�X���̓N���X
//
#include "Input.h"
#include "define.h"

#pragma comment(lib, "winmm")

// �ÓI�����o�ϐ�
BYTE					CInput::m_diKeyState[MAX_KEY] = {0};// �L�[�{�[�h���擾�o�b�t�@
BYTE					CInput::m_diPrevKeyState[MAX_KEY] = {0};// ���O�̃L�[�{�[�h���
MOUSESTATE				CInput::m_diMouseState = {0};			// �}�E�X���擾�o�b�t�@
MOUSESTATE				CInput::m_diPrevMouseState = {0};		// ���O�̃}�E�X���
JOYINFOEX 				CInput::m_diJoyState[MAX_JOY] = {0};	// �W���C�X�e�B�b�N���擾�o�b�t�@
JOYINFOEX 				CInput::m_diPrevJoyState[MAX_JOY] = {0};// ���O�̃W���C�X�e�B�b�N���
int						CInput::m_nJoy = 0;						// �W���C�X�e�B�b�N�ڑ���
HWND					CInput::m_hWnd = NULL;
TCHAR					g_szTitle[] = _T("CInput");

// �R���X�g���N�^
CInput::CInput(void)
{
}

// �f�X�g���N�^
CInput::~CInput(void)
{
}

// ������
void CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	// �W���C�X�e�B�b�N�ڑ����𒲂ׂ�
	m_nJoy = 0;
	UINT uID = JOYSTICKID1;
	JOYINFO ji;
	while (joyGetPos(uID, &ji) == JOYERR_NOERROR) {
		uID++;
		m_nJoy++;
		if (m_nJoy >= MAX_JOY) {
			break;
		}
	}
}

// ���
void CInput::Fin(void)
{
	// ����
}

// �X�V
void CInput::Update(void)
{
	int		i;
	USHORT	u;

	CopyMemory(m_diPrevKeyState, m_diKeyState, MAX_KEY);
	// ���݂̃L�[�{�[�h�̏�Ԃ��擾
	for (i = 0; i < MAX_KEY; i++) {
		u = (USHORT)GetAsyncKeyState(i);
		m_diKeyState[i] = (u & 0x8000) >> 8;
	}

	m_diPrevMouseState = m_diMouseState;
	// ���݂̃}�E�X�̏�Ԃ��擾
	u = (USHORT)GetAsyncKeyState(VK_LBUTTON);
	m_diMouseState.rgbButtons[0] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_RBUTTON);
	m_diMouseState.rgbButtons[1] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_MBUTTON);
	m_diMouseState.rgbButtons[2] = (u & 0x8000) >> 8;
	u = (USHORT)GetAsyncKeyState(VK_XBUTTON1);
	m_diMouseState.rgbButtons[3] = (u & 0x8000) >> 8;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	m_diMouseState.lX = pt.x;
	m_diMouseState.lY = pt.y;

	for (i = 0; i < MAX_JOY; i++) {
		m_diPrevJoyState[i] = m_diJoyState[i];
	}
	// ���݂̃W���C�X�e�B�b�N�̏�Ԃ��擾
	for (i = 0; i < MAX_JOY; i++) {
		if (joyGetPosEx(i, &m_diJoyState[i]) == JOYERR_NOERROR) {
			m_nJoy = i;
			break;
		}
	}
}

// �L�[�̉�����Ԃ��擾
bool CInput::GetKeyPress(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diKeyState[nKey] & 0x80) != 0;
}

// �L�[�̃g���K��Ԃ��擾
bool CInput::GetKeyTrigger(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) == 0 &&
		(m_diKeyState[nKey] & 0x80) != 0;
}

// �L�[�̃����|�X��Ԃ��擾
bool CInput::GetKeyRelease(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) != 0 &&
		(m_diKeyState[nKey] & 0x80) == 0;
}

// �W���C�X�e�B�b�N���擾
int CInput::GetNumJoystick(void)
{
	return m_nJoy;
}

// �W���C�X�e�B�b�N�̃{�^���̉�����Ԏ擾
bool CInput::GetJoyPress(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diJoyState[nJoy].dwButtons & (1 << nButton)) != 0;
}

// �W���C�X�e�B�b�N�̃{�^���̃g���K��Ԏ擾
bool CInput::GetJoyTrigger(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].dwButtons & (1 << nButton)) == 0 &&
		(m_diJoyState[nJoy].dwButtons & (1 << nButton)) != 0;
}

// �W���C�X�e�B�b�N�̃{�^���̃����[�X��Ԏ擾
bool CInput::GetJoyRelease(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].dwButtons & (1 << nButton)) != 0 &&
		(m_diJoyState[nJoy].dwButtons & (1 << nButton)) == 0;
}

// �W���C�X�e�B�b�N�̎���Ԏ擾
float CInput::GetJoyAxis(int nJoy, int nAxis)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return 0.0f;
	switch (nAxis) {
	case JOY_X:
		if (m_diJoyState[nJoy].dwXpos < 32768) {
			return ((int)m_diJoyState[nJoy].dwXpos - 32768) / (float)JOY_MIN_X;
		}
		return (m_diJoyState[nJoy].dwXpos - 32768) / (float)JOY_MAX_X;
	case JOY_Y:
		if (m_diJoyState[nJoy].dwYpos < 32768) {
			return ((int)m_diJoyState[nJoy].dwYpos - 32768) / (float)JOY_MIN_X;
		}
		return (m_diJoyState[nJoy].dwYpos - 32768) / (float)JOY_MAX_X;
	default:
		break;
	}
	return 0.0f;
}

// �}�E�X�̃{�^���̉�����Ԏ擾
bool CInput::GetMousePress(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// �}�E�X�̃{�^���̃g���K��Ԏ擾
bool CInput::GetMouseTrigger(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) == 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// �}�E�X�̃{�^���̃����[�X��Ԏ擾
bool CInput::GetMouseRelease(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) != 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) == 0;
}

// �}�E�X�̕ω��ʎ擾
long CInput::GetMouseAxis(int nAxis)
{
	switch (nAxis) {
	case MOUSE_X:
		return m_diMouseState.lX;
	case MOUSE_Y:
		return m_diMouseState.lY;
	case MOUSE_Z:
		return m_diMouseState.lZ;
	default:
		break;
	}
	return 0L;
}

// �}�E�X�̕ω��ʍX�V
void CInput::SetMouseAxis(int nAxis, long lValue)
{
	switch (nAxis) {
	case MOUSE_X:
		m_diMouseState.lX = lValue;
		break;
	case MOUSE_Y:
		m_diMouseState.lY = lValue;
		break;
	case MOUSE_Z:
		m_diMouseState.lZ = lValue;
		break;
	default:
		break;
	}
}
