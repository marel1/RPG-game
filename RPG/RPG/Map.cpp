#include "Map.h"
#include <regex>
#include <iostream>
#include <cmath>
#include <fstream>

	Tile::Tile(sf::Vector2i location, Shared* _shared) : window(_shared->renderWindow), shared(_shared) {
	sprite.setTexture(*shared->textureManager.GetResource("dep\\ts\\lev0.png"));
	sprite.setTextureRect({ location.x*TileSize, location.y*TileSize, TileSize, TileSize });
	sprite.setOrigin(TileSize / 2, TileSize / 2);
}
Tile::~Tile() {
	shared->textureManager.FreeResource("dep\\ts\\lev0.png");
	std::cout << "tile destroyed" << std::endl;
}
void Tile::Draw() {
	window->draw(sprite);
}
void Tile::Position(sf::Vector2f _position) {
	sprite.setPosition(_position);
}


Array3d::~Array3d() {
	delete[]arr;
};
void Array3d::Create(int _layer, int _column, int _row) {
	delete[]arr;
	layer = _layer;
	column = _column;
	row = _row;
	arr = new TileInfo[layer*column*row];
}
TileInfo& Array3d::operator()(int K, int L, int M) {
	return arr[(K*column + L)*row + M];
}


Map::Map(Shared* _shared) : shared(_shared),player() {
	//player.setSize({ TileSize/2, TileSize });
	/*player.setFillColor(sf::Color::White);*/
	//player.setOrigin({ player.getSize().x/2, 3* player.getSize().y / 4 });

	
}

void Map::Load(int _level) {
	std::string tmp = "dep\\map\\lev" + std::to_string(_level) + ".txt";

	std::ifstream file;
	file.open(tmp);
	if (!file) {
		std::cout << "couldn't open map for level" << _level << std::endl;
		return;
	}

	int layer = 0;
	sf::Vector2i position = { 0,0 }, tiletype;
	std::regex r("(\\d*):(\\d*),(\\w{1})");
	std::regex pp("(\\d*):(\\d*)");
	std::regex size("(\\d*)x(\\d*)x(\\d*)");
	std::smatch m;

	getline(file, tmp);
	std::regex_search(tmp, m, size);
	layers = stoi(m[1]);
	mapSize = { stoi(m[2]), stoi(m[3]) };

	getline(file, tmp);
	std::regex_search(tmp, m, pp);
	playerposition = { stof(m[1]),stof(m[2]) };

	map.Create(layers, mapSize.x, mapSize.y);


	std::map<std::pair<int, int>, std::shared_ptr<Tile>> existingTiles;
	while (getline(file, tmp)) {
		if (!std::regex_search(tmp, m, r)) {
			position = { 0,0 };
			layer++;
			continue;
		}
		position.x = 0;
		while (std::regex_search(tmp, m, r)) {
			tiletype.x = stoi(m[1]);
			tiletype.y = stoi(m[2]);

			if (!existingTiles[{tiletype.x, tiletype.y}]) {
				std::cout << "creating tile: " << tiletype.x << ":" << tiletype.y << std::endl;
				std::shared_ptr<Tile> ptr(new Tile(tiletype, shared));
				existingTiles[{tiletype.x, tiletype.y}] = ptr;
			}

			map(layer, position.x, position.y).tile = existingTiles[{tiletype.x, tiletype.y}];
			if (m[3] == 'y')
				map(layer, position.x, position.y).solid = true;
			else map(layer, position.x, position.y).solid = false;
			tmp = m.suffix().str();
			position.x++;
		}
		position.y++;
	}

}
bool Map::Draw(int _layer) {
	if (_layer + 1 > layers)return false;
	sf::Vector2f windowsize =
	{ (float)shared->renderWindow->getSize().x, (float)shared->renderWindow->getSize().y };
	sf::Vector2i margin = 
	{ (int)ceil((windowsize.x / 2 - (TileSize / 2)) / TileSize),(int)ceil((windowsize.y / 2 - (TileSize / 2)) / TileSize) };

	sf::Vector2i firstDrawn = { 0,0 }, numberDrawn = { 2 * margin.x + 1, 2 * margin.y + 1 };
	sf::Vector2f offset = { windowsize.x/2 - margin.x*TileSize, windowsize.y/2 - margin.y*TileSize };
	
	if ((float)mapSize.x < windowsize.x / TileSize) {
		numberDrawn.x = mapSize.x;
		offset.x = TileSize / 2 + windowsize.x / 2 - mapSize.x*TileSize/2;
	}
	else if (playerposition.x < margin.x) {
		numberDrawn.x = windowsize.x/TileSize +1;
		offset.x = TileSize / 2;
	}
	else if (mapSize.x - playerposition.x - 1 < margin.x) {
		numberDrawn.x = windowsize.x / TileSize + 1;
		firstDrawn.x = mapSize.x - numberDrawn.x;
		offset.x = windowsize.x - numberDrawn.x*TileSize+TileSize/2;
	}
	else {
		firstDrawn.x = playerposition.x - margin.x;
	}

	if ((float)mapSize.y < windowsize.y / TileSize) {
		numberDrawn.y = mapSize.y;
		offset.y = TileSize / 2 + windowsize.y / 2 - mapSize.y*TileSize/2;
	}
	else if (playerposition.y < margin.y) {
		numberDrawn.y = windowsize.y / TileSize + 1;
		offset.y = TileSize / 2;
	}
	else if (mapSize.y - playerposition.y - 1 < margin.y) {
		numberDrawn.y = windowsize.y / TileSize + 1;
		firstDrawn.y = mapSize.y - numberDrawn.y;
		offset.y = windowsize.y - numberDrawn.y*TileSize+TileSize/2;
	}
	else {
		firstDrawn.y = playerposition.y - margin.y;
	}


	for (int x = 0; x < numberDrawn.x; x++) {
		for (int y = 0; y < numberDrawn.y; y++) {
			if (firstDrawn.x + x == playerposition.x&&firstDrawn.y + y == playerposition.y) {
				player.SetPosition( (float)offset.x + x * 72, (float)offset.y + y * 72 );
			}
			if (map(_layer, firstDrawn.x + x,firstDrawn.y + y).tile) {
				
				map(_layer, firstDrawn.x + x, firstDrawn.y + y).tile->Position
				({ (float)offset.x + x * 72, (float)offset.y + y * 72 });
				map(_layer, firstDrawn.x + x, firstDrawn.y + y).tile->Draw();
			}
		}
	}
	if (_layer == 0)
		//shared->renderWindow->draw(player);
		shared->renderWindow->draw(player.getSprite());

	return true;

}



bool Map::MakeMove(int _x, int _y) {
	bool s = false;
	for (unsigned int i = 0; i < layers; i++) {
		if (map(i, playerposition.x + _x, playerposition.y + _y).solid) s = true;
	}
	if (!s) {
		playerposition = { playerposition.x + _x, playerposition.y + _y };
		return true;
	}
	return false;
}


