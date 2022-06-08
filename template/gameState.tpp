#ifndef GAME_STATE_TPP
#define GAME_STATE_TPP

#ifndef GAME_STATE_H
#error __FILE__ should only be included from gameState.h.
#endif // GAME_STATE_H

template <typename T>
std::shared_ptr<T> GameState::shareObject(const T *searchedObj) const
{
    for (auto obj : objects_)
    {
        if (obj.get() == searchedObj)
        {
            return std::static_pointer_cast<T>(obj);
        }
    }
    return nullptr;
}

#endif