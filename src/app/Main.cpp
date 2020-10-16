#include <gba.hpp>

#include "IRQ.hpp"
#include "SceneManager.hpp"

int main() {
  gba::irq::SetISR(CommonISR);
  SetCommonIRQFlags();

  auto& sceneManager = Root::SceneManager::GetInstance();

  while (true) {
    sceneManager.Render();
    sceneManager.Update();
  }
}
