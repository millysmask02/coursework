#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace sf;
int cellSize = 105;
Vector2i offset(35, 35);

struct piece {
	int x, y, col, row, kind, match, alpha;

	piece() {
		alpha = 255;
		match = 0;
		col = 0;
		row = 0;
		grid[0][0];
	}
	
	void swapPiece(piece other) {
		std::swap(this->col, other.col);
		std::swap(this->row, other.row);
		grid[this->row][this->col] = *this;
		grid[other.row][other.col] = other;
	}
	
} grid[6][6];


int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(900, 600), "3 in a row with kittens");
	window.setFramerateLimit(60);

	// Фон
	Image imBackgroundLevel1;
	imBackgroundLevel1.loadFromFile("images/lvl1.jpg");

	Texture textBackgroundLevel1;
	textBackgroundLevel1.loadFromImage(imBackgroundLevel1);

	Sprite sprBackgroundLevel1;
	sprBackgroundLevel1.setTexture(textBackgroundLevel1);

	// Сетка
	Image imSquareLevel1;
	imSquareLevel1.loadFromFile("images/square1.png");
	imSquareLevel1.createMaskFromColor(Color(255, 255, 255));
	imSquareLevel1.createMaskFromColor(Color(200, 191, 231), 150);

	Texture textSquareLevel1;
	textSquareLevel1.loadFromImage(imSquareLevel1);

	Sprite sprSquareLevel1;
	sprSquareLevel1.setTexture(textSquareLevel1);

	// Лапки
	Image imLapa;
	imLapa.loadFromFile("images/lapa1.png");

	Texture textLapa;
	textLapa.loadFromImage(imLapa);

	Sprite sprLapa;
	sprLapa.setTexture(textLapa);

	// Ячейки для данных уровня и цели
	Image imLabel;
	imLabel.loadFromFile("images/label1.png");
	imLabel.createMaskFromColor(Color(255, 255, 255));
	imLabel.createMaskFromColor(Color(200, 191, 231), 150);

	Texture textLabel;
	textLabel.loadFromImage(imLabel);

	Sprite sprLabel, sprSmallLabels;
	sprLabel.setTexture(textLabel);
	sprSmallLabels.setTexture(textLabel);

	// Шрифт
	Font font;
	font.loadFromFile("Comic_Sans_MS.ttf");

	// Текст уровня
	Text textLevel("", font, 30);
	textLevel.setStyle(sf::Text::Bold);

	// Текст набранных баллов
	Text textPoints("", font, 30);
	textPoints.setStyle(sf::Text::Bold);


	// Массив цветов лапок
	// пока будет рандом, потом пропишу в отдельном файле конкретные цвета
	// мб придется даже поредачить саму лапку, чтобы цвета лучше ложились
	std::vector<Sprite> colorLapa;
	for (int i = 0; i < 4; i++) {
		sprLapa.setColor(Color(rand() % 255, rand() % 200, rand() % 150, 220));
		colorLapa.push_back(sprLapa);
	}

	// Распределение цветов (типов лапок) и запись координат
	for (int i = 1; i <= 5; i++) {
		for (int j = 1; j <= 5; j++) {
			grid[i][j].kind = rand() % 4;
			grid[i][j].col = j;
			grid[i][j].row = i;
			grid[i][j].x = offset.x + (j - 1) * cellSize;
			grid[i][j].y = offset.y + (i - 1) * cellSize;
		}
	}


	int x0, y0, x, y;
	int click = 0;
	Vector2i pos;
	bool isSwap = false, isMoving = false;
	int points = 0;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					if (!isSwap && !isMoving) {
						click++;
					}
					pos = Mouse::getPosition(window) - offset;
				}
			}
		}

		// Кликанье - обмен
		if (click == 1) {
			x0 = pos.x / cellSize + 1;
			y0 = pos.y / cellSize + 1;
		}
		if (click == 2) {
			x = pos.x / cellSize + 1;
			y = pos.y / cellSize + 1;
			if (abs(x - x0) + abs(y - y0) == 1) {
				grid[y0][x0].swapPiece(grid[y][x]);
				isSwap = 1;
				click = 0;
			}
			else {
				click = 1;
			}
		}

		//Match finding
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 5; j++) {
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
		
		isMoving = 0;
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 5; j++) {
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

		
		/* Здесь надо разобраться, оно не работает 
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
			for (int i = 5; i > 0; i--) {
				for (int j = 1; j <= 5; j++) {
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
			for (int j = 1; j <= 5; j++) {
				for (int i = 5, n = 0; i > 0; i--) {
					if (grid[i][j].match) {
						grid[i][j].kind = rand() % 4;
						grid[i][j].y = -cellSize * n++;
						grid[i][j].match = 0;
						//grid[i][j].alpha = 255;
					}
				}
			}
		}


		window.clear();
		// Рисуем фон
		window.draw(sprBackgroundLevel1);

		// Рисуем сетку
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				sprSquareLevel1.setPosition(35 + 105 * i, 35 + 105 * j);
				window.draw(sprSquareLevel1);
			}
		}

		// Рисуем лапки
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 5; j++) {
				piece p = grid[i][j];
				sprLapa = colorLapa[p.kind];
				//sprLapa.setColor(colorLapa[p.kind]);
				//sprLapa.setColor(sprLapa.getColor() + Color(0, 0, 0, p.alpha));
				sprLapa.setPosition(p.x, p.y);
				sprLapa.move(offset.x - cellSize, offset.y - cellSize);
				window.draw(sprLapa);
			}
		}

		// Рисуем ячейку для данных об уровне
		sprLabel.setPosition(600, 70);
		window.draw(sprLabel);
		// текст для нее
		textLevel.setString("Level 1");
		textLevel.setPosition(675, 80);
		window.draw(textLevel);
		textLevel.setString("Purpose: 6000");
		textLevel.setPosition(620, 120);
		window.draw(textLevel);

		// Рисуем ячейку набранных баллов
		sprLabel.setPosition(600, 190);
		window.draw(sprLabel);
	
		textPoints.setString("Points scored:");
		textPoints.setPosition(622, 200);
		window.draw(textPoints);
		// надо сделать переменную под баллы и вывести ее
		// например, 100/6000 и тд.. шоб она менялась по мере удаления фишек

		// Рисуем ячейку "поменять цвета"
		sprSmallLabels.setScale(Vector2f(1, 0.75));
		sprSmallLabels.setPosition(600, 360);
		window.draw(sprSmallLabels);

		// "Рисуем ячейку "перемешать"
		sprSmallLabels.setPosition(600, 455);
		window.draw(sprSmallLabels);


		window.display();
	}
}
