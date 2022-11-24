#include <iostream>
#include <windows.h>

volatile int ind_min, ind_max, min, max, average, size;

void min_max_thread(LPVOID* param)
{
	int* mass = (int*)param;
	min = mass[0];
	max = mass[0];

	for (int i = 0; i < size; i++) {
		if (mass[i] < min) {
			min = mass[i];
			ind_min = i;
		}
		if (mass[i] > max) {
			max = mass[i];
			ind_max = i;
		}
		Sleep(7);
	}
	std::cout << std::endl << "Минимальный - " << min << "\tМаксимальный - " << max << std::endl;
	std::cout << "Поток min_max завершил свою работу." << std::endl;
}

void average_thread(LPVOID* param) {
	int* arr = (int*)param;
	average = 0;
	for (int i = 0; i < size; i++) {
		average += arr[i];
		Sleep(12);
	}
	average /= size;
	std::cout << std::endl << "Средний - " << average << std::endl;
	std::cout << "Поток Average завершил свою работу." << std::endl;
}

void main_thread() {
	std::cout << "Введите размер массива: ";

	std::cout << "Введите элементы массива: ";
	int* arr = new int[size];
	for (int i = 0; i < size; i++) {
		std::cin >> arr[i];
	}

	HANDLE min_max, Average;
	DWORD IDmin_max, IDaverage;

	min_max = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)min_max_thread, (void*)arr, 0, &IDmin_max);
	Average = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)average_thread, (void*)arr, 0, &IDaverage);

	if (min_max == NULL || Average == NULL) {
		return (void)GetLastError();
	}

	WaitForSingleObject(min_max, INFINITE);
	WaitForSingleObject(Average, INFINITE);

	CloseHandle(min_max);
	CloseHandle(Average);

	arr[ind_min] = average;
	arr[ind_max] = average;

	std::cout << std::endl << "Итоговый массив:" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}

	std::cout << std::endl << "Поток main завершил свою работу." << std::endl;
}

int main() {
	setlocale(LC_ALL, ".1251");
	HANDLE main; DWORD IDmain;

	main = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main_thread, NULL, 0, &IDmain);

	if (main == NULL) {
		return GetLastError();
	}
	WaitForSingleObject(main, INFINITE);

	CloseHandle(main);

	return 0;
}