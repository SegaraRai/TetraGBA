#pragma once

#include "Scene.hpp"

#include <cstdint>


namespace GameTetra {
  class GameReadyScene : public Scene {
    unsigned int mFrameCount;

  public:
    GameReadyScene(SceneManager& sceneManager);
    ~GameReadyScene();

    void Render();
    void Update();
  };
}   // namespace GameTetra
