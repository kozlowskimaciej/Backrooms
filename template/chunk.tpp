#ifndef CHUNK_TPP
#define CHUNK_TPP

#ifndef CHUNK_H
#error __FILE__ should only be included from chunk.h.
#endif

template <typename T>
void Chunk::spawnItem(unsigned int chance, b2World &world, Mediator &mediator)
{
    bool willSpawn = chunkGenerator.drawLots(chance, 100u);
    if (willSpawn && std::find(Chunk::spawnedItems_.begin(), Chunk::spawnedItems_.end(), position_) == Chunk::spawnedItems_.end())
    {
        mediator.notify(std::make_shared<T>(world, position_, 0, mediator), Mediator::ITEM_CREATED);
        Chunk::spawnedItems_.push_back(position_);
    }
}

#endif