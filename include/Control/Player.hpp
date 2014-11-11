#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "stdafx.hpp"
#include "Intelligence.hpp"
#include "Camera.hpp"




struct InputConfig
{
    InputConfig() :
        up(sf::Keyboard::W),
        down(sf::Keyboard::S),
        rollCCW(sf::Keyboard::A),
        rollCW(sf::Keyboard::D),

        primary(sf::Mouse::Left),
        secondary(sf::Mouse::Right),

        nextWeapon(sf::Keyboard::E),
        prevWeapon(sf::Keyboard::Q),


        special_1(sf::Keyboard::Space),
        special_2(sf::Keyboard::R),
        special_3(sf::Keyboard::F),
        special_4(sf::Keyboard::C),
        special_5(sf::Keyboard::X),
        special_6(sf::Keyboard::Z),
        special_7(sf::Keyboard::LShift),


        cameraUp(sf::Keyboard::Up),
        cameraDown(sf::Keyboard::Down),
        cameraLeft(sf::Keyboard::Left),
        cameraRight(sf::Keyboard::Right)
    {}

    sf::Keyboard::Key up;
    sf::Keyboard::Key down;
    sf::Keyboard::Key rollCCW;
    sf::Keyboard::Key rollCW;

    sf::Mouse::Button primary;
    sf::Mouse::Button secondary;

    sf::Keyboard::Key nextWeapon;
    sf::Keyboard::Key prevWeapon;

    sf::Keyboard::Key special_1;
    sf::Keyboard::Key special_2;
    sf::Keyboard::Key special_3;
    sf::Keyboard::Key special_4;
    sf::Keyboard::Key special_5;
    sf::Keyboard::Key special_6;
    sf::Keyboard::Key special_7;


    sf::Keyboard::Key cameraUp;
    sf::Keyboard::Key cameraDown;
    sf::Keyboard::Key cameraLeft;
    sf::Keyboard::Key cameraRight;
};
struct PlayerData : public IntelligenceData
{
    PlayerData() :
        IntelligenceData(),
        keyConfig(),
        ioComp(game.getCoreIO()),
        tracking(true)
    {
        ioComp.name = "local_player";
    }

    InputConfig keyConfig;
    IOComponentData ioComp;
    bool tracking;
};


/**This class is exclusively for 1 local player**/
class Player : public Intelligence
{
public:
    Player(const PlayerData& rData);
    virtual ~Player();

    Camera& getCamera();
    IOComponent& getIOComp();
    const InputConfig& getInCfg() const;
    bool inGuiMode() const;//is the player in GUI mode?
    bool toggleGuiMode(bool isGuiModeOn);

    void getLiveInput();//get direct feed from keyboard and mouse, just gets their states though (up, down, position)
    void getWindowEvents(sf::RenderWindow& rWindow);//process window events

protected:
    void input(std::string rCommand, sf::Packet rData);

private:
    IOComponent m_io;
    float m_desiredZoom;//for smooth zooming
    b2Vec2 m_desiredCameraPos;//for smooth zooming

    Camera m_camera;//players camera
    InputConfig m_inCfg;
    bool m_inGuiMode;//true if we are in GUI mode
    bool m_tracking;
};

#endif // PLAYER_HPP
