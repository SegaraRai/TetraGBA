#pragma once

#include "Scene.hpp"


namespace Root {
  // `GameTetraScene`に遷移するだけのシーン
  // 現在のシーンへの遷移要求を無視する実装にしても大丈夫なようにこのシーンを経由させる
  class GameTetraRestartScene : public Scene {
  public:
    GameTetraRestartScene(SceneManager& sceneManager);

    void Render();
    void Update();
  };
}
