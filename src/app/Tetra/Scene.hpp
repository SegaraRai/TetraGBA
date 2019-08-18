#pragma once

#include "SceneManager.hpp"


namespace GameTetra {
  class Scene {
  protected:
    SceneManager& sceneManager;

    Scene(SceneManager& sceneManager);

    void SetScene(SceneId sceneId);

  public:
    virtual ~Scene();

    virtual void Render() = 0;
    virtual void Update() = 0;
  };
}   // namespace GameTetra
