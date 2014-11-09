#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "UniversalContactListener.hpp"
#include "DebugDraw.hpp"

class BatchLayers;
class GraphicsComponentUpdater;
class IOManager;
class GameObject;

class Universe
{
public:
    Universe(const IOComponentData& rData);
    virtual ~Universe();


    BatchLayers& getBatchLayers();
    GraphicsComponentUpdater& getGfxUpdater();
    IOManager& getUniverseIO();
    b2World& getWorld();

    float update(sf::RenderTarget& rTarget);

    float getTime() const;
    void togglePause(bool pause);

    void loadBP(const std::string& bluePrints);//loads blueprints
    void loadLevel(const std::string& level);//loads a level using blueprints

protected:
    void input(std::string rCommand, sf::Packet rData);
private:

    /**PHYSICS**/
    float m_remainingTime;
    int m_iterations;
    int m_maxIterations;
    float m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;

    UniversalContactListener m_contactListener;
    DebugDraw m_debugDraw;

    b2World m_physWorld;
    /**PHYSICS**/


    std::tr1::shared_ptr<BatchLayers> m_spBatchLayers;
    std::tr1::shared_ptr<GraphicsComponentUpdater> m_spGfxUpdater;
    std::tr1::shared_ptr<IOManager> m_spUniverseIO;//manages IO for the game objects

    std::vector<std::tr1::shared_ptr<GameObject> > m_goList;//list of game objects that WE need to keep track of
    /**keep in mind this is not all game objects, because most are modules which are owned by Chunks**/
    /**but some are**/

    IOComponent m_io;
    float m_lastTime;//used for update method//cant use timer because timer references us!
    bool m_debugDrawEnabled;


    /**TIME**/
    float m_pauseTime;
    float m_skippedTime;
    bool m_paused;
    /**TIME**/
};

#endif // UNIVERSE_HPP
