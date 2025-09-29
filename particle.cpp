//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "particle.h"
#include "game.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle()
{
	// 値のクリア
	m_Dir			= VECTOR3_NULL;// 方向
	m_nLife			= 0;		// 寿命
	m_nMaxParticle	= 0;		// 粒子の最大数
}
//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{
	// なし
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	// エフェクトの初期化
	CEffect::Init();

	return S_OK;
}
//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 寿命が尽きたら
	m_nLife--;

	if (m_nLife <= 0)
	{
		// 終了
		Uninit();
		return;
	}

	// エフェクトの更新
	CEffect::Update();
}


//=============================================================================
// 炎パーティクルのコンストラクタ
//=============================================================================
CFireParticle::CFireParticle()
{
	
}
//=============================================================================
// 炎パーティクルのデストラクタ
//=============================================================================
CFireParticle::~CFireParticle()
{
	// なし
}
//=============================================================================
// 炎パーティクルの初期化処理
//=============================================================================
HRESULT CFireParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/smoke.jpg");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// 炎パーティクルの更新処理
//=============================================================================
void CFireParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)//発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/smoke.jpg";

		// 位置の設定
		desc.pos = GetPos();

		// ランダムな角度で横に広がる
		float angle = ((rand() % 360) / 180.0f) * D3DX_PI;
		float speed = (rand() % 150) / 300.0f + 0.2f;

		desc.move.x = cosf(angle) * speed;
		desc.move.z = sinf(angle) * speed;
		desc.move.y = (rand() % 300) / 100.0f + 0.9f; // 上方向

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 5.0f + (rand() % 9);

		// 寿命の設定
		desc.nLife = GetLife();

		// 重力の設定
		desc.fGravity = 0.1f;

		// 半径の減衰量の設定
		desc.fDecRadius = 1.2f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}


//=============================================================================
// 水しぶきパーティクルのコンストラクタ
//=============================================================================
CWaterParticle::CWaterParticle()
{

}
//=============================================================================
// 水しぶきパーティクルのデストラクタ
//=============================================================================
CWaterParticle::~CWaterParticle()
{
	// なし
}
//=============================================================================
// 水しぶきパーティクルの初期化処理
//=============================================================================
HRESULT CWaterParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/smoke.jpg");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// 水しぶきパーティクルの更新処理
//=============================================================================
void CWaterParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)//発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/smoke.jpg";

		// 位置
		desc.pos = GetPos();

		// ランダムな角度で横に広がる
		float angle = ((rand() % 360) / 180.0f) * D3DX_PI;
		float speed = (rand() % 200) / 30.0f + 0.2f;

		desc.move.x = cosf(angle) * speed;
		desc.move.z = sinf(angle) * speed;
		desc.move.y = (rand() % 300) / 50.0f + 0.05f; // 少しだけ上方向

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 15.0f + (rand() % 30);

		// 寿命の設定
		desc.nLife = GetLife();

		// 重力の設定
		desc.fGravity = 0.2f;

		// 半径の減衰量の設定
		desc.fDecRadius = 1.0f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}


//=============================================================================
// オーラパーティクルのコンストラクタ
//=============================================================================
CAuraParticle::CAuraParticle()
{

}
//=============================================================================
// オーラパーティクルのデストラクタ
//=============================================================================
CAuraParticle::~CAuraParticle()
{
	// なし
}
//=============================================================================
// オーラパーティクルの初期化処理
//=============================================================================
HRESULT CAuraParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/treasure_effect.png");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// オーラパーティクルの更新処理
//=============================================================================
void CAuraParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)//発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/treasure_effect.png";

		// 位置
		desc.pos = GetPos();

		// ランダムな角度で横に広がる
		float angle = ((rand() % 360) / 180.0f) * D3DX_PI;
		float speed = (rand() % 150) / 300.0f + 0.2f;

		desc.move.x = cosf(angle) * speed;
		desc.move.z = sinf(angle) * speed;
		desc.move.y = (rand() % 300) / 100.0f + 0.9f; // 上方向

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 35.0f + (rand() % 40);

		// 寿命の設定
		desc.nLife = GetLife();

		// 重力の設定
		desc.fGravity = 0.0f;

		// 半径の減衰量の設定
		desc.fDecRadius = 1.5f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}


