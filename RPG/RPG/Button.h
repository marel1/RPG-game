#pragma once
#include "Text.h"

/*przycisk, reaguje na klikniecie, moze byc podswietlony*/
class Button {
public:
	Button();
	~Button();

	void Draw(sf::RenderWindow*);

	void SetLabel(const std::string&);
	void SetFont(const std::string&, unsigned int);
	void SetColor(sf::Color);
	void SetSize(sf::Vector2f);
	void SetPosition(sf::Vector2f);

	void Select();
	void Deselect();
	bool IsSelected();

	sf::Vector2f GetSize();
	std::string GetLabel();
private:
	bool selected;

	sf::Font font;
	sf::RectangleShape rect;
	Text label;
};