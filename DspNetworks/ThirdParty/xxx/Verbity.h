
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::verbity_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/Verbity/Verbity.h"

#include "../../External/airwindows/plugins/LinuxVST/src/Verbity/Verbity.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/Verbity/VerbityProc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::verbity_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(Verbity, verbity_ns);

}  // namespace project
