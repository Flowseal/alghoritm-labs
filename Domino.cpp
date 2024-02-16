/*
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

struct Domino
{
	std::string first_side;
	std::string second_side;
	bool processed;
};

void read_input(std::vector<Domino>& dominoes)
{
	std::ifstream input_file;
	input_file.open("input.txt");

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
	if (s1 == s2)
		return false;

	if (s1.length() == s2.length())
	{
		// Сравниваем посимвольно по кодам
		bool first_longer = false;
		for (size_t i = 0; i < s1.size(); i++)
		{
			int ch1 = s1[i];
			int ch2 = s2[i];

			if (ch1 != ch2)
			{
				first_longer = ch1 > ch2;
				break;
			}
		}

		return first_longer;
	}

	return s1.length() > s2.length();
}

std::string get_max_chain(std::vector<Domino>& dominoes, bool start = true, const std::string& last_side = "0")
{
	// В данную строку будет записываться текущая в процессе рекурсии цепочка
	// В конце shortest_chain будет иметь максимальную цепочку из домино
	std::string longest_chain;

	for (size_t i = 0; i < dominoes.size(); i++)
	{
		if (dominoes.at(i).processed)
		{
			// Если данное домино уже было использовано в постройке цепочки - пропускаем
			continue;
		}

		// Случай, если домнио не надо переворачивать
		if (start || last_side == dominoes.at(i).first_side)
		{
			std::string chain = dominoes.at(i).first_side + dominoes.at(i).second_side;
			dominoes.at(i).processed = true;
			chain.append(get_max_chain(dominoes, false, dominoes.at(i).second_side));
			dominoes.at(i).processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain_is_longer(chain, longest_chain))
			{
				longest_chain = std::move(chain);
			}
		}

		// Случай, если домино можно перевернуть
		if (start || last_side == dominoes.at(i).second_side)
		{
			std::string chain = dominoes.at(i).second_side + dominoes.at(i).first_side;
			dominoes.at(i).processed = true;
			chain.append(get_max_chain(dominoes, false, dominoes.at(i).first_side));
			dominoes.at(i).processed = false;

			// Если получившаяся цепочка больше longest_chain, то заменяем её
			if (chain_is_longer(chain, longest_chain))
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
		std::vector<Domino> dominoes{};
		read_input(dominoes);

		std::string max_chain = get_max_chain(dominoes);
		write_output(max_chain);
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
		return 1;
	}

	return 0;
}