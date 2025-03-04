/* Autogenerated template file for RNBO exported class. */
#pragma once

#include <hi_dsp_library/node_api/nodes/rnbo_wrapper.h>
#include "src/granular.cpp"

namespace project
{
using namespace scriptnode;

template <int NV> struct granular: public wrap::rnbo_wrapper<RNBO::granular, NV>
{
	
	granular()
	{
	}
	
	SN_GET_SELF_AS_OBJECT(granular);
	
	static constexpr bool isPolyphonic() { return false; };
	static constexpr int getFixChannelAmount() { return 2; };
	
	struct MetadataClass
	{
		SN_NODE_ID("granular");
	};
};
}

