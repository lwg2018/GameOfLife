/*
작성일 : 2017. 05. 13
작성자 : 컴퓨터소프트웨어학과 2014726075 이운기
수정일 : 2017. 05. 24, 2017. 05. 25
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// 클래스 선언
class LifeGame
{
public:
	LifeGame(); // 생성자
	~LifeGame(); // 소멸자
	void initialize(int w, int h); // 초기화

	int getWidth() const; // get 함수
	int getHeight() const;
	bool getState(int i, int j) const;

	void setState(int i, int j, bool s); // set 함수
	void clear(); // 모든 칸 false
	void update(); // 현재 상태를 다음 상태로 전환
	int check(int i, int j); // 주위 세포 검사

	friend ostream& operator<<(ostream& os, const LifeGame& game);
private:
	int width;
	int height;
	bool* current_cells; // 현재 상태
	bool* next_cells; // 다음 상태
};


// 멤버 함수 정의
LifeGame::LifeGame() : width(0), height(0) {
	current_cells = nullptr;
	next_cells = nullptr;
}

LifeGame::~LifeGame() {
	delete[] current_cells; // 포인터를 delete
	delete[] next_cells;

	current_cells = nullptr;
	next_cells = nullptr;
}

void LifeGame::initialize(int w, int h) {
	width = w;
	height = h;

	if (current_cells != nullptr || next_cells != nullptr) { // 비어있지 않다면 delete
		delete[] current_cells;
		delete[] next_cells;

		current_cells = nullptr;
		next_cells = nullptr;
	}

	current_cells = new bool[width * height]; // 동적 할당
	next_cells = new bool[width * height];

	if (current_cells == nullptr || next_cells == nullptr) { // 할당이 실패했을 경우
		cout << "메모리 할당 실패" << endl;
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

bool LifeGame::getState(int i, int j) const { // j번째 행, i번째 열
	return *(current_cells + j*width + i);
}

void LifeGame::setState(int i, int j, bool s) { // j번째 행, i번째 열
	*(current_cells + j*width + i) = s;
}

void LifeGame::clear() {
	int i = 0, j = 0;

	for (i = 0; i < height; i++) // i번째 행, j번째 열
		for (j = 0; j < width; j++) {
			*(current_cells + i*width + j) = false; // 모두 죽어있는 상태로 초기화
			*(next_cells + i*width + j) = false;
		}
}

int LifeGame::check(int i, int j) { // i번째 행, j번째 열
	int life_cell = 0;

	// 주위 8개의 세포 검사(살아있다면 life_cell을 1 늘려준다)
	// i와 j가 맨 끝이거나 0일 경우를 대비해 너비나 높이값을 더한후 나눠준다
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
	int life_cell = 0; // 자신 주위에 살아있는 cell의 갯수

	for (i = 0; i < height; i++) { // i번째 행, j번째 열
		for (j = 0; j < width; j++) {
			// 주위 8개의 세포를 검사하는 함수
			life_cell = check(i, j);

			// 주위의 세포 현황에 따라 next_cells에 저장
			if (getState(j, i)) { // 검사한 위치의 세포가 살아있다면
								  // 주위의 살아있는 세포 갯수에 따라 next_cell에 상태 저장
				if (life_cell <= 1 || life_cell >= 4)
					*(next_cells + i*width + j) = false;
				else if (life_cell == 2 || life_cell == 3)
					*(next_cells + i*width + j) = true;
			}
			else { // 검사한 위치의 세포가 죽어있다면
				   // 주위의 살아있는 세포 갯수에 따라 next_cell에 상태 저장
				if (life_cell == 3)
					*(next_cells + i*width + j) = true;
				else
					*(next_cells + i*width + j) = false;
			}

			life_cell = 0; // 다음 셀을 검사하기 위해 초기화
		}
	}

	// 현재 상태를 다음 상태로 업데이트
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			*(current_cells + i*width + j) = *(next_cells + i*width + j);
}


// friend 함수 정의
ostream& operator<<(ostream& os, const LifeGame& game) {
	int i = 0, j = 0;

	for (i = 0; i < game.height; i++) { // i번째 행, j번째 열
		for (j = 0; j < game.width; j++) {
			if (game.getState(j, i)) // 살아있으면(참이면) O, 거짓이면 .
				os << "O ";
			else
				os << ". ";
		}
		os << endl;
	}

	return os;
}


// 아래 내용은 수정하지 말 것
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
