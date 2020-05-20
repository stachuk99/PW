#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <time.h>
#include <queue>
#include <ppl.h>
#include <windows.h>
#include <mutex>


void haslo_init(int n, char* alfabet, char* haslo) {
	for (int i = 0; i < n; i++)
	{
		haslo[i] = alfabet[0];
		haslo[n] = 0;
	}
}

int haslo_next(int n, int L, char* alfabet, char* haslo) {
	for (int i = 0; i < L - 1; i++)
	{
		if (haslo[0] == alfabet[i])
		{
			haslo[0] = alfabet[i + 1];
			return 1;
		}
	}
	if (n == 0)
	{
		return 0;
	}
	haslo[0] = alfabet[0];
	return haslo_next(n - 1, L, alfabet, &haslo[1]);
}

void Producent(int n, int L, char* alfabet, char* haslo, std::queue<char*> *bufor, std::mutex *mutex_, bool* stoped)
{
	int bufor_size = 0;
	haslo = (char*)malloc((n + 1) * sizeof(char));
	char *swap_bufor = (char*)malloc((n + 1) * sizeof(char));
	haslo_init(n, alfabet, haslo);
	//std::cout << "producent " << haslo << std::endl;
	int liczba_hasel=0;
	do 
	{
		mutex_->lock();
		bufor->push(haslo);
		bufor_size = bufor->size();
		mutex_->unlock();
		strcpy_s(swap_bufor, n + 1, haslo);
		haslo = (char*)malloc((n + 1) * sizeof(char));
		strcpy_s(haslo, n + 1, swap_bufor);
		if (bufor_size > 100)
		{
			Sleep(1);
		}
		//liczba_hasel++;
		if (haslo_next(n, L, alfabet, haslo) == 0)
		{
			//std::cout<<"wyprodukowano " << liczba_hasel << " hasel\n";
			*stoped = true;
			return;
		}
		//std::cout << "producent " << haslo << std::endl;
	} while (!*stoped);
}

void Konsument(int n, std::queue<char*>* bufor,std::mutex *mutex_,  bool* stoped)
{
	char* haslo = (char*)"qwerty1234"; 
	int liczba_hasel = 0;
	while (1)
	{
		if (!bufor->empty())
		{
			mutex_->lock();
			char *haslo2 = bufor->front();
			bufor->pop();
			mutex_->unlock();
			if (!strcmp(haslo, haslo2))
			{
				std::cout << "odgadnieto haslo " << std::endl;
				*stoped = true;
			}
			//std::cout << "konsument " << haslo2 << std::endl;
			//liczba_hasel++;
			free(haslo2);
		}
		else
		{
			if (*stoped) break;
		}
		
	}
	//std::cout << "odebrano " << liczba_hasel << " hasel\n";
	return;
}

int main()
{
	for (int i = 1; i < 20; i++)
	{
		int dlugosc_hasla = i;
		char* alfabet = (char*)"abcd";
		int dlugosc_alfabetu = strlen(alfabet);
		char* haslo;
		bool* stoped = new bool(false);
		std::queue<char*>* bufor = new std::queue<char*>();
		std::mutex* mutex_ = new std::mutex();

		clock_t begin = clock();
		std::thread producent(Producent, dlugosc_hasla, dlugosc_alfabetu, alfabet, haslo, bufor, mutex_, stoped);
		std::thread konsument(Konsument, dlugosc_hasla, bufor, mutex_, stoped);
		producent.join();
		konsument.join();
		clock_t end = clock();

		int time_spent = (int)(1000 * (end - begin) / CLOCKS_PER_SEC);
		std::cout << i << ". czas: " << time_spent << "ms\n";
	}
	
}
