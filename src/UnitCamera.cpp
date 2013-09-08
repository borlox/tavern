#include "pch.h"
#include "UnitCamera.h"
#include "GameObject.h"

UnitCamera::UnitCamera(const GameObject* unit, int tileWidth, int tileHeight, float width, float height)
: unit(unit), tileWidth(tileWidth), tileHeight(tileHeight), sftile::SfCamera(width, height)
{ }

void UnitCamera::Update()
{
	auto unitTilePos = unit->GetPosition();
	SetCenterPosition(unitTilePos.x * tileWidth, unitTilePos.y * tileHeight);
}

void UnitCamera::HandleEvents(sf::Event& event)
{
}
