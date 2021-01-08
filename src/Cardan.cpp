#include "include/Cardan.h"

using boolVector2d = std::vector<std::vector<bool>>;
using stringVector2d = std::vector<std::vector<std::string>>;


// TODO console arguments
// TODO alphabet, grid size

void checkFile(std::string fileName)
{
	std::ifstream temp(fileName);

	if (!temp.is_open())
	{
		std::cout << "���������� ������� ���� '" << fileName << "'\n";
		system("pause");
		exit(0);
	}
	else
	{
		std::string tmp;
		while (temp)
		{
			getline(temp, tmp);
			if (!tmp.empty())
				return;
		}
		temp.close();

		std::cout << "���� '" << fileName << "' ����!\n";
		system("pause");
		exit(0);

	}
}

void readFile(std::string& text, std::string fileName)
{
	checkFile(fileName);
	std::ifstream in(fileName);

	std::string  str;
	while (!in.eof())
	{
		str.clear();
		getline(in, str);
		text.append(str);
	}
	in.close();

	std::string alphabet = "��������������������������������";
	transform(text.begin(), text.end(), text.begin(), tolower);
	text.erase(remove_if(text.begin(), text.end(), isAlpha(alphabet)), text.end());
}

void save(boolVector2d constChosenGridPositions, std::string encriptedText, size_t blocksCount)
{
	size_t gridSize = constChosenGridPositions.size();
	std::ofstream out("Clear text.txt", std::ios::app);

	for (auto row : constChosenGridPositions)
	{
		for (auto i : row)
			out << i << " ";
		out << "\n";
	}

	out << "\n�������� �����:\n";

	for (auto i = 0; i < 4; i++)
	{
		for (auto k = 0; k < blocksCount; k++)
		{
			for (auto j = 0; j < (gridSize * gridSize) / 4; j++)
				out << encriptedText[k * gridSize * gridSize + i * ((gridSize * gridSize) / 4) + j];
			out << "   ";
		}
		out << "\n";
	}
	out << "\n";

	for (auto i = 0; i < 2 * blocksCount * ((gridSize * gridSize) / 4) + blocksCount - 1; i++)
		out << "-";
	out << "\n";
}


void createNewGrid(boolVector2d& newGrid, size_t size)
{
	std::vector<bool> buff;

	for (auto i = 0; i < size; i++)  // ���������� ������ ���� ������
	{
		for (auto j = 0; j < size; j++)
			buff.push_back(false);

		newGrid.push_back(buff);
		buff.clear();
	}
}

void turnLeft(boolVector2d& grid)
{
	boolVector2d newGrid;
	createNewGrid(newGrid, grid.size());

	for (auto i = 0; i < grid.size(); i++)
		for (auto j = 0; j < grid.size(); j++)
			if (grid[grid.size() - j - 1][i] == 1)
			{
				newGrid[grid.size() - i - 1][grid.size() - j - 1] = 1;
			}
	grid = newGrid;
}

void turnRight(boolVector2d& grid)
{
	boolVector2d newGrid;
	createNewGrid(newGrid, grid.size());

	for (auto i = 0; i < grid.size(); i++)
		for (auto j = 0; j < grid.size(); j++)
			if (grid[grid.size() - j - 1][i] == 1)
			{
				newGrid[i][j] = 1;
			}
	grid = newGrid;
}


void createEncryptedGrid(stringVector2d& encryptedGridBlocks, std::string text, int sizeGrid)
{
	std::vector<std::string> encryptedGrid;
	std::string buff;

	for (auto k = 0; k < (int)text.size() / (sizeGrid * sizeGrid); k++)
	{
		for (auto i = k * sizeGrid; i < (k + 1) * sizeGrid; i++)//���������� ������� ������������
		{
			for (auto j = i * sizeGrid; j < (i + 1) * sizeGrid; j++)
				buff.push_back(text[j]);
			encryptedGrid.push_back(buff);
			buff.clear();
		}
		encryptedGridBlocks.push_back(encryptedGrid);
		encryptedGrid.clear();
	}
}

void createPositionsGrid(boolVector2d& chosenGridPositions, size_t size)
{
	std::vector<bool> buff;
	for (auto i = 0; i < size; i++)
	{
		for (auto j = 0; j < size; j++)
			buff.push_back(false);
		chosenGridPositions.push_back(buff);
		buff.clear();
	}
}

void changeForbiddenGridPositions(boolVector2d chosenGridPositions, boolVector2d& forbiddenGridPositions)
{
	size_t GrigSize = chosenGridPositions.size();
	boolVector2d newGrid;
	createNewGrid(newGrid, GrigSize);

	for (auto n = 0; n < 3; n++)
	{
		turnRight(chosenGridPositions);
		for (auto i = 0; i < GrigSize; i++)
			for (auto j = 0; j < GrigSize; j++)
				if (chosenGridPositions[i][j])
					newGrid[i][j] = true;
	}

	forbiddenGridPositions = newGrid;
	turnRight(chosenGridPositions);
}

void changeConstChosenGridPositions(boolVector2d& constChosenGridPositions, boolVector2d chosenGridPositions, int constTurn)
{
	if (constTurn > 0)
		for (auto k = 0; k < constTurn; k++)
			turnLeft(chosenGridPositions);
	else
		for (auto k = 0; k < abs(constTurn); k++)
			turnRight(chosenGridPositions);

	constChosenGridPositions = chosenGridPositions;
}


