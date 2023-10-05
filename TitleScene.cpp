#include "TitleScene.h"

TitleScene::TitleScene(){};

TitleScene::~TitleScene() {};

void TitleScene::Initialize(){
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	isSceneEnd = false; // trueだとシーンチェンジする

	Title_ = TextureManager::Load("./Resources/Title.png");
	sprite2DTitle_ = Sprite::Create(Title_, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0});
};

void TitleScene::Update(){ 
	if (input_->PushKey(DIK_SPACE)) {
		isSceneEnd = true;
	}
};

void TitleScene::Draw() { 
	
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite2DTitle_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

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
};