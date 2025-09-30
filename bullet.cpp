//==============================
//
// 弾[bullet.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CBullet::CBullet(int nPriority) : CObjectX(nPriority)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_pModel = NULL;
	m_type = TYPE_NONE;
	m_user = USER_PLAYER;
	m_nLife = 0;
	m_bUse = true;
	m_bSkill = false;
}
//================
// デストラクタ
//================
CBullet::~CBullet()
{

}
//===========
// 生成処理
//===========
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user)
{
	// 生成
	CBullet* pBullet = new CBullet();
	// 初期化
	if (FAILED(pBullet->Init(pos, rot, user)))
	{// NULLチェック
		delete pBullet;
		return nullptr;
	}
	return pBullet;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user)
{
	// 値を代入
	m_pos = pos;
	m_rot = rot;
	m_user = user;

	// メンバ変数を初期化
	m_bUse = true;		// 使用している状態にする
	m_bSkill = false;	// スキルではない状態にする

	// モデル名格納用変数＆その初期化
	const char* pFilename = "data\\MODELS\\can00.x";
	
	// 移動量を設定
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * BULLET_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * BULLET_SPEED);
	m_move.y = 10.0f;
	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{
		m_nLife = BULLET_LIFE;			// 寿命を設定
	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{
		m_nLife = BULLET_LIFE / 7;	// 寿命を設定
	}

	// モデル生成
	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	//サイズを設定


	// オブジェクトの種類設定
	SetObjType(TYPE_BULLET);
	
	return S_OK;
}
//============
// 終了処理
//============
void CBullet::Uninit(void)
{	
	if (m_pModel != NULL)
	{// NULLチェック
		// モデルの終了処理＆破棄
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============
// 更新処理
//============
void CBullet::Update(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			// バウンド移動
			MoveBounce();
		}
		// ユーザーが敵なら
		else if(m_user == USER_ENEMY)
		{
			// 移動
			Move();
		}
		// 当たり判定
		Collision();

		// 寿命を減らす
		m_nLife--;
		// 寿命が０以下なら
		if (m_nLife <= 0)
		{
			m_bUse = false;	// 使用していない状態に
		}
	}
	// 使っていないなら
	else if (m_bUse == false)
	{
		// 終了処理
		Uninit();
		return;
	}
}
//============
// 描画処理
//============
void CBullet::Draw(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxTrans;
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			// モデル描画
			m_pModel->Draw();
		}
	}
}
//===========
// 移動処理
//===========
void CBullet::Move(void)
{
	// 位置更新
	m_pos += m_move;

	// モデルの位置更新
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//==================
// バウンド移動
//==================
void CBullet::MoveBounce(void)
{
	const float fRestitution = 0.8f;	// 反発係数
	const float fMinY = 0.2f;			// Ｙ軸の最低移動量

	// 重力加算
	m_move.y -= 0.7f;

	// 位置更新
	m_pos += m_move;

	// 地面との衝突
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;	// 位置を地面と同じにする

		// Y速度の反転
		m_move.y *= -fRestitution;

		// 移動量が一定量小さくなったら
		if (fabs(m_move.y) < fMinY)
		{
			m_move.y = 0.0f;	// Y軸の移動量を０に
		}
	}

	// モデルの位置更新
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//====================
// 敵との当たり判定
//====================
bool CBullet::Collision(void)
{
	D3DXVECTOR3 contactNormal;	// 接触法線

	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{

	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{

	}


	return false;	 // 判定を返す
}
