#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>

using namespace sf;
int cellSize = 105;
Vector2i offset(35, 35);
int sizeLevel = 5;

struct piece {
	int x, y; // ���������� �����
	int col, row; // ����� ������� � ������
	int kind; // ��� �����
	int match; // �������� �� ����� �� ��������
	int alpha;

	piece() {
		alpha = 255;
		match = 0;
		col = 0;
		row = 0;
		grid[0][0].kind = -1;
		grid[6][6].kind = -1;
	}
	
	void swapPiece(piece other) {
		std::swap(this->col, other.col);
		std::swap(this->row, other.row);
		grid[this->row][this->col] = *this;
		grid[other.row][other.col] = other;
	}
	
} grid[6][6];


// ������������� ������ (����� �����) � ������ ���������
void completion() {
	for (int i = 1; i <= sizeLevel; i++) {
		for (int j = 1; j <= sizeLevel; j++) {
			grid[i][j].kind = rand() % (sizeLevel - 1);
			grid[i][j].col = j;
			grid[i][j].row = i;
			grid[i][j].x = offset.x + (j - 1) * cellSize;
			grid[i][j].y = offset.y + (i - 1) * cellSize;
		}
	}
}


//Match finding ����� ���������� 
//*�.� ������� ��� ������ 3-�� � �������� ������ ��� ����������*/
void matchFinding() {
	for (int i = 1; i <= sizeLevel; i++) {
		for (int j = 1; j <= sizeLevel; j++) {
			if (grid[i][j].kind == grid[i + 1][j].kind) {
				if (grid[i][j].kind == grid[i - 1][j].kind) {
					for (int n = -1; n <= 1; n++) {
						grid[i + n][j].match++;
					}
				}
			}
			if (grid[i][j].kind == grid[i][j + 1].kind) {
				if (grid[i][j].kind == grid[i][j - 1].kind) {
					for (int n = -1; n <= 1; n++) {
						grid[i][j + n].match++;
					}
				}
			}
		}
	}
}


