#include "Factory.hpp"

using namespace std;

#include "Module.hpp"
#include "Chunk.hpp"
#include "ShipModule.hpp"

Factory::Factory()
{

}
Factory::~Factory()
{

}
Chunk* Factory::createChunk(std::tr1::shared_ptr<const ChunkData> spData)//things that aren't default override the BP!
{
    if(spData->type == ChunkType::Chunk)
    {
        return new Chunk(*spData);
    }
    else if(false)
    {

    }
    else
    {

    }
}
