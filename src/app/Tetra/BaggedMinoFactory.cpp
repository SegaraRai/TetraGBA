#include "BaggedMinoFactory.hpp"
#include "Config.hpp"
#include "../DbgPrintf.hpp"

#include <algorithm>
#include <cstddef>
#include <random>

#include <gba.hpp>


BaggedMinoFactory::BaggedMinoFactory() :
  mIndex(mBag.size()),
  mBag{},
  mDists{},
  mRandom(gba::reg::TM2CNT_L, gba::reg::TM3CNT_L ^ gba::reg::VCOUNT)
#ifndef RELEASE_BUILD
  ,mDebugMinos{}
#endif
{
#ifndef RELEASE_BUILD
  {
    using namespace GameTetra;

    [[maybe_unused]] constexpr auto I = Tetra::MinoType::I;
    [[maybe_unused]] constexpr auto O = Tetra::MinoType::O;
    [[maybe_unused]] constexpr auto S = Tetra::MinoType::S;
    [[maybe_unused]] constexpr auto Z = Tetra::MinoType::Z;
    [[maybe_unused]] constexpr auto J = Tetra::MinoType::J;
    [[maybe_unused]] constexpr auto L = Tetra::MinoType::L;
    [[maybe_unused]] constexpr auto T = Tetra::MinoType::T;

    switch (Config::Debug::DebugBoard) {
      case Config::Debug::DebugBoardType::DoubleQuad:
        mDebugMinos = std::deque<Tetra::MinoType>{I, I};
        break;

      case Config::Debug::DebugBoardType::QuadTST:
        mDebugMinos = std::deque<Tetra::MinoType>{I, T, T};
        break;

      case Config::Debug::DebugBoardType::DTPC:
        mDebugMinos = std::deque<Tetra::MinoType>{T, T, T, T, T};
        break;

      case Config::Debug::DebugBoardType::REN:
        mDebugMinos = std::deque<Tetra::MinoType>{L, J, L, J, L, J, L, J, L, J, L, J, L, J, L, J, L, J, L, J};
        break;

      default:
        // do nothing; for suppressing warning
        break;
    }
  }
#endif

  for (std::size_t i = 0; i < mBag.size(); i++) {
    mBag[i] = static_cast<Tetra::MinoType>(i);
  }

  mDists.reserve(mBag.size());
  for (std::size_t i = 0; i < mBag.size(); i++) {
    mDists.emplace_back(0, i);
  }

  DistAging();
}


void __attribute__((optimize("O0"))) BaggedMinoFactory::DistAging() {
  for (auto& dist : mDists) {
    for (unsigned int i = 0; i < DistAgingCount; i++) {
      dist(mRandom);
    }
  }
}


Tetra::MinoType BaggedMinoFactory::operator()([[maybe_unused]] const Tetra::Game::Game& game) {
#ifndef RELEASE_BUILD
  if (!mDebugMinos.empty()) {
    const auto ret = mDebugMinos[0];
    mDebugMinos.pop_front();
    return ret;
  }
#endif

  if (mIndex == mBag.size()) {
    //DbgPrintf("BaggedMinoFactory: shuffle\n");

    mIndex = 0;

    // shuffle (Algorithm P)
    for (std::size_t i = mBag.size() - 1; i >= 1; i--) {
      const std::size_t j = mDists[i](mRandom);
      std::swap(mBag[i], mBag[j]);
    }
  }

  return mBag[mIndex++];
}