int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(900, 600), "3 in a row with kittens");
	window.setFramerateLimit(60);

	// ��� ������� 1
	Image imBackgroundLevel1;
	imBackgroundLevel1.loadFromFile("images/lvl1.jpg");

	Texture textBackgroundLevel1;
	textBackgroundLevel1.loadFromImage(imBackgroundLevel1);

	Sprite sprBackgroundLevel1;
	sprBackgroundLevel1.setTexture(textBackgroundLevel1);

	// ��� ������� 2
	Image imBackgroundLevel2;
	imBackgroundLevel2.loadFromFile("images/lvl2.jpg");

	Texture textBackgroundLevel2;
	textBackgroundLevel2.loadFromImage(imBackgroundLevel2);

	Sprite sprBackgroundLevel2;
	sprBackgroundLevel2.setTexture(textBackgroundLevel2);

	// ��� ������� 3
	Image imBackgroundLevel3;
	imBackgroundLevel3.loadFromFile("images/lvl3.jpg");

	Texture textBackgroundLevel3;
	textBackgroundLevel3.loadFromImage(imBackgroundLevel3);

	Sprite sprBackgroundLevel3;
	sprBackgroundLevel3.setTexture(textBackgroundLevel3);

	// �����
	Image imSquareLevel1;
	imSquareLevel1.loadFromFile("images/square.png");
	imSquareLevel1.createMaskFromColor(Color(255, 255, 255));
	imSquareLevel1.createMaskFromColor(Color(200, 191, 231), 150);

	Texture textSquareLevel1;
	textSquareLevel1.loadFromImage(imSquareLevel1);

	Sprite sprSquareLevel1;
	sprSquareLevel1.setTexture(textSquareLevel1);

	// �����
	Image imLapa;
	imLapa.loadFromFile("images/lapa.png");

	Texture textLapa;
	textLapa.loadFromImage(imLapa);

	Sprite sprLapa;
	sprLapa.setTexture(textLapa);

	// ������ ��� ������ ������ � ����
	Image imLabel;
	imLabel.loadFromFile("images/label.png");
	imLabel.createMaskFromColor(Color(255, 255, 255));
	imLabel.createMaskFromColor(Color(200, 191, 231), 150);

	Texture textLabel;
	textLabel.loadFromImage(imLabel);

	Sprite sprLabel;
	sprLabel.setTexture(textLabel);

	// �����
	Font font;
	font.loadFromFile("Comic_Sans_MS.ttf");

	// ����� ������, ������, ������
	Text textLevel("", font, 50);
	textLevel.setStyle(sf::Text::Bold);

	// ����� ��������� ������
	Text textLabels("", font, 30);
	textLabels.setStyle(sf::Text::Bold);
	

	// ������ ������ �����
	std::vector<Sprite> colorLapa;
	sprLapa.setColor(Color(208, 9, 199)); // ����������
	colorLapa.push_back(sprLapa);
	sprLapa.setColor(Color(0, 0, 255)); // �����
	colorLapa.push_back(sprLapa);
	sprLapa.setColor(Color(255, 0, 0)); // �������
	colorLapa.push_back(sprLapa);
	sprLapa.setColor(Color(0, 255, 0)); // �������
	colorLapa.push_back(sprLapa);
	sprLapa.setColor(Color(37, 244, 187)); // �������
	colorLapa.push_back(sprLapa);
	sprLapa.setColor(Color(239, 234, 22)); // ������ 
	colorLapa.push_back(sprLapa);
	
	completion();

	int x0, y0, x, y; // ������� � ������ �����, ������� ������ �������
	int click = 0; // ��������� �����
	Vector2i pos; // ����������, ���� �� �������. ������ ������� �� ������ ������
	bool isSwap = false; // ����, ��� �� ���-�� ��������
	bool isMoving = false;
	int points = 0; // ���-�� ��������� �����
	bool win = false; // ����, ����� �� ������ ������ ������ ����� ������
	int start_click = 0;
	int level = 0;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) { 
					if (!isSwap && !isMoving && !win) {
						click++;
					}
					pos = Mouse::getPosition(window) - offset;
					if ((Mouse::getPosition(window).x >= 600) && // ��� ������ mix
						(Mouse::getPosition(window).x <= 850) &&
						(Mouse::getPosition(window).y >= 455) &&
						(Mouse::getPosition(window).y <= 540) && !win) {
							completion();
					}
					if ((Mouse::getPosition(window).x >= 325) && // ��� ������ next
						(Mouse::getPosition(window).x <= 575) &&
						(Mouse::getPosition(window).y >= 420) &&
						(Mouse::getPosition(window).y <= 520) && level == 0) {
						start_click++;
					}
				}
			}
		}
		
		// �������� - �����
		if (click == 1) {
			x0 = pos.x / cellSize + 1; // �����������, ����� ��� ������
			y0 = pos.y / cellSize + 1; // ��������, 1-1, 3-5 � ��
		}
		if (click == 2) {
			x = pos.x / cellSize + 1; // ����� ������ ������
			y = pos.y / cellSize + 1;
			if (abs(x - x0) + abs(y - y0) == 1) { // ���������, ��� ������� �� ��������
				grid[y0][x0].swapPiece(grid[y][x]);
				isSwap = 1;
				click = 0;
			}
			else {
				click = 1;
			}
		}
		
		matchFinding();

		isMoving = 0;
		for (int i = 1; i <= sizeLevel; i++) {
			for (int j = 1; j <= sizeLevel; j++) {
				piece& p = grid[i][j];
				int dx, dy;
				for (int n = 0; n < 4; n++) {  // 4 - speed
					dx = p.x - p.col * cellSize;
					dy = p.y - p.row * cellSize;
					if (dx) {
						p.x -= dx / abs(dx);
					}
					if (dy) {
						p.y -= dy / abs(dy);
					}
				}
				if (dx || dy) {
					isMoving = 1;
				}
			}
		}

		
		/* ����� ���� �����������, ��� �� �������� 
		//Deleting amimation
		if (!isMoving) {
			for (int i = 1; i <= 5; i++) {
				for (int j = 1; j <= 5; j++) {
					if (grid[i][j].match) {
						if (grid[i][j].alpha > 10) {
							grid[i][j].alpha -= 10;
							isMoving = true;
						}
					}
				}
			}
		} 

		
		//Get score
		int score = 0;
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 5; j++) {
				score += grid[i][j].match;
			}
		}
		*/ 

		//Second swap if no match
		if (isSwap && !isMoving) {
			//if (!score) {
				grid[y0][x0].swapPiece(grid[y][x]);
				isSwap = 0;
			//}
		}
		
		//Update grid
		if (!isMoving) {
			for (int i = sizeLevel; i > 0; i--) {
				for (int j = 1; j <= sizeLevel; j++) {
					if (grid[i][j].match) {
						for (int n = i; n > 0; n--) {
							if (!grid[n][j].match) {
								grid[n][j].swapPiece(grid[i][j]);
								break;
							}
						}
					}
				}
			}
			
			for (int j = 1; j <= sizeLevel; j++) {
				for (int i = sizeLevel, n = 0; i > 0; i--) {
					if (grid[i][j].match) {
						points += 10;
						grid[i][j].kind = rand() % (sizeLevel - 1);
						grid[i][j].y = -cellSize * n++;
						grid[i][j].match = 0;
						//grid[i][j].alpha = 255;
					}
				}
			} 
		}
		if (points > 1000) {
			win = true;
			start_click = 0;
		}

		window.clear();
		// ������ ���
		window.draw(sprBackgroundLevel1);

		
		if (start_click == 0 && level == 0) {
			sprLabel.setScale(Vector2f(2, 3));
			sprLabel.setPosition(200, 100);
			window.draw(sprLabel);
			textLevel.setString(" Find out the story\nof two cats by\ncompleting the task \nof levels");
			textLevel.setPosition(210, 105);
			window.draw(textLevel);
			
		}

		if ((start_click == 0 && level == 0) ||
			(start_click == 1) || (win)) {
			sprLabel.setScale(Vector2f(1, 1));
			sprLabel.setPosition(325, 420);
			window.draw(sprLabel);
			textLevel.setString("next");
			textLevel.setPosition(390, 435);
			window.draw(textLevel);
		}

		if (start_click == 1) {
			sprLabel.setScale(Vector2f(2, 3));
			sprLabel.setPosition(200, 100);
			window.draw(sprLabel);
			if (level == 0) {
				// ������� 1
			}
			if (level == 1) {
				// ������� 2
			}
			if (level == 2) {
				// ������� 3
			}
		}

		if (win) {
			sprLabel.setScale(Vector2f(2, 3));
			sprLabel.setPosition(200, 100);
			window.draw(sprLabel);
			textLevel.setString("�����-�� �����");
			textLevel.setPosition(390, 435);
			window.draw(textLevel);
		}

		if (start_click == 2) {
			level++;
		}

		if (level > 0 && !win) {
			// ������ �����
			for (int i = 0; i < sizeLevel; i++) {
				for (int j = 0; j < sizeLevel; j++) {
					switch (sizeLevel) {
					case 5: 
						sprSquareLevel1.setPosition(35 + 105 * i, 35 + 105 * j);
					case 6:
						sprSquareLevel1.setPosition(35 + 89 * i, 35 + 89 * j);//84
					case 7:
						sprSquareLevel1.setPosition(35 + 77 * i, 35 + 77 * j);//72
					default:
						break;
					}
					window.draw(sprSquareLevel1);
				}
			}

			// ������ �����
			for (int i = 1; i <= sizeLevel; i++) {
				for (int j = 1; j <= sizeLevel; j++) {
					piece p = grid[i][j];
					sprLapa = colorLapa[p.kind];
					//sprLapa.setColor(colorLapa[p.kind]);
					//sprLapa.setColor(sprLapa.getColor() + Color(0, 0, 0, p.alpha));
					sprLapa.setPosition(p.x, p.y);
					sprLapa.move(offset.x - cellSize, offset.y - cellSize);
					window.draw(sprLapa);
				}
			}


			// ������ ��� ����
			sprLabel.setScale(Vector2f(1, 0.75));

			sprLabel.setPosition(600, 55);
			window.draw(sprLabel);
			textLevel.setString("Level 1");
			textLevel.setPosition(645, 63);
			window.draw(textLevel);

			sprLabel.setPosition(600, 140);
			window.draw(sprLabel);
			textLabels.setString("Purpose: 3000");
			textLabels.setPosition(620, 160);
			window.draw(textLabels);

			sprLabel.setPosition(600, 250);
			window.draw(sprLabel);
			textLabels.setString("Points Scored:");
			textLabels.setPosition(622, 270);
			window.draw(textLabels);

			sprLabel.setPosition(600, 335);
			window.draw(sprLabel);
			textLevel.setString(std::to_string(points));
			if (points < 100) {
				textLevel.setPosition(690, 345);
			}
			else if (points < 1000) {
				textLevel.setPosition(680, 345);
			}
			else {
				textLevel.setPosition(670, 345);
			}
			window.draw(textLevel);

			sprLabel.setScale(Vector2f(1, 0.85));

			sprLabel.setPosition(600, 455);
			window.draw(sprLabel);
			textLevel.setString("MIX");
			textLevel.setPosition(670, 465);
			window.draw(textLevel);
		}
		window.display();
	}
}
