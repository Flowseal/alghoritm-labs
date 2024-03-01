/*
1.6
7 баллов
Имеется N костей игры домино.
На каждой кости имеется 2 числа (каждое от 0 до 6).
Требуется написать программу, которая  будет определять максимальное число, составленное из цифр на костях цепочки, составленной по правилам домино из имеющихся костей.

Выполнил: Вещев Артём ПС-21
IDE: Visual Studio 2022
C++ 20
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>
#include <chrono>

struct Domino
{
	int first;
	int second;
	bool processed;
	Domino(int f = 0, int s = 0)
		: first(f)
		, second(s)
		, processed(false)
	{
	}
};

bool operator==(const Domino& lhs, const Domino& rhs)
{
	return lhs.first == rhs.first && lhs.second == rhs.second
		|| lhs.first == rhs.second && lhs.second == rhs.first;
}

std::vector<Domino> read_input()
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

	std::vector<Domino> dominoes;
	dominoes.reserve(dominoes_count);

	// Заполняем вектор доминошками
	for (int i = 0; i < dominoes_count; i++)
	{
		dominoes.push_back(Domino());
		int l = 0, r = 0;
		input_file >> l >> r;
		dominoes.at(i).first = std::max(l, r);
		dominoes.at(i).second = std::min(l, r);
	}

	if (input_file.bad())
	{
		throw std::runtime_error("Error reading input.txt");
	}

	return dominoes;
}

void write_output(const std::vector<Domino>& chain)
{
	std::ofstream output_file;
	output_file.open("output.txt");

	if (!output_file.is_open())
	{
		throw std::runtime_error("Error opening output.txt");
	}

	for (auto& d : chain)
	{
		output_file << d.first << d.second;
		std::cout << d.first << d.second;
	}

	output_file << std::endl;
	std::cout << std::endl;

	if (!output_file.flush())
	{
		throw std::runtime_error("Error writing output.txt");
	}
}

std::vector<Domino> get_max_chain(std::vector<Domino>& dominoes, int last_side = -1)
{
	std::vector<Domino> longest_chain;
	std::vector<Domino> checked_dominoes;

	for (Domino& domino : dominoes)
	{
		// Если длиннейшая цепочка по кол-ву совпадает с кол-вом домино - это ответ.
		if (longest_chain.size() == dominoes.size())
			break;
		
		// Если домино уже была проверена в этой цепочке
		if (domino.processed)
			continue;

		// Если мы уже проверяли в текущей итерации такую домино
		if (std::find(checked_dominoes.begin(), checked_dominoes.end(), domino) != checked_dominoes.end())
			continue;

		checked_dominoes.push_back(domino);

		// Случай, если домнио не надо переворачивать
		if (last_side < 0 && domino.first != 0 || last_side == domino.first)
		{
			std::vector<Domino> chain = { domino };
			domino.processed = true;
			auto next_chain = get_max_chain(dominoes, domino.second);
			chain.insert(chain.end(), next_chain.begin(), next_chain.end());
			domino.processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain.size() > longest_chain.size())
			{
				longest_chain = std::move(chain);
			}
		}

		// Случай, если домино можно перевернуть
		if (last_side < 0 && domino.second != 0 || last_side == domino.second)
		{
			std::vector<Domino> chain = { Domino(domino.second, domino.first) };
			domino.processed = true;
			auto next_chain = get_max_chain(dominoes, domino.first);
			chain.insert(chain.end(), next_chain.begin(), next_chain.end());
			domino.processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain.size() > longest_chain.size())
			{
				longest_chain = std::move(chain);
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
		std::vector<Domino> dominoes = read_input();
		
		auto start_time = std::chrono::high_resolution_clock::now();

		std::sort(dominoes.begin(), dominoes.end(), [](const Domino& lhs, const Domino& rhs) {
			return lhs.first == rhs.first ? lhs.second > rhs.second : lhs.first > rhs.first;
		});
		auto max_chain = get_max_chain(dominoes);

		std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start_time;
		printf("Time taken: %.3fs\n", duration.count());

		write_output(max_chain);
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return 1;
	}

	return 0;
}