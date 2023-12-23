#include "CoreMinimal.h"
#include "MockBackoff.h"
#include "Tests/MockWebSocket.h"
#include "Tests/TestDto.h"
#include "Dto/PragmaCoreDto.h"
#include "Tests/PragmaAutomationSpecBase.h"
#include "PragmaWebSocket.h"

namespace Pragma
{
	namespace Tests
	{
		PRAGMA_BEGIN_DEFINE_SPEC(FPragmaWebSocketTest, "PragmaSDK.WebSocket")

			TSharedPtr<FMockBackoff> MockBackoff;
			TSharedPtr<FPragmaWebSocket> Socket;
			TSharedPtr<FMockWebSocket> MockWebSocket;
			float ExpectedDelay = 123.0;
			bool OnConnectedCalled = false;
			bool OnDisconnectedCalled = false;

			// Fast path initialize the Socket so it's ready for messages.
			void ConnectSuccessfully() const
			{
				Socket->Init("blah");
				Socket->SetAuthToken("12345");
				Socket->Connect();
				MockWebSocket->MockConnect();
			}

			struct FJumpTableInitData
			{
				UScriptStruct* StaticStruct;
				FString Name;
			};

			void InitJumpTable(TArray<FJumpTableInitData> InitData) const
			{
				int i = 1;
				TArray<FPragmaBackendTypeData> TypeData;
				for (const auto& Data : InitData)
				{
					TypeData.Add(FPragmaBackendTypeData{Data.StaticStruct, Data.Name, i++});
				}
				const auto BackendTypeConverter = FPragmaBackendTypeConverter::GetTestInstance(MoveTemp(TypeData));
				Socket->SetBackendTypeConverter(BackendTypeConverter);
			}

			void InitJumpTableDefaults() const
			{
				InitJumpTable({
					{FPragmaTestRequestPayload::StaticStruct(), "FPragmaTestRequestPayload"},
					{FPragmaTestResponsePayload::StaticStruct(), "FPragmaTestResponsePayload"},
					{FPragmaTestNotificationPayload0::StaticStruct(), "FPragmaTestNotificationPayload0"},
					{FPragmaTestNotificationPayload1::StaticStruct(), "FPragmaTestNotificationPayload1"}
				});
			}

		PRAGMA_END_DEFINE_SPEC(FPragmaWebSocketTest)

		TSharedPtr<FPragmaWebSocket> MakeSocket(TFunction<TSharedRef<FMockWebSocket>(const FString& ApiUrl, const FString& Protocol, const FUpgradeHeaders& UpgradeHeaders)> InFunctor,
			TSharedPtr<FMockBackoff> InMockBackoff)
		{
			return MakeShared<FPragmaWebSocket>(FPragmaWebSocket::FCreateWebSocket::CreateLambda(InFunctor),
				InMockBackoff.Get());
		}

