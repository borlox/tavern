#include "pch.h"
#include "Tilemap.h"

#include "sf_camera.h"

using namespace sftile;
using namespace priv;

Tilemap::Tilemap()
: camera(nullptr)
, tilesets()
, tile_layers()
, collisionLayer(nullptr)
, object_layers()
, map_dimensions(-1, -1)
, tile_dimensions(32, 32)
, version(1.0)
{}

Tilemap::Tilemap(const Tilemap& copy)
: camera(copy.camera)
, tilesets(copy.tilesets)
, tile_layers(copy.tile_layers)
, collisionLayer(nullptr)
, object_layers(copy.object_layers)
, map_dimensions(copy.map_dimensions)
, tile_dimensions(copy.tile_dimensions)
, version(copy.version)
{
	UpdateCollisionLayer();
}

Tilemap::Tilemap(Tilemap&& moved)
: camera(moved.camera)
, tilesets(std::move(moved.tilesets))
, tile_layers(std::move(moved.tile_layers))
, collisionLayer(nullptr)
, object_layers(std::move(moved.object_layers))
, map_dimensions(moved.map_dimensions)
, tile_dimensions(moved.tile_dimensions)
, version(moved.version)
{
	moved.camera = nullptr;
	UpdateCollisionLayer();
}

Tilemap& Tilemap::operator=(Tilemap copy)
{
	using std::swap;

	swap(camera, copy.camera);
	swap(tilesets, copy.tilesets);
	swap(tile_layers, copy.tile_layers);
	swap(object_layers, copy.object_layers);
	swap(map_dimensions, copy.map_dimensions);
	swap(tile_dimensions, copy.tile_dimensions);

	UpdateCollisionLayer();

	return *this;
}

Tilemap::~Tilemap()
{
	camera = nullptr;
}

SfTileset& Tilemap::TilesetForGID(size_t gid)
{
	for (size_t i=0; i < tilesets.size(); ++i) {
		if (tilesets[i].HasTileForGID(gid))
			return tilesets[i];
	}
	throw std::out_of_range("Failed to find tileset for global id");
}

void Tilemap::RegisterCamera(SfCamera* cam)
{
	camera = cam;
}

void Tilemap::HandleEvents(sf::Event evt)
{
	camera->HandleEvents(evt);
}

void Tilemap::Update()
{
	camera->Update();
}

void Tilemap::Render(sf::RenderTarget& window, bool cliff)
{
	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);

	for (size_t l =0; l < tile_layers.size(); ++l) {
		if (cliff == tile_layers[l].IsCliffLayer())
			RenderLayer(window, l);
	}
}

void Tilemap::RenderLayer(sf::RenderTarget& target, size_t index)
{
	auto& layer = tile_layers[index];
	if (!layer.IsVisible())
		return;

	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);

	auto renderOffset = camera->GetRenderOffset();

	for (int y = 0, tile_y = bounds.top; y < bounds.height; ++y, ++tile_y) {
		for (int x = 0, tile_x = bounds.left; x < bounds.width; ++x, ++tile_x) {
			if (tile_x < 0 || tile_y < 0)
				continue;
			if (tile_x >= map_dimensions.x || tile_y >= map_dimensions.y)
				continue;

			int gid = layer.GetTileGID(tile_x, tile_y);
			if (gid == 0)
				continue;

			float pos_x = static_cast<float>(x * tile_dimensions.x - offset.x);
			float pos_y = static_cast<float>(y * tile_dimensions.y - offset.y);
			TilesetForGID(gid).RenderTile(target, gid, pos_x + renderOffset.x, pos_y + renderOffset.y);
		}
	}
}

sf::Vector2f Tilemap::TileToScreen(sf::Vector2f tilePos)
{
	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);
	auto renderOffset = camera->GetRenderOffset();

	float x = tilePos.x - bounds.left;
	float y = tilePos.y - bounds.top;

	float px = x * tile_dimensions.x - offset.x;
	float py = y * tile_dimensions.y - offset.y;

	return sf::Vector2f(px + renderOffset.x, py + renderOffset.y);
}

sf::Vector2f Tilemap::ScreenToTile(sf::Vector2f screenPos)
{
	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);
	auto renderOffset = camera->GetRenderOffset();

	float x = (screenPos.x - renderOffset.x + offset.x) / tile_dimensions.x;
	float y = (screenPos.y - renderOffset.y + offset.y) / tile_dimensions.y;

	float px = x + bounds.left;
	float py = y + bounds.top;

	return sf::Vector2f(px, py);
}

void Tilemap::UpdateCollisionLayer()
{
	collisionLayer = nullptr;
	for (auto& layer: tile_layers)
		if (layer.GetName() == "collision")
			collisionLayer = &layer;
}

