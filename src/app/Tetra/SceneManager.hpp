#pragma once

#include <memory>


namespace GameTetra {
  enum class SceneId {
    Game,
    GameClear,
    GameOver,
    GamePause,
    GameReady,
  };


  class Scene;
  class GameScene;


  class SceneManager {
    static constexpr auto InitialSceneId = SceneId::GameReady;

    std::unique_ptr<GameScene> mPtrGameScene;      // just for deferring initialization
    std::unique_ptr<Scene> mPtrOverrideScene;

    std::unique_ptr<Scene> CreateOverrideSceneFromSceneId(SceneId sceneId);

  public:
    ~SceneManager();

    SceneManager();

    void SetScene(SceneId sceneId);

    GameScene& GetGameScene();

    void GameRender();
    void GameUpdate();

    void Render();
    void Update();

  private:
    SceneManager(SceneId initialSceneId);
  };
}   // namespace GameTetra
