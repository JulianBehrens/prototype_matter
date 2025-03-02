
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::tapedelay2_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay2/TapeDelay2.h"

#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay2/TapeDelay2.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/TapeDelay2/TapeDelay2Proc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::tapedelay2_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(TapeDelay2, tapedelay2_ns);

}  // namespace project