void displayEncryptedGrid(stringVector2d encryptedGridBlocks, boolVector2d chosenGridPositions, boolVector2d forbiddenGridPositions)
{
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	for (auto i = 0; i < encryptedGridBlocks.size(); i++)
	{
		std::cout << "                ";
		for (auto j = 0; j < encryptedGridBlocks[0].size(); j++)
			std::cout << j << std::setw(2);
	}
	SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "\n";

	for (auto i = 0; i < encryptedGridBlocks[0].size(); i++)
	{
		for (auto k = 0; k < encryptedGridBlocks.size(); k++)
		{
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << std::setw(15) << i << std::setw(2);
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

			for (auto j = 0; j < encryptedGridBlocks[0].size(); j++)
			{
				if (chosenGridPositions[i][j])
				{
					SetConsoleTextAttribute(hOUTPUT, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					std::cout << encryptedGridBlocks[k][i][j];
					std::cout << std::setw(2);
					SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				}
				else
					if (forbiddenGridPositions[i][j])
					{
						SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_INTENSITY);
						std::cout << encryptedGridBlocks[k][i][j];
						std::cout << std::setw(2);
						SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					}
					else {
						std::cout << encryptedGridBlocks[k][i][j];
						std::cout << std::setw(2);
					}
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

void displayEncriptedText(stringVector2d encryptedGridBlocks, boolVector2d chosenGridPositions, int constTurn, std::string& encriptedText)
{
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	size_t blocksCount = encryptedGridBlocks.size();
	size_t gridSize = encryptedGridBlocks[0].size();
	size_t textSize = blocksCount * gridSize * gridSize;
	encriptedText.clear();

	for (auto i = 0; i < textSize; i++)
		encriptedText.append(".");

	int posText = 0;

	for (auto k = 0; k < blocksCount; k++)
		for (auto n = 0; n < 4; n++)
		{
			for (auto i = 0; i < gridSize; i++)
				for (auto j = 0; j < gridSize; j++)
					if (chosenGridPositions[i][j])
					{
						encriptedText[k * gridSize * gridSize + n * ((gridSize * gridSize) / 4) + posText] = encryptedGridBlocks[k][i][j];
						posText++;
					}

			turnRight(chosenGridPositions);
			posText = 0;
		}

	std::cout << "�������� �����:\n";

	for (auto i = 0; i < 4; i++)
	{
		if (i == constTurn)
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		for (auto k = 0; k < blocksCount; k++)
		{
			for (auto j = 0; j < (gridSize * gridSize) / 4; j++)
				std::cout << encriptedText[k * gridSize * gridSize + i * ((gridSize * gridSize) / 4) + j];
			std::cout << "   ";
		}
		if (i == constTurn)
			SetConsoleTextAttribute(hOUTPUT, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "\n";
	}
	std::cout << "\n";
}

void work(stringVector2d encryptedGridBlocks)
{
	boolVector2d chosenGridPositions;
	boolVector2d constChosenGridPositions;
	boolVector2d forbiddenGridPositions;
	std::string encriptedText;
	const size_t sizeGrid = encryptedGridBlocks[0].size();

	createPositionsGrid(chosenGridPositions, sizeGrid);
	constChosenGridPositions = chosenGridPositions;
	createPositionsGrid(forbiddenGridPositions, sizeGrid);

	int i = 0;
	int j = 0;
	int turn = 0;
	int constTurn = 0;

	while (i != -1)
	{
		system("cls");
		displayEncryptedGrid(encryptedGridBlocks, chosenGridPositions, forbiddenGridPositions);
		displayEncriptedText(encryptedGridBlocks, constChosenGridPositions, constTurn, encriptedText);

		std::cout << "������� ����������: ";
		std::cin >> i;  // ���� ������ ����������

		if (i == -1)  // ����� 
			break;
		if (i == -2)  // ������� 
		{
			chosenGridPositions.clear();
			createPositionsGrid(chosenGridPositions, sizeGrid);
			changeConstChosenGridPositions(constChosenGridPositions, chosenGridPositions, constTurn);
			changeForbiddenGridPositions(chosenGridPositions, forbiddenGridPositions);
			continue;
		}

		if (i == -3)  // ������� �������
		{
			std::cout << "������� ������� �������: ";
			std::cin >> turn;

			constTurn += turn;

			if (constTurn > 3)
				do {
					constTurn -= 4;
				} while (constTurn > 3);

				if (turn > 0)
					for (auto k = 0; k < turn; k++)
						turnRight(chosenGridPositions);
				else
					for (auto k = 0; k < abs(turn); k++)
						turnLeft(chosenGridPositions);
				changeForbiddenGridPositions(chosenGridPositions, forbiddenGridPositions);
				continue;
		}

		std::cin >> j;  // ���� ������ ����������

		if ((i < 0) || (i >= sizeGrid) || (j < 0) || (j >= sizeGrid))
		{
			std::cout << "��������� ����������.\n";
			system("pause");
			continue;
		}

		if (!forbiddenGridPositions[i][j])
		{
			if (chosenGridPositions[i][j])
				chosenGridPositions[i][j] = false;
			else
				chosenGridPositions[i][j] = true;

			changeConstChosenGridPositions(constChosenGridPositions, chosenGridPositions, constTurn);
			changeForbiddenGridPositions(chosenGridPositions, forbiddenGridPositions);
		}
		else
		{
			std::cout << "��������� ���������.\n";
			system("pause");
		}
	}

	save(constChosenGridPositions, encriptedText, encryptedGridBlocks.size());
}

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(.1251);
	SetConsoleOutputCP(.1251);

	const int gridSize = 8;  // ������ �������
	std::string encryptedText;
	std::string encryptedTextFileName = "Encrypted text.txt";

	readFile(encryptedText, encryptedTextFileName);  // ��������� ���������� 

	stringVector2d encryptedGridBlocks;
	createEncryptedGrid(encryptedGridBlocks, encryptedText, gridSize);  // ������ ������ ������� �����������

	work(encryptedGridBlocks);

	system("pause");

	return 0;
}