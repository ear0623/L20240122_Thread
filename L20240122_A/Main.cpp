#include <process.h>
#include <Windows.h>
#include <iostream>

#pragma comment(lib,"winmm")// 윈도우 멀티미디어

using namespace std;

int Money = 0;
int Mymoney = 0;

CRITICAL_SECTION MoneySection;
CRITICAL_SECTION MoneySection2;

HANDLE MoneyMutex;

//race condition 동일한 메모리에 접근
unsigned Increase(void* Arg)
{//레이스 컨디션에 따른 돈복사는 DB왔다갔다해서그럼
	//EnterCriticalSection(&MoneySection);
	WaitForSingleObject(MoneyMutex, INFINITE);
	for (int i = 0; i < 10000; i++)//critical seession 
	{
		
		Money = Money + 1;//템복사 돈복사. // 
		Mymoney++;
		Sleep(0);// contextswich 발생시켜줌
	}
	ReleaseMutex(MoneyMutex);
	//LeaveCriticalSection(&MoneySection);// 동ㄱㅣ화 오브젝트 등등 락을 건거임 방법중 하나


	return 0;
}
unsigned Decrease(void* Arg)
{
	//EnterCriticalSection(&MoneySection);//실행중 방해금지
	WaitForSingleObject(MoneyMutex, INFINITE); 
	for (int i = 0; i < 10000; i++)
	{
		
		Mymoney--; 
		Money = Money - 1;
		
	}
	ReleaseMutex(MoneyMutex); 
	//LeaveCriticalSection(&MoneySection);
	return 0;
}
//main thread
int main()
{
	//Thread Pooling, IOCP;
	//KernalMode LockObject

	//USerMode Lock Object
	InitializeCriticalSection(&MoneySection);
	InitializeCriticalSection(&MoneySection2);
	DWORD StartTime= timeGetTime();//윈도우용/// 고해상도타임스템프 찾아보기
	MoneyMutex = CreateMutex(nullptr, FALSE, nullptr);//os 커널용

	//DWORD StartTime = timeGetTime();

	HANDLE ThreadHandle[2];

	ThreadHandle[0] = (HANDLE)_beginthreadex(nullptr, 0, Increase, nullptr, CREATE_SUSPENDED, nullptr);//메모리사이즈/함수이름/인자 플레그/주소
	
	ResumeThread(ThreadHandle[0]);
	ResumeThread(ThreadHandle[1]);

	SuspendThread(ThreadHandle[0]);
	SuspendThread(ThreadHandle[1]);
	
	ThreadHandle[1] = (HANDLE)_beginthreadex(nullptr, 0, Decrease, nullptr, 0, nullptr);//메모리사이즈/함수이름/인자 플레그/주소

	//handle-> 부탁한 일에 대한 번호표
	// 바로 리턴해줌 -> 핸들
	//주 스레드가 끝나면 자 식은 0
	

	Sleep(10);
	WaitForMultipleObjects(2, ThreadHandle, TRUE, INFINITE);
	//WaitForSingleObject(ThreadHandle1, INFINITE);
	//WaitForSingleObject(ThreadHandle2, INFINITE);

	cout << "ProcessTime : " << timeGetTime() - StartTime << endl;

	DeleteCriticalSection(&MoneySection2);
	DeleteCriticalSection(&MoneySection);

	CloseHandle(MoneyMutex);
	CloseHandle(ThreadHandle[0]);
	CloseHandle(ThreadHandle[1]);
	//mutx 클라리언트 중복방지
	//다른프로그램이랑 같이 동기화 혹은 같이 사용하려고, 사용

	cout << Money << endl;
	cout << Mymoney << endl;

	return 0;
}