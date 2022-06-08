#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "world.h"
#include "page.h"
#include "gameState.h"
#include "player.h"
#include "userio.h"
#include "config.h"
#include <box2d/box2d.h>

TEST_CASE("Item contact and picking up items.", "[ITEMS]")
{
    b2World world{b2Vec2(0.0f, 0.0f)};
    b2Vec2 position{0.0f, 0.0f};
    float angle = 0.0f;
    GameState game;
    Player player(world, position, angle);
    std::shared_ptr<Page> page = std::make_shared<Page>(world, position, angle, game);
    std::shared_ptr<Emf> emf = std::make_shared<Emf>(world, position, angle, game);
    UserIO io("test");

    SECTION("Default values.")
    {
        auto owned_items = player.getOwnedItems();
        REQUIRE_FALSE(owned_items.size());
        REQUIRE_FALSE(player.getNearbyItem());
        REQUIRE_FALSE(Page::pickedPages());
    }
    SECTION("Page pick-up.")
    {
        player.itemContact(page);

        REQUIRE(player.getNearbyItem() == page);

        io.setKeyPress(sf::Keyboard::Key::E);
        player.control(io, Conf::timeStep);
        player.itemLost();

        auto owned_items = player.getOwnedItems();
        REQUIRE(std::find(owned_items.begin(), owned_items.end(), page) != owned_items.end());
        REQUIRE(Page::pickedPages() == 1);
        REQUIRE_FALSE(player.getNearbyItem());
    }
    SECTION("EMF pick-up.")
    {
        player.itemContact(emf);

        REQUIRE(player.getNearbyItem() == emf);

        io.setKeyPress(sf::Keyboard::Key::E);
        player.control(io, Conf::timeStep);
        player.itemLost();

        auto owned_items = player.getOwnedItems();
        REQUIRE(std::find(owned_items.begin(), owned_items.end(), emf) != owned_items.end());
        REQUIRE_FALSE(player.getNearbyItem());
    }
}