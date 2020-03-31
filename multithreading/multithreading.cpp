#include <iostream>// для cout
#include <ctime> //для time
#include <thread> //для работы с потоками
#include <algorithm> // подключаем sort
#include <vector> // подключаем vector
#include <windows.h>//winapi

#define SIZE 50000//размер массива
using namespace std;

struct MyStruct
{
	int* arr = new int[SIZE];//инициалиизация массива
	int* bubbleResult = new int[SIZE]; //Выделение памяти для массива
	int* quickResult = new int[SIZE]; //Выделение памяти для массива
};

DWORD WINAPI BubbleSort(LPVOID param) {
	MyStruct* mystrct = (MyStruct*)param;
	int temp;// временная переменная для обмена элементов местами
	for (int i = 0; i < SIZE - 1; i++) {// Сортировка массива пузырьком
		for (int j = 0; j < SIZE - i - 1; j++) {// меняем элементы местами
			if (mystrct->bubbleResult[j] > mystrct->bubbleResult[j + 1]) {
				temp = mystrct->bubbleResult[j];
				mystrct->bubbleResult[j] = mystrct->bubbleResult[j + 1];
				mystrct->bubbleResult[j + 1] = temp;
			}
		}
	}
	return 0;
}
DWORD WINAPI QuickSort(LPVOID param) {
	MyStruct* mystrct = (MyStruct*)param;
	vector<int> arrVector(mystrct->quickResult, mystrct->quickResult + SIZE);
	sort(arrVector.begin(), arrVector.end());//сортировка сравнением
	copy(arrVector.begin(), arrVector.end(), mystrct->quickResult);//преобразовываем вектор в массив элементов
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	cout << "Wait..." << endl;
	MyStruct* mystrct = new MyStruct;
	for (int i = 0; i < SIZE; i++)//заполнение массива рандомными числами [-25000;25000)
	{
		mystrct->arr[i] = rand() % SIZE - (SIZE / 2);
	}
	for (int i = 0; i < SIZE; i++) { //копируем изначальный массив в новые массивы
		mystrct->bubbleResult[i] = mystrct->arr[i];
		mystrct->quickResult[i] = mystrct->arr[i];
	}
	DWORD ThreadIDFirst, ThreadIDSec;
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, BubbleSort, mystrct, 0, &ThreadIDFirst);//создание потока
	hThread[1] = CreateThread(NULL, 0, QuickSort, mystrct, 0, &ThreadIDSec);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);//ожидание завершения работы потоков
	FILETIME ftFirstTh[4], ftSecTh[4];
	SYSTEMTIME stFirstTh[4], stSecTh[4];
	GetThreadTimes(
		hThread[0],      //HANDLE потока 
		&ftFirstTh[0],   //Время создания потока
		&ftFirstTh[1],   //Время заверешения потока
		&ftFirstTh[2],   //Время в режиме ядра
		&ftFirstTh[3]);  //Время в режиме юзера
	GetThreadTimes(hThread[1], &ftSecTh[0], &ftSecTh[1], &ftSecTh[2], &ftSecTh[3]);
	CloseHandle(hThread[0]);//закрывает дескриптор открытого объекта
	CloseHandle(hThread[1]);
	for (DWORD i = 0; i < 4; i++)
	{
		FileTimeToSystemTime(&ftFirstTh[i], &stFirstTh[i]);
		FileTimeToSystemTime(&ftSecTh[i], &stSecTh[i]);
	}
	hThread[0] = CreateThread(NULL, 0, BubbleSort, mystrct, 0, &ThreadIDFirst);
	hThread[1] = CreateThread(NULL, 0, QuickSort, mystrct, 0, &ThreadIDSec);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	SetThreadPriority(hThread[0], THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(hThread[1], THREAD_PRIORITY_LOWEST);
	FILETIME ftFirstThPRIORITY[4], ftSecThPRIORITY[4];
	SYSTEMTIME stFirstThPRIORITY[4], stSecThPRIORITY[4];
	GetThreadTimes(hThread[0], &ftFirstThPRIORITY[0], &ftFirstThPRIORITY[1], &ftFirstThPRIORITY[2], &ftFirstThPRIORITY[3]);
	GetThreadTimes(hThread[1], &ftSecThPRIORITY[0], &ftSecThPRIORITY[1], &ftSecThPRIORITY[2], &ftSecThPRIORITY[3]);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	for (DWORD i = 0; i < 4; i++)
	{
		FileTimeToSystemTime(&ftFirstThPRIORITY[i], &stFirstThPRIORITY[i]);
		FileTimeToSystemTime(&ftSecThPRIORITY[i], &stSecThPRIORITY[i]);
	}
	string firstThTime[4], secThTime[4], firstThTimePRIORIPY[4], secThTimePRIORIPY[4];
	for (int i = 0; i < 4; i++)
	{
		char buffer[256], buffer2[256], buffer3[256], buffer4[256];
		if (i == 3 || i == 2)
		{
			sprintf_s(buffer,
				"%02d:%02d.%03d",
				stFirstTh[i].wMinute,
				stFirstTh[i].wSecond,
				stFirstTh[i].wMilliseconds);
			sprintf_s(buffer2,
				"%02d:%02d.%03d",
				stSecTh[i].wMinute,
				stSecTh[i].wSecond,
				stSecTh[i].wMilliseconds);
			sprintf_s(buffer3,
				"%02d:%02d.%03d",
				stFirstThPRIORITY[i].wMinute,
				stFirstThPRIORITY[i].wSecond,
				stFirstThPRIORITY[i].wMilliseconds);
			sprintf_s(buffer4,
				"%02d:%02d.%03d",
				stSecThPRIORITY[i].wMinute,
				stSecThPRIORITY[i].wSecond,
				stSecThPRIORITY[i].wMilliseconds);

			firstThTime[i] = buffer;
			secThTime[i] = buffer2;
			firstThTimePRIORIPY[i] = buffer3;
			secThTimePRIORIPY[i] = buffer4;
		}
		else
		{
			sprintf_s(buffer,
				"%d-%02d-%02d %02d:%02d:%02d.%03d",
				stFirstTh[i].wYear,
				stFirstTh[i].wMonth,
				stFirstTh[i].wDay,
				stFirstTh[i].wHour,
				stFirstTh[i].wMinute,
				stFirstTh[i].wSecond,
				stFirstTh[i].wMilliseconds);
			sprintf_s(buffer2,
				"%d-%02d-%02d %02d:%02d:%02d.%03d",
				stSecTh[i].wYear,
				stSecTh[i].wMonth,
				stSecTh[i].wDay,
				stSecTh[i].wHour,
				stSecTh[i].wMinute,
				stSecTh[i].wSecond,
				stSecTh[i].wMilliseconds);
			sprintf_s(buffer3,
				"%d-%02d-%02d %02d:%02d:%02d.%03d",
				stFirstThPRIORITY[i].wYear,
				stFirstThPRIORITY[i].wMonth,
				stFirstThPRIORITY[i].wDay,
				stFirstThPRIORITY[i].wHour,
				stFirstThPRIORITY[i].wMinute,
				stFirstThPRIORITY[i].wSecond,
				stFirstThPRIORITY[i].wMilliseconds);
			sprintf_s(buffer4,
				"%d-%02d-%02d %02d:%02d:%02d.%03d",
				stSecThPRIORITY[i].wYear,
				stSecThPRIORITY[i].wMonth,
				stSecThPRIORITY[i].wDay,
				stSecThPRIORITY[i].wHour,
				stSecThPRIORITY[i].wMinute,
				stSecThPRIORITY[i].wSecond,
				stSecThPRIORITY[i].wMilliseconds);

			firstThTime[i] = buffer;
			secThTime[i] = buffer2;
			firstThTimePRIORIPY[i] = buffer3;
			secThTimePRIORIPY[i] = buffer4;
		}
	}
	cout << "\t\t\t\t    Same priority \t\t\t\t\t   Different priority" << endl;
	cout << "\t\t\t    1\t\t\t\t2\t\t\t  1(High)\t\t       2(Low)" << endl;
	cout << "Start time\t" << firstThTime[0] << "     " << secThTime[0] << "      " << firstThTimePRIORIPY[0] << "     " << secThTimePRIORIPY[0] << endl;
	cout << "End time\t" << firstThTime[1] << "     " << secThTime[1] << "      " << firstThTimePRIORIPY[1] << "     " << secThTimePRIORIPY[1] << endl;
	cout << "Kernel time\t\t" << firstThTime[2] << "\t\t    " << secThTime[2] << "\t\t\t " << firstThTimePRIORIPY[2] << "\t\t     " << secThTimePRIORIPY[2] << endl;
	cout << "User time\t\t" << firstThTime[3] << "\t\t    " << secThTime[3] << "\t\t\t " << firstThTimePRIORIPY[3] << "\t\t     " << secThTimePRIORIPY[3] << endl;
	cout << endl;
	cout << "Вывести массивы? 1 - да, 0 - нет : " << endl;
	int choise;
	cin >> choise;
	if (choise == 1)
	{
		cout << "Изначальный массив: " << endl;
		for (int i = 0; i < SIZE; i++)
		{
			cout << mystrct->arr[i] << " ";//вывод изначального массива на экран
		}
		cout << "\n" << endl;
		cout << "Результат работы первого потока: " << endl;
		for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива из первого потока
			cout << mystrct->bubbleResult[i] << " ";
		}
		cout << "\n" << endl;
		cout << "Результат работы второго потока: " << endl;
		for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива со второго потока
			cout << mystrct->quickResult[i] << " ";
		}
		cout << "\n" << endl;
	}
	delete[] mystrct->arr; // очистка памяти
	delete[] mystrct->bubbleResult;
	delete[] mystrct->quickResult;
	system("pause");
	return 0;
}
