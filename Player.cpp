#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "MyMath.h"

//Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);


void Player::Initialize(Model* model, uint32_t Orb,Vector3 playerPosition) {
	assert(model);

	model_ = model;
	Orb_ = Orb;


	// X.Y.Z軸方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.translation_ = playerPosition;

	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

Player::~Player() { 
	for ( PlayerBullet*bullet : bullets_) {
		delete bullet;
	}
}

void Player::Update() {

	////スケーリング行列を宣言
	//Matrix4x4 matScale = {0};//4行4列

	//matScale.m[0][0] = worldTransform_.scale_.x;
	//matScale.m[1][1] = worldTransform_.scale_.y;
	//matScale.m[2][2] = worldTransform_.scale_.z;
	//matScale.m[3][3] = 1;

	////x軸回転行列を宣言
	//Matrix4x4 matRotX = {0};
	//matRotX.m[0][0] = 1;
	//matRotX.m[1][1] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[2][1] = -sinf(worldTransform_.rotation_.x);
	//matRotX.m[1][2] = sinf(worldTransform_.rotation_.x);
	//matRotX.m[2][2] = cosf(worldTransform_.rotation_.x);
	//matRotX.m[3][3] = 1;

	////y軸回転行列を宣言
	//Matrix4x4 matRotY = {0};
	//matRotY.m[0][0] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[1][1] = 1;
	//matRotY.m[0][2] = -sinf(worldTransform_.rotation_.y);
	//matRotY.m[2][0] = sinf(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cosf(worldTransform_.rotation_.y);
	//matRotY.m[3][3] = 1;

	////z軸回転行列を宣言
	//Matrix4x4 matRotZ = {0};
	//matRotZ.m[0][0] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = sinf(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = -sinf(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cosf(worldTransform_.rotation_.z);
	//matRotZ.m[2][2] = 1;
	//matRotZ.m[3][3] = 1;

	//Matrix4x4 matRot = Multiply(matRotZ,Multiply(matRotX,matRotY));

	////平行移動行列を宣言
	//Matrix4x4 matTrans = {0};

	//matTrans.m[0][0] = 1;
	//matTrans.m[1][1] = 1;
	//matTrans.m[2][2] = 1;
	//matTrans.m[3][3] = 1;
	//matTrans.m[3][0] = worldTransform_.translation_.x;
	//matTrans.m[3][1] = worldTransform_.translation_.y;
	//matTrans.m[3][2] = worldTransform_.translation_.z;

	//worldTransform_.matWorld_ = Multiply(matScale, Multiply(matRot, matTrans));

	////行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	worldTransform_.UpdateMatrix();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//////////プレイヤーの移動///////////////
	// キャラクターの移動ベクトル
	Vector3 playerMove = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	////押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		playerMove.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		playerMove.x += kCharacterSpeed;
	}
	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		playerMove.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		playerMove.y -= kCharacterSpeed;
	}
	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += playerMove.x;
	worldTransform_.translation_.y += playerMove.y;
	worldTransform_.translation_.z += playerMove.z;
	//////////////////////////////////////////

	

	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("Debug");*/
	float playerPos[] = {
	    worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	// 移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 17;

	// 範囲を超えない処理
	playerPos[0] = max(playerPos[0], -kMoveLimitX);
	playerPos[0] = min(playerPos[0], +kMoveLimitX);
	playerPos[1] = max(playerPos[1], -kMoveLimitY);
	playerPos[1] = min(playerPos[1], +kMoveLimitY);
	/*ImGui::SliderFloat3("PlayerPos", playerPos, -30.0f, 30.0f);*/
	//ここで実際の座標を変更する
	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	/*ImGui::End();*/

	//回転速さ[ラジアン/flame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾があれば解放する
		/*if (bullet_) {
			delete bullet_;
			bullet_ = nullptr;
		}*/
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(),velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
	//XINPUT_STATE joyState;

	//// ゲームパッド未接続なら何もせず抜ける
	//if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	return;
	//}

	//// Rトリガーを押していたら
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	//	// 弾の速度
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	// 速度ベクトルを時機向きに合わせて回転させる
	//	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//	velocity = Subtract(
	//	    {worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	//	     worldTransform3DReticle_.matWorld_.m[3][2]},
	//	    GetWorldPosition());

	//	velocity = Multiply(kBulletSpeed, Normalize(velocity));
	//	// 弾を生成し、初期化
	//	PlayerBullet* newBullet = new PlayerBullet();
	//	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	//	// 弾を登録する
	//	bullets_.push_back(newBullet);
	//}
}
void Player::OnCollision(){};

void Player::Draw(ViewProjection& viewProjection) {
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, Orb_);
	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
