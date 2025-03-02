
#pragma once

#include <JuceHeader.h>

#include "../../AdditionalSourceCode/AirWindows.h"

namespace airwindows::matrixverb_ns {
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wmultichar")
#include "../../External/airwindows/plugins/LinuxVST/src/MatrixVerb/MatrixVerb.h"

#include "../../External/airwindows/plugins/LinuxVST/src/MatrixVerb/MatrixVerb.cpp"
#include "../../External/airwindows/plugins/LinuxVST/src/MatrixVerb/MatrixVerbProc.cpp"
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}  // namespace airwindows::matrixverb_ns

namespace project {

using namespace juce;
using namespace hise;
using namespace scriptnode;

DECLARE_AIRWINDOWS_NODE(MatrixVerb, matrixverb_ns);

}  // namespace project
