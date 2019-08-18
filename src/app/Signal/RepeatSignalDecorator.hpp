#pragma once

#include <memory>

#include "SignalBase.hpp"


/*
  Repeat Signal Decorator

  DELAY = 5, INTERVAL = 3
    IN   - * * * * *  *  * *  *  * *  *  * - - * - -
    OUT  - * - - - -  *  - -  *  - -  *  - - - * - -
*/
class RepeatSignalDecorator : public SignalBase {
  std::unique_ptr<SignalBase> mOrg;
  unsigned int mDelay;
  unsigned int mInterval;
  bool mRepeat;
  unsigned int mCount;

protected:
  bool StepImpl() override;

public:
  RepeatSignalDecorator(std::unique_ptr<SignalBase>&& org, unsigned int delay, unsigned int interval);
};
