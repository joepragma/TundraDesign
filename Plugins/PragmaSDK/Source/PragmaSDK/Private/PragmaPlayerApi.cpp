#include "PragmaPlayerApi.h"
#include "PragmaPlayer.h"

void UPragmaPlayerApi::Init(Pragma::FPlayerPtr Player)
{
	PlayerPtr = Player;
	if (Player->IsInitialized())
	{
		OnInitialized();
	}
	else
	{
		Player->OnInitialized.AddUObject(this, &UPragmaPlayerApi::OnInitialized);
	}
}

Pragma::FPlayerPtr UPragmaPlayerApi::Player() const
{
	checkf(PlayerPtr, TEXT("UPragmaPlayerApi::Player() -- Must call Init before accessing the Player."));
	return PlayerPtr;
}

Pragma::FSessionPtr UPragmaPlayerApi::Session() const
{
	return PlayerPtr;
}
