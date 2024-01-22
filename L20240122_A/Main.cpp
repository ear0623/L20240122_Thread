#include <process.h>
#include <Windows.h>
#include <iostream>

#pragma comment(lib,"winmm")// ������ ��Ƽ�̵��

using namespace std;

int Money = 0;
int Mymoney = 0;

CRITICAL_SECTION MoneySection;
CRITICAL_SECTION MoneySection2;

HANDLE MoneyMutex;

//race condition ������ �޸𸮿� ����
unsigned Increase(void* Arg)
{//���̽� ����ǿ� ���� ������� DB�Դٰ����ؼ��׷�
	//EnterCriticalSection(&MoneySection);
	WaitForSingleObject(MoneyMutex, INFINITE);
	for (int i = 0; i < 10000; i++)//critical seession 
	{
		
		Money = Money + 1;//�ۺ��� ������. // 
		Mymoney++;
		Sleep(0);// contextswich �߻�������
	}
	ReleaseMutex(MoneyMutex);
	//LeaveCriticalSection(&MoneySection);// ������ȭ ������Ʈ ��� ���� �ǰ��� ����� �ϳ�


	return 0;
}
unsigned Decrease(void* Arg)
{
	//EnterCriticalSection(&MoneySection);//������ ���ر���
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
	DWORD StartTime= timeGetTime();//�������/// ���ػ�Ÿ�ӽ����� ã�ƺ���
	MoneyMutex = CreateMutex(nullptr, FALSE, nullptr);//os Ŀ�ο�

	//DWORD StartTime = timeGetTime();

	HANDLE ThreadHandle[2];

	ThreadHandle[0] = (HANDLE)_beginthreadex(nullptr, 0, Increase, nullptr, CREATE_SUSPENDED, nullptr);//�޸𸮻�����/�Լ��̸�/���� �÷���/�ּ�
	
	ResumeThread(ThreadHandle[0]);
	ResumeThread(ThreadHandle[1]);

	SuspendThread(ThreadHandle[0]);
	SuspendThread(ThreadHandle[1]);
	
	ThreadHandle[1] = (HANDLE)_beginthreadex(nullptr, 0, Decrease, nullptr, 0, nullptr);//�޸𸮻�����/�Լ��̸�/���� �÷���/�ּ�

	//handle-> ��Ź�� �Ͽ� ���� ��ȣǥ
	// �ٷ� �������� -> �ڵ�
	//�� �����尡 ������ �� ���� 0
	

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
	//mutx Ŭ�󸮾�Ʈ �ߺ�����
	//�ٸ����α׷��̶� ���� ����ȭ Ȥ�� ���� ����Ϸ���, ���

	cout << Money << endl;
	cout << Mymoney << endl;

	return 0;
}