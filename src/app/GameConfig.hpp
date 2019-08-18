#pragma once


namespace Root {
  struct GameConfig {
    enum class Mode {
      Line150,
      Line999,
      End,
      HiddenSince = End,
    };

    enum class Music {
      Tetris99,
      Wataten,
      NkPop,
      End,
      HiddenSince = NkPop,
    };

    enum class Background {
      Gray,
      Flame,
      Black,
      End,
      HiddenSince = End,
    };


    Mode mode;
    Music music;
    Background background;
    unsigned int level;
    bool extreme;
    bool easterEgg;


    static GameConfig& GetGlobalConfig();
  };
}
