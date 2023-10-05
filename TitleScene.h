#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"

class TitleScene {

public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kGamePlay; }

private://メンバ変数
	bool isSceneEnd = false;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t Title_ = 0;
	// モデル
	Model* model_ = nullptr;
	// 2Dレティクル用スプライト
	Sprite* sprite2DTitle_ = nullptr;
};
