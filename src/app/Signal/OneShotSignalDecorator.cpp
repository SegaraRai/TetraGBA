#include "OneShotSignalDecorator.hpp"
#include "SignalBase.hpp"

#include <memory>
#include <utility>


OneShotSignalDecorator::OneShotSignalDecorator(std::unique_ptr<SignalBase>&& org) :
  mOrg(std::move(org)),
  mState(false)
{}


bool OneShotSignalDecorator::StepImpl() {
  const auto prevState = mState;
  const auto state = (mOrg->Step(), mOrg->GetState());
  mState = state;
  return !prevState && state;
}