		void FPragmaWebSocketTest::Define()
		{
			BeforeEach([this]()
			{
				MockWebSocket = MakeShared<FMockWebSocket>();
				MockBackoff = MakeShared<FMockBackoff>();
				Socket = MakeSocket([this](const FString&, const FString&, const FUpgradeHeaders&)
				{
					return MockWebSocket.ToSharedRef();
				}, MockBackoff);

				InitJumpTableDefaults();
			});

			Describe("SetAuthToken()", [this]()
			{
				It(
					"changes out the underlying auth token for the new websocket created when websocket goes degraded and attempts reconnect",
					[this]()
					{
						ExpectErrorOrWarning(*this, TEXT("FPragmaWebSocket::HandleWebSocketClosed"));
						MockBackoff->IsBackoffFuncSetResult = false;
						int CreationCallCount = 0;
						const FString& ApiUrl = "127.0.0.1:9000";
						TMap<FString, FString> InUpgradeHeaders;

						Socket = MakeSocket([this, &CreationCallCount, &InUpgradeHeaders](const FString& ApiUrl, const FString& Protocol, const FUpgradeHeaders& UpgradeHeaders)
						{
							CreationCallCount = CreationCallCount + 1;
							InUpgradeHeaders.Append(UpgradeHeaders);
							return MockWebSocket.ToSharedRef();
						}, MockBackoff.ToSharedRef());
						Socket->Init(ApiUrl);
						Socket->SetAuthToken("12345");
						Socket->Connect();
						Socket->OnDisconnected().AddLambda([this](FPragmaWebSocket&)
						{
							UE_LOG(LogTemp, Error, TEXT("Disconnected event was picked up"));
						});

						Socket->SetAuthToken("hello world");
						MockWebSocket->MockDisconnect(1006, "", false);

						TestEqual("Websocket is recreated", CreationCallCount, 2);
						TestTrue("Found Authorization header", InUpgradeHeaders.Contains("Authorization"));
						TestEqual("Found Accept header", InUpgradeHeaders["Accept"], "application/json");
						TestTrue("Found pragma-reconnect header", InUpgradeHeaders.Contains("pragma-reconnect"));
					});
			});

			Describe("Connect()", [this]()
			{
				BeforeEach([this]()
				{
					OnConnectedCalled = false;
					OnDisconnectedCalled = false;
					Socket->OnConnected().AddLambda([this](FPragmaWebSocket& CallbackSocket)
					{
						OnConnectedCalled = true;
					});
					Socket->OnDisconnected().AddLambda([this](FPragmaWebSocket& CallbackSocket)
					{
						OnDisconnectedCalled = true;
					});
				});

				It("should call OnConnected when connected successfully", [this]()
				{
					Socket->Init("127.0.0.1:9000");
					Socket->SetAuthToken("12345");
					Socket->Connect();
					MockWebSocket->MockConnect();
					TestTrue("OnConnected() was called", OnConnectedCalled);
					TestFalse("OnDisconnected() was called", OnDisconnectedCalled);
					TestTrue("Backoff->Cancel() was called", MockBackoff->CancelCalled);
				});

				It("should result in failure when no ApiUrl is set", [this]()
				{
					ExpectErrorOrWarning(*this, TEXT("FPragmaWebSocket::Connect"));
					// Socket->Init("127.0.0.1:9000"); Intentionally excluded to check for error.
					Socket->SetAuthToken("12345");
					Socket->Connect();
					TestFalse("OnConnected() was called", OnConnectedCalled);
					TestTrue("OnDisconnected() was called", OnDisconnectedCalled);
				});

				It("should not call OnDisconnected when there is a websocket connection error", [this]()
				{
					ExpectErrorOrWarning(*this, TEXT("FPragmaWebSocket::HandleWebSocketConnectionError"));
					Socket->Init("127.0.0.1:9000");
					Socket->SetAuthToken("12345");
					Socket->Connect();
					MockWebSocket->MockConnectionError("blah error");
					TestFalse("OnConnected() was called", OnConnectedCalled);
					TestFalse("OnDisconnected() was called", OnDisconnectedCalled);
				});

				It("should call OnDisconnected when the websocket closes cleanly", [this]()
				{
					Socket->Init("127.0.0.1:9000");
					Socket->SetAuthToken("12345");
					Socket->Connect();
					MockWebSocket->MockDisconnect(1000, "As requested", true);
					TestFalse("OnConnected() was called", OnConnectedCalled);
					TestTrue("OnDisconnected() was called", OnDisconnectedCalled);
				});

				It("should set ConnectionStatus to Connecting", [this]()
				{
					Socket = MakeSocket([this](const FString& ApiUrl, const FString& Protocol, const FUpgradeHeaders& UpgradeHeaders)
					{
						TestEqual("ConnectionStatus set", Socket->GetConnectionStatus(),
							EPragmaConnectionStatus::Connecting);
						return MockWebSocket.ToSharedRef();
					}, MockBackoff);
				});
			});

			Describe("SendMessage()", [this]()
			{
				It(
					"should call OnComplete with a successful TPragmaResult with the deserialized payload when server sends a successful response",
					[this]()
					{
						ConnectSuccessfully();

						bool bResponseCallbackCalled = false;
						const FPragmaTestRequestPayload RequestPayload{"ping"};
						const FPragmaTestResponsePayload ResponsePayload{"pong"};
						auto ResponseCallback = [this, &bResponseCallbackCalled, &ResponsePayload](
							const TPragmaResult<FPragmaTestResponsePayload> Result, const FPragmaMessageMetadata&)
						{
							bResponseCallbackCalled = true;
							TestTrue("Result.IsSuccessful()", Result.IsSuccessful());
							TestEqual("Response data", Result.Payload().ResStr, ResponsePayload.ResStr);
						};

						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(ResponseCallback));

						const int32 RequestId = MockWebSocket->GetMockLastSendRequestId();
						TestTrue("MockServerMessage 2",
							MockWebSocket->MockServerResponse(RequestId, "FPragmaTestResponsePayload",
								ResponsePayload));
						TestTrue("Response callback was called", bResponseCallbackCalled);
					});

				It(
					"should call OnComplete with a failed TPragmaResult with no payload when server sends an error response",
					[this]()
					{
						ExpectErrorOrWarning(*this, TEXT("RESPONSE ERROR "));
						ConnectSuccessfully();

						bool bResponseCallbackCalled = false;
						const FPragmaTestRequestPayload RequestPayload{"ping"};
						const FString ExpectedErrorCode = "EPragma_PlatformError::AccountService_Error";
						auto ResponseCallback = [this, &bResponseCallbackCalled, &ExpectedErrorCode](
							const TPragmaResult<FPragmaTestResponsePayload> Result, const FPragmaMessageMetadata&)
						{
							bResponseCallbackCalled = true;
							TestFalse("Result.IsSuccessful()", Result.IsSuccessful());
							TestEqual("Response ErrorCode", Result.ErrorCode(), ExpectedErrorCode);
							TestTrue("Response Error IsPlatform()", Result.Error().IsPlatform());
						};

						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(ResponseCallback));

						const int32 RequestId = MockWebSocket->GetMockLastSendRequestId();
						TestTrue("MockServerMessage 2",
							MockWebSocket->MockServerMessageError(RequestId, "AccountService_Error"));
						TestTrue("Response callback was called", bResponseCallbackCalled);
					});

				// Skip this test currently until IsExt, etc check doesn't rely on default values of the enums
				xIt(
					"should call OnComplete with a failed TPragmaResult with no payload when server sends an error response with ExtError",
					[this]()
					{
						ExpectErrorOrWarning(*this, TEXT("RESPONSE ERROR "));
						ConnectSuccessfully();

						bool bResponseCallbackCalled = false;
						const FPragmaTestRequestPayload RequestPayload{"ping"};
						const FString ExpectedErrorCode = "UNKNOWN_ERROR";
						auto ResponseCallback = [this, &bResponseCallbackCalled, &ExpectedErrorCode](
							const TPragmaResult<FPragmaTestResponsePayload> Result, const FPragmaMessageMetadata&)
						{
							bResponseCallbackCalled = true;
							TestFalse("Result.IsSuccessful()", Result.IsSuccessful());
							TestEqual("Response ErrorCode", Result.ErrorCode(), ExpectedErrorCode);
							// TestTrue("Response Error IsExt()", Result.Error().IsExt());
						};

						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(ResponseCallback));

						const int32 RequestId = MockWebSocket->GetMockLastSendRequestId();
						TestTrue("MockServerMessage 2",
							MockWebSocket->MockServerMessageExtError(RequestId, "UNKNOWN_EXT_ERROR"));
						TestTrue("Response callback was called", bResponseCallbackCalled);
					});

