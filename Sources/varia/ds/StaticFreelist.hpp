#pragma once

namespace vds
{

template<typename T, int Size>
struct StaticFreelist
{
	//TODO(zshoals): Remove me after template creation
	T freelist[1];
	//T freelist[Size];
	

};

}