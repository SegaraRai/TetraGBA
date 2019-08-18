#pragma once

#include <memory>

#include "Scene.hpp"
#include "../Signal/SignalBase.hpp"


namespace GameTetra {
  class GameEndSceneBase : public Scene {
  public:
    enum class Type {
      GameClear,
      GameOver,
    };

  private:
    static constexpr unsigned int NumPages = 2;

    Type mType;
    unsigned int mPage;
    unsigned int mFrameCountForKeyInput;

    std::unique_ptr<SignalBase> mKeyInputRight;
    std::unique_ptr<SignalBase> mKeyInputLeft;

  public:
    GameEndSceneBase(SceneManager& sceneManager, Type type);
    virtual ~GameEndSceneBase();

    virtual void Render() override;
    virtual void Update() override;
  };
}   // namespace GameTetra
