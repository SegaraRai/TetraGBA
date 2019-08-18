#pragma once


class SignalBase {
  bool mState;

protected:
  SignalBase();

  virtual bool StepImpl() = 0;

public:
  virtual ~SignalBase() = default;

  void Step();
  bool GetState() const;
};
