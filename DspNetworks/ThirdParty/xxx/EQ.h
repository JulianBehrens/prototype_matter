
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::eq_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/EQ/EQ.h"

#include "../../External/airwindows/plugins/LinuxVST/src/EQ/EQ.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/EQ/EQProc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::eq_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(EQ, eq_ns);

}  // namespace project
