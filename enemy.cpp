//==============================
//
// 敵[enemy.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include "score.h"

int CEnemy::m_nCntKill = 0;
//==================
// コンストラクタ
//==================
CEnemy::CEnemy(int nPriority) :CObjectX(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_type = TYPE_NONE;

	m_bUse = true;
	
	m_State = STATE_NONE;
	m_nCntState = 0;
}
//================
// デストラクタ
//================
CEnemy::~CEnemy()
{

}
//===========
// 生成処理
//===========
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CEnemy* pBullet = new CEnemy;
	if (!pBullet)
		return nullptr;

	//
	pBullet->m_pos = pos;
	pBullet->m_rot = rot;
	pBullet->m_type = type;

	if (FAILED(pBullet->Init()))
	{
		delete pBullet;
		return nullptr;
	}

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CEnemy::Init(void)
{
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * 1.5f, 0.0f, cosf(m_rot.y - D3DX_PI) * 1.5f);
	m_nLife = ENEMY_LIFE;
	m_bUse = true;
	m_State = STATE_STAY;
	m_nStayCounter = rand() % 120 + 60;
	m_fRandomAngle = D3DXToRadian(rand() % 360);

	//モデル生成
	const char* pFilename = "data/MODELS/pot00.x";

	m_pModel = CModel::Create(pFilename, m_pos, m_rot);

	m_posHalf = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z);

	float GeuseBase = m_nLife / 10.0f;
	//m_pGauge = CEnemyGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 1.5f), m_pos.z), GeuseBase, 5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	//オブジェクトの種類設定
	SetObjType(TYPE_ENEMY);

	return S_OK;
}
//============
// 終了処理
//============
void CEnemy::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//if (m_pGauge != NULL)
	//{
	//	m_pGauge->Uninit();
	//}

	CObject::Release();
}
//============
// 更新処理
//============
void CEnemy::Update(void)
{
	if (m_bUse == true)
	{
		float rate = (float)m_nLife / (float)ENEMY_LIFE;
		rate = max(0.0f, min(rate, 1.0f));

		//m_pGauge->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y * 1.5f), m_pos.z));
		//m_pGauge->SetRate(rate);

		switch (m_State)
		{
		case STATE_STAY:
			//移動
			Move();
			break;
		case STATE_MOVE:
			//移動
			Move();
			break;
		case STATE_HIT:

			break;

		default:
			break;
		}

		//if (m_State != STATE_HIT)
		//{
		//}
		//if (m_State == STATE_STAY)
		//{
		//	m_pGauge->SetDraw(false);
		//}
		//else
		//{
		//	m_pGauge->SetDraw(true);
		//}

		//当たり判定
		Collision();

		//寿命
		if (m_nLife <= 0)
		{
			m_bUse = false;
			//m_pGauge->SetDraw(false);
		}
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
	}
}
//============
// 描画処理
//============
void CEnemy::Draw(void)
{
	if (m_bUse == true)
	{
		//デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//モデル描画
		m_pModel->Draw();
	}
}
//===========
// 移動処理
//===========
void CEnemy::Move(void)
{
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

	// プレイヤーに向かって動く
	Move_Normal();	// 通常

	// 角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 向きを変える
	m_rot += (m_rotDest - m_rot) * 0.5f;

	// 前回の位置保存
	m_posOld = m_pos;
	// 移動
	m_pos += m_move;

	// 地面判定
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//================
// 通常移動処理
//================
void CEnemy::Move_Normal(void)
{
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toPlayer = PlayerPos - m_pos;
	// 正規化
	D3DXVec3Normalize(&toPlayer, &toPlayer);

	//移動量
	m_move.x = toPlayer.x * ENEMY_SPEED;
	m_move.z = toPlayer.z * ENEMY_SPEED;
	//角度
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotDest.y = angle + D3DX_PI;
}
//================
// ダメージ処理
//================
void CEnemy::Hit(const int nDamage)
{
	m_nLife -= nDamage;

	// パーティクル生成
	CParticle::Create<CWaterParticle>(VECTOR3_NULL, GetPos(), D3DXCOLOR(0.3f, 0.6f, 1.0f, 0.8f), 50, 10);
	CParticle::Create<CWaterParticle>(VECTOR3_NULL, GetPos(), D3DXCOLOR(0.3f, 0.5f, 1.0f, 0.5f), 50, 10);
	CScore::AddScore(31000);
}
//=================
// 状態遷移処理
//=================
void CEnemy::State(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NONE:

		break;
	case STATE_HIT:
		m_nCntState = 45;
		break;
	case STATE_DEAD:
		ItemSet();
		m_bUse = false;
		break;

	default:
		break;
	}
}
//===================
// アイテムセット
//===================
void CEnemy::ItemSet()
{
}
//====================
// 敵との当たり判定
//====================
bool CEnemy::Collision(void)
{
	bool bColl = false;

	for (int nCnt = 0; nCnt < CObject::PRIORITY_MAX; nCnt++)
	{
		CObject* pObj = CObject::GetTop(nCnt);
		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();
			CObject::TYPE type = pObj->GetObjType();//タイプ取得
			switch (type)
			{
			case CObject::TYPE_PLAYER:
				//オブジェクトがプレイヤーなら
				bColl = CollRadius(pObj, type);
				break;
			case CObject::TYPE_BULLET:
				bColl = CollRadius(pObj, type);
				break;
			default:
				break;
			}
			if (bColl == true)
			{
				return bColl;
			}

			pObj = pObjNext;
		}
	}

	return bColl;
}

