#include "GameScene.h"
#include "AxisIndicator.h"
#include "MyMath.h"
#include "Player.h"
#include "RailCamera.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playerModel_;
	delete debugCamera_;
	// 自キャラの解放
	delete player_;
	// 敵の解放
	/*delete enemy_;*/
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	bullets_.push_back(enemyBullet);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	Kasu_ = TextureManager::Load("./Resources/kasu.png");
	// 天球の読み込み
	Skydome_ = TextureManager::Load("./Resources/skydome/uvChecker.png");
	// プレイヤー3Dモデルの生成
	playerModel_ = Model::Create();
	// 敵3Dモデルの生成
	enemyModel_ = Model::Create();
	// 天球3Dモデルの生成
	modelSkydome_ = Model::Create();
	// レールカメラモデル
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();


	// レールカメラの生成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->Initialize({0.0f, 0.0f, -40.0f}, rotate);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	/*Vector3 playerPosition(0, 0, 50);*/
	player_->Initialize(model_, Kasu_, {0.0f, 0.0f, 40.0f});
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//// 敵の生成
	//enemy_ = new Enemy();
	//// 敵の初期化
	//enemy_->Initialize(enemyModel_, Teki_);
	//// 敵キャラに自分のアドレスを渡す
	//enemy_->SetPlayer(player_);

	// 天球の生成
	skydome_ = new Skydome();
	// 3Dモデルの作成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, Skydome_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//// 軸方向が表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	LoadEnemyPopData();
}

void GameScene::Update() {

	// レールカメラの更新
	railCamera_->Update();
	// 自キャラの更新
	player_->Update();

	UpdeteEnemyPopCommand();

	// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// レールカメラの更新
		railCamera_->Update();
		// デバッグカメラの更新
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C)) {
		isDebugCameraActive_ = false;
	}
	if (input_->TriggerKey(DIK_X)) {
		isDebugCameraActive_ = true;
	}

#endif
	//// デバッグカメラの更新
	//railCamera_->Update();
	//viewProjection_.matView = railCamera_->GetViewProjection().matView;
	//viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	//// ビュープロジェクション行列の転送
	//viewProjection_.TransferMatrix();
	CheckAllCollisions();

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
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

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : bullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = sqrtf(a * a + b * b + c * c);

		if (d <= 6.0f) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 自弾と敵キャラのすべての当たり判定
	for (Enemy* enemy : enemys_) {
		for (PlayerBullet* bullet : playerBullets) {
			//敵の判定
			posA=enemy->GetWorldPosition();
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			float a = posB.x - posA.x;
			float b = posB.y - posA.y;
			float c = posB.z - posA.z;

			float d = sqrtf(a * a + b * b + c * c);

			if (d <= 6.0f) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自弾と敵弾の当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		for (EnemyBullet* bullet2 : bullets_) {
			// 自弾の座標
			posA = bullet->GetWorldPosition();
			// 敵弾の座標
			posB = bullet2->GetWorldPosition();


			float a = posB.x - posA.x;
			float b = posB.y - posA.y;
			float c = posB.z - posA.z;

			float d = sqrtf(a * a + b * b + c * c);
			if (d <= 6.0f) {
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bullet2->OnCollision();
			}
		}
	}
#pragma endregion
};

void GameScene::EnemyOccurrence(Vector3 position, Vector3 velocity) {
	Enemy* enemy = new Enemy();

	// 初期化
	enemy->Initialize(model_, position, velocity);
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵キャラにゲームシーン渡す
	enemy->SetGameScene(this);
	// リストに登録
	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}


void GameScene::UpdeteEnemyPopCommand() {

	// 待機処理
	if (standFlag) {
		standTime--;
		if (standTime <= 0) {
			// 待機完了
			standFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyOccurrence(Vector3(x, y, z), {0, 0, 0});

		} // WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機時間
			standFlag = true;
			standTime = waitTime;

			// コマンドループを抜ける
			break; // 重要。なぜ重要か考えてみよう
		}
	}
}
