//==============================
//
//  �Q�[�W����[Gauge.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
//
int CGauge::m_nIdxTexture = 0;

//==================
// �R���X�g���N�^
//==================
CGauge::CGauge(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_nIdxTexture = 0;
	m_Base = 0;
	m_fHeight = 0;
}
//================
// �f�X�g���N�^
//================
CGauge::~CGauge()
{
}
//===========
// ��������
//===========
CGauge* CGauge::Create(D3DXVECTOR3 pos, int base, float fHeight, D3DXCOLOR col)
{
	CGauge* pGause = new CGauge;

	//������
	if (FAILED(pGause->Init()))
	{
		delete pGause;
		return nullptr;
	}

	//�e�N�X�`�����蓖��
	std::string TexName = "data\\TEXTURE\\gauge00.jpeg";

	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// �p�X�ݒ�
	std::string TexPath = "data\\TEXTURE\\";

	// �p�X�A��
	TexPath += TexName;

	// �e�N�X�`�����蓖��
	m_nIdxTexture = pTexture->RegisterDynamic(TexPath.c_str());

	return pGause;
}
//=============
// ����������
//=============
HRESULT CGauge::Init(void)
{
	//�f�o�C�X�̎擾
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);
	Set();

	return S_OK;
}
//==========
// �I������
//==========
void CGauge::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//==========
// �X�V����
//==========
void CGauge::Update(void)
{

}
//===========
// �`�揈��
//===========
void CGauge::Draw(void)
{
	//�f�o�C�X�̎擾
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	
	// �e�N�X�`���߂�
	pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(m_nIdxTexture));

	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//
//
//
void CGauge::Set()
{
	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (GAUGE_X * m_Base), m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (GAUGE_X * m_Base), m_pos.y + m_fHeight, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}