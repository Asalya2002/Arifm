// Арифм.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <process.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
// Количество битов в регистре
constexpr auto BITS_IN_REGISTER = 16;
// Максимально возможное значение в регистре
#define TOP_VALUE (((long) 1 << BITS_IN_REGISTER) - 1)
// Диапазоны
#define FIRST_QTR (TOP_VALUE / 4 + 1)
#define HALF (2 * FIRST_QTR)
#define THIRD_QTR (3 * FIRST_QTR)
// Количество символов алфавита
constexpr auto NO_OF_CHARS = 256;
// Специальный символ КонецФайла
#define EOF (NO_OF_CHARS + 1)
// Всего символов в модели
#define NO_OF_SYMBOLS (NO_OF_CHARS + 1)
// Порог частоты для масштабирования
constexpr auto MAX_FREQUENCY = 16383;
constexpr auto n = 40;
// Таблицы перекодировки
unsigned char index_to_char[NO_OF_SYMBOLS];
int char_to_index[NO_OF_CHARS];
// Таблицы частот
int cum_freq[NO_OF_SYMBOLS + 1];
int freq[NO_OF_SYMBOLS + 1];
// Регистры границ и кода
long low, high;
long value;
// Поддержка побитлвых операций с файлами
long bits_to_follow;
int buffer;
int bits_to_go;
int garbage_bits;
// Обрабатываемые файлы
FILE* in, * out;
void compress_filename(const char* file,char* input_file, char* output_file)
{
	
	FILE* fp_in = fopen(input_file, "DataFile.txt");
	FILE* fp_out = fopen(output_file, "DataFile.txt");
	fclose(fp_in);
	fclose(fp_out);
}

// Ввод очередного бита сжатой информации
int input_bit(void)
{
	int t;

	if (bits_to_go == 0)
	{
		buffer = getc(in);
		if (buffer == EOF)
		{
			garbage_bits += 1;
			if (garbage_bits > BITS_IN_REGISTER - 2)
			{
				printf("Ошибка в сжатом файле\n");
				exit(-1);
			}
		}
		bits_to_go = 8;
	}
	t = buffer & 1;
	buffer >>= 1;
	bits_to_go -= 1;
	return t;
}

// Вывод очередного бита сжатой информации
void output_bit(int bit)
{
	buffer >>= 1;
	if (bit)
		buffer |= 0x80;
	bits_to_go -= 1;
	if (bits_to_go == 0)
	{
		putc(buffer, out);
		bits_to_go = 8;
	}
}



// Вывод указанного бита и отложенных ранее
void output_bit_plus_follow(int bit)
{
	output_bit(bit);
	while (bits_to_follow > 0)
	{
		output_bit(~bit);
		bits_to_follow--;
	}
}
	void encode(int symbol)
	{
		char c;
		int i, j;
		low = 0; high = 1000;
		bits_to_go = 0; // Сколько битов сбрасывать
		while (symbol != EOF)
		{
		 ifstream fp_in("DataFile.txt", std::ios::in); // Читаем
			low = low + cum_freq[symbol] * (high - low + 1) / cum_freq[0];
			high = low + cum_freq[symbol - 1] * (high - low + 1) / cum_freq[0 - 1];
		}
		for (;;) // Обрабатываем варианты
		{
			if (high < HALF)// переполнения
			{
				output_bit_plus_follow(0);
			}
			else
				if (low >= HALF)
				{
					output_bit_plus_follow(1);
					low -= HALF; high -= HALF;
				}
				else
					if (low >= FIRST_QTR && high < THIRD_QTR)
					{
						bits_to_follow += 1;
						low -= FIRST_QTR;
						high -= FIRST_QTR;
					}
					else break;
			low += low;
			high += high + 1;
		}
	}
	// Процедура переноса найденных битов в файл
void Bits_to_follow(int bit)
	{
	compress_filename;
		for (; bits_to_follow > 0; bits_to_follow--)
			compress_filename;
	}
void decode()
{
	int i, j;
	int cum_freq = 0, symbol;
	long freq, cum;
	char c;
	low = 0; high = 1000;
	for (i = 1; i < n; i++)
	{
		freq = ((value - low + 1) * cum_freq - 1) / (high - low + 1);
		for (int j = 1; j < cum_freq; j++); // Поиск символа
		low = low + cum_freq * (high - low + 1) / cum_freq;
		high = low + cum_freq * (high - low + 1) / cum_freq - 1;
		for (;;)
		 // Обрабатываем варианты
			if (high < HALF) // переполнения
			{      
				 // Ничего    
			}
			else 
			if (low >= HALF)
			{ 
				low -= HALF; high -= HALF; value -= HALF;
			}
			else
			if (low >= FIRST_QTR && high < THIRD_QTR)
			{
				low -= FIRST_QTR;
			    high -= FIRST_QTR;
				value -= FIRST_QTR;
			}
			else break; 
			value += value + input_bit();
		
	}
	
}

int main()
{
	encode(NO_OF_SYMBOLS+1);
	decode();
	
}

