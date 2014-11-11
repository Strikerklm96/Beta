#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "stdafx.hpp"

class Module;
class ModuleData;
class Chunk;
class ChunkData;

class Factory
{
public:
    Factory();
    virtual ~Factory();

    /**BLUEPRINTS ARE LOADED**/
    /**LEVEL IS LOADED**/
    /**WHEN AN OBJECT IS LOADED, IT SPECIFIES A BLUEPRINT**/
    /**AND THEN HAS A SECTION WHERE IT CAN MAKE MODIFICATIONS**/
    /**THE GAME LOADS THE BLUEPRINT AND THEN OVERWRITES WHAT THE OVERWRITE SECTION SPECIFIES**/

    Chunk* createChunk(std::tr1::shared_ptr<const ChunkData> data);//things that aren't default override the BP!

protected:
private:
};

#endif // FACTORY_HPP
