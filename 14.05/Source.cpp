#include <iostream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>     
#include <windows.h>
#include "AStar.h"

using namespace sf;

int main()
{
	double zel;
	double drob;
	float krd;
	
			int WIDTH_MAP;
			int HEIGHT_MAP;
			int r;
			const int N = 32;//размер в пикселях картинки
			int x,y;
			int klvx = 0;
			int x1, y1, x2, y2;
			int dl = 0;
			int psdl = 0;
			std::cout << "ЛКМ - поставить камешек; ПКМ - убрать камешек; Space - поставить ворота; BackSpace - убрать ворота "<< "\n";
			std::cout<<"wwedite razmernost' polya dlina, shirina v pixel(example: 800x600 )"<<"\n";
			//ввод
			std::cin >> WIDTH_MAP;
			WIDTH_MAP = WIDTH_MAP/N;
			std::cin >> HEIGHT_MAP;
			HEIGHT_MAP = HEIGHT_MAP / N;
			if (WIDTH_MAP < 4) {
				std::cout << "wwedeni neprav znach" << "\n";
			}
			if (HEIGHT_MAP < 4) {
				std::cout << "wwedeni neprav znach" << "\n";
			}
			
			
			AStar::Generator generator;
			generator.setWorldSize({  HEIGHT_MAP,WIDTH_MAP });
			generator.setHeuristic(AStar::Heuristic::euclidean);
			generator.setDiagonalMovement(false);//хотьба по диагонали
			//создание динам матрицы
			String **TileMap;
			TileMap = new String * [HEIGHT_MAP];
			for (int i = 0; i < HEIGHT_MAP; i++)
				TileMap[i] = new String [WIDTH_MAP];

			//заполнениц матрицы
			
			std::cout << "esli vi xotite stroit' to wwedite <1>, inache wwedite luboe drygoe chislo"<<"\n";
			std::cin >> r;
			if (r==1){
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						TileMap[i][j] = ' ';
						generator.addCollision({ i, j });
				}
					
			}
			else{
				for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
					TileMap[i][j] = 's';
			}
			for (int j = 0; j < WIDTH_MAP; j++){
					TileMap[0][j] = '0';
					TileMap[HEIGHT_MAP-1][j]='0';
					generator.addCollision({ 0, j });
					generator.addCollision({ HEIGHT_MAP - 1, j });
				}
				for (int j = 1; j < HEIGHT_MAP-1; j++){
					TileMap[j][0] = '0';
					TileMap[j][WIDTH_MAP-1] = '0';
					generator.addCollision({ j, 0 });
					generator.addCollision({ j, WIDTH_MAP - 1 });
				}
			//создание окна
			RenderWindow window(VideoMode(WIDTH_MAP*N, HEIGHT_MAP*N), "Labirint");
			Image map_image;
			map_image.loadFromFile("images/map.png");
			Texture map;
			map.loadFromImage(map_image);
			Sprite s_map;
			s_map.setTexture(map);
			Event event;

			
			while (window.isOpen())
			{
				Vector2i pixelPos = Mouse::getPosition(window);//pos-вектор мышки с координатами
				Vector2f pos = window.mapPixelToCoords(pixelPos);



				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}

				x=(int)(pos.x/N);
				y=(int)(pos.y/N);
				if (Mouse::isButtonPressed(Mouse::Left))
					if (x>0 && x<(WIDTH_MAP-1) && y>0 && y<(HEIGHT_MAP-1))
						if (TileMap[y][x] == ' ') {
							generator.removeCollision({ y,x });
							TileMap[y][x] = 's';
						}

				if (Mouse::isButtonPressed(Mouse::Right))
					if (x > 0 && x < (WIDTH_MAP - 1) && y>0 && y < (HEIGHT_MAP - 1)) {
						if (TileMap[y][x] == 's') TileMap[y][x] = ' ';
						generator.addCollision({ y, x });
					}
				if (Keyboard::isKeyPressed(Keyboard::Space))
						if (TileMap[y][x] == 's' && TileMap[y-1][x] != 'q' && TileMap[y+1][x] != 'q' && TileMap[y][x-1] != 'q' && TileMap[y][x+1]!='q' && klvx < 2){
							TileMap[y][x] = 'q';
							klvx++;
							if (klvx == 1) {
								x1 = y;
								y1 = x;
								std::cout << "1 point: x = " << x1 << " y = " << y1 << "\n";
							}
							if (klvx == 2) {
								x2 = y;
								y2 = x;
								std::cout << "1 point: x = " << x2 << " y = " << y2 << "\n";
							}
						}
				if (Keyboard::isKeyPressed(Keyboard::BackSpace))
						if (TileMap[y][x] == 'q'){
							TileMap[y][x] = 's';
							klvx--;
						}
				if (Keyboard::isKeyPressed(Keyboard::Enter)) {
					std::cout << "Generate path ... \n";
					auto path = generator.findPath({ x1, y1 }, { x2, y2 });
					float  zel, dr;
					int xp, yp;
					for (auto u : koord) {
						std::cout << "tochki111: " << " xy = " << u << "\n";
						
						dr = modf(u, &zel);
							xp = round(zel);
							yp = round(dr * 100);
						std::cout << "tochki222: " << " x = " << xp << " y = " << yp << "\n";
						if (TileMap[xp][yp] == 's' || TileMap[xp][yp] == 'q')
							TileMap[xp][yp] = 't';
					}
					
						
					
					for (auto& coordinate : path) {
						TileMap[coordinate.x][coordinate.y] = 'z';
						
						dl++;
					}
					
					
				}
					

				if (Keyboard::isKeyPressed(Keyboard::Escape))
						window.close();



				window.clear();

				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++)
					{
						if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(1* N, 0, N, N));//камень
						if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(0, 0, N, N));//проход
						if (TileMap[i][j] == '0')  s_map.setTextureRect(IntRect(2 * N, 0, N, N));//барьер
						if (TileMap[i][j] == 'q')  s_map.setTextureRect(IntRect(3 * N, 0, N, N));//вход\выход
						if (TileMap[i][j] == 'z')  s_map.setTextureRect(IntRect(4 * N, 0, N, N));
						if (TileMap[i][j] == 't')  s_map.setTextureRect(IntRect(5 * N, 0, N, N));//pyt'
							
						
							
						s_map.setPosition(j * N, i * N);

						window.draw(s_map);
					}
				//if (psdl == dl)
				//	window.close();
				window.display();

			}
			for (int i = 0; i < HEIGHT_MAP; i++)
				delete [] TileMap[i];
			delete [] TileMap;
			return 0;
			
	}






