#pragma once

#include <memory>

#include "Signal/SignalBase.hpp"
#include "Signal/KonamiCommandSignal.hpp"


namespace Root {
  enum class SceneId {
    Config,
    GameTetra,
    GameTetraRestart,
    Title,
  };


  class Scene;


  class SceneManager {
    static constexpr auto InitialSceneId = SceneId::Title;


    SceneId mSceneId;
    std::unique_ptr<Scene> mPtrScene;

  public:
    std::unique_ptr<SignalBase> oneShotKeyInputA;
    std::unique_ptr<SignalBase> oneShotKeyInputB;
    std::unique_ptr<SignalBase> oneShotKeyInputSelect;
    std::unique_ptr<SignalBase> oneShotKeyInputStart;
    std::unique_ptr<SignalBase> repeatKeyInputRight;
    std::unique_ptr<SignalBase> repeatKeyInputLeft;
    std::unique_ptr<SignalBase> repeatKeyInputUp;
    std::unique_ptr<SignalBase> repeatKeyInputDown;
    KonamiCommandSignal konamiCommandSignal;

  private:
    std::unique_ptr<Scene> CreateSceneFromSceneId(SceneId sceneId);

  public:
    SceneManager(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    ~SceneManager();

    static SceneManager& GetInstance();

    void SetScene(SceneId sceneId);

    void Render();
    void Update();

  private:
    SceneManager(SceneId initialSceneId);
  };
}   // namespace Root
