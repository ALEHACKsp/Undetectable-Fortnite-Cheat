#include "render.h"

VOID CreateConsole()
{
	AllocConsole();
	for (;;)
	{
		printf("Undetectable");
		printf("            Soft Aim");
	}
	
}



BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		
		SigsInitalize();
		for (;;)
		{
			CreateConsole();
			MH_Initialize();
		}
	}

	return TRUE;
}