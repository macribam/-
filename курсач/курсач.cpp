#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <cstring>

using namespace std;

struct HimChi
{
	int Number;
	char Group[20];
	int Cost;
	char Date[20];
	char Ready[20];
};
// начну
void CreateFile(FILE* file);
void ViewFile(FILE* file);
void AddFile(FILE* file);
void DeleteElemFile(FILE* file);
void SearchFile(FILE* file);
void SimpleSortFile(FILE* file);
void QuickSortFile(FILE* file);
void BinarySearchFile(FILE* file);
void ViewByDateFile(FILE* file);

void InfoFile(HimChi* kvit);

bool IsLeapYear(int year);
bool IsValidDate(int day, int month, int year);

int DateDiffFile(char* date1, char* date2);

void QuickSort(HimChi* arr, int low, int high);
int Partition(HimChi* arr, int low, int high);
void Swap(HimChi& a, HimChi& b);

int BinarySearch(FILE* file, char* date, int low, int high);




void Menu()
{
	bool menu = 1;
	FILE* file = fopen("Himchistka.txt", "rb");
	while (file == nullptr)
	{
		system("cls");
		cout << "1. Создать файл." << endl;
		cout << "2. Выйти из программы" << endl;
		int c;
		cin >> c;
		switch (c)
		{
		case 1:
			file = fopen("Himchistka.txt", "wb");
			fclose(file);
			break;
		case 2: return;
		default: cout << "Такого пункта нет\n\n";
		}
	}
	if (file != nullptr)
	{
		fclose(file);
	}

	while (menu == 1) {

		cout << "1. Пересоздать файл (очистка)." << endl;
		cout << "2. Просмотреть все квитанции." << endl;
		cout << "3. Добавить квитанцию." << endl;
		cout << "4. Удаление квитанции." << endl;
		cout << "5. Линейный поиск." << endl;
		cout << "6. Прямая сортировка." << endl;
		cout << "7. Быстрая сортировка." << endl;
		cout << "8. Двоичный поиск (автосортировка)." << endl;
		cout << "9. Вывод состояние готовности на определённую дату." << endl;
		cout << "10. Выйти из программы." << endl;
		int n;
		cin >> n;
		system("cls");
		switch (n)
		{
		case 1: CreateFile(file); break;
		case 2: ViewFile(file); break;
		case 3: AddFile(file); break;
		case 4: DeleteElemFile(file); break;
		case 5: SearchFile(file); break;
		case 6: SimpleSortFile(file); break;
		case 7: QuickSortFile(file); break;
		case 8: BinarySearchFile(file); break;
		case 9: ViewByDateFile(file); break;
		case 10: menu = 0; break;
		default: cout << "Такого пункта нет\n\n";
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	Menu();
	return 0;
}

void CreateFile(FILE* file)
{
	file = fopen("Himchistka.txt", "wb");
	fclose(file);
}

void InfoFile(HimChi* kvit)
{
	cout << "Номер заказа: " << kvit->Number;
	cout << "\nНаименование группы изделия: " << kvit->Group;
	cout << "\nСтоимость заказа: " << kvit->Cost;
	cout << "\nДата приёма: " << kvit->Date;
	cout << "\nГотовность заказа: " << kvit->Ready << "\n\n";
}

void ViewFile(FILE* file)
{
	file = fopen("Himchistka.txt", "rb");
	HimChi kvit;
	int kolvo = _filelength(_fileno(file)) / sizeof(HimChi);
	for (int i = 0; i < kolvo; i++)
	{
		fread(&kvit, sizeof(HimChi), 1, file);
		InfoFile(&kvit);
	}
	fclose(file);
}

bool IsLeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool IsValidDate(int day, int month, int year)
{
	if (year < 0 || month < 1 || month > 12 || day < 1)
		return false;

	int daysInMonth = 31;

	if (month == 4 || month == 6 || month == 9 || month == 11)
		daysInMonth = 30;
	else if (month == 2)
	{
		if (IsLeapYear(year))
			daysInMonth = 29;
		else
			daysInMonth = 28;
	}

	return day <= daysInMonth;
}

void AddFile(FILE* file)
{
	file = fopen("Himchistka.txt", "ab");
	HimChi kvit;
	cout << "Номер квитанции (уникальный): ";
	cin >> kvit.Number;
	cout << "Наименование группы изделия: ";
	cin >> kvit.Group;
	cout << "Стоимость заказа: ";
	cin >> kvit.Cost;

	bool validDate = false;
	while (!validDate)
	{
		cout << "Дата приема (дд.мм.гггг): ";
		cin >> kvit.Date;

		int day, month, year;
		sscanf(kvit.Date, "%d.%d.%d", &day, &month, &year);

		validDate = IsValidDate(day, month, year);

		if (!validDate)
			cout << "Некорректная дата, повторите ввод." << endl;
	}

	bool validReady = false;
	while (!validReady)
	{
		cout << "Готовность заказа (done или working): ";
		cin >> kvit.Ready;

		if (!strcmp(kvit.Ready, "done") || !strcmp(kvit.Ready, "working"))
			validReady = true;
		else
			cout << "Некорректное значение готовности заказа, повторите ввод." << endl;
	}
	cout << "\n";
	fwrite(&kvit, sizeof(HimChi), 1, file);
	fclose(file);
}

void DeleteElemFile(FILE* file)
{
	file = fopen("Himchistka.txt", "r+b");
	int kolvo = _filelength(_fileno(file)) / sizeof(HimChi);
	int number;
	cout << "Введите номер квитанции: ";
	cin >> number;
	HimChi kvit;
	int l = -1;
	for (int i = 0; i < kolvo; i++)
	{
		fread(&kvit, sizeof(HimChi), 1, file);
		if (kvit.Number == number)
		{
			l = i;
			break;
		}
	}
	if (l != -1)
	{
		for (int i = l + 1; i < kolvo; i++)
		{
			fseek(file, i * sizeof(HimChi), 0);
			fread(&kvit, sizeof(HimChi), 1, file);
			fseek(file, (i - 1) * sizeof(HimChi), 0);
			fwrite(&kvit, sizeof(HimChi), 1, file);
		}
		_chsize(_fileno(file), (kolvo - 1) * sizeof(HimChi));
		cout << "Квитанция успешно удалена." << endl << endl;
	}
	else
	{
		cout << "Квитанция с таким номером не найдена." << endl << endl;
	}
	fclose(file);
}

void SearchFile(FILE* file)
{
	file = fopen("Himchistka.txt", "r+b");
	char date[20];
	cout << "Введите дату приёма: ";
	cin.ignore(1, '\n');
	cin.getline(date, 20);
	HimChi kvit;
	while (fread(&kvit, sizeof(HimChi), 1, file))
	{
		if (!strcmp(kvit.Date, date))
		{
			InfoFile(&kvit);
		}
	}
	fclose(file);
}

int DateDiffFile(char* date1, char* date2)
{
	int day1, month1, year1;
	int day2, month2, year2;
	sscanf(date1, "%d.%d.%d", &day1, &month1, &year1);
	sscanf(date2, "%d.%d.%d", &day2, &month2, &year2);
	if (year1 > year2) return 1;
	else if (year1 < year2) return -1;

	if (month1 > month2) return 1;
	else if (month1 < month2) return -1;

	if (day1 > day2) return 1;
	else if (day1 < day2) return -1;

	return 0;
}

void SimpleSortFile(FILE* file)
{
	file = fopen("Himchistka.txt", "r+b");
	int kolvo = _filelength(_fileno(file)) / sizeof(HimChi);
	HimChi* kvit = new HimChi[kolvo];
	rewind(file);
	fread(kvit, sizeof(HimChi), kolvo, file);
	for (int i = 0; i < kolvo - 1; i++)
	{
		for (int j = i + 1; j < kolvo; j++)
		{
			if (DateDiffFile(kvit[i].Date, kvit[j].Date) > 0)
			{
				HimChi temp = kvit[i];
				kvit[i] = kvit[j];
				kvit[j] = temp;
			}
		}
	}
	rewind(file);
	fwrite(kvit, sizeof(HimChi), kolvo, file);
	delete[] kvit;
	fclose(file);
	cout << "Квитанции были отсортированы по дате приёма.\n\n";
}

void QuickSortFile(FILE* file)
{
	file = fopen("Himchistka.txt", "r+b");
	int kolvo = _filelength(_fileno(file)) / sizeof(HimChi);
	HimChi* kvit = new HimChi[kolvo];
	fread(kvit, sizeof(HimChi), kolvo, file);
	QuickSort(kvit, 0, kolvo - 1);
	rewind(file);
	fwrite(kvit, sizeof(HimChi), kolvo, file);
	delete[] kvit;
	fclose(file);
	cout << "Квитанции были отсортированы по дате приёма.\n\n";
}

void QuickSort(HimChi* arr, int low, int high)
{
	if (low < high)
	{
		int pivotIndex = Partition(arr, low, high);
		QuickSort(arr, low, pivotIndex - 1);
		QuickSort(arr, pivotIndex + 1, high);
	}
}

int Partition(HimChi* arr, int low, int high)
{
	HimChi pivot = arr[high];
	int i = low - 1;
	for (int j = low; j < high; j++)
	{
		if (DateDiffFile(arr[j].Date, pivot.Date) < 0)
		{
			i++;
			Swap(arr[i], arr[j]);
		}
	}
	Swap(arr[i + 1], arr[high]);
	return i + 1;
}

void Swap(HimChi& a, HimChi& b)
{
	HimChi temp = a;
	a = b;
	b = temp;
}

void BinarySearchFile(FILE* file)
{
	file = fopen("Himchistka.txt", "r+b");
	QuickSortFile(file);
	char date[20];
	cout << "Введите дату приёма: ";
	cin.ignore(1, '\n');
	cin.getline(date, 20);
	HimChi kvit;
	int low = 0;
	int high = _filelength(_fileno(file)) / sizeof(HimChi) - 1;
	int foundIndex = BinarySearch(file, date, low, high);
	if (foundIndex != -1)
	{
		int startIndex = foundIndex;
		int endIndex = foundIndex;
		while (startIndex > 0)
		{
			fseek(file, (startIndex - 1) * sizeof(HimChi), SEEK_SET);
			fread(&kvit, sizeof(HimChi), 1, file);
			if (DateDiffFile(kvit.Date, date) != 0)
			{
				break;
			}
			startIndex--;
		}
		while (endIndex < high)
		{
			fseek(file, (endIndex + 1) * sizeof(HimChi), SEEK_SET);
			fread(&kvit, sizeof(HimChi), 1, file);
			if (DateDiffFile(kvit.Date, date) != 0)
			{
				break;
			}
			endIndex++;
		}
		fseek(file, startIndex * sizeof(HimChi), SEEK_SET);
		while (fread(&kvit, sizeof(HimChi), 1, file))
		{
			if (DateDiffFile(kvit.Date, date) != 0)
			{
				break;
			}
			InfoFile(&kvit);
			if (ftell(file) >= (endIndex + 1) * sizeof(HimChi))
			{
				break;
			}
		}
	}
	else
	{
		cout << "Квитанции на указанную дату не найдены.\n\n";
	}
	fclose(file);
}

int BinarySearch(FILE* file, char* date, int low, int high)
{
	if (low <= high)
	{
		int mid = (low + high) / 2;
		HimChi kvit;
		fseek(file, mid * sizeof(HimChi), SEEK_SET);
		fread(&kvit, sizeof(HimChi), 1, file);
		if (DateDiffFile(kvit.Date, date) == 0)
		{
			return mid;
		}
		else if (DateDiffFile(kvit.Date, date) < 0)
		{
			return BinarySearch(file, date, mid + 1, high);
		}
		else
		{
			return BinarySearch(file, date, low, mid - 1);
		}
	}
	return -1;
}

void ViewByDateFile(FILE* file)
{
	file = fopen("Himchistka.txt", "rb");
	HimChi kvit;
	int kolvo = _filelength(_fileno(file)) / sizeof(HimChi);
	char date[20];
	cout << "Введите дату (в формате, как в структуре): ";
	cin.ignore(1, '\n');
	cin.getline(date, 20);
	cout << "Заказы на дату " << date << ":\n\n";
	char currentGroup[20] = "";
	bool isFirstGroup = true;
	for (int i = 0; i < kolvo; i++)
	{
		fread(&kvit, sizeof(HimChi), 1, file);
		if (strcmp(kvit.Date, date) == 0)
		{
			if (strcmp(kvit.Group, currentGroup) != 0)
			{
				if (!isFirstGroup)
				{
					cout << "\n";
				}
				cout << "Группа: " << kvit.Group << "\n";
				strcpy(currentGroup, kvit.Group);
				isFirstGroup = false;
			}
			cout << "Номер: " << kvit.Number << "\tСтатус заказа: " << kvit.Ready << "\n";
		}
	}
	cout << "\n";
	fclose(file);
}