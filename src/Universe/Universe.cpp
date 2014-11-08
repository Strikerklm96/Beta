#include "Universe.hpp"

#include "Globals.hpp"
#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "IOManager.hpp"
#include "Player.hpp"

using namespace std;

Universe::Universe(const IOComponentData& rData) : m_io(rData)
{
    m_spBatchLayers = std::tr1::shared_ptr<BatchLayers>(new BatchLayers);
    m_spGfxUpdater = std::tr1::shared_ptr<GraphicsComponentUpdater>(new GraphicsComponentUpdater);
    m_spUniverseIO = std::tr1::shared_ptr<IOManager>(new IOManager(true));

    m_io.bindCallback(&Universe::input, this);
    m_spUniverseIO->give(&m_io);

    m_paused = false;
    m_skippedTime = 0;
    m_paused = false;

    m_velocityIterations = 1;///how should these be set?
    m_positionIterations = 1;///how should these be set?
    m_timeStep = 1.0f/120.0f;///LOAD FROM FILE
    m_maxIterations = 6;

    m_spUniverseIO->give(&game.getLocalPlayer().getIOComp());
}
Universe::~Universe()
{

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



void Universe::update(sf::RenderTarget& rTarget)
{
    float dT = getTime()-m_lastTime;
    m_lastTime = getTime();

    m_spGfxUpdater->update();
    m_spBatchLayers->draw(rTarget);
    m_spUniverseIO->update(dT);

    ///update b
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



void Universe::input(const std::string& rCommand, const sf::Packet& rData)
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
