#pragma once

#include <array>
#include <cstddef>
#include <deque>
#include <random>

#include "XorShift128.hpp"
#include "Tetra/Game.hpp"


class BaggedMinoFactory {
  // std::uniform_int_distributionの最初の方の値が偏っているため何度か空回ししている、その回数
  static constexpr unsigned int DistAgingCount = 32;

  unsigned int mIndex;
  std::array<Tetra::MinoType, Tetra::NumMinoTypes> mBag;
  std::vector<std::uniform_int_distribution<std::size_t>> mDists;
  random_xorshift128 mRandom;

#ifndef RELEASE_BUILD
  std::deque<Tetra::MinoType> mDebugMinos;
#endif

  void DistAging();

public:
  BaggedMinoFactory();

  Tetra::MinoType operator()(const Tetra::Game::Game& game);
};
