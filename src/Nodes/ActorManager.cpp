#include "Nodes/ActorManager.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include <memory>
#include <algorithm>

void ActorManager::initialize() {
    // Safe to use shared_from_this() here 
    player_party_ = std::dynamic_pointer_cast<Party>(add_child<Party>().lock());

    enemy_party_ = std::dynamic_pointer_cast<Party>(add_child<Party>().lock());
}

void ActorManager::update(float delta)
{

}

std::shared_ptr<Party> ActorManager::add_party(std::vector<Actor>& actors)
{
    auto party = add_child<Party>().lock();
    for(auto& actor : actors)
    {
        if(!party->add_actor(actor))
        {
            break;
        }
    }

    return party;
}