				It("should call OnComplete handlers in the order that responses come in, not the order of requests", [
						this]()
					{
						ConnectSuccessfully();

						// Setup 3 different response handlers.
						// Send 3 messages.
						// Mock the responses in a different order.
						// Test that our flags flip in the correct order.
						bool bCallback0Called = false;
						bool bCallback1Called = false;
						bool bCallback2Called = false;
						const FPragmaTestRequestPayload RequestPayload{"ping"};
						const FPragmaTestResponsePayload ResponsePayload{"pong"};
						auto Response0Callback = [&bCallback0Called](const TPragmaResult<FPragmaTestResponsePayload>,
							const FPragmaMessageMetadata&)
						{
							bCallback0Called = true;
						};
						auto Response1Callback = [&bCallback1Called](const TPragmaResult<FPragmaTestResponsePayload>,
							const FPragmaMessageMetadata&)
						{
							bCallback1Called = true;
						};
						auto Response2Callback = [&bCallback2Called](const TPragmaResult<FPragmaTestResponsePayload>,
							const FPragmaMessageMetadata&)
						{
							bCallback2Called = true;
						};

						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(Response0Callback));
						const int32 RequestId0 = MockWebSocket->GetMockLastSendRequestId();
						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(Response1Callback));
						const int32 RequestId1 = MockWebSocket->GetMockLastSendRequestId();
						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
							MoveTemp(Response2Callback));
						const int32 RequestId2 = MockWebSocket->GetMockLastSendRequestId();

