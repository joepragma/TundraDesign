#include "Tests/TestPlayer.h"

#include "Tests/AsyncTest.h"
#include "PragmaRuntime.h"
#include "Services/PragmaAccountService.h"
#include "Services/Party/PragmaPartyPlayer.h"
#include "PragmaPlayer.h"
#include "PragmaPromiseAll.h"

namespace Pragma
{
	namespace Tests
	{
		const TArray<FString>& PreferredGameServerZone = {"local"};
		const TMap<FString, int>& GameServerZoneToPing = {{"local", 10}};

		FTestPlayer::FTestPlayer(TSharedPtr<FAsyncTest> OwningTest) : TestPtr(MoveTemp(OwningTest))
		{
			checkf(TestPtr.IsValid(), TEXT("FTestPlayer must be initialized with a valid TSharedPtr<FAsyncTest>"));
			Player = TestPtr->Runtime()->CreatePlayer();
		}

		FTestPlayer::~FTestPlayer()
		{
			if (TestPtr.IsValid() && TestPtr->Runtime() != nullptr)
			{
				OwningTest()->Runtime()->DestroyPlayer(Player);
			}
		}

		const FString& FTestPlayer::GetUsername(const int UnsafeUsernameNumber)
		{
			static const TArray<FString> UnsafeUsernames{
				TEXT("test01"), TEXT("test02"), TEXT("test03"),
				TEXT("test04"), TEXT("test05"), TEXT("test06"),
				TEXT("test07"), TEXT("test08"), TEXT("test09"),
				TEXT("test10"), TEXT("test11"), TEXT("test12"),
				TEXT("test13"), TEXT("test14"), TEXT("test15"),
				TEXT("test16"), TEXT("test17"), TEXT("test18"),
				TEXT("test19"), TEXT("test20"),
			};
			const int Index = UnsafeUsernameNumber - 1;
			checkf(UnsafeUsernames.IsValidIndex(Index), TEXT("Username for number %d not found."),
				UnsafeUsernameNumber);
			return UnsafeUsernames[Index];
		}

		void FTestPlayer::LoginOrFail(const int UnsafeUsernameNumber)
		{
			LoginOrFail(GetUsername(UnsafeUsernameNumber));
		}

		void FTestPlayer::LoginOrFail(const FString& Username)
		{
			Login(Username);

			OwningTest()->AddStep("CheckForValidLogin", [this]
			{
				if (!LoginResult.IsSet())
				{
					OwningTest()->Fail(FString::Printf(TEXT("Missing login result?!")));
				}
				if (LoginResult.GetValue().IsFailure())
				{
					OwningTest()->Fail(FString::Printf(TEXT("TEST FAILED: Failed to Login. Error: %s"),
						*LoginResult.GetValue().ErrorCode()));
				}
			});
		}

		void FTestPlayer::Login(const int UnsafeUsernameNumber)
		{
			Login(GetUsername(UnsafeUsernameNumber));
		}

		void FTestPlayer::Login(const FString& Username)
		{
			OwningTest()->AddStepLatent(FString::Printf(TEXT("LogIn %s"), *Username),
				[this, Username](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					if (Player->IsLoggedIn())
					{
						OwningTest()->Fail(FString::Printf(TEXT("Player '%s' is already logged in via Login."),
							*Username));
						return;
					}

					UE_LOG(LogTemp, Log, TEXT("Player %s logging in..."), *Username);
					Player->LogIn(EPragma_Account_IdProvider::UNSAFE, Username,
						FPlayer::FLoggedInDelegate::CreateLambda(
							[this, Username, OnStepComplete](const TPragmaResult<>& Result)
							{
								UE_LOG(LogTemp, Log, TEXT("Player %s log in result: %s"), *Username,
									Result.IsSuccessful() ? TEXT("success!") : *FString::Printf(TEXT(
										"failure. error: %s"), *Result.ErrorCode()));
								LoginResult = Result;
								(*OnStepComplete)();
							}));
				});
		}

		void FTestPlayer::EndTest(const FEndedDelegate& Delegate) const
		{
			if (Player->IsLoggedIn())
			{
				Player->LogOut(Delegate);
			}
			else
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				Delegate.ExecuteIfBound();
			}
		}

