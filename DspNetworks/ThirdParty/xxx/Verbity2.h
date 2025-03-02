
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::verbity2_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/Verbity2/Verbity2.h"

#include "../../External/airwindows/plugins/LinuxVST/src/Verbity2/Verbity2.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/Verbity2/Verbity2Proc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::verbity2_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(Verbity2, verbity2_ns);

}  // namespace project
