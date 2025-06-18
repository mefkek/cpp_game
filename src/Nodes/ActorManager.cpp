#include "Nodes/ActorManager.hpp"
#include "Nodes/Party.hpp"
#include "Nodes/Actor.hpp"
#include <memory>
#include <algorithm>

void ActorManager::initialize() {
    // Safe to use shared_from_this() here 
    playerParty_ = std::dynamic_pointer_cast<Party>(add_child<Party>().lock());

    enemyParty_ = std::dynamic_pointer_cast<Party>(add_child<Party>().lock());
}

void ActorManager::update(float delta)
{

}

std::shared_ptr<Party> ActorManager::addParty(std::vector<Actor>& actors)
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