		UPragmaGameLoopApi::FOnCompleteDelegate FTestPlayer::OnCompleteDelegate(
			TFunction<void(TPragmaResult<>)> Assertions, const FAsyncTest::FOnStepComplete& OnStepComplete) const
		{
			return UPragmaGameLoopApi::FOnCompleteDelegate::CreateLambda(
				[this, Assertions, OnStepComplete](TPragmaResult<> Result)
				{
					if (Assertions != nullptr)
					{
						Assertions(MoveTemp(Result));
					}
					(*OnStepComplete)();
				});
		}

		void FTestPlayer::CreateParty(TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent([this, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
			{
				Session()->GameLoopApi().CreateParty({}, {}, PreferredGameServerZone, GameServerZoneToPing,
					OnCompleteDelegate(Assertions, OnStepComplete));
			});
		}

		void FTestPlayer::SendInvite(const TSharedRef<FTestPlayer>& InviteePlayer,
			TFunction<void(const FPragmaPartyInvite&)> OnInviteReceivedAssertions) const
		{
			OwningTest()->AddStepLatent(
				[this, OnInviteReceivedAssertions, InviteePlayer](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					const auto OnInviteReceivedHandler = MakeShared<FDelegateHandle>();
					*OnInviteReceivedHandler = InviteePlayer->Session()->GameLoopApi().OnPartyInviteReceived.AddLambda(
						[this, InviteePlayer, OnInviteReceivedAssertions, OnStepComplete, OnInviteReceivedHandler](
						const FPragmaPartyInvite& Result)
						{
							if (OnInviteReceivedAssertions != nullptr)
							{
								OnInviteReceivedAssertions(Result);
							}
							(*OnStepComplete)();
							InviteePlayer->Session()->GameLoopApi().OnPartyInviteReceived.Remove(*OnInviteReceivedHandler);
						});

					Session()->GameLoopApi().SendPartyInvite(InviteePlayer->Session()->Id(),
						UPragmaGameLoopApi::FOnInviteSentDelegate::CreateLambda(
							[this](const TPragmaResult<FString>& Result)
							{
								if (Result.IsFailure())
								{
									OwningTest()->Fail("SendInvite failed");
								}
							}
						));
				});
		}

		void FTestPlayer::RespondToInviteWithAccept(TSharedRef<FString> InviteId,
			const TSharedRef<FTestPlayer>& Inviter,
			TFunction<void(const UPragmaPartyPlayer*)> OnPlayerJoinedAssertions) const
		{
			OwningTest()->AddStepLatent(
				[this, Inviter, InviteId, OnPlayerJoinedAssertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					const auto PromiseAll = MakeShared<TPromiseAll<bool>>();
					auto PlayerJoinedPromise = MakeShared<TPromise<bool>>();
					auto JoinedPartyPromise = MakeShared<TPromise<bool>>();
					PromiseAll->Add(PlayerJoinedPromise->GetFuture());
					PromiseAll->Add(JoinedPartyPromise->GetFuture());
					PromiseAll->Initialize();
					PromiseAll->GetFuture().Next([OnStepComplete](auto) { (*OnStepComplete)(); });

					const auto OnPlayerJoinedHandler = MakeShared<FDelegateHandle>();
					*OnPlayerJoinedHandler = Inviter->Session()->GameLoopApi().OnPlayerJoinedParty.AddLambda(
						[this, Inviter, OnPlayerJoinedAssertions, PlayerJoinedPromise, PromiseAll, OnPlayerJoinedHandler
						](
						const UPragmaPartyPlayer* NewPlayer)
						{
							if (OnPlayerJoinedAssertions != nullptr)
							{
								OnPlayerJoinedAssertions(NewPlayer);
							}
							PlayerJoinedPromise->EmplaceValue();
							Inviter->Session()->GameLoopApi().OnPlayerJoinedParty.Remove(*OnPlayerJoinedHandler);
						});

					const auto OnJoinedPartyHandler = MakeShared<FDelegateHandle>();
					*OnJoinedPartyHandler = Session()->GameLoopApi().OnJoinedParty.AddLambda(
						[this, JoinedPartyPromise, PromiseAll, OnJoinedPartyHandler](const UPragmaParty*)
						{
							JoinedPartyPromise->EmplaceValue();
							Session()->GameLoopApi().OnJoinedParty.Remove(*OnJoinedPartyHandler);
						});

					Session()->GameLoopApi().RespondToPartyInvite({}, *InviteId, true, GameServerZoneToPing,
						UPragmaGameLoopApi::FOnCompleteDelegate::CreateLambda(
							[this](const TPragmaResult<>& Result)
							{
								if (Result.IsFailure())
								{
									OwningTest()->Fail("RespondToPartyInvite failed");
								}
							}
						));
				});
		}

		void FTestPlayer::RespondToInviteWithRejection(TSharedRef<FString> InviteId,
			TFunction<void()> OnInviteRejectedAssertions) const
		{
			OwningTest()->AddStepLatent(
				[this, InviteId, OnInviteRejectedAssertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					Session()->GameLoopApi().RespondToPartyInvite({}, *InviteId, false, GameServerZoneToPing,
						UPragmaGameLoopApi::FOnCompleteDelegate::CreateLambda(
							[this, OnInviteRejectedAssertions, OnStepComplete](const TPragmaResult<>& Result)
							{
								if (Result.IsFailure())
								{
									OwningTest()->Fail("RespondToPartyInvite failed");
								}
								if (OnInviteRejectedAssertions != nullptr)
								{
									OnInviteRejectedAssertions();
								}
								(*OnStepComplete)();
							}
						));
				});
		}

		void FTestPlayer::JoinWithInviteCode(const TSharedRef<FTestPlayer>& PlayerInPartyToJoin,
			TFunction<void(const UPragmaPartyPlayer*)> OnPlayerJoinedAssertions) const
		{
			OwningTest()->AddStepLatent(
				[this, OnPlayerJoinedAssertions, PlayerInPartyToJoin](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					const auto PromiseAll = MakeShared<TPromiseAll<bool>>();
					auto PlayerJoinedPromise = MakeShared<TPromise<bool>>();
					auto PartyJoinedPromise = MakeShared<TPromise<bool>>();
					PromiseAll->Add(PlayerJoinedPromise->GetFuture());
					PromiseAll->Add(PartyJoinedPromise->GetFuture());
					PromiseAll->Initialize();
					PromiseAll->GetFuture().Next([OnStepComplete](auto) { (*OnStepComplete)(); });

					const auto OnPlayerJoinedHandler = MakeShared<FDelegateHandle>();
					*OnPlayerJoinedHandler = PlayerInPartyToJoin->Session()->GameLoopApi().OnPlayerJoinedParty.AddLambda(
						[this, PlayerInPartyToJoin, OnPlayerJoinedAssertions, PlayerJoinedPromise, PromiseAll,
							OnPlayerJoinedHandler](
						const UPragmaPartyPlayer* NewPlayer)
						{
							if (OnPlayerJoinedAssertions != nullptr)
							{
								OnPlayerJoinedAssertions(NewPlayer);
							}
							PlayerJoinedPromise->EmplaceValue();
							PlayerInPartyToJoin->Session()->GameLoopApi().OnPlayerJoinedParty.Remove(*OnPlayerJoinedHandler);
						});

					const auto OnJoinedPartyHandler = MakeShared<FDelegateHandle>();
					*OnJoinedPartyHandler = Session()->GameLoopApi().OnJoinedParty.AddLambda(
						[this, PartyJoinedPromise, PromiseAll, OnJoinedPartyHandler](const UPragmaParty*)
						{
							PartyJoinedPromise->EmplaceValue();
							Session()->GameLoopApi().OnJoinedParty.Remove(*OnJoinedPartyHandler);
						});

					Session()->GameLoopApi().JoinPartyWithInviteCode({},
						PlayerInPartyToJoin->Session()->GameLoopApi().GetParty()->GetInviteCode(),
						GameServerZoneToPing,
						UPragmaGameLoopApi::FOnCompleteDelegate::CreateLambda(
							[this](const TPragmaResult<>& Result)
							{
								if (Result.IsFailure())
								{
									OwningTest()->Fail("JoinWithInviteCode failed");
								}
							}
						));
				});
		}

		void FTestPlayer::JoinWithPartyId(const TSharedRef<FTestPlayer>& PlayerInPartyToJoin,
			TFunction<void(const UPragmaPartyPlayer*)> OnPlayerJoinedAssertions) const
		{
			OwningTest()->AddStepLatent(
				[this, OnPlayerJoinedAssertions, PlayerInPartyToJoin](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					const auto PromiseAll = MakeShared<TPromiseAll<bool>>();
					auto PlayerJoinedPromise = MakeShared<TPromise<bool>>();
					auto JoinedPartyPromise = MakeShared<TPromise<bool>>();
					PromiseAll->Add(PlayerJoinedPromise->GetFuture());
					PromiseAll->Add(JoinedPartyPromise->GetFuture());
					PromiseAll->Initialize();
					PromiseAll->GetFuture().Next([OnStepComplete](auto) { (*OnStepComplete)(); });

					const auto OnPlayerJoinedHandler = MakeShared<FDelegateHandle>();
					*OnPlayerJoinedHandler = PlayerInPartyToJoin->Session()->GameLoopApi().OnPlayerJoinedParty.AddLambda(
						[this, PlayerInPartyToJoin, OnPlayerJoinedAssertions, PlayerJoinedPromise, PromiseAll,
							OnPlayerJoinedHandler](
						const UPragmaPartyPlayer* NewPlayer)
						{
							if (OnPlayerJoinedAssertions != nullptr)
							{
								OnPlayerJoinedAssertions(NewPlayer);
							}
							PlayerJoinedPromise->EmplaceValue();
							PlayerInPartyToJoin->Session()->GameLoopApi().OnPlayerJoinedParty.Remove(*OnPlayerJoinedHandler);
						});

					const auto OnJoinedPartyHandler = MakeShared<FDelegateHandle>();
					*OnJoinedPartyHandler = Session()->GameLoopApi().OnJoinedParty.AddLambda(
						[this, JoinedPartyPromise, PromiseAll, OnJoinedPartyHandler](const UPragmaParty*)
						{
							JoinedPartyPromise->EmplaceValue();
							Session()->GameLoopApi().OnJoinedParty.Remove(*OnJoinedPartyHandler);
						});

					Session()->GameLoopApi().JoinPartyWithId({},
						PlayerInPartyToJoin->Session()->GameLoopApi().GetParty()->GetId(),
						GameServerZoneToPing,
						UPragmaGameLoopApi::FOnCompleteDelegate::CreateLambda(
							[this](const TPragmaResult<>& Result)
							{
								if (Result.IsFailure())
								{
									OwningTest()->Fail("JoinWithPartyId failed");
								}
							}
						));
				});
		}

		void FTestPlayer::UpdatePartyPlayer(const FPragma_Party_ExtUpdatePartyPlayerRequest& ExtRequest,
			TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent(
				[this, ExtRequest, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					Session()->GameLoopApi().UpdatePartyPlayer(ExtRequest,
						OnCompleteDelegate(Assertions, OnStepComplete));
				});
		}

		void FTestPlayer::UpdateParty(const FPragma_Party_ExtUpdatePartyRequest& ExtRequest,
			TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent(
				[this, ExtRequest, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					Session()->GameLoopApi().UpdateParty(ExtRequest,
						OnCompleteDelegate(Assertions, OnStepComplete));
				});
		}

		void FTestPlayer::SetPreferredGameServerZones(const TArray<FString>& GameServerZones,
			TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent(
				[this, GameServerZones, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					Session()->GameLoopApi().SetPartyPreferredGameServerZones(GameServerZones,
						OnCompleteDelegate(Assertions, OnStepComplete));
				});
		}

		void FTestPlayer::SetGameServerZoneToPing(const TMap<FString, int32>& ZoneToPing,
			TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent(
				[this, ZoneToPing, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
				{
					Session()->GameLoopApi().SetPartyPlayerGameServerZoneToPing(ZoneToPing,
						OnCompleteDelegate(Assertions, OnStepComplete));
				});
		}

		void FTestPlayer::SetReadyForPlayer(const bool bReady, TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent([this, bReady, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
			{
				Session()->GameLoopApi().SetPartyPlayerReady(bReady, OnCompleteDelegate(Assertions, OnStepComplete));
			});
		}

		void FTestPlayer::EnterMatchmaking(TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent([this, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
			{
				Session()->GameLoopApi().EnterMatchmaking(OnCompleteDelegate(Assertions, OnStepComplete));
			});
		}

		void FTestPlayer::LeaveMatchmaking(TFunction<void(TPragmaResult<>)> Assertions) const
		{
			OwningTest()->AddStepLatent([this, Assertions](const FAsyncTest::FOnStepComplete& OnStepComplete)
			{
				Session()->GameLoopApi().LeaveMatchmaking(OnCompleteDelegate(Assertions, OnStepComplete));
			});
		}
	}
}
