#pragma once

#include "Scene.hpp"
#include "tetra/SceneManager.hpp"


namespace Root {
  class GameTetraScene : public Scene {
    GameTetra::SceneManager childSceneManager;

  public:
    GameTetraScene(SceneManager& sceneManager);

    void Render();
    void Update();
  };
}
