#include <iostream>// для cout
#include <ctime> //для time
#include <thread> //для работы с потоками
//#include <chrono> //для sleep
#include <algorithm> // подключаем sort
#include <vector> // подключаем vector
#include <windows.h>


#define SIZE 100000//размер массива
using namespace std;


struct MyStruct
{
	//int arr[SIZE];
	int* arr = new int[SIZE];//инициалиизация массива
	int* bubbleResult = new int[SIZE]; //Выделение памяти для массива
	int* quickResult = new int[SIZE]; //Выделение памяти для массива
};



/*void* bubble(int array[], int &test)//первый поток
{
	test = 12345;

	int temp;// временная переменная для обмена элементов местами
	for (int i = 0; i < SIZE - 1; i++) {// Сортировка массива пузырьком
		for (int j = 0; j < SIZE - i - 1; j++) {// меняем элементы местами
			if (array[j] > array[j + 1]) {
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
	return array;//возвращаем отсортированный массив
}*/

/*void* quick(int array1[])//второй поток
{
	vector<int> arrVector(array1, array1 + SIZE);
	sort(arrVector.begin(), arrVector.end());//сортировка сравнением
	copy(arrVector.begin(), arrVector.end(), array1);//преобразовываем вектор в массив элементов
	return array1;//возвращаем отсортированный массив
}
*/



DWORD WINAPI ThreadFun(LPVOID param) {
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
/*DWORD WINAPI ThreadFunSec(LPVOID lpParam) {
	vector<int> arrVector(array1, array1 + SIZE);
	sort(arrVector.begin(), arrVector.end());//сортировка сравнением
	copy(arrVector.begin(), arrVector.end(), array1);//преобразовываем вектор в массив элементов
	return array1;//возвращаем отсортированный массив
}
*/

int main()
{
	setlocale(LC_ALL, "Rus");
	//int arr[SIZE];//инициалиизация массива
	srand(time(NULL));
	HANDLE hThreadFirts;
	//HANDLE hThreadSec;
	DWORD ThreadIDFirst;
	//DWORD ThreadIDSec;
	//MyStruct ms;
	MyStruct* mystrct = new MyStruct;
	for (int i = 0; i < SIZE; i++)//заполнение массива рандомными числами [-25000;25000)
	{
		mystrct->arr[i] = rand() % SIZE - (SIZE / 2);
	}

	//int* bubbleResult = new int[SIZE]; //Выделение памяти для массива
	//int* quickResult = new int[SIZE]; //Выделение памяти для массива
	for (int i = 0; i < SIZE; i++) { //копируем изначальный массив в новые массивы
		mystrct->bubbleResult[i] = mystrct->arr[i];
		mystrct->quickResult[i] = mystrct->arr[i];
	}
	int test = 0;

	//MyStruct* mystrct = (MyStruct*)malloc(sizeof(MyStruct));

	hThreadFirts = CreateThread(
		NULL,
		0,
		ThreadFun,
		mystrct,
		0,
		&ThreadIDFirst);
	/*hThreadSec = CreateThread(
		NULL,
		0,
		ThreadFunSec,
		NULL,
		0,
		&ThreadIDSec);*/

	cout << "IDFirst: " << ThreadIDFirst << endl;
	/*cout << "IDSec: " << ThreadIDSec << endl;*/


	/*HANDLE hThread[2];

	hThread[0] = CreateThread(NULL, 0, ThreadFun,mystrct,0,&ThreadIDFirst);
	hThread[1] = CreateThread(NULL, 0, IdleThread, 0, 0, &id);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);*/

	WaitForSingleObject(hThreadFirts, INFINITE);

	/*FILETIME ftCreate, ftExit, ftKernel, ftUser;
	for (DWORD i = 0; i < 2; i++)
	{
		GetThreadTimes(hThread[i], &ftCreate, &ftExit, &ftKernel, &ftUser);

		printf("Reported time for thread %dn", i + 1);

		SYSTEMTIME st;
		FileTimeToSystemTime(&ftKernel, &st);
		printf("Kernel: %2.2d:%2.2d.%3.3dn", st.wMinute, st.wSecond,
			st.wMilliseconds);

		FileTimeToSystemTime(&ftUser, &st);
		printf("User: %2.2d:%2.2d.%3.3dnn", st.wMinute, st.wSecond,
			st.wMilliseconds);
	}
	*/










	// выделить непосредственно память
	FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;
	SYSTEMTIME stCreationTime, stExitTime, stKernelTime, stUserTime;
	// использовать так:
	GetThreadTimes(
		hThreadFirts,        //HANDLE потока
		&ftCreationTime, //Время до создания потока
		&ftExitTime,     //Время до заврешения потока
		&ftKernelTime,   //Время, затраченное потоком на код ОС
		&ftUserTime);    //Время затраченное потоком на код программы

	FileTimeToSystemTime(&ftCreationTime, &stCreationTime);
	cout << stCreationTime.wYear << " " << stCreationTime.wMonth << " " << stCreationTime.wDay << " " << stCreationTime.wHour << " " << stCreationTime.wMinute << " " << stCreationTime.wSecond << endl;
	FileTimeToSystemTime(&ftExitTime, &stExitTime);
	cout << stExitTime.wYear << " " << stExitTime.wMonth << " " << stExitTime.wDay << " " << stExitTime.wHour << " " << stExitTime.wMinute << " " << stExitTime.wSecond << endl;



	FileTimeToSystemTime(&ftKernelTime, &stKernelTime);
	//printf("Kernel: %2.2d:%2.2d.%3.3dn", stKernelTime.wMinute, stKernelTime.wSecond, stKernelTime.wMilliseconds);
	cout << stKernelTime.wSecond << " " << stKernelTime.wMilliseconds << endl;
	FileTimeToSystemTime(&ftUserTime, &stUserTime);
	cout << stUserTime.wSecond << " " << stUserTime.wMilliseconds << endl;



	CloseHandle(hThreadFirts);
	/*CloseHandle(hThreadSec);*/



	/*thread th1(bubble, bubbleResult,ref(test));//старт первый поток
	thread th2(quick, quickResult);//старт второй поток
	th1.join();//ожидаем пока потоки завершатся
	th2.join();
cout << test << endl;*/


/*cout << "Изначальный массив: " << endl;
for (int i = 0; i < SIZE; i++)
{
	cout << mystrct->arr[i] << " ";//вывод изначального массива на экран
}
cout << "\n" << endl;
cout << "Результат работы первого потока: " << endl;
for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива из первого потока на экран
	cout << mystrct->bubbleResult[i] << " ";
}
cout << "\n" << endl;
cout << "Результат работы второго потока: " << endl;
for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива со второго потока на экран
	cout << mystrct->quickResult[i] << " ";
}
cout << "\n" << endl;*/
	delete[] mystrct->arr; // очистка памяти
	delete[] mystrct->bubbleResult; // очистка памяти
	delete[] mystrct->quickResult; // очистка памяти
	system("pause");
	return 0;
}
