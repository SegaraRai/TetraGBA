#pragma once

#include <memory>

#include "SignalBase.hpp"


/*
  OneShot Signal Decorator
    IN   - * * * - * - - * * - - -
    OUT  - * - - - * - - * - - - -
*/
class OneShotSignalDecorator : public SignalBase {
  std::unique_ptr<SignalBase> mOrg;
  bool mState;

protected:
  bool StepImpl() override;

public:
  OneShotSignalDecorator(std::unique_ptr<SignalBase>&& org);
};
