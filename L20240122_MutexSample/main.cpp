#include <Windows.h>
#include <process.h>

int main()
{
	HANDLE OnceMutex = CreateMutexA(NULL, TRUE, "HellowWorld");//A-asci W-nui 
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"�ѹ��� �����ض�.", L"���", MB_OK);
		return 0;
	}

	while (true);

	CloseHandle(OnceMutex);

	return 0;
}//