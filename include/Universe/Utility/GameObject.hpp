#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "IOComponent.hpp"
#include "Universe.hpp"

struct GameObjectData
{
    GameObjectData() :
        ioComp(game.getUniverse().getUniverseIO()),
        nwComp()
    {

    }
    IOComponentData ioComp;
    NetworkComponentData nwComp;
};


class GameObject
{
public:
    GameObject(const GameObjectData& rData);
    virtual ~GameObject();

    virtual void update(float dT) = 0;

protected:
    IOComponent m_io;
    NetworkComponent m_nw;
private:
};

#endif // GAMEOBJECT_HPP
