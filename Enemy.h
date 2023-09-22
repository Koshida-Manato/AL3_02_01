﻿#pragma once
#include "Input.h"
#include "Model.h"
#include "MyMath.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言(苦肉の策)
class GameScene;
/// <summary>
/// 敵
/// </summary>
enum Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};
class Enemy {
	public:

		void Initialize(Model* model, Vector3& position, const Vector3& velocity);
	    /// <summary>
	    /// デストラクタ
	    /// </summary>
	    ~Enemy();

		void Update();

		void Draw(ViewProjection& viewProjection);

	    /// <summary>
	    /// 弾発射
	    /// </summary>
	    void Fire();

		 /// <summary>
	    /// 接近フェーズ初期化
	    /// </summary>
	    void ApproachInitialize();

		void SetPlayer(Player* player) { player_ = player; }

		//衝突を検出したら呼び出せされるコールバック関数
	    void OnCollision();

		/// <summary>
		/// 発射間隔
		/// </summary>
	    static const int kFireInterval = 60;

		/// <summary>
	    /// 発射タイマー
	    /// </summary>
	    int32_t fireTimer = 0;

		 Vector3 GetWorldPosition();

		 bool IsDead() const { return isDead_; }

		  void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
		 // ゲームシーン
	     GameScene* gameScene_ = nullptr;
	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
		//モデル
	    /*Model* enemyModel_ = nullptr;*/
	    // モデル
	    Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t Teki_ = 0u;
		//フェーズ
	    Phase phase_ = Phase::Approach;
	    // キーボード入力
	    /*Input* input_ = nullptr;*/
		//自キャラ
	    Player* player_ = nullptr;
	    // デスフラグ
	    bool isDead_ = false;
		//速度
	    Vector3 velocity_;


	    float kEnemySpeed = 0;
};
