#include "SplashScene.h"
#include "Texture.h"
//#include "Billboard.h"

/*
// ���_�t�H�[�}�b�g��`
struct FVERTEX {
	// GL_T2F_V3F
	GLfloat		tu, tv;			// �e�N�X�`�����W
	GLfloat		x, y, z;		// ���_���W
};
*/

// �R���X�g���N�^
CSplashScene::CSplashScene(void)
{
	m_pSound = NULL;
	m_pCamera = NULL;
}

// �f�X�g���N�^
CSplashScene::~CSplashScene(void)
{
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X������
//---------------------------------------------------------------------------------------
bool CSplashScene::Initialize()
{
	if (!CScene::Initialize()) return false;

	//-------- �Q�[���p�I�u�W�F�N�g�̏�����
	m_pSound = new CSound(this);
	m_pCamera = new CCamera(this);

	//CSound::Play(SE_SPLASH);
	m_dwCount = 0;
	m_uTexID = LoadTexture("../data/texture/logo.tga");

	return true;
}

//---------------------------------------------------------------------------------------
// �f�o�C�X�A���\�[�X���
//---------------------------------------------------------------------------------------
void CSplashScene::Finalize()
{
	//-------- �Q�[���p�I�u�W�F�N�g�̌�n��
	ReleaseTexture(m_uTexID);
	m_uTexID = 0;

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSound);

	CScene::Finalize();
}

//---------------------------------------------------------------------------------------
// �Q�[�����C�������i���C�����[�v����R�[�������j
//---------------------------------------------------------------------------------------
void CSplashScene::Update()
{
	CScene::Update();

	//----- �����ɃQ�[���{�̏���
	++m_dwCount;
	if (m_dwCount >= 383) {
		CGameWindow::Get()->ChangeScene(1);
		return;
	}

	m_pCamera->PostUpdate2();	// �J�����X�V
}

//---------------------------------------------------------------------------------------
// �`�揈���iCGraphics::Render() ����R�[�������j
//---------------------------------------------------------------------------------------
void CSplashScene::Draw()
{
	CScene::Draw();

	//----- �����ɕ`�揈��
	m_pCamera->InitDraw();	// �J�����ݒ�

	// �f�o�b�O������`��
	m_pGraph->DrawText(0, 0, m_szDebug);

	// �X�v���b�V����ʕ`��
	float f, a;
	if (m_dwCount < 64) {
		a = 1.0f;
		f = 0.0f;
	} if (m_dwCount < 128) {
		f = 1.0f;
		a = 1.0f;
	} else {
		f = 1.0f;
		a = 1.0f - (m_dwCount - 128) * 2 / 127.0f;
	}
	glColor4f(f, f, f, a);

	FVERTEX	vtxQuad[4];

	vtxQuad[0].tu = 0.0f;
	vtxQuad[0].tv = 0.0f;
	vtxQuad[0].x  = (SCREEN_WIDTH / 2) - 300.0f;
	vtxQuad[0].y  = (SCREEN_HEIGHT / 2) + 170.0f;
	vtxQuad[0].z  = 0.0f;
	vtxQuad[1].tu = 1.0f;
	vtxQuad[1].tv = 0.0f;
	vtxQuad[1].x  = (SCREEN_WIDTH / 2) + 300.0f;
	vtxQuad[1].y  = (SCREEN_HEIGHT / 2) + 170.0f;
	vtxQuad[1].z  = 0.0f;
	vtxQuad[2].tu = 1.0f;
	vtxQuad[2].tv = 1.0f;
	vtxQuad[2].x  = (SCREEN_WIDTH / 2) + 300.0f;
	vtxQuad[2].y  = (SCREEN_HEIGHT / 2) - 170.0f;
	vtxQuad[2].z  = 0.0f;
	vtxQuad[3].tu = 0.0f;
	vtxQuad[3].tv = 1.0f;
	vtxQuad[3].x  = (SCREEN_WIDTH / 2) - 300.0f;
	vtxQuad[3].y  = (SCREEN_HEIGHT / 2) - 170.0f;
	vtxQuad[3].z  = 0.0f;

	glEnable(GL_TEXTURE_2D);	// �e�N�X�`���L����
	glBindTexture(GL_TEXTURE_2D, m_uTexID);
	glInterleavedArrays(GL_T2F_V3F, 0, vtxQuad);
	glDrawArrays(GL_QUADS, 0, 4);
}