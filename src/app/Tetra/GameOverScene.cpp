#include "GameOverScene.hpp"
#include "GameEndSceneBase.hpp"


namespace GameTetra {
  GameOverScene::GameOverScene(SceneManager& sceneManager) :
    GameEndSceneBase(sceneManager, GameEndSceneBase::Type::GameOver)
  {}
}   // namespace GameTetra
