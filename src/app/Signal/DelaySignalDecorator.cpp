#include "DelaySignalDecorator.hpp"
#include "SignalBase.hpp"

#include <memory>
#include <utility>


DelaySignalDecorator::DelaySignalDecorator(std::unique_ptr<SignalBase>&& org, unsigned int delay) :
  mOrg(std::move(org)),
  mDelay(delay),
  mCount(0)
{}


bool DelaySignalDecorator::StepImpl() {
  const auto state = (mOrg->Step(), mOrg->GetState());

  mCount++;

  if (!state) {
    mCount = 0;
  }

  return mCount > mDelay && state;
}