std::vector<std::pair<sf::Vector2i, float>> Tilemap::FindNeighbors(sf::Vector2i tile)
{
	std::vector<std::pair<sf::Vector2i, float>> res;

	bool up    = false;
	bool left  = false;
	bool down  = false;
	bool right = false;

	if (IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x-1), static_cast<float>(tile.y)))) {
		res.emplace_back(sf::Vector2i(tile.x-1, tile.y), 1.0f);
		left = true;
	}
	if (IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x+1), static_cast<float>(tile.y)))) {
		res.emplace_back(sf::Vector2i(tile.x+1, tile.y), 1.0f);
		right = true;
	}
	if (IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x), static_cast<float>(tile.y-1)))) {
		res.emplace_back(sf::Vector2i(tile.x, tile.y-1), 1.0f);
		up = true;
	}
	if (IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x), static_cast<float>(tile.y+1)))) {
		res.emplace_back(sf::Vector2i(tile.x, tile.y+1), 1.0f);
		down = true;
	}

	if (up && left && IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x-1), static_cast<float>(tile.y-1))))
		res.emplace_back(sf::Vector2i(tile.x-1, tile.y-1), 1.4f);
	if (down && right && IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x+1), static_cast<float>(tile.y+1))))
		res.emplace_back(sf::Vector2i(tile.x+1, tile.y+1), 1.4f);
	if (up && right && IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x+1), static_cast<float>(tile.y-1))))
		res.emplace_back(sf::Vector2i(tile.x+1, tile.y-1), 1.4f);
	if (down && left && IsTileAccessible(sf::Vector2f(static_cast<float>(tile.x-1), static_cast<float>(tile.y+1))))
		res.emplace_back(sf::Vector2i(tile.x-1, tile.y+1), 1.4f);

	return res;
}

sf::Vector2f Tilemap::FindNearestAccessible(sf::Vector2f start, sf::Vector2f end)
{
	sf::Vector2f dir = end - start;
	dir /= std::sqrt(dir.x*dir.x + dir.y*dir.y) * 2;

	while (ContainsTilePos(end) && !IsTileAccessible(end))
		end -= dir;

	if (!ContainsTilePos(end))
		return start;

	return end;
}

struct Node
{
	int x, y;
	float g, f;

	Node* parent;

	explicit Node(sf::Vector2i pos)
	: x(pos.x), y(pos.y), g(.0f), f(.0f), parent(nullptr)
	{ }

	bool operator==(const Node& rhs) const
	{
		return x==rhs.x && y==rhs.y;
	}

	bool operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	operator sf::Vector2i() const
	{
		return sf::Vector2i(x, y);
	}

	float H(const Node* goal) const
	{
		return 1.0f * (std::abs(x - goal->x) + std::abs(y - goal->y));
	}
		
};

static bool NodePtrCmp(const Node* lhs, const Node *rhs)
{
	return lhs->f > rhs->f;
}

struct NodeAtPos
{
	const Node& ref;

	explicit NodeAtPos(const Node& r)
	: ref(r)
	{ }

	bool operator()(const Node* nptr) const
	{
		return (*nptr) == ref;
	}
};

Path Tilemap::FindPath(sf::Vector2f start, sf::Vector2f end)
{
	assert(IsTileAccessible(start));

	sf::Vector2f target = FindNearestAccessible(start, end);

	sf::Vector2i absStart(start);
	sf::Vector2i absTarget(target);

	std::vector<Node*> allNodes;
	std::vector<Node*> open, closed;

	auto targetNode = new Node(absTarget);
	allNodes.push_back(targetNode);

	auto startNode = new Node(absStart);
	allNodes.push_back(startNode);

	open.push_back(startNode);
	boost::push_heap(open, NodePtrCmp);

	while (!open.empty() && *open.front() != *targetNode) {
		Node* cur = open.front();

		boost::pop_heap(open, NodePtrCmp);
		open.pop_back();

		for (auto& nbCost: FindNeighbors(*cur)) {
			auto& nb = nbCost.first;
			float cost = nbCost.second;

			auto suc = new Node(nb);
			allNodes.push_back(suc);

			float newg = cur->g + cost;

			auto openIt = boost::find_if(open, NodeAtPos(*suc));
			auto closedIt = boost::find_if(closed, NodeAtPos(*suc));

			if (openIt != open.end()) {
				if ((*openIt)->g <= newg)
					continue;
			}

			if (closedIt != closed.end()) {
				if ((*closedIt)->g <= newg)
					continue;
			}

			suc->parent = cur;
			suc->g = newg;
			suc->f = suc->g + suc->H(targetNode);

			if (openIt != open.end()) {
				open.erase(openIt);
				boost::make_heap(open, NodePtrCmp);
			}
			if (closedIt != closed.end()) {
				closed.erase(closedIt);
			}

			open.push_back(suc);
			boost::push_heap(open, NodePtrCmp);
		}

		closed.push_back(cur);
	}

	Path path;
	if (!open.empty()) {
		path.push(target);

		auto n = open.front();
		bool first = true;
		while (n) {
			if (!first && n->parent) // do not use first or last step of the computed path, they just point to the upper left corner of the start/end tile
				path.push(sf::Vector2f(static_cast<float>(n->x), static_cast<float>(n->y)));
			n = n->parent;
			first = false;
		}
	}

	for (auto& n: allNodes)
		delete n;

	return path;
}
