//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "player.h"
#include "texture.h"
#include "model.h"
#include "game.h"

// ���O���std�̎g�p
using namespace std;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_pos				= VECTOR3_NULL;					// �ʒu
	m_colliderPos		= VECTOR3_NULL;					// �J�v�Z�����S
	m_rot				= VECTOR3_NULL;					// ����
	m_rotDest			= VECTOR3_NULL;					// ����
	m_move				= VECTOR3_NULL;					// �ړ���
	m_targetMove		= VECTOR3_NULL;					// �ڕW���x
	m_currentMove		= VECTOR3_NULL;					// ���ۂ̈ړ����x
	m_size				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);// �T�C�Y
	m_mtxWorld			= {};							// ���[���h�}�g���b�N�X
	m_nNumModel			= 0;							// ���f��(�p�[�c)�̑���
	m_pShadowS			= nullptr;						// �X�e���V���V���h�E�ւ̃|�C���^
	m_pMotion			= nullptr;						// ���[�V�����ւ̃|�C���^
	m_currentMotion		= PLAYER_MOTION::NEUTRAL;		// ���݂̃��[�V����
	m_pCollider			= nullptr;						// �J�v�Z���R���C�_�[
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = {};							// ���f��(�p�[�c)�ւ̃|�C���^
	}
	m_pHpGauge = nullptr;								// �g�o�Q�[�W�ւ̃|�C���^
	m_nLife = 0;										// ���C�t
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
	// �Ȃ�
}
//=============================================================================
// ��������
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = new CPlayer;

	pPlayer->m_pos = pos;
	pPlayer->m_rot = D3DXToRadian(rot);

	// ����������
	pPlayer->Init();

	return pPlayer;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	CModel* pModels[MAX_PARTS];
	int nNumModels = 0;

	// �p�[�c�̓ǂݍ���
	m_pMotion = CMotion::Load("data/motion.txt", pModels, nNumModels, MAX);

	for (int nCnt = 0; nCnt < nNumModels && nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = pModels[nCnt];

		// �I�t�Z�b�g�l��
		m_apModel[nCnt]->SetOffsetPos(m_apModel[nCnt]->GetPos());
		m_apModel[nCnt]->SetOffsetRot(m_apModel[nCnt]->GetRot());
	}

	// �p�[�c������
	m_nNumModel = nNumModels;

	// �ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	m_rot = D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f);

	// �X�e���V���V���h�E�̐���
	m_pShadowS = CShadowS::Create("data/MODELS/stencilshadow.x",m_pos);

	// �C���X�^���X�̃|�C���^��n��
	m_stateMachine.Start(this);

	// ������Ԃ̃X�e�[�g���Z�b�g
	m_stateMachine.ChangeState<CPlayer_StandState>();

	// �J�v�Z���R���C�_�[�̐���
	m_pCollider = new CCapsuleCollider(18.5f, 70.5f);

	// �J�v�Z���̒��S = ���� + �I�t�Z�b�g
	m_colliderPos = m_pos + D3DXVECTOR3(0, m_pCollider->GetHeight() * 0.5f, 0);

	// ���C�t
	m_nLife = PLAYER_LIFE;
	// �g�o�Q�[�W����
	m_pHpGauge = CHpGauge::Create(D3DXVECTOR3(50.0f, 40.0f, 0.0f), PLAYER_LIFE, GAUGE_Y, D3DCOLOR_RGBA(1, 255, 1, 255));

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �R���C�_�[�̔j��
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}

	// ���f���̔j��
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	// ���[�V�����̔j��
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// �I�u�W�F�N�g�̔j��(�������g)
	this->Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�����̊p�x�̎擾
	D3DXVECTOR3 CamRot = pCamera->GetRot();

	// �J�������[�h�̎擾
	CCamera::MODE camMode = pCamera->GetMode();

	//// ���͔���̎擾
	//InputData input = GatherInput();

	// �X�e�[�g�}�V���X�V
	m_stateMachine.Update();

	m_colliderPos = m_pos + D3DXVECTOR3(0, 50.0f, 0);

	if (m_pCollider)
	{
		// �R���C�_�[�̍X�V
		m_pCollider->UpdateTransform(m_colliderPos, VECTOR3_NULL, VECTOR3_NULL);
	}

	//D3DXVECTOR3 targetMove = input.moveDir * PLAYER_SPEED;

	//// ���X�� m_currentMove �� targetMove �ɋ߂Â���
	//float accel = 0.08f; // �����x�W��
	//m_currentMove += (targetMove - m_currentMove) * accel;

	//// �ړ��ʂ𔽉f
	//m_pos += m_currentMove;

	// �����̐��K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	m_rot.y += (m_rotDest.y - m_rot.y) * 0.09f;

	//// �ړ����͂�����΃v���C���[��������͕�����
	//if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	//{
	//	m_rotDest.y = atan2f(-input.moveDir.x, -input.moveDir.z);
	//}

	if (m_pShadowS != nullptr)
	{
		// �X�e���V���V���h�E�̈ʒu�ݒ�
		m_pShadowS->SetPosition(m_pos);
	}

	int nNumModels = 1;

	// ���[�V�����̍X�V����
	m_pMotion->Update(m_apModel, nNumModels);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < m_nNumModel; nCntMat++)
	{
		// ���f��(�p�[�c)�̕`��
		if (m_apModel[nCntMat])
		{
			m_apModel[nCntMat]->Draw();
		}
	}
}
//=============================================================================
// ���͔���擾�֐�
//=============================================================================
InputData CPlayer::GatherInput(void)
{
	InputData input{};
	input.moveDir = D3DXVECTOR3(0, 0, 0);
	input.jump = false;

	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad* pJoypad = CManager::GetJoyPad();				// �W���C�p�b�h�̎擾
	XINPUT_STATE* pStick = CInputJoypad::GetStickAngle();		// �X�e�B�b�N�̎擾
	CCamera* pCamera = CManager::GetCamera();					// �J�����̎擾
	D3DXVECTOR3 CamRot = pCamera->GetRot();						// �J�����p�x�̎擾
	CCamera::MODE camMode = pCamera->GetMode();					// �J�������[�h�̎擾

	//// ---------------------------
	//// �e����
	//// ---------------------------
	//if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
	//{
	//	CBullet::Create(m_pos, m_rot, CBullet::USER_PLAYER);
	//}

	// ---------------------------
	// �Q�[���p�b�h����
	// ---------------------------
	if (pJoypad->GetStick() && pStick)
	{
		float stickX = pStick->Gamepad.sThumbLX;
		float stickY = pStick->Gamepad.sThumbLY;
		float magnitude = sqrtf(stickX * stickX + stickY * stickY);
		const float DEADZONE = 10922.0f;

		if (magnitude >= DEADZONE)
		{
			stickX /= magnitude;
			stickY /= magnitude;
			float normMag = min((magnitude - DEADZONE) / (32767.0f - DEADZONE), 1.0f);
			stickX *= normMag;
			stickY *= normMag;

			D3DXVECTOR3 dir;
			float yaw = CamRot.y;

			dir.x = -(stickX * cosf(yaw) + stickY * sinf(yaw));
			dir.z = stickX * sinf(-yaw) + stickY * cosf(yaw);
			dir.z = -dir.z;

			input.moveDir += D3DXVECTOR3(dir.x, 0, dir.z);
		}
	}

	// ---------------------------
	// �L�[�{�[�h����
	// ---------------------------
	if (pKeyboard->GetPress(DIK_W))
	{
		input.moveDir += D3DXVECTOR3(-sinf(CamRot.y), 0, -cosf(CamRot.y));
	}
	if (pKeyboard->GetPress(DIK_S))
	{
		input.moveDir += D3DXVECTOR3(sinf(CamRot.y), 0, cosf(CamRot.y));
	}
	if (pKeyboard->GetPress(DIK_A))
	{
		input.moveDir += D3DXVECTOR3(cosf(CamRot.y), 0, -sinf(CamRot.y));
	}
	if (pKeyboard->GetPress(DIK_D))
	{
		input.moveDir += D3DXVECTOR3(-cosf(CamRot.y), 0, sinf(CamRot.y));
	}

	// ���K��
	if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	{
		D3DXVec3Normalize(&input.moveDir, &input.moveDir);
	}

	return input;
}
//=============================================================================
// ���[�V�����̐ݒ菈��
//=============================================================================
void CPlayer::SetMotion(int type, int nBlendFrame)
{
	m_pMotion->StartBlendMotion(type, nBlendFrame);
	m_currentMotion = type;
}