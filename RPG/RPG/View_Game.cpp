#include "View_Game.h"
#include "ViewManager.h"
#include <iostream>

View_Game::View_Game(ViewManager* _manager) : View(_manager), map(manager->GetShared()), entityMgr(manager->GetShared()), inventory(manager->GetShared()) {
	map.Load(0);
	entityMgr.FindEntity(entityMgr.AddEntity())->setPosition(map.getPlayerPosition());
	inventory.FindObj(inventory.AddObjects("dep\\Player\\test.png", { 72,72 }))->SetObjPosition({ 72 * 10, 72 * 5 });
	
}
View_Game::~View_Game() {}

void View_Game::Activate() {
	Position();
	manager->GetShared()->eventManager->AddCallback("select", &View_Game::Interact, this);
	manager->GetShared()->eventManager->AddCallback("interact", &View_Game::Interact, this);
	manager->GetShared()->eventManager->AddCallback("escape", &View_Game::Escape, this);
	manager->GetShared()->eventManager->AddCallback("up", &View_Game::Up, this);
	manager->GetShared()->eventManager->AddCallback("down", &View_Game::Down, this);
	manager->GetShared()->eventManager->AddCallback("right", &View_Game::Right, this);
	manager->GetShared()->eventManager->AddCallback("left", &View_Game::Left, this);
}

void View_Game::Deactivate() {
	manager->GetShared()->eventManager->RemoveCallback("select");
	manager->GetShared()->eventManager->RemoveCallback("interact");
	manager->GetShared()->eventManager->RemoveCallback("escape");
	manager->GetShared()->eventManager->RemoveCallback("up");
	manager->GetShared()->eventManager->RemoveCallback("down");
	manager->GetShared()->eventManager->RemoveCallback("right");
	manager->GetShared()->eventManager->RemoveCallback("left");
}

void View_Game::Update() {
	map.Update();
	entityMgr.Update();
	inventory.Update();
}

void View_Game::Draw() {
	map.Draw(0);
	map.Draw(1);
	entityMgr.Draw();
	inventory.Draw();
	map.Draw(2);

}

void View_Game::Position() {
	map.Move({ 0, 0 });
	entityMgr.FindEntity(0)->setPosition( map.getPlayerPosition());
}

void View_Game::Interact(sf::Event::KeyEvent) {
	manager->SwitchTo(MainMenu);
}

void View_Game::Escape(sf::Event::KeyEvent) {
	manager->SwitchTo(MainMenu);
}
void View_Game::Up(sf::Event::KeyEvent) {
	if (!entityMgr.FindEntity(0)->isMoving())
		if (map.MakeMove( 0, -1 ))
			entityMgr.FindEntity(0)->Move(map.getPlayerShift(), animation_type::Up);
}
void View_Game::Down(sf::Event::KeyEvent) {
	if (!entityMgr.FindEntity(0)->isMoving())
		if (map.MakeMove(0, 1))
			entityMgr.FindEntity(0)->Move(map.getPlayerShift(), animation_type::Down);
}
void View_Game::Right(sf::Event::KeyEvent) {
	if (!entityMgr.FindEntity(0)->isMoving())
		if (map.MakeMove(1, 0))
			entityMgr.FindEntity(0)->Move(map.getPlayerShift(), animation_type::Right);
}
void View_Game::Left(sf::Event::KeyEvent) {
	if (!entityMgr.FindEntity(0)->isMoving())
		if (map.MakeMove(-1, 0))
			entityMgr.FindEntity(0)->Move(map.getPlayerShift(), animation_type::Left);
}