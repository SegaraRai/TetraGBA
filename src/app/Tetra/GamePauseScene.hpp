#pragma once

#include "Scene.hpp"
#include "../Signal/SignalBase.hpp"

#include <cstdint>
#include <memory>


namespace GameTetra {
  class GamePauseScene : public Scene {
    std::uint_fast16_t mSOUNDCNT_L;

    std::unique_ptr<SignalBase> mSelectKeyInput;

  public:
    GamePauseScene(SceneManager& sceneManager);
    ~GamePauseScene();

    void Render();
    void Update();
  };
}   // namespace GameTetra
