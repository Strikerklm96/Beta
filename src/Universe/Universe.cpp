#include "Universe.hpp"

#include "BlueprintLoader.hpp"
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
#include "Sensor.hpp"
#include "Thruster.hpp"
#include "Reactor.hpp"
#include "Capacitor.hpp"
#include "Turret.hpp"
#include "LaserWeapon.hpp"
#include "LinearMeter.hpp"

using namespace std;

Universe::Universe(const IOComponentData& rData) : m_io(rData, &Universe::input, this), m_physWorld(b2Vec2(0,0))
{
    m_spBPLoader = std::tr1::shared_ptr<BlueprintLoader>(new BlueprintLoader);//MUST BE AFTER IO
    m_spSlaveLocator = std::tr1::shared_ptr<SlaveLocator>(new SlaveLocator);
    m_spBatchLayers = std::tr1::shared_ptr<BatchLayers>(new BatchLayers);
    m_spGfxUpdater = std::tr1::shared_ptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);
    m_spControlFactory = std::tr1::shared_ptr<ControlFactory>(new ControlFactory);

    /**IO**/
    m_spUniverseIO = std::tr1::shared_ptr<IOManager>(new IOManager(true));
    m_spUniverseIO->give(&m_io);
    m_spUniverseIO->give(&game.getLocalPlayer().getIOComp());
    /**IO**/




    /**PHYControlCS**/
    m_paused = false;
    m_skippedTime = 0;
    m_pauseTime = game.getTime();

    m_velocityIterations = 1;
    m_positionIterations = 1;
    m_timeStep = 1.0f/120.0f;///LOAD FROM FILE

    m_physWorld.SetContactListener(&m_contactListener);
    m_physWorld.SetDebugDraw(&m_debugDraw);
    m_debugDraw.SetFlags(b2Draw::e_shapeBit);
    /**PHYControlCS**/

    m_debugDrawEnabled = false;
}
Universe::~Universe()
{
    cout << "\nUniverse Destroying...";
    game.getLocalPlayer().universeDestroyed();
    cout << "\nEnd.";
}
void Universe::toggleDebugDraw()
{
    m_debugDrawEnabled = !m_debugDrawEnabled;
}
ControlFactory& Universe::getControllerFactory()
{
    return *m_spControlFactory;
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
void Universe::prePhysUpdate()
{
    if(not m_paused)
        for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
            (*it)->prePhysUpdate();
}
void Universe::physUpdate()
{
    if(not m_paused)
    {
        m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
        ///m_projAlloc.recoverProjectiles();
    }
}
void Universe::postPhysUpdate()
{
    if(not m_paused)
        for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
            (*it)->postPhysUpdate();
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

void Universe::loadBlueprints(const std::string& bpDir)//loads blueprints
{
    m_spBPLoader->storeRoster(bpDir);
}
void Universe::loadLevel(const std::string& levelDir, int localController, const std::string& bluePrints, const std::vector<std::string>& rControllerList)//loads a level using blueprints
{
    loadBlueprints(bluePrints);

    string configFile = "level.lcfg";
    string modDir = "mods/";

    ifstream level(levelDir+configFile, std::ifstream::binary);
    Json::Reader reader;
    Json::Value root;
    bool parsedSuccess = reader.parse(level, root, false);


    if(not parsedSuccess)
    {
        cout << "\nProblem Parsing [" << levelDir+configFile << "].";
        ///error log
        return;
    }
    else
    {
        /**ADDITIONAL BLUEPRINTS**/
        if(not root["AdditionalBlueprints"].isNull())
        {
            const Json::Value bpList = root["AdditionalBlueprints"];
            for(auto it = bpList.begin(); it != bpList.end(); ++it)
            {
                m_spBPLoader->storeRoster(modDir+it->asString());
            }
        }
        else
        {
            cout << FILELINE;
            ///ERROR LOG
        }

        /**CHUNKS**/
        std::tr1::shared_ptr<ChunkData> spCnk;
        if(not root["Chunks"].isNull())
        {
            const Json::Value chunks = root["Chunks"];
            for(auto it = chunks.begin(); it != chunks.end(); ++it)
            {
                if(not (*it)["Title"].isNull())
                {
                    spCnk.reset(m_spBPLoader->getChunkSPtr((*it)["Title"].asString())->clone());
                    spCnk->bodyComp.coords.x = (*it)["Coordinates"][0].asFloat();
                    spCnk->bodyComp.coords.y = (*it)["Coordinates"][1].asFloat();
                }
                else if(not (*it)["ClassName"].isNull())
                {
                    spCnk.reset(m_spBPLoader->loadChunk(*it)->clone());
                }
                else
                {
                    cout << "\n" << FILELINE;
                    ///ERROR LOG
                }
                add(spCnk->generate());
            }
        }
    }


    /**HARD CODED**/

    ChunkData chunkdata_1;
    chunkdata_1.bodyComp.coords = b2Vec2(-2,5);
    chunkdata_1.ioComp.name = "chunk_99";

    ThrusterData thrust;
    thrust.force = 40;
    thrust.torque = 20;
    thrust.energyConsumption = 0.1;
    thrust.fixComp.offset = b2Vec2(0,0);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new ThrusterData(thrust)));

    SensorData sens;
    sens.fixComp.offset = b2Vec2(3,3);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new SensorData(sens)));

    ReactorData data;
    data.fixComp.offset = b2Vec2(1,0);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new ReactorData(data)));

    CapacitorData data3;
    data3.fixComp.offset = b2Vec2(-2,0);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new CapacitorData(data3)));

    TurretData data4;
    data4.fixComp.offset = b2Vec2(-2,1);
    LaserWeaponData* pWep = new LaserWeaponData;
    pWep->beamColor = sf::Color::Red;
    pWep->beamWidth = 32;
    data4.startWep.reset(pWep);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new TurretData(data4)));


    TurretData* pData5 = (TurretData*)m_spBPLoader->getModuleSPtr("DefaultTurret")->clone();
    pData5->fixComp.offset = b2Vec2(0, 2);
    chunkdata_1.moduleData.push_back(std::tr1::shared_ptr<ModuleData>(new TurretData(*pData5)));

    add(chunkdata_1.generate());
    /**HARD CODED**/




    game.getLocalPlayer().loadOverlay("overlayconfig");



    /**CONTROL**/
    m_spControlFactory->resetControllers(rControllerList);
    game.getLocalPlayer().setController(localController);
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
        cout << m_io.getName() << ":[" << rCommand << "] not found." << FILELINE;
    }
}
