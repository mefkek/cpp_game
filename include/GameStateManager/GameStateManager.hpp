#pragma once
#include "GameStates.hpp"

class GameStateManager : public Node
{
    private:
    std::weak_ptr<GameStateBase> current;
    std::shared_ptr<GameStateBase> scheduled;
    public:

    void initialize() override;

    template<typename State, typename... Args>
    void change_state(Args... args)
    {
        static_assert(std::is_base_of_v<GameStateBase, State>, "State can only be changed to another state");
        
        auto ptr = current.lock();
        for(auto ch : children)
        {
            if(ch == ptr)
            {
                ch->kill();
                break;
            }
        }

        current = add_child<State>(std::forward<Args>(args)...);
    }

    template<typename State, typename... Args>
    void schedule_change(Args... args)
    {
        static_assert(std::is_base_of_v<GameStateBase, State>, "State can only be changed to another state");

        scheduled = create<State>(std::forward<Args>(args)...);
    }

    void update(float delta) override;
};