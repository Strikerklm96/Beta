#include "Universe.hpp"

#include "Globals.hpp"
#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "QuadComponent.hpp"
#include "GameObject.hpp"

using namespace std;

Universe::Universe(const IOComponentData& rData) : m_io(rData), m_physWorld(b2Vec2(0,0))
{
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
    m_paused = false;

    m_velocityIterations = 1;
    m_positionIterations = 1;
    m_timeStep = 1.0f/120.0f;///LOAD FROM FILE
    m_maxIterations = 6;

    m_physWorld.SetContactListener(&m_contactListener);
    m_physWorld.SetDebugDraw(&m_debugDraw);
    /**PHYSICS**/

    m_debugDrawEnabled = false;
}
Universe::~Universe()
{
    cout << "\nUniverse Destroying...";
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



float Universe::update(sf::RenderTarget& rTarget)
{
    float dT = getTime()-m_lastTime;
    m_lastTime = getTime();

    m_spUniverseIO->update(dT);
    m_spGfxUpdater->update();

    if(m_debugDrawEnabled)
        m_physWorld.DrawDebugData();
    else
        m_spBatchLayers->draw(rTarget);


    if(not m_paused)
    {
        for(auto it = m_goList.begin(); it != m_goList.end(); ++it)
            (*it)->update(dT);

        m_physWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);

        ///m_projAlloc.recoverProjectiles();
    }

    return m_timeStep;
}





void Universe::togglePause(bool pause)
{
    m_paused = pause;

    if(not m_paused)
        m_skippedTime += game.getTime()-m_pauseTime;
    else
        m_pauseTime = game.getTime();
}
float Universe::getTime() const
{
    if(m_paused)
        return m_pauseTime-m_skippedTime;
    else
        return game.getTime()-m_skippedTime;
}

void loadBP(const std::string& bluePrints)//loads blueprints
{

}
void loadLevel(const std::string& level)//loads a level using blueprints
{
    QuadComponentData data;
    data.dimensions.x = 128;
    QuadComponent comp(data);
    comp.setPosition(b2Vec2(1,-1));
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
