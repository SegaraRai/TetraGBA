#include <gba.hpp>

#include "IRQ.hpp"
#include "SceneManager.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>


const char binary_header[] __attribute__((section("._binary_header"))) __attribute__((used)) = "\0\0\0\0\0\0\0\0\0\0\0\0################TETRA GBA   v1.1    BY SegaraRai################";


int main() {
  gba::irq::SetISR(CommonISR);
  SetCommonIRQFlags();

  auto& sceneManager = Root::SceneManager::GetInstance();

  while (true) {
    sceneManager.Render();
    sceneManager.Update();
  }
}
