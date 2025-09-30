//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "player.h"
#include "texture.h"
#include "model.h"
#include "game.h"

// 名前空間stdの使用
using namespace std;

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pos				= VECTOR3_NULL;					// 位置
	m_colliderPos		= VECTOR3_NULL;					// カプセル中心
	m_rot				= VECTOR3_NULL;					// 向き
	m_rotDest			= VECTOR3_NULL;					// 向き
	m_move				= VECTOR3_NULL;					// 移動量
	m_targetMove		= VECTOR3_NULL;					// 目標速度
	m_currentMove		= VECTOR3_NULL;					// 実際の移動速度
	m_size				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);// サイズ
	m_mtxWorld			= {};							// ワールドマトリックス
	m_nNumModel			= 0;							// モデル(パーツ)の総数
	m_pShadowS			= nullptr;						// ステンシルシャドウへのポインタ
	m_pMotion			= nullptr;						// モーションへのポインタ
	m_currentMotion		= PLAYER_MOTION::NEUTRAL;		// 現在のモーション
	m_pCollider			= nullptr;						// カプセルコライダー
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = {};							// モデル(パーツ)へのポインタ
	}
	m_pHpGauge = nullptr;								// ＨＰゲージへのポインタ
	m_nLife = 0;										// ライフ
}
//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = new CPlayer;

	pPlayer->m_pos = pos;
	pPlayer->m_rot = D3DXToRadian(rot);

	// 初期化処理
	pPlayer->Init();

	return pPlayer;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	CModel* pModels[MAX_PARTS];
	int nNumModels = 0;

	// パーツの読み込み
	m_pMotion = CMotion::Load("data/motion.txt", pModels, nNumModels, MAX);

	for (int nCnt = 0; nCnt < nNumModels && nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = pModels[nCnt];

		// オフセット考慮
		m_apModel[nCnt]->SetOffsetPos(m_apModel[nCnt]->GetPos());
		m_apModel[nCnt]->SetOffsetRot(m_apModel[nCnt]->GetRot());
	}

	// パーツ数を代入
	m_nNumModel = nNumModels;

	// 変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	m_rot = D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f);

	// ステンシルシャドウの生成
	m_pShadowS = CShadowS::Create("data/MODELS/stencilshadow.x",m_pos);

	// インスタンスのポインタを渡す
	m_stateMachine.Start(this);

	// 初期状態のステートをセット
	m_stateMachine.ChangeState<CPlayer_StandState>();

	// カプセルコライダーの生成
	m_pCollider = new CCapsuleCollider(18.5f, 70.5f);

	// カプセルの中心 = 足元 + オフセット
	m_colliderPos = m_pos + D3DXVECTOR3(0, m_pCollider->GetHeight() * 0.5f, 0);

	// ライフ
	m_nLife = PLAYER_LIFE;
	// ＨＰゲージ生成
	m_pHpGauge = CHpGauge::Create(D3DXVECTOR3(50.0f, 40.0f, 0.0f), PLAYER_LIFE, GAUGE_Y, D3DCOLOR_RGBA(1, 255, 1, 255));

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// コライダーの破棄
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}

	// モデルの破棄
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	// モーションの破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// オブジェクトの破棄(自分自身)
	this->Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの角度の取得
	D3DXVECTOR3 CamRot = pCamera->GetRot();

	// カメラモードの取得
	CCamera::MODE camMode = pCamera->GetMode();

	//// 入力判定の取得
	//InputData input = GatherInput();

	// ステートマシン更新
	m_stateMachine.Update();

	m_colliderPos = m_pos + D3DXVECTOR3(0, 50.0f, 0);

	if (m_pCollider)
	{
		// コライダーの更新
		m_pCollider->UpdateTransform(m_colliderPos, VECTOR3_NULL, VECTOR3_NULL);
	}

	//D3DXVECTOR3 targetMove = input.moveDir * PLAYER_SPEED;

	//// 徐々に m_currentMove を targetMove に近づける
	//float accel = 0.08f; // 加速度係数
	//m_currentMove += (targetMove - m_currentMove) * accel;

	//// 移動量を反映
	//m_pos += m_currentMove;

	// 向きの正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	m_rot.y += (m_rotDest.y - m_rot.y) * 0.09f;

	//// 移動入力があればプレイヤー向きを入力方向に
	//if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	//{
	//	m_rotDest.y = atan2f(-input.moveDir.x, -input.moveDir.z);
	//}

	if (m_pShadowS != nullptr)
	{
		// ステンシルシャドウの位置設定
		m_pShadowS->SetPosition(m_pos);
	}

	int nNumModels = 1;

	// モーションの更新処理
	m_pMotion->Update(m_apModel, nNumModels);
}
//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < m_nNumModel; nCntMat++)
	{
		// モデル(パーツ)の描画
		if (m_apModel[nCntMat])
		{
			m_apModel[nCntMat]->Draw();
		}
	}
}
//=============================================================================
// 入力判定取得関数
//=============================================================================
InputData CPlayer::GatherInput(void)
{
	InputData input{};
	input.moveDir = D3DXVECTOR3(0, 0, 0);
	input.jump = false;

	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad* pJoypad = CManager::GetJoyPad();				// ジョイパッドの取得
	XINPUT_STATE* pStick = CInputJoypad::GetStickAngle();		// スティックの取得
	CCamera* pCamera = CManager::GetCamera();					// カメラの取得
	D3DXVECTOR3 CamRot = pCamera->GetRot();						// カメラ角度の取得
	CCamera::MODE camMode = pCamera->GetMode();					// カメラモードの取得

	//// ---------------------------
	//// 弾発射
	//// ---------------------------
	//if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
	//{
	//	CBullet::Create(m_pos, m_rot, CBullet::USER_PLAYER);
	//}

	// ---------------------------
	// ゲームパッド入力
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
	// キーボード入力
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

	// 正規化
	if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	{
		D3DXVec3Normalize(&input.moveDir, &input.moveDir);
	}

	return input;
}
//=============================================================================
// モーションの設定処理
//=============================================================================
void CPlayer::SetMotion(int type, int nBlendFrame)
{
	m_pMotion->StartBlendMotion(type, nBlendFrame);
	m_currentMotion = type;
}