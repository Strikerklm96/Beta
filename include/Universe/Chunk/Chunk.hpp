#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "GameObject.hpp"
#include "BodyComponent.hpp"

class Module;
class ModuleData;
enum class Directive;
enum class Request;

enum class ChunkType
{
    Chunk,
};

struct ChunkData : public GameObjectData
{
    ChunkData() :
        GameObjectData(),
        type(ChunkType::Chunk),
        bodyComp()
    {

    }

    ChunkType type;
    BodyComponentData bodyComp;
    std::vector<std::tr1::shared_ptr<const ModuleData> > moduleData;
};

class Chunk : public GameObject
{
public:
    Chunk(const ChunkData& rData);
    virtual ~Chunk();

    virtual void prePhysUpdate();
    virtual void postPhysUpdate();

    const std::string& getName() const;

    void setAim(const b2Vec2& world);//send our aim coordinates
    void directive(Directive issue);//send command to target
    float get(Request value) const;//return the requested value
    b2Body* getBodyPtr();


protected:
private:
    int m_slavePosition;
    BodyComponent m_body;
    std::vector<std::tr1::shared_ptr<Module> > m_modules;
};

#endif // CHUNK_HPP
