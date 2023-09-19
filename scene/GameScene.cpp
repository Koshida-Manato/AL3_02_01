#include "GameScene.h"
#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "MyMath.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
		delete playerModel_; 
	    delete debugCamera_;
	    // 自キャラの解放
	    delete player_;
		//敵の解放
	    delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	Kasu_ = TextureManager::Load("./Resources/kasu.png");
	// テクスチャ読み込み
	Teki_ = TextureManager::Load("./resources/Enemy.png");
	//プレイヤー3Dモデルの生成
	playerModel_ = Model::Create();
	//敵3Dモデルの生成
	enemyModel_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(playerModel_, Kasu_);
	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(enemyModel_, Teki_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向が表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//敵キャラに自分のアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
	//カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C)) {
		isDebugCameraActive_ = false; 
	}
	if (input_->TriggerKey(DIK_X)) {
		isDebugCameraActive_ = true;
	}

#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region  前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion  
}
void GameScene::CheckAllCollisions(){
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();


	#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		
		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= 2) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//自弾と敵キャラのすべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrt(a * a + b * b + c * c);

		if (d <= 2) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region  自弾と敵弾の当たり判定

	//自弾と敵弾の当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		for (EnemyBullet* bullet2 : enemyBullets) {
			//自弾の座標
			posA = bullet->GetWorldPosition();
			//敵弾の座標
			posB = bullet2->GetWorldPosition();

			float a = posB.x - posA.x;
			float b = posB.y - posA.y;
			float c = posB.z - posA.z;

			float d = sqrt(a * a + b * b + c * c);
			if (d <= 2) {
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet2->OnCollision();
			}
		}
	}
	#pragma endregion
};
