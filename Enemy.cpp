#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
#include <cassert>
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, Vector3& position, const Vector3& velocity) { 
	assert(model); 

	model_ = model;
	// テクスチャ読み込み
	Teki_ = TextureManager::Load("./Resources/Enemy.png");

	// X.Y.Z軸方向のスケーリングを設定
	worldTransform_.scale_ = {4.0f, 4.0f, 4.0f};
	worldTransform_.translation_ = {-10.0f, 0.0f, 50.0f};

	worldTransform_.Initialize();

	//接近フェーズ初期化
	ApproachInitialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

Enemy::~Enemy() {
	/*for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}*/
}

void Enemy::Update() {

	// 敵の移動ベクトル
	Vector3 enemyMove = {0, 0, 0};

	//////////敵の移動//////////////
	switch (phase_) {
	case Phase::Approach:
	default:
		// キャラクターの移動の速さ
		kEnemySpeed = -0.2f;
		enemyMove.z += kEnemySpeed;
		// 座標移動(ベクトルの加算)
		worldTransform_.translation_.x += enemyMove.x;
		worldTransform_.translation_.y += enemyMove.y;
		worldTransform_.translation_.z += enemyMove.z;
		//既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		// 発射タイマーをカウントダウン
		fireTimer--;
		// 指定時間に達した
		if (fireTimer <= 0) {
			// 弾を発射
			Fire();
			// 発射タイマーを初期化
			fireTimer = kFireInterval;
		}
		break;
	case Phase::Leave:
		kEnemySpeed = 1.0f;
		enemyMove.x += kEnemySpeed;
		enemyMove.y += kEnemySpeed;
		enemyMove.z -= kEnemySpeed;
		// 座標移動(ベクトルの加算)
		worldTransform_.translation_.x += enemyMove.x;
		worldTransform_.translation_.y += enemyMove.y;
		worldTransform_.translation_.z += enemyMove.z;
		break;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::ApproachInitialize() {
	////発射タイマーをカウントダウン
	//fireTimer--;
	////指定時間に達した
	//if (fireTimer <= 0) {
	//	//弾を発射
	//	Fire();
	//	//発射タイマーを初期化
	//	fireTimer = kFireInterval;
	//}
}

void Enemy::OnCollision() { isDead_ = true; }

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 0.75f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 速度ベクトルを自機の向きに合わせて回転させる
	/*velocity = TransformNormal(velocity, worldTransform_.matWorld_);*/

	Vector3 vector;
	Vector3 normal;

	player_->GetWorldPosition();
	GetWorldPosition();
	vector = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	normal = Normalize(vector);
	velocity = Velocity(normal, kBulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	    model_->Draw(worldTransform_, viewProjection, Teki_);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
