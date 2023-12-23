#pragma once

#include "CoreMinimal.h"
#include "PragmaConnectionInfo.h"

#include "Tests/AsyncTest.h"
#include "PragmaMessageMetadata.h"
#include "PragmaPlayer.h"
#include "PragmaResult.h"
#include "PragmaPtr.h"
#include "Pragma/Api/Player/PragmaGameLoopApi.h"

class UPragmaSdkConfig;

PRAGMA_FWD(FPlayer);

namespace Pragma
{
	namespace Tests
	{
		//
		// This class wraps a PragmaPlayer during a test and provides common convenience functions.
		//
		// All functions operate by adding steps to the OwningTest.
		//
		class PRAGMASDK_API FTestPlayer final : public TSharedFromThis<FTestPlayer>
		{
		public:
			explicit FTestPlayer(TSharedPtr<FAsyncTest> OwningTest);
			~FTestPlayer();

			FPlayerPtr Session() const { return Player; }
			TSharedRef<FAsyncTest> OwningTest() const { return TestPtr.ToSharedRef(); }
			FConnectionInfo ConnectionInfo() { return Player->ConnectionInfo(); }

			TOptional<TPragmaResult<>> LoginResult;
			DECLARE_DELEGATE(FEndedDelegate);

			static const FString& GetUsername(const int UnsafeUsernameNumber);
			void LoginOrFail(const int UnsafeUsernameNumber);
			void LoginOrFail(const FString& Username);
			void Login(const int UnsafeUsernameNumber);
			void Login(const FString& Username);
			void EndTest(const FEndedDelegate& Delegate) const;

			UPragmaGameLoopApi::FOnCompleteDelegate OnCompleteDelegate(TFunction<void(TPragmaResult<>)> Assertions,
				const FAsyncTest::FOnStepComplete& OnStepComplete) const;

			void CreateParty(
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void UpdatePartyPlayer(const FPragma_Party_ExtUpdatePartyPlayerRequest& ExtRequest,
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void UpdateParty(const FPragma_Party_ExtUpdatePartyRequest& ExtRequest,
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void SetPreferredGameServerZones(const TArray<FString>& GameServerZones,
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void SetGameServerZoneToPing(const TMap<FString, int32>& GameServerZoneToPing,
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void SetReadyForPlayer(const bool bReady,
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void JoinWithInviteCode(const TSharedRef<FTestPlayer>& PlayerInPartyToJoin,
				TFunction<void(const UPragmaPartyPlayer*)> OnPlayerJoinedAssertions) const;
			void JoinWithPartyId(const TSharedRef<FTestPlayer>& PlayerInPartyToJoin,
				TFunction<void(const UPragmaPartyPlayer*)> OnPlayerJoinedAssertions) const;
			void EnterMatchmaking(
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void LeaveMatchmaking(
				TFunction<void(TPragmaResult<>)> Assertions) const;
			void SendInvite(const TSharedRef<FTestPlayer>& InviteePlayer,
				TFunction<void(const FPragmaPartyInvite&)> Assertions) const;
			void RespondToInviteWithAccept(TSharedRef<FString> InviteId, const TSharedRef<FTestPlayer>& Inviter,
				TFunction<void(const UPragmaPartyPlayer*)> Assertions) const;
			void RespondToInviteWithRejection(TSharedRef<FString> InviteId,
				TFunction<void()> Assertions) const;

		private:
			TSharedPtr<FAsyncTest> TestPtr;
			FPlayerPtr Player;
		};
	}
}
