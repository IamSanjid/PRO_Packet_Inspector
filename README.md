# PRO_Packet_Inspector
A simple function hooker which will print out all the on going packets of PRO Client. And only works with 32 Bit version of PRO. You'll need to find the function offsets by yourself of the 64 Bit version.

# Build
Visual Studio 2017 is enough to build this. (Make sure you got all the SDKs)

# How to update (This may not work in the future)
So, you just need to find the offset where the function resides in the GameAssembly.dLL binary.
You'll need Cheate Engine (this is the easiest tool).
1. Open CE and attach to PRO Client and open Mono -> Dissect mono(Ctrl+Alt+M).
2. Then find `Assembly-CSharp.dll` -> `DSSock` -> `methods` -> `ProcessCommands` and `Assembly-CSharp.dll` -> `DSSock` -> `methods` -> `SendPacket`
3. Then right click -> Jit, a Memory Viewer window will open and get the offset.
You may see something like this:
[![Example 1](https://i.imgur.com/Vbi4zFS.png)](https://i.imgur.com/Vbi4zFS.png)
Ignore all the `DSSock.ProcessCommands` things and notice `GameAssembly.dll + 1480A3` for this case.
And the offset is 0x1480A0 for ProcessCommands function in my case. How can you know for your case? It's simple just substract all the bytes which are lebelled with `DSSock.ProcessCommands`, for my case it's 3 bytes:
[![Example 2](https://i.imgur.com/SdAifWw.png)](https://i.imgur.com/SdAifWw.png)
So, 0x1480A3 - 3 = 0x1480A0, and don't forget to substract the bytes from the first encountered `GameAssembly.dll + offset` offset after ignoring all the bytes lebelled with `DSSock.ProcessCommands`.
Follow the same for `SendPacket` function.
