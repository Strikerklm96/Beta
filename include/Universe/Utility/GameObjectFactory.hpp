#ifndef GAMEOBJECTFACTORY_HPP
#define GAMEOBJECTFACTORY_HPP

#include "stdafx.hpp"

class Module;
class ModuleData;
class Chunk;
class ChunkData;

class GameObjectFactory
{
public:
    GameObjectFactory();
    virtual ~GameObjectFactory();

    /**BLUEPRINTS ARE LOADED**/
    /**LEVEL IS LOADED**/
    /**WHEN AN OBJECT IS LOADED, IT SPECIFIES A BLUEPRINT**/
    /**AND THEN HAS A SECTION WHERE IT CAN MAKE MODIFICATIONS**/
    /**THE GAME LOADS THE BLUEPRINT AND THEN OVERWRITES WHAT THE OVERWRITE SECTION SPECIFIES**/
    Module* createModule(std::tr1::shared_ptr<const ModuleData> data);

    Chunk* createChunk(std::tr1::shared_ptr<const ChunkData> data);//things that aren't default override the BP!


protected:
private:
};

#endif // GAMEOBJECTFACTORY_HPP
