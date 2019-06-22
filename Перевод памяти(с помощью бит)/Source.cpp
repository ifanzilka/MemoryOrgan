/*
 Проверим, как вы поняли paging. Для этого вам предлагается выступить в качестве процессора и преобразовать несколько логических адресов в физические. Формат входных данных следующий:

в первой строке вам даны 3 числа m,q,r≥0 , где q - это количество запросов, на которые вам нужно ответить, r - физический адрес корневой таблицы страниц
следующих m строках записаны пары paddr и value - описание физической памяти, каждая пара значит, что по физическому адресу paddr хранится 64 битное значение value, при этом гарантируется, что все paddr различны, выровнены на границу 8 байт и помещаются в 64 бита
в последних q строках идут целые числа - логические адреса, которые вам нужно преобразовать в физические, для каждого из этих чисел нужно вывести на отдельной строке либо физический адрес, либо слово "fault", если преобразовать логический адрес в физический нельзя.
Считайте, что таблица страниц имеет формат 64 битного режима x86 (4 уровня, каждая страница 4 KB, каждая запись 8 байт, формат записи был показан в лекциях), но вы можете игнорировать все поля, кроме бита присутствия (на картинке бит P - нулевой бит) и собственно физического адреса.

Для всех физических адресов, не указанных во входных данных (среди m пар paddr value), считайте, что по этим адресам хранятся нули.
 */


#include <clocale>
#include <string>
#include <math.h>
#include <fstream>
#include <ctype.h>
#include <iostream>
#include <map>
uint64_t logic_addr[6000];  //массив логических адресов
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	ifstream input("input.txt");//файл который считываем


	uint64_t m, q, r, phys_addr = 0, cnt_fault = 0;
	uint64_t addr[64], mpl4[9];
	uint64_t mpl4_ = 0, physic = 0, value = 0;
	uint64_t cnt_of_phys = 0;
	ofstream fout("output.txt");//файл в который запишем физические адреса

	input >> m >> q >> r;




	//записываем пару ключ-значение
	map <uint64_t, uint64_t> map;
	//map <uint64_t, uint64_t>:: iterator it, it2;
	uint64_t paddr = 0, value1 = 0;

	for (int i = 0; i < m; i++)
	{
		input >> paddr; input >> value1;
		map[paddr] = value1;
	}

	//записываем логический адреса
	for (uint64_t i = 0; i < q; i++) {
		input >> logic_addr[i];
	}
	for (uint64_t k = 0; k < q; k++) {

		
		physic = 0;
		value = 0;
		
		
		cnt_of_phys = 0;

		uint64_t logic = logic_addr[k];//получили первое число
		uint64_t logic_pml4 = logic;
		logic_pml4=logic_pml4 << 16; 
		logic_pml4=logic_pml4 >> 16;		//обнулил биты 48-64




		logic_pml4=logic_pml4 >> 39; //logic_pml4 << 40;			//обнулил биты	0-40




	


		//Находим значение и ищем его в таблице физических адресов
		
		physic = r + logic_pml4 * 8;
		//cout << "PML4-(" << logic_pml4 << ") " << physic << "[" << k << "]" << endl;

		//проверяю если значение в нашем контейнере
		if(map.find(physic)!=map.end())
		{
			value = map[physic];
			cnt_of_phys = 1;
		}
		
		
		if (cnt_of_phys == 0) {
			cnt_fault++;
		}
		//value
		value = value >> 12;
		value = value << 12;	//обнулили биты [0 , 12]

		value = value << 12;
		value = value >> 12;	//обнулил биты [52 , 64]
		

	
		uint64_t DirectoryPtr = logic;
		DirectoryPtr = DirectoryPtr << 25;
		DirectoryPtr = DirectoryPtr >> 25;		//обнулили биты [38,64]


		DirectoryPtr = DirectoryPtr >> 30;     //обнулили и сместили первые 32 бита





		//Находим значение физического адреса и ищем его в таблице
		physic = value + DirectoryPtr * 8;
		//cout << "DirectoryPtr-(" << DirectoryPtr << ") " << physic << "[" << k << "]" << endl;
		cnt_of_phys = 0;

		//проверяю если значение в нашем контейнере
		if (map.find(physic) != map.end())
		{
			value = map[physic];
			cnt_of_phys = 1;
		}

		if (cnt_of_phys == 0) {
			cnt_fault++;
		}



		value = value >> 12;
		value = value << 12;	//обнулили биты [0 , 12]

		value = value << 12;
		value = value >> 12;	//обнулил биты [52 , 64]


		uint64_t Directory = logic;
		Directory = Directory << 34;
		Directory = Directory >>34;	//обнулили биты [32 64]

		Directory = Directory >> 21;//обнулили и сместили на 24 бита





		physic = value + Directory * 8;
		//cout << "Directory-(" << Directory << ") " << physic << "[" << k << "]" << endl;
		cnt_of_phys = 0;






		//проверяю если значение в нашем контейнере
		if (map.find(physic) != map.end())
		{
			value = map[physic];
			cnt_of_phys = 1;
		}
		
		if (cnt_of_phys == 0) {
			cnt_fault++;
		}



		value = value >> 12;
		value = value << 12;	//обнулили биты [0 , 12]

		value = value << 12;
		value = value >> 12;	//обнулил биты [52 , 64]

		uint64_t Table = logic;
		Table = Table << 43;
		Table = Table >> 43;	//обнулил биты 


		Table = Table >> 12;//обнулил и сместил на 16 бит





		physic = value + Table * 8;
		//cout << "Table-(" << Table << ") " << physic << "[" << k << "]" << endl;
		cnt_of_phys = 0;

		//проверяю если значение в нашем контейнере
		if (map.find(physic) != map.end())
		{
			value = map[physic];
			cnt_of_phys = 1;
		}


		if (cnt_of_phys == 0) {
			cnt_fault++;
		}

		/*if (value % 2 == 0) {
			cnt_fault++;
		}*/
		else {
			
			value = value >> 12;
			value = value << 12;	//обнулили биты [0 , 12]

			value = value << 12;
			value = value >> 12;	//обнулил биты [52 , 64]


		}
		



		uint64_t Offset = logic;
		Offset = Offset << 51;
		Offset = Offset >> 51;//обнулил последние  бит


		if (cnt_fault != 0)
			fout << "fault" << endl;
		else {
			physic = value + Offset;
			//cout << "Offset-(" << Offset << ") " << physic << "[" << k << "]" << endl;
			fout << physic << endl;
		}
		cnt_fault = 0;
	}
	fout.close();
	cout << endl;
	system("pause");
	return 0;
		

	
}






















	