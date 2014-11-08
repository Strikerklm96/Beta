#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"

class BatchLayers;
class GraphicsComponentUpdater;
class IOManager;

class Universe
{
public:
    Universe(const IOComponentData& rData);
    virtual ~Universe();


    BatchLayers& getBatchLayers();
    GraphicsComponentUpdater& getGfxUpdater();
    IOManager& getUniverseIO();

    void update(sf::RenderTarget& rTarget);

    float getTime() const;
    void togglePause(bool pause);


protected:
    void input(const std::string& rCommand, const sf::Packet& rData);
private:
    std::tr1::shared_ptr<BatchLayers> m_spBatchLayers;
    std::tr1::shared_ptr<GraphicsComponentUpdater> m_spGfxUpdater;
    std::tr1::shared_ptr<IOManager> m_spUniverseIO;//manages IO for the game objects

    IOComponent m_io;
    float m_lastTime;//used for update method//cant use timer because timer references us!



    /**TIME**/
    float m_pauseTime;
    float m_skippedTime;
    bool m_paused;

    /**PHYSICS**/
    float m_remainingTime;
    int m_iterations;
    int m_maxIterations;
    float m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;

};

#endif // UNIVERSE_HPP