bool CEnemy::CollRadius(CObject* pObj, CObject::TYPE type)
{
	bool bColl = false;
	if (type == CObject::TYPE_PLAYER)
	{
		CPlayer* pPlayer = (CPlayer*)pObj;
			
		float PlayerRadius = 30.0f;

		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();//位置取得

		D3DXVECTOR3 enemySize = m_pModel->GetModelSize();
		float EnemyRadius = max(enemySize.x, max(enemySize.y, enemySize.z)) * 0.5f;

		//球での判定
		bColl = Radius(m_pos, EnemyRadius, PlayerPos, PlayerRadius);
		if (bColl == true)
		{//当たったら
			//弾と敵の相性を調べる
			//敵にダメージ
			pPlayer->Hit(1);
		}
	}
	else if (type == CObject::TYPE_BULLET)
	{
		CBullet* pBullet = (CBullet*)pObj;
		bool bUse = pBullet->GetUse();
		
		if (bUse == true)
		{
			D3DXVECTOR3 BulletSize = pBullet->GetModel()->GetModelSize();
			float BulletRadius = max(BulletSize.x, max(BulletSize.y, BulletSize.z)) * 0.5f;

			D3DXVECTOR3 BulletPos = pBullet->GetPos();//位置取得

			D3DXVECTOR3 enemySize = m_pModel->GetModelSize();
			float EnemyRadius = max(enemySize.x, max(enemySize.y, enemySize.z)) * 0.5f;

			//球での判定
			bool bCollBullet = Radius(m_pos, EnemyRadius, BulletPos, BulletRadius);
			if (bCollBullet == true)
			{//当たったら
				//弾と敵の相性を調べる
				//敵にダメージ
				Hit(1);
			}
		}
	}

	return bColl;
}
//
//
//
bool CEnemy::Radius(D3DXVECTOR3 pos0, float radius0, D3DXVECTOR3 pos1, float radius1)
{
	//距離
	float fDistance = (((pos1.x - pos0.x) * (pos1.x - pos0.x))
		+ ((pos1.y - pos0.y) * (pos1.y - pos0.y))
		+ ((pos1.z - pos0.z) * (pos1.z - pos0.z)));

	//半径
	float RADIUS = (radius0 + radius1) * (radius0 + radius1);

	if (fDistance <= RADIUS)
	{
		return true;
	}
	return false;
}
