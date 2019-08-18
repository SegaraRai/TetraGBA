#include "GameConfig.hpp"
#include "Config.hpp"


namespace Root {
  GameConfig& GameConfig::GetGlobalConfig() {
    static GameConfig globalGameConfig(Config::DefaultConfig);
    return globalGameConfig;
  }
}
