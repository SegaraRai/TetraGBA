#pragma once

#include <memory>

#include "SignalBase.hpp"


/*
  Delay Signal Decorator

  DELAY = 2
    IN   - * * * * * - * - * * * - -
    OUT  - - - * * * - - - - - * - -
*/
class DelaySignalDecorator : public SignalBase {
  std::unique_ptr<SignalBase> mOrg;
  unsigned int mDelay;
  unsigned int mCount;

protected:
  bool StepImpl() override;

public:
  DelaySignalDecorator(std::unique_ptr<SignalBase>&& org, unsigned int delay);
};
