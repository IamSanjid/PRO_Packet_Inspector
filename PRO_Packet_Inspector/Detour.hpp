#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <winnt.h>
#include <fstream>

#include "Detours/include/detours.h"

#if _WIN32 || _WIN64
#if _WIN64
#pragma comment(lib, "Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "Detours/lib.X86/detours.lib")
#endif
#endif

#pragma comment(lib, "Ws2_32.lib")

namespace Detour
{
	typedef void(__cdecl *ProcessCommandsType)(void*, void*);
	typedef void(__cdecl *SendPacketType)(void*, void*);

	unsigned long GameAssembly;
	ProcessCommandsType originalProcessCommands;
	SendPacketType originalSendPacket;

	void Print_System__String(void* ptr, bool isCSPacket)
	{
		/*
		 * Structure:
			Base Address (0x00)
			Length		 (0x08)
			Content		 (0x0C)	(a null byte is placed after each char)
		 */
		unsigned long addr = reinterpret_cast<unsigned long>(ptr);
		int len = *(int*)(addr + 0x8);
		if (isCSPacket)
		{
			std::cout << "[>] ";
		}
		for (int i = 0; i < len; i++)
		{
			std::cout << *(char*)(addr + 0xC + (i * 2));
		}
		std::cout << std::endl;
	}

	void __cdecl hookedProcessCommands(void* _this, void* strPtr)
	{
		Print_System__String(strPtr, false);
		// returning to the original function
		return originalProcessCommands(_this, strPtr);
	}

	void __cdecl hookedSendPacket(void* _this, void* strPtr)
	{
		Print_System__String(strPtr, true);
		// returning to the original function
		return originalSendPacket(_this, strPtr);
	}

	void InintDLL(LPVOID hModule)
	{
		// ah detours; they are awesome!!
		// some recommended initial calls of detour lib
		DisableThreadLibraryCalls((HMODULE)hModule);		
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		// get the handle of the actual game logic binary
		GameAssembly = reinterpret_cast<unsigned long>(GetModuleHandle(L"GameAssembly.dll"));
		// get the original ProcessCommands function address, '0x1480A0' is the offset
		originalProcessCommands = reinterpret_cast<ProcessCommandsType>(GameAssembly + 0x1480A0);
		// get the original SendPacket function address, '0x1392D0' is the offset
		originalSendPacket = reinterpret_cast<SendPacketType>(GameAssembly + 0x1392D0);
		
		// attach our function to the original function address
		DetourAttach(&(PVOID&)originalProcessCommands, hookedProcessCommands);
		DetourAttach(&(PVOID&)originalSendPacket, hookedSendPacket);

		DetourTransactionCommit();
	}
}