#include "GameTetraRestartScene.hpp"


namespace Root {
  GameTetraRestartScene::GameTetraRestartScene(SceneManager& sceneManager) :
    Scene(sceneManager)
  {}


  void GameTetraRestartScene::Render() {}


  void GameTetraRestartScene::Update() {
    SetScene(SceneId::GameTetra);
  }
}
