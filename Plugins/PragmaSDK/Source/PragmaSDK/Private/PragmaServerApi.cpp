#include "PragmaServerApi.h"
#include "PragmaServer.h"

void UPragmaServerApi::Init(Pragma::FServerPtr Server)
{
	ServerPtr = Server;
	if (Server->IsInitialized())
	{
		OnInitialized();
	}
	else
	{
		Server->OnInitialized.AddUObject(this, &UPragmaServerApi::OnInitialized);		
	}
}

Pragma::FServerPtr UPragmaServerApi::Server() const
{
	checkf(ServerPtr, TEXT("UPragmaServerApi::Server() -- Must call Init before accessing the Server."));
	return ServerPtr;
}

Pragma::FSessionPtr UPragmaServerApi::Session() const
{
	return Server(); 
}
