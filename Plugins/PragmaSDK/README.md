# Pragma SDK - Unreal Engine 4.

## Getting Started

This code should be added to your `<UNREAL PROJECT ROOT>/Plugins/PragmaSDK` folder.

## Updating to a new version of the SDK

First you need to run `make gen-sdk-types-unreal4` in your `pragma-engine/platform` folder. This generates and copies protobuf types to this SDK location.

We recommending doing a **full stomp** of your PragmaSDK folder with this SDK folder after the above each time you want to update. This will ensure you have the most recent SDK changes from Pragma as well as updated proto types.

There is a script that automates this process in `pragma-engine/sdk/unreal4`.

