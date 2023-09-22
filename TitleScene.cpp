#include "TitleScene.h"

TitleScene::TitleScene(){};

TitleScene::~TitleScene() {};

void TitleScene::Initialize(){
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	isSceneEnd = false; // trueだとシーンチェンジする

	Title_ = TextureManager::Load("./Resources/Title.png");

};

void TitleScene::Update(){ 
	if (input_->PushKey(DIK_SPACE)) {
		isSceneEnd = true;
	}
};

void TitleScene::Draw() { Title_ = Sprite::Create(Title_, {0, 0}, {0, 0, 0, 0}, {0, 0}); };