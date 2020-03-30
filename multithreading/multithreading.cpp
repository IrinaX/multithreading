#include <iostream>// для cout
#include <ctime> //для time
#include <thread> //для работы с потоками
//#include <chrono> //для sleep
#include <algorithm> // подключаем sort
#include <vector> // подключаем vector
#include <windows.h>


#define SIZE 8//размер массива
using namespace std;

void* bubble(int array[], int &test)//первый поток
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
}

void* quick(int array1[])//второй поток
{
	vector<int> arrVector(array1, array1 + SIZE);
	sort(arrVector.begin(), arrVector.end());//сортировка сравнением
	copy(arrVector.begin(), arrVector.end(), array1);//преобразовываем вектор в массив элементов
	return array1;//возвращаем отсортированный массив
}

int main()
{
	setlocale(LC_ALL, "Rus");
	int arr[SIZE];//инициалиизация массива
	srand(time(NULL));
	for (int i = 0; i < SIZE; i++)//заполнение массива рандомными числами [-25000;25000)
	{
		arr[i] = rand() % 50000 - 25000;
	}
	int* bubbleResult = new int[SIZE]; //Выделение памяти для массива
	int* quickResult = new int[SIZE]; //Выделение памяти для массива
	for (int i = 0; i < SIZE; i++) { //копируем изначальный массив в новые массивы
		bubbleResult[i] = arr[i];
		quickResult[i] = arr[i];
	}
	int test = 0;
	thread th1(bubble, bubbleResult,ref(test));//старт первый поток
	thread th2(quick, quickResult);//старт второй поток
	th1.join();//ожидаем пока потоки завершатся
	th2.join();
	cout << test << endl;
	cout << "Изначальный массив: " << endl;
	for (int i = 0; i < SIZE; i++)
	{
		cout << arr[i] << " ";//вывод изначального массива на экран
	}
	cout <<"\n"<< endl;
	cout << "Результат работы первого потока: " << endl;
	for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива из первого потока на экран
		cout << bubbleResult[i] << " ";
	}
	cout << "\n" << endl;
	cout << "Результат работы второго потока: " << endl;
	for (int i = 0; i < SIZE; i++) {// Вывод отсортированного массива со второго потока на экран
		cout << quickResult[i] << " ";
	}
	cout << "\n" << endl;
	delete[] bubbleResult; // очистка памяти
	delete[] quickResult; // очистка памяти
	system("pause");
	return 0;
}
