#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdeteEnemyPopCommand();

	 // 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	void EnemyOccurrence(Vector3 position, Vector3 velocity);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//テクスチャハンドル
	uint32_t Kasu_ = 0;
	//敵テクスチャハンドル
	uint32_t Teki_ = 0;
	//天球テクスチャハンドル
	uint32_t Skydome_ = 0;
	//プレイヤー3Dモデル
	Model* playerModel_ = nullptr;
	//敵3Dモデル
	Model* enemyModel_ = nullptr;
	//天球
	Model* modelSkydome_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	Player* player_ = nullptr;
	//敵
	/*Enemy* enemy_ = nullptr;*/
	//天球
	Skydome* skydome_ = nullptr;
	//レールカメラ
	RailCamera* railCamera_ = nullptr;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	Vector3 rotate;
	//モデル
	Model* model_ = nullptr;
	//	敵弾
	std::list<EnemyBullet*> bullets_;
	// 敵
	std::list<Enemy*> enemys_;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	int standFlag = false;
	int standTime = 0;
};


