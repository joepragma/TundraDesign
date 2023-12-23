#pragma once
#include "PragmaResult.h"

class PRAGMASDK_API FFriendResponseHandler final 
{

public:
	static TPragmaResult<> HandleResponse(const FString& Response);
};
