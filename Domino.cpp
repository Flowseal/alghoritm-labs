/*
1.6
7 баллов
Имеется N костей игры домино.
На каждой кости имеется 2 числа (каждое от 0 до 6).
Требуется написать программу, которая  будет определять максимальное число, составленное из цифр на костях цепочки, составленной по правилам домино из имеющихся костей.

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++ 14
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

struct Domino
{
	std::string first_side;
	std::string second_side;
	bool processed;
};

void read_input(std::vector<Domino>& dominoes)
{
	std::ifstream input_file;
	input_file.open("input9.txt");

	if (!input_file.is_open())
	{
		throw std::runtime_error("Error opening input.txt");
	}

	// Читаем количество домино
	int dominoes_count;
	input_file >> dominoes_count;

	// Заполняем вектор доминошками
	for (int i = 0; i < dominoes_count; i++)
	{
		dominoes.push_back(Domino());
		input_file >> dominoes.at(i).first_side >> dominoes.at(i).second_side;
	}

	if (input_file.bad())
	{
		throw std::runtime_error("Error reading input.txt");
	}
}

void write_output(const std::string& chain)
{
	std::ofstream output_file;
	output_file.open("output.txt");

	if (!output_file.is_open())
	{
		throw std::runtime_error("Error opening output.txt");
	}

	output_file << chain << std::endl;

	if (!output_file.flush())
	{
		throw std::runtime_error("Error writing output.txt");
	}
}

// Возвращает true, если цепочка s1 больше цепочки s2
// Если цепочки длины по равне - сравнивает их числовое значение
bool chain_is_longer(const std::string& s1, const std::string& s2)
{
	if (s2.empty() || s2[0] == '0' && (s1[0] !='0' || s1.length() > s2.length()))
		return true;

	if (s1 == s2)
		return false;

	for (size_t i = 0; i < s1.length(); ++i)
	{
		if (s1[i] != s2[i])
			return s1[i] > s2[i];
	}

	return s1.length() > s2.length();
}

std::string get_max_chain(std::vector<Domino>& dominoes, bool start = true, const std::string& last_side = "0")
{
	// В данную строку будет записываться текущая в процессе рекурсии цепочка
	// В конце shortest_chain будет иметь максимальную цепочку из домино
	std::string longest_chain;

	for (Domino& domino : dominoes)
	{
		if (domino.processed)
		{
			// Если данное домино уже было использовано в постройке цепочки - пропускаем
			continue;
		}

		// Случай, если домнио не надо переворачивать
		if (start || last_side == domino.first_side)
		{
			std::string chain = domino.first_side + domino.second_side;
			domino.processed = true;
			chain += get_max_chain(dominoes, false, domino.second_side);
			domino.processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain_is_longer(chain, longest_chain))
			{
				std::swap(chain, longest_chain);
			}
		}

		// Случай, если домино можно перевернуть
		if (start || last_side == domino.second_side)
		{
			std::string chain = domino.second_side + domino.first_side;
			domino.processed = true;
			chain += get_max_chain(dominoes, false, domino.first_side);
			domino.processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain_is_longer(chain, longest_chain))
			{
				std::swap(chain, longest_chain);
			}
		}

		// Во втором IF нельзя поставить ELSE IF, т.к. если домино у нас первое, то его можно поставить двумя сторонами
	}

	return longest_chain;
}

int main()
{
	try
	{
		std::vector<Domino> dominoes{};
		read_input(dominoes);

		clock_t tStart = clock();
		std::string max_chain = get_max_chain(dominoes);
		printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

		write_output(max_chain);
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return 1;
	}

	return 0;
}