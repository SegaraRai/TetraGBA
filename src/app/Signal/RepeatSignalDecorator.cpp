#include "RepeatSignalDecorator.hpp"
#include "SignalBase.hpp"

#include <memory>
#include <utility>


RepeatSignalDecorator::RepeatSignalDecorator(std::unique_ptr<SignalBase>&& org, unsigned int delay, unsigned int interval) :
  mOrg(std::move(org)),
  mDelay(delay),
  mInterval(interval),
  mRepeat(false),
  mCount(0)
{}


bool RepeatSignalDecorator::StepImpl() {
  // TODO: provide option which prohibits key repeating faster than Interval

  /*
  DELAY = 5, INTERVAL = 3

  IN   - * * * * *  *  * *  *  * *  *  * - - * - -
  CNT  0 0 1 2 3 4 5/0 1 2 3/0 1 2 3/0 1 0 0 0 0 0
  OUT  - * - - - -  *  - -  *  - -  *  - - - * - -
  */

  const auto state = (mOrg->Step(), mOrg->GetState());

  if (!state) {
    mCount = 0;
    return false;
  }

  if (mCount == 0) {
    mRepeat = false;
    mCount = 1;
    return true;
  }

  if (mCount == (mRepeat ? mInterval : mDelay)) {
    mRepeat = true;
    mCount = 1;
    return true;
  }

  mCount++;

  return false;
}
