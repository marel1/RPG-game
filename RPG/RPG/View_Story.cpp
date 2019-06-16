#include "View_Story.h"
#include "ViewManager.h"

View_Story::View_Story(ViewManager* _manager) : View(_manager) {
	left.setTexture(*manager->GetShared()->textureManager.GetResource("dep\\im\\f1.png"));
	right.setTexture(*manager->GetShared()->textureManager.GetResource("dep\\im\\fp.png"));
	left.setOrigin({ (float)left.getTextureRect().width / 2, (float)left.getTextureRect().height / 2 });
	right.setOrigin({ (float)right.getTextureRect().width / 2, (float)right.getTextureRect().height / 2 });
	activeSpeaker = 1;
	textL.SetText("Left Player!");
	textR.SetText("Just an NPC...");
}
View_Story::~View_Story() {
	manager->GetShared()->textureManager.FreeResource("dep\\im\\f1.png");
	manager->GetShared()->textureManager.FreeResource("dep\\im\\fp.png");
}

void View_Story::Activate() {
	Position();
	manager->GetShared()->eventManager->AddCallback("select", &View_Story::Interact, this);
	manager->GetShared()->eventManager->AddCallback("interact", &View_Story::Interact, this);
	manager->GetShared()->eventManager->AddCallback("escape", &View_Story::Escape, this);
}
void View_Story::Deactivate() {
	manager->GetShared()->eventManager->RemoveCallback("select");
	manager->GetShared()->eventManager->RemoveCallback("interact");
	manager->GetShared()->eventManager->RemoveCallback("escape");
}

void View_Story::Update() {}

void View_Story::Draw() {
	if (activeSpeaker % 2) {
		manager->GetShared()->renderWindow->draw(right);
		textR.Draw(manager->GetShared()->renderWindow);
	}
	else {
		manager->GetShared()->renderWindow->draw(left);
		textL.Draw(manager->GetShared()->renderWindow);
	}
}

void View_Story::Position() {
	sf::Vector2u windowSize = manager->GetShared()->renderWindow->getSize();
	left.setPosition({ windowSize.x / 4.0f, windowSize.y / 2.0f });
	right.setPosition({  windowSize.x / 4.0f * 3, windowSize.y / 2.0f });
	textL.SetPosition({ windowSize.x / 4.0f * 3, windowSize.y / 2.0f });
	textR.SetPosition({ windowSize.x / 4.0f, windowSize.y / 2.0f });
}

void View_Story::Interact(sf::Event::KeyEvent) {
	activeSpeaker++;
}
void View_Story::Escape(sf::Event::KeyEvent) {
	manager->GetShared()->window->Close();
}