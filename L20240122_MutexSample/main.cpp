#include <Windows.h>
#include <process.h>

int main()
{
	HANDLE OnceMutex = CreateMutexA(NULL, TRUE, "HellowWorld");//A-asci W-nui 
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(nullptr, L"한번만 실행해라.", L"경고", MB_OK);
		return 0;
	}

	while (true);

	CloseHandle(OnceMutex);

	return 0;
}//