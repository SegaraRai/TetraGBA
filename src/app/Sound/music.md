# 曲データ仕様

```cpp
struct MusicData {
  std::uint16_t divisor;
  std::uint16_t offsetToEnd;
  std::uint16_t offsetToLoopPoint;
  std::uint16_t numRates;             // 音階定義, 最大128
  std::uint16_t numEnvelopes;         // エンベロープ値定義, 最大256
  std::uint16_t numChannel3Values;    // SOUND3CNT_H値定義, 最大256
  std::uint16_t rates[numRates];
  std::uint16_t envelopes[numEnvelopes];
  std::uint16_t channel3Values[numChannel3Values];
  std::uint8_t  commands[offsetToEnd - offsetof(MusicData, command)];
};
```

## コマンド

曲データはコマンドの羅列で表現される。
コマンドは可変長で、最初のバイトがコマンドの種別を表す。
コマンドの一覧は以下の通り。

|先頭バイト|内容|バイト数|説明|
|:----|:----|:----|:----|
|0x00|NOP|1|何もしない。|
|0x01|休止|3|2〜3バイト目で示す時間だけ休止する。|
|0x02|BPM設定|3|2〜3バイト目で示すBPMに変更する。|
|0x04|`SOUNDCNT_L`設定|3|`SOUNDCNT_L`に2〜3バイト目の値を設定する。|
|0x05|`SOUNDCNT_H`設定|3|`SOUNDCNT_H`に2〜3バイト目の値を設定する。|
|0x06|`SOUNDCNT_X`設定|3|`SOUNDCNT_X`に2〜3バイト目の値を設定する。|
|0x07|`SOUNDBIAS`設定|3|`SOUNDBIAS`に2〜3バイト目の値を設定する。|
|0x08|WAVE_RAM設定|9|2〜9バイト目で`WAVE_RAM0_L`〜`WAVE_RAM3_H`を設定する。|
|0x0C|`SOUND1CNT_L`設定|2|`SOUND1CNT_L`に2バイト目の値を設定する。|
|0x0D|`SOUND1CNT_L`リセット|1|`SOUND1CNT_L`に0を設定する。|
|0x10〜0x13|SOUND1オン|3〜5|`SOUND1CNT_H`と`SOUND1CNT_X`を順に設定しSOUND1を再生する。[^1]|
|0x14〜0x17|SOUND2オン|3〜5|`SOUND2CNT_L`と`SOUND2CNT_H`を順に設定しSOUND2を再生する。[^1]|
|0x18〜0x1B|SOUND3オン|3〜5|`SOUND3CNT_H`と`SOUND3CNT_X`を順に設定しSOUND3を再生する。[^1]|
|0x1C〜0x1F|SOUND4オン|3〜4|`SOUND4CNT_L`と`SOUND4CNT_H`を順に設定しSOUND4を再生する。[^2]|
|0x20|SOUND1オフ|1|`SOUND1CNT_H`を設定しSOUND1を停止する。|
|0x21|SOUND2オフ|1|`SOUND2CNT_L`を設定しSOUND2を停止する。|
|0x22|SOUND3オフ|1|`SOUND3CNT_H`を設定しSOUND3を停止する。|
|0x23|SOUND4オフ|1|`SOUND4CNT_L`を設定しSOUND4を停止する。|
|0x40〜0x4F|レジスタ設定|3|`0x04000060 + (VALUE - 0x40) * 2`のレジスタに2〜3バイト目の表す16ビットの値を書き込む。|
|0x80〜0xFF|休止|1|先頭バイトから0x7Fを引いた分だけ休止する。|

[^1]: CH1〜CH3のサウンドオンコマンドは、コマンド番号のビットによって動作が変わる。
0x01 オフ: 1バイトで`envelopes`（CH1およびCH2）または`channel3Values`（CH3）のインデックスを表す
0x01 オン: 2バイトで直接レジスタの値を表す
0x02 オフ: 1バイトで音階（`rates`のインデックス、下位7ビット）とLengthフラグ（最上位ビット）を表す
0x02 オン: 2バイトで直接レジスタの値を表す

[^2]: CH4のサウンドオンコマンドは、コマンド番号のビットによって動作が変わる。
0x01 オフ: 1バイトで`envelopes`のインデックスを表す
0x01 オン: 2バイトで直接レジスタの値を表す
0x02 オフ: 1バイトで`SOUND4CNT_H`の下位1バイトを表す（Lengthフラグは**オフ**）
0x02 オン: 1バイトで`SOUND4CNT_H`の下位1バイトを表す（Lengthフラグは**オン**）

## Waitについて

Waitで待つ時間は、`Wait`を指定された値、`SPB`を1拍あたりの秒数とすると`SPB * Wait / MusicData::divisor`秒。
