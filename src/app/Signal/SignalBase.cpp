#include "SignalBase.hpp"


SignalBase::SignalBase() :
  mState(false)
{}


void SignalBase::Step() {
  mState = StepImpl();
}


bool SignalBase::GetState() const {
  return mState;
}
