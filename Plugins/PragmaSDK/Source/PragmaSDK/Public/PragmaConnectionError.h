#pragma once
#include "PragmaProtocolError.h"

class FPragmaConnectionError
{
public:
	FPragmaConnectionError(const EPragmaProtocolError SocialBackendError, const EPragmaProtocolError GameBackendError)
		: SocialBackendError(SocialBackendError), GameBackendError(GameBackendError) {}
	EPragmaProtocolError SocialBackendError{ EPragmaProtocolError::Unknown };
	EPragmaProtocolError GameBackendError{ EPragmaProtocolError::Unknown };

	FPragmaError ToPragmaError() const;
private:
	FPragmaConnectionError() {}
};
