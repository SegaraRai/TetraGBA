# 音声データ仕様

```cpp
struct AudioData {
  std::uint32_t length;
  std::uint32_t timerPrescaler;
  std::int8_t   pcm[length * 16];
};
```
