#include "Universe.hpp"

#include "Globals.hpp"
#include "SlaveLocator.hpp"
#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "QuadComponent.hpp"
#include "GameObject.hpp"
#include "Chunk.hpp"
#include "ShipModule.hpp"

using namespace std;

Universe::Universe(const IOComponentData& rData) : m_io(rData), m_physWorld(b2Vec2(0,0))
{
    m_spSlaveLocator = std::tr1::shared_ptr<SlaveLocator>(new SlaveLocator);
    m_spBatchLayers = std::tr1::shared_ptr<BatchLayers>(new BatchLayers);
    m_spGfxUpdater = std::tr1::shared_ptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);

    /**IO**/
    m_spUniverseIO = std::tr1::shared_ptr<IOManager>(new IOManager(true));
    m_io.bindCallback(&Universe::input, this);
    m_spUniverseIO->give(&m_io);
    m_spUniverseIO->give(&game.getLocalPlayer().getIOComp());
    /**IO**/


    /**PHYSICS**/
    m_paused = false;
    m_skippedTime = 0;
    m_pauseTime = game.getTime();

    m_velocityIterations = 1;
    m_positionIterations = 1;
    m_timeStep = 1.0f/120.0f;///LOAD FROM FILE

    m_physWorld.SetContactListener(&m_contactListener);
    m_physWorld.SetDebugDraw(&m_debugDraw);
    m_debugDraw.SetFlags(b2Draw::e_shapeBit);
    /**PHYSICS**/

    m_debugDrawEnabled = false;
}
Universe::~Universe()
{
    cout << "\nUniverse Destroying...";
}
void Universe::toggleDebugDraw()
{
    m_debugDrawEnabled = !m_debugDrawEnabled;
}

SlaveLocator& Universe::getSlaveLocator()
{
    return *m_spSlaveLocator;
}
b2World& Universe::getWorld()
{
    return m_physWorld;
}
BatchLayers& Universe::getBatchLayers()
{
    return *m_spBatchLayers;
}
GraphicsComponentUpdater& Universe::getGfxUpdater()
{
    return *m_spGfxUpdater;
}
IOManager& Universe::getUniverseIO()
{
    return *m_spUniverseIO;
}


float Universe::getTimeStep() const
{
    return m_timeStep;
}
void Universe::physUpdate()
{
    if(not m_paused)
    {
        for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
            (*it)->prePhysUpdate();

        m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);

        for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
            (*it)->postPhysUpdate();
        ///m_projAlloc.recoverProjectiles();
    }
}




bool Universe::debugDraw() const//should we draw debug or normal?
{
    return m_debugDrawEnabled;
}
void Universe::togglePause(bool pause)
{
    m_paused = pause;

    if(not m_paused)
        m_skippedTime += game.getTime()-m_pauseTime;
    else
        m_pauseTime = game.getTime();
}
void Universe::togglePause()
{
    togglePause(!m_paused);
}
float Universe::getTime() const
{
    if(m_paused)
        return m_pauseTime-m_skippedTime;
    else
        return game.getTime()-m_skippedTime;
}

void Universe::loadBP(const std::string& bluePrints)//loads blueprints
{

}
void Universe::loadLevel(const std::string& level)//loads a level using blueprints
{
    ChunkData chunkdata_1;
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new ShipModuleData));
    chunkdata_1.ioComp.name = "chunk_1";

    add(chunkdata_1.generate());

    chunkdata_1.bodyComp.coords = b2Vec2(2,0);
    chunkdata_1.ioComp.name = "chunk_2";
    add(chunkdata_1.generate());



    game.getLocalPlayer().setSlave("chunk_1");
}
void Universe::add(std::tr1::shared_ptr<GameObject> spGO)
{
    m_goList.push_back(spGO);
}
void Universe::add(GameObject* pGO)
{
    m_goList.push_back(std::tr1::shared_ptr<GameObject>(pGO));
}


void Universe::input(std::string rCommand, sf::Packet rData)
{
    sf::Packet data(rData);
    if(rCommand == "togglePause")
    {
        togglePause(!m_paused);
    }
    else if(rCommand == "setPause")
    {
        bool mode;
        data >> mode;
        togglePause(mode);
    }
    else
    {
        ///ERROR LOG
        cout << m_io.getName() << ": [" << rCommand << "] not found." << FILELINE;
    }
}
