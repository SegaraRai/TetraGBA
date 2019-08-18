#include "GameClearScene.hpp"
#include "GameEndSceneBase.hpp"


namespace GameTetra {
  GameClearScene::GameClearScene(SceneManager& sceneManager) :
    GameEndSceneBase(sceneManager, GameEndSceneBase::Type::GameClear)
  {}
}   // namespace GameTetra