						TestTrue("MockServerMessage 2",
							MockWebSocket->MockServerResponse(RequestId2, "FPragmaTestResponsePayload",
								ResponsePayload));
						TestFalse("Response 0 callback was NOT called", bCallback0Called);
						TestFalse("Response 1 callback was NOT called", bCallback1Called);
						TestTrue("Response 2 callback was called", bCallback2Called);

						TestTrue("MockServerMessage 0",
							MockWebSocket->MockServerResponse(RequestId0, "FPragmaTestResponsePayload",
								ResponsePayload));
						TestTrue("Response 0 callback was called", bCallback0Called);
						TestFalse("Response 1 callback was NOT called", bCallback1Called);

						TestTrue("MockServerMessage 1",
							MockWebSocket->MockServerResponse(RequestId1, "FPragmaTestResponsePayload",
								ResponsePayload));
						TestTrue("Response 1 callback was called", bCallback1Called);
					});

				It("should wrap serialized payload with a serialized envelope", [this]()
				{
					ConnectSuccessfully();
					const FPragmaTestRequestPayload Payload{"test"};
					auto ResponseCallback = [](TPragmaResult<FPragmaTestResponsePayload>, const FPragmaMessageMetadata&)
					{
					};
					Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(Payload,
						MoveTemp(ResponseCallback));

					const TSharedRef<FJsonObject> RequestObj = MockWebSocket->GetMockLastSentMessageObj();
					const TSharedPtr<FJsonObject> PayloadObj = FPragmaRequestJson::GetPayload(RequestObj);
					// Test envelope.
					FPragmaRequestJson Request;
					const bool bRequestDeserialized = PragmaJson::JsonObjToStruct(RequestObj, Request);
					TestTrue("Message deserialized to FPragmaRequestJson", bRequestDeserialized);
					if (!bRequestDeserialized)
					{
						return;
					}
					TestEqual("Type", Request.Type, "FPragmaTestRequestPayload");
					TestTrue("Request ID is set", Request.RequestId > -1);
					// Test payload.
					FPragmaTestRequestPayload ActualPayload;
					const bool bPayloadDeserialized = PragmaJson::JsonObjToStruct(PayloadObj.ToSharedRef(),
						ActualPayload);
					TestTrue("Message Payload deserialized to FPragmaTestRequestPayload", bPayloadDeserialized);
					if (!bPayloadDeserialized)
					{
						return;
					}
					TestEqual("Payload.Str", ActualPayload.ReqStr, Payload.ReqStr);
				});

				It("should call OnComplete with failure when the socket is not connected", [this]()
				{
					ExpectErrorOrWarning(*this, TEXT("FPragmaProtocol::SendJsonMessage"));
					bool bCalled = false;
					const FPragmaTestRequestPayload Request{"test"};
					auto ResponseCallback = [this, &bCalled](const TPragmaResult<FPragmaTestResponsePayload> Result,
						const FPragmaMessageMetadata&)
					{
						bCalled = true;
						TestFalse("IsSuccessful", Result.IsSuccessful());
					};
					Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(Request,
						MoveTemp(ResponseCallback));
					TestTrue("OnComplete called", bCalled);
				});

				It("should error and call OnComplete with a failed TPragmaResult when the type is invalid", [this]()
				{
					ConnectSuccessfully();

					// Overwrite jump table defaults with empty so nothing will work.
					InitJumpTable({});

					bool bResponseCallbackCalled = false;
					const FPragmaTestRequestPayload RequestPayload{"ping"};
					const FPragmaTestResponsePayload ResponsePayload{"pong"};
					auto ResponseCallback = [this, &bResponseCallbackCalled, &ResponsePayload](
						const TPragmaResult<FPragmaTestResponsePayload> Result, const FPragmaMessageMetadata&)
					{
						bResponseCallbackCalled = true;
						TestFalse("Result.IsSuccessful()", Result.IsSuccessful());
					};

					ExpectErrorOrWarning(*this, TEXT("FPragmaProtocol::SendMessage"));
					Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(RequestPayload,
						MoveTemp(ResponseCallback));

					TestTrue("Response callback was called", bResponseCallbackCalled);
				});

				//
				// While these deal with input from the server, it's important to test that we raise issues correctly when that
				// input is unexpected/malformed.
				//

				It("should error when we fail to deserialize the response envelope", [this]()
				{
					// If we can't deserialize the envelope, we can't get a message ID, so we don't know which callback to call.
					// So the test can't test inside the callback, we just have to expect an error.
					ExpectErrorOrWarning(*this, TEXT("FPragmaProtocol::JsonMessageReceived"));
					ConnectSuccessfully();
					bool bCalled = false;
					const FPragmaTestRequestPayload Payload{"test"};
					auto ResponseCallback = [this, &bCalled](const TPragmaResult<FPragmaTestResponsePayload> Result,
						const FPragmaMessageMetadata&)
					{
						bCalled = true;
					};
					Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(Payload,
						MoveTemp(ResponseCallback));
					MockWebSocket->MockServerMessageFullJson("{this is invalid json}");
					TestFalse("Response callback was called", bCalled);
				});

				It(
					"should call OnComplete with a failure TPragmaResult when failed to deserialize the response payload",
					[this]()
					{
						ConnectSuccessfully();
						bool bCalled = false;
						const FPragmaTestRequestPayload Payload{"ping"};
						auto ResponseCallback = [this, &bCalled](const TPragmaResult<FPragmaTestResponsePayload> Result,
							const FPragmaMessageMetadata&)
						{
							bCalled = true;
							TestFalse("Response successful", Result.IsSuccessful());
						};
						Socket->SendMessage<FPragmaTestRequestPayload, FPragmaTestResponsePayload>(Payload,
							MoveTemp(ResponseCallback));

						// Expect an error about unable to serialize json.
						ExpectErrorOrWarning(*this, TEXT("FPragmaProtocol::JsonMessageReceived"));
						const FString MessageJson = R"({"response":{"payload":"invalid json object}})";
						MockWebSocket->MockServerMessageFullJson(MessageJson);
						TestFalse("Response callback was called", bCalled);
					});
			});

			Describe("AddNotificationHandler()", [this]()
			{
				It("should call handlers with a successful Result when a notification arrives", [this]()
				{
					ConnectSuccessfully();
					bool bHandlerCalled = false;
					const FString ResponseStr = "hello!";
					Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(
						[this, &bHandlerCalled, &ResponseStr](const FPragmaTestNotificationPayload0& Payload,
						const FPragmaMessageMetadata&)
						{
							bHandlerCalled = true;
							TestEqual("Result payload str", Payload.ResStr, ResponseStr);
						});
					TestTrue("Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
							FPragmaTestNotificationPayload0{ResponseStr}));
					TestTrue("Handler called", bHandlerCalled);
				});

				It("should call all registered handlers when a notification arrives", [this]()
				{
					ConnectSuccessfully();
					bool bHandlerCalled0 = false;
					bool bHandlerCalled1 = false;
					Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(
						[&bHandlerCalled0](const FPragmaTestNotificationPayload0&, const FPragmaMessageMetadata&)
						{
							bHandlerCalled0 = true;
						});
					Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(
						[&bHandlerCalled1](const FPragmaTestNotificationPayload0&, const FPragmaMessageMetadata&)
						{
							bHandlerCalled1 = true;
						});
					TestTrue("Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
							FPragmaTestNotificationPayload0{"hi"}));
					TestTrue("Handler 0 called", bHandlerCalled0);
					TestTrue("Handler 1 called", bHandlerCalled1);
				});

				It("should return a different id for each handler added", [this]()
				{
					auto MakeEmptyHandler = []()
					{
						return [](const FPragmaTestNotificationPayload0& Result, const FPragmaMessageMetadata&)
						{
						};
					};
					TSet<FPragmaWebSocket::FNotificationHandlerId> SeenIds;
					for (int i = 0; i < 5; ++i)
					{
						bool bIsAlreadySet = false;
						SeenIds.Add(Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(MakeEmptyHandler()),
							&bIsAlreadySet);
						TestFalse("Already seen notification id", bIsAlreadySet);
					}
				});

				It("should call only handlers registered for specific notifications", [this]()
				{
					ConnectSuccessfully();
					bool bHandlerCalled0 = false;
					bool bHandlerCalled1 = false;
					Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(
						[&bHandlerCalled0](const FPragmaTestNotificationPayload0&, const FPragmaMessageMetadata&)
						{
							bHandlerCalled0 = true;
						});
					Socket->AddNotificationHandler<FPragmaTestNotificationPayload1>(
						[&bHandlerCalled1](const FPragmaTestNotificationPayload1&, const FPragmaMessageMetadata&)
						{
							bHandlerCalled1 = true;
						});
					TestTrue("FPragmaTestNotificationPayload0: 1st Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
							FPragmaTestNotificationPayload0{"hi"}));
					TestTrue("FPragmaTestNotificationPayload0: Handler 0 called", bHandlerCalled0);
					TestFalse("FPragmaTestNotificationPayload0: Handler 1 called", bHandlerCalled1);
					bHandlerCalled0 = false;
					bHandlerCalled1 = false;
					TestTrue("FPragmaTestNotificationPayload1: Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload1",
							FPragmaTestNotificationPayload1{"hi"}));
					TestFalse("FPragmaTestNotificationPayload1: Handler 0 called", bHandlerCalled0);
					TestTrue("FPragmaTestNotificationPayload1: Handler 1 called", bHandlerCalled1);
				});
			});

			Describe("RemoveNotificationHandler()", [this]()
			{
				It("should not call handlers after they have been removed", [this]()
				{
					ExpectErrorOrWarning(*this, TEXT("FPragmaProtocol::HandleWebSocketTextMessage"));
					ConnectSuccessfully();
					bool bHandlerCalled = false;
					const FString ResponseStr = "hello!";
					const FPragmaWebSocket::FNotificationHandlerId HandlerId = Socket->AddNotificationHandler<
						FPragmaTestNotificationPayload0>(
						[this, &bHandlerCalled, &ResponseStr](const FPragmaTestNotificationPayload0& Result,
						const FPragmaMessageMetadata&)
						{
							bHandlerCalled = true;
						});
					TestTrue("Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
							FPragmaTestNotificationPayload0{ResponseStr}));
					TestTrue("Handler called", bHandlerCalled);
					bHandlerCalled = false;
					Socket->RemoveNotificationHandler<FPragmaTestNotificationPayload0>(HandlerId);
					TestTrue("Server message sent",
						MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
							FPragmaTestNotificationPayload0{ResponseStr}));
					TestFalse("Handler called after removal", bHandlerCalled);
				});

				It(
					"should still call other handlers of the same notification id that were not removed after removing a handler",
					[this]()
					{
						ConnectSuccessfully();

						bool bHandlerCalled0 = false;
						bool bHandlerCalled1 = false;
						const FPragmaWebSocket::FNotificationHandlerId HandlerId = Socket->AddNotificationHandler<
							FPragmaTestNotificationPayload0>(
							[&bHandlerCalled0](const FPragmaTestNotificationPayload0&, const FPragmaMessageMetadata&)
							{
								bHandlerCalled0 = true;
							});
						Socket->AddNotificationHandler<FPragmaTestNotificationPayload0>(
							[&bHandlerCalled1](const FPragmaTestNotificationPayload0&, const FPragmaMessageMetadata&)
							{
								bHandlerCalled1 = true;
							});
						TestTrue("Server message sent",
							MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
								FPragmaTestNotificationPayload0{"hi"}));
						TestTrue("Handler 0 called", bHandlerCalled0);
						TestTrue("Handler 1 called", bHandlerCalled1);
						Socket->RemoveNotificationHandler<FPragmaTestNotificationPayload0>(HandlerId);
						bHandlerCalled0 = false;
						bHandlerCalled1 = false;
						TestTrue("Server message sent",
							MockWebSocket->MockServerNotification("FPragmaTestNotificationPayload0",
								FPragmaTestNotificationPayload0{"hi"}));
						TestFalse("Handler 0 called after handler removal", bHandlerCalled0);
						TestTrue("Handler 1 called after other handler removed", bHandlerCalled1);
					});
			});

			Describe("Reconnect()", [this]()
			{
				It("Starts reconnect on error", [this]()
				{
					ExpectErrorOrWarning(*this, TEXT("FPragmaWebSocket::HandleWebSocketClosed"));
					ConnectSuccessfully();
					MockWebSocket->MockDisconnect(1234, "blah", false);

					TestEqual("Backoff->Continue called once", MockBackoff->ContinueCount, 1);
				});
			});
		}
	}
}
