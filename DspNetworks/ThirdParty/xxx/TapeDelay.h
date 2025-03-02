
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::tapedelay_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay/TapeDelay.h"

#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay/TapeDelay.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay/TapeDelayProc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::tapedelay_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(TapeDelay, tapedelay_ns);

}  // namespace project
