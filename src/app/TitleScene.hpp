#pragma once

#include "Scene.hpp"
#include "Signal/SignalBase.hpp"

#include <memory>


namespace Root {
  class TitleScene : public Scene {
    enum class Item {
      NewGame,
      Config,
      End,
    };

    Item mSelection;

  public:
    TitleScene(SceneManager& sceneManager);

    void Render();
    void Update();
  };
}   // namespace Root
