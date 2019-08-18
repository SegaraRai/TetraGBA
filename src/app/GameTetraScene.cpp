#include "GameTetraScene.hpp"


namespace Root {
  GameTetraScene::GameTetraScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    childSceneManager()
  {}


  void GameTetraScene::Render() {
    childSceneManager.Render();
  }


  void GameTetraScene::Update() {
    childSceneManager.Update();
  }
}