//=============================================================================
// 移動時パーティクルのコンストラクタ
//=============================================================================
CMoveParticle::CMoveParticle()
{

}
//=============================================================================
// 移動時パーティクルのデストラクタ
//=============================================================================
CMoveParticle::~CMoveParticle()
{
	// なし
}
//=============================================================================
// 移動時パーティクルの初期化処理
//=============================================================================
HRESULT CMoveParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/smoke.jpg");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// 移動時パーティクルの更新処理
//=============================================================================
void CMoveParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)//発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/smoke.jpg";

		// 位置
		desc.pos = GetPos();

		// ランダムな角度で横に広がる
		float angle = ((rand() % 360) / 180.0f) * D3DX_PI;
		float speed = (rand() % 150) / 300.0f + 0.2f;

		desc.move.x = cosf(angle) * speed;
		desc.move.z = sinf(angle) * speed;
		desc.move.y = (rand() % 100) / 50.0f + 0.05f; // 少しだけ上方向

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 15.0f + (rand() % 10);

		// 寿命の設定
		desc.nLife = GetLife();

		// 重力の設定
		desc.fGravity = 0.0f;

		// 半径の減衰量の設定
		desc.fDecRadius = 1.5f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}


//=============================================================================
// 浮遊パーティクルのコンストラクタ
//=============================================================================
CFloatingParticle::CFloatingParticle()
{

}
//=============================================================================
// 浮遊パーティクルのデストラクタ
//=============================================================================
CFloatingParticle::~CFloatingParticle()
{
	// なし
}
//=============================================================================
// 浮遊パーティクルの初期化処理
//=============================================================================
HRESULT CFloatingParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/smoke.jpg");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// 浮遊パーティクルの更新処理
//=============================================================================
void CFloatingParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)//発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/smoke.jpg";

		// 位置
		desc.pos = GetPos();

		// ランダムな角度で横に広がる
		float angle = ((rand() % 360) / 180.0f) * D3DX_PI;
		float speed = (rand() % 150) / 300.0f + 0.2f;

		desc.move.x = cosf(angle) * speed;
		desc.move.z = sinf(angle) * speed;
		desc.move.y = -((rand() % 300) / 100.0f + 0.9f); // 下方向

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 5.0f + (rand() % 30);

		// 寿命の設定
		desc.nLife = GetLife();

		// 重力の設定
		desc.fGravity = 0.0f;

		// 半径の減衰量の設定
		desc.fDecRadius = 0.8f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}


//=============================================================================
// 水流パーティクルのコンストラクタ
//=============================================================================
CWaterFlowParticle::CWaterFlowParticle()
{
	// 値のクリア
}
//=============================================================================
// 水流パーティクルのデストラクタ
//=============================================================================
CWaterFlowParticle::~CWaterFlowParticle()
{
	// なし
}
//=============================================================================
// 水流パーティクルの初期化処理
//=============================================================================
HRESULT CWaterFlowParticle::Init(void)
{
	// テクスチャを設定しておく
	SetPath("data/TEXTURE/smoke.jpg");

	// パーティクルの初期化処理
	CParticle::Init();

	return S_OK;
}
//=============================================================================
// 水流パーティクルの更新処理
//=============================================================================
void CWaterFlowParticle::Update(void)
{
	int nMaxParticle = GetMaxParticle();

	// パーティクル生成
	for (int nCnt = 0; nCnt < nMaxParticle; nCnt++)// 発生させたい粒子の数
	{
		EffectDesc desc;

		// テクスチャの指定
		desc.path = "data/TEXTURE/smoke.jpg";

		// 位置の設定
		desc.pos = GetPos();

		// 扇状にランダムで拡散させる
		float angleRange = D3DXToRadian(15.0f); // 拡散角度
		float randAngleX = (((rand() % 200) - 100) / 100.0f) * angleRange;
		float randAngleY = (((rand() % 200) - 100) / 100.0f) * angleRange;
		float randAngleZ = (((rand() % 200) - 100) / 100.0f) * angleRange;

		D3DXVECTOR3 dir = GetDir();

		// ランダム回転を加える
		D3DXMATRIX matRotX, matRotY, matRotZ, matRot;
		D3DXMatrixRotationX(&matRotX, randAngleX);
		D3DXMatrixRotationY(&matRotY, randAngleY);
		D3DXMatrixRotationZ(&matRotZ, randAngleZ);
		matRot = matRotY * matRotZ;
		D3DXVec3TransformNormal(&dir, &dir, &matRot);

		// 正規化
		D3DXVec3Normalize(&dir, &dir);

		// 速度
		float speed = 4.2f + (rand() % 5); // 横に吹き出す

		desc.move = dir * speed;

		// 色の設定
		desc.col = GetCol();

		// 半径の設定
		desc.fRadius = 5.0f + (rand() % 23);

		// 寿命の設定
		desc.nLife = 20 + (rand() % 20);

		// 重力の設定
		desc.fGravity = 0.5f;

		// 半径の減衰量の設定
		desc.fDecRadius = 0.3f;

		// エフェクトの設定
		CEffect::Create(desc);
	}

	// パーティクルの更新処理
	CParticle::Update();
}