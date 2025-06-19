#pragma once

#include "cemi/NPDUFrame.h"
#include "cemi/Control.h"

class Cemi {
private:
  // messageCode;
  Control control;
  // source
  // destination
  NPDUFrame npdu;
};
