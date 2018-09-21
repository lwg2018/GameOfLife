/*
�ۼ��� : 2017. 05. 13
�ۼ��� : ��ǻ�ͼ���Ʈ�����а� 2014726075 �̿��
������ : 2017. 05. 24, 2017. 05. 25
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Ŭ���� ����
class LifeGame
{
public:
	LifeGame(); // ������
	~LifeGame(); // �Ҹ���
	void initialize(int w, int h); // �ʱ�ȭ

	int getWidth() const; // get �Լ�
	int getHeight() const;
	bool getState(int i, int j) const;

	void setState(int i, int j, bool s); // set �Լ�
	void clear(); // ��� ĭ false
	void update(); // ���� ���¸� ���� ���·� ��ȯ
	int check(int i, int j); // ���� ���� �˻�

	friend ostream& operator<<(ostream& os, const LifeGame& game);
private:
	int width;
	int height;
	bool* current_cells; // ���� ����
	bool* next_cells; // ���� ����
};


// ��� �Լ� ����
LifeGame::LifeGame() : width(0), height(0) {
	current_cells = nullptr;
	next_cells = nullptr;
}

LifeGame::~LifeGame() {
	delete[] current_cells; // �����͸� delete
	delete[] next_cells;

	current_cells = nullptr;
	next_cells = nullptr;
}

void LifeGame::initialize(int w, int h) {
	width = w;
	height = h;

	if (current_cells != nullptr || next_cells != nullptr) { // ������� �ʴٸ� delete
		delete[] current_cells;
		delete[] next_cells;

		current_cells = nullptr;
		next_cells = nullptr;
	}

	current_cells = new bool[width * height]; // ���� �Ҵ�
	next_cells = new bool[width * height];

	if (current_cells == nullptr || next_cells == nullptr) { // �Ҵ��� �������� ���
		cout << "�޸� �Ҵ� ����" << endl;
		exit(1);
	}

	clear();
}

int LifeGame::getWidth() const {
	return width;
}

int LifeGame::getHeight() const {
	return height;
}

bool LifeGame::getState(int i, int j) const { // j��° ��, i��° ��
	return *(current_cells + j*width + i);
}

void LifeGame::setState(int i, int j, bool s) { // j��° ��, i��° ��
	*(current_cells + j*width + i) = s;
}

void LifeGame::clear() {
	int i = 0, j = 0;

	for (i = 0; i < height; i++) // i��° ��, j��° ��
		for (j = 0; j < width; j++) {
			*(current_cells + i*width + j) = false; // ��� �׾��ִ� ���·� �ʱ�ȭ
			*(next_cells + i*width + j) = false;
		}
}

int LifeGame::check(int i, int j) { // i��° ��, j��° ��
	int life_cell = 0;

	// ���� 8���� ���� �˻�(����ִٸ� life_cell�� 1 �÷��ش�)
	// i�� j�� �� ���̰ų� 0�� ��츦 ����� �ʺ� ���̰��� ������ �����ش�
	if (getState((j - 1 + width) % width, (i - 1 + height) % height))
		life_cell++;

	if (getState((j + width) % width, (i - 1 + height) % height))
		life_cell++;

	if (getState((j + 1 + width) % width, (i - 1 + height) % height))
		life_cell++;

	if (getState((j - 1 + width) % width, (i + height) % height))
		life_cell++;

	if (getState((j + 1 + width) % width, (i + height) % height))
		life_cell++;

	if (getState((j - 1 + width) % width, (i + 1 + height) % height))
		life_cell++;

	if (getState((j + width) % width, (i + 1 + height) % height))
		life_cell++;

	if (getState((j + 1 + width) % width, (i + 1 + height) % height))
		life_cell++;

	return life_cell;
}

void LifeGame::update() {
	int i = 0, j = 0;
	int life_cell = 0; // �ڽ� ������ ����ִ� cell�� ����

	for (i = 0; i < height; i++) { // i��° ��, j��° ��
		for (j = 0; j < width; j++) {
			// ���� 8���� ������ �˻��ϴ� �Լ�
			life_cell = check(i, j);

			// ������ ���� ��Ȳ�� ���� next_cells�� ����
			if (getState(j, i)) { // �˻��� ��ġ�� ������ ����ִٸ�
								  // ������ ����ִ� ���� ������ ���� next_cell�� ���� ����
				if (life_cell <= 1 || life_cell >= 4)
					*(next_cells + i*width + j) = false;
				else if (life_cell == 2 || life_cell == 3)
					*(next_cells + i*width + j) = true;
			}
			else { // �˻��� ��ġ�� ������ �׾��ִٸ�
				   // ������ ����ִ� ���� ������ ���� next_cell�� ���� ����
				if (life_cell == 3)
					*(next_cells + i*width + j) = true;
				else
					*(next_cells + i*width + j) = false;
			}

			life_cell = 0; // ���� ���� �˻��ϱ� ���� �ʱ�ȭ
		}
	}

	// ���� ���¸� ���� ���·� ������Ʈ
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			*(current_cells + i*width + j) = *(next_cells + i*width + j);
}


// friend �Լ� ����
ostream& operator<<(ostream& os, const LifeGame& game) {
	int i = 0, j = 0;

	for (i = 0; i < game.height; i++) { // i��° ��, j��° ��
		for (j = 0; j < game.width; j++) {
			if (game.getState(j, i)) // ���������(���̸�) O, �����̸� .
				os << "O ";
			else
				os << ". ";
		}
		os << endl;
	}

	return os;
}


// �Ʒ� ������ �������� �� ��
void setBoat(LifeGame& life);
void setToad(LifeGame& life);
void setGlider(LifeGame& life);
void setCross(LifeGame& life);
void setRandom(LifeGame& life);

void main()
{
	int width = 0, height = 0;
	char answer = ' ';

	cout << "Input grid width: ";
	cin >> width;

	cout << "Input grid height: ";
	cin >> height;

	LifeGame life;
	life.initialize(width, height);
	do
	{
		int n, i;

		cout << endl << "[Menu]" << endl;
		cout << "(0) No change\n";
		cout << "(1) Boat\n";
		cout << "(2) Toad\n";
		cout << "(3) Glider\n";
		cout << "(4) Cross\n";
		cout << "(5) Random\n";
		cout << "Select a command (0-5) : ";
		cin >> n;

		switch (n) {
		case 0:							break;
		case 1:	setBoat(life);		break;
		case 2:	setToad(life);		break;
		case 3:	setGlider(life);		break;
		case 4: setCross(life);		break;
		case 5:	setRandom(life);		break;
		default:						break;
		}

		for (i = 0; i < 10; i++)
		{
			cout << "[" << i << "] generation: " << endl;
			cout << life;
			cout << endl;
			life.update();
		}

		cout << "Continue (Y/N)? ";
		cin >> answer;
	} while (answer == 'Y' || answer == 'y');
}

void setBoat(LifeGame& life)
{
	int w = life.getWidth();
	int h = life.getHeight();

	if (w < 3 || h < 3)
	{
		return;
	}

	int ci = w / 2;
	int cj = h / 2;

	life.clear();
	life.setState(ci - 1, cj, true);
	life.setState(ci + 1, cj, true);
	life.setState(ci, cj - 1, true);
	life.setState(ci, cj + 1, true);
	life.setState(ci - 1, cj - 1, true);
}

void setToad(LifeGame& life)
{
	int w = life.getWidth();
	int h = life.getHeight();

	if (w < 5 || h < 3)
	{
		return;
	}

	int ci = w / 2;
	int cj = h / 2;

	life.clear();
	life.setState(ci, cj, true);
	life.setState(ci - 1, cj, true);
	life.setState(ci - 2, cj, true);
	life.setState(ci, cj - 1, true);
	life.setState(ci - 1, cj - 1, true);
	life.setState(ci + 1, cj - 1, true);
}

void setGlider(LifeGame& life)
{
	int w = life.getWidth();
	int h = life.getHeight();

	if (w < 3 || h < 3)
	{
		return;
	}

	int ci = w / 2;
	int cj = h / 2;

	life.clear();
	life.setState(ci - 1, cj, true);
	life.setState(ci, cj - 1, true);
	life.setState(ci, cj + 1, true);
	life.setState(ci - 1, cj - 1, true);
	life.setState(ci + 1, cj - 1, true);
}

void setCross(LifeGame& life)
{
	int w = life.getWidth();
	int h = life.getHeight();

	if (w < 3 || h < 3)
	{
		return;
	}

	int ci = w / 2;
	int cj = h / 2;

	life.clear();
	life.setState(ci, cj, true);
	life.setState(ci - 1, cj, true);
	life.setState(ci + 1, cj, true);
	life.setState(ci, cj - 1, true);
	life.setState(ci, cj + 1, true);
}

void setRandom(LifeGame& life)
{
	int w = life.getWidth();
	int h = life.getHeight();
	int num_cells = w * h;
	int num_live_cells = (int)((float)num_cells * 0.1f);

	life.clear();
	for (int c = 0; c < num_live_cells; c++)
	{
		int i = rand() % w;
		int j = rand() % h;

		life.setState(i, j, true);
	}
}
