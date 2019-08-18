#include "Scene.hpp"
#include "SceneManager.hpp"


namespace GameTetra {
  Scene::Scene(SceneManager& sceneManager) :
    sceneManager(sceneManager)
  {}


  Scene::~Scene() {}


  void Scene::SetScene(SceneId sceneId) {
    sceneManager.SetScene(sceneId);
  }
}   // namespace GameTetra
