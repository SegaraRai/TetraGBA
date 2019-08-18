# VRAMの使用

このプログラムではBGモードは全て0を用いている。

メモリマップは以下の通り
|メモリ（+0x06000000）|種類|インデックス|使途                           |備考                                         |
|:--------------------|:---|:-----------|:------------------------------|:--------------------------------------------|
|+0000 ~ +4000        |TILE|0           |bg                             |512タイルに収まると仮定して半分だけ用いている|
|+4000 ~ +C000        |TILE|1           |bg_background                  |1024タイルフルに用いると仮定する             |
|+C000 ~ +C800        |MAP |24          |共通/BG3                       |                                             |
|+C800 ~ +D000        |MAP |25          |Root::TitleScene/BG0           |                                             |
|+C800 ~ +D000        |MAP |25          |Root::ConfigScene/BG0          |                                             |
|+C800 ~ +D000        |MAP |25          |GameTetra::GameScene/BG0       |スコア表示                                   |
|+C800 ~ +D000        |MAP |25          |GameTetra::GameEndSceneBase/BG0|リザルト表示                                 |
|+D000 ~ +D800        |MAP |26          |GameTetra::GameScene/BG1       |盤面（ミノ）表示                             |
|+D800 ~ +E000        |MAP |27          |GameTetra::GamePauseScene/BG1  |                                             |
|+E000 ~ +E800        |MAP |28          |GameTetra::GameReadyScene/BG1  |                                             |
|+E800 ~ +F000        |MAP |29          |GameTetra::GameScene/BG2       |枠、全シーン共通                             |
