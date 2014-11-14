#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "GameObject.hpp"
#include "BodyComponent.hpp"
#include "Pool.hpp"

struct ChunkData;
class Module;
class ModuleData;
enum class Directive;
enum class Request;



enum class ChunkType
{
    Chunk,
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

    Pool<Ballistic> m_ballisticPool;
    Pool<Missiles> m_missilePool;
    Pool<Energy> m_energyPool;
    Pool<float> m_zoomPool;

    int m_slavePosition;
    BodyComponent m_body;
    std::vector<std::tr1::shared_ptr<Module> > m_modules;
};


struct ChunkData : public GameObjectData
{
    ChunkData() :
        GameObjectData(),
        type(ChunkType::Chunk),
        bodyComp()
    {

    }

    PoolData<Missiles> missileData;
    PoolData<Ballistic> ballisticData;
    PoolData<Energy> energyData;
    PoolData<float> zoomData;



    ChunkType type;
    BodyComponentData bodyComp;
    std::vector<std::tr1::shared_ptr<const ModuleData> > moduleData;

    virtual Chunk* generate() const
    {
        ChunkData copy(*this);
        return new Chunk(copy);
    }

};


#endif // CHUNK_HPP
