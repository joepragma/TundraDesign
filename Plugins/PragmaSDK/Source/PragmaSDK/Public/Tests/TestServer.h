#pragma once

#include "CoreMinimal.h"

#include "Tests/AsyncTest.h"
#include "PragmaPtr.h"

class UPragmaSdkConfig;

PRAGMA_FWD(FServer);

namespace Pragma
{
namespace Tests
{

//
// This class wraps a PragmaPlayer during a test and provides common convenience functions.
//
// All functions operate by adding steps to the OwningTest.
//
class PRAGMASDK_API FTestServer final : public TSharedFromThis<FTestServer>
{
public:
	DECLARE_DELEGATE(FEndedDelegate);
	
	explicit FTestServer(TSharedPtr<FAsyncTest> OwningTest);
	~FTestServer();

	FServerPtr Session() const { return Server; }

	//
	// The owning test.
	//
	TSharedRef<FAsyncTest> OwningTest() const { return TestPtr.ToSharedRef(); }

	void ConnectOrFail() const;

private:
	TSharedPtr<FAsyncTest> TestPtr;

public:
	FServerPtr Server;
};
}
}
