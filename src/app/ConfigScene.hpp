#pragma once

#include "Scene.hpp"


namespace Root {
  class ConfigScene : public Scene {
    enum class Item {
      Mode,
      Level,
      Music,
      Background,
      Return,
      End,
    };

    static constexpr unsigned int NumSelections = 5;

    Item mSelection;

  public:
    ConfigScene(SceneManager& sceneManager);

    void Render();
    void Update();
  };
}   // namespace Root
