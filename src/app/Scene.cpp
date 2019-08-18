#include "Scene.hpp"
#include "SceneManager.hpp"


namespace Root {
  Scene::Scene(SceneManager& sceneManager) :
    sceneManager(sceneManager)
  {}


  Scene::~Scene() {}


  void Scene::SetScene(SceneId sceneId) {
    sceneManager.SetScene(sceneId);
  }
}   // namespace Root
