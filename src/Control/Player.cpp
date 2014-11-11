#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"

using namespace std;
using namespace sf;

Player::Player(const PlayerData& rData) : Intelligence(rData), m_io(rData.ioComp)
{
    m_inGuiMode = true;
    m_io.bindCallback(Player::input, this);
    m_tracking = rData.tracking;
}
Player::~Player()
{
    cout << "\nPlayer Destroying...";
}
Camera& Player::getCamera()
{
    return m_camera;
}
IOComponent& Player::getIOComp()
{
    return m_io;
}
const InputConfig& Player::getInCfg() const
{
    return m_inCfg;
}
bool Player::inGuiMode() const//is the player in GUI mode?
{
    return m_inGuiMode;
}
bool Player::toggleGuiMode(bool isGuiModeOn)
{
    return m_inGuiMode = isGuiModeOn;
}
void Player::getInput()
{

    b2Body* pBody = getBodyPtr();
    sf::RenderWindow& rWindow = game.getWindow();
    sf::Event event;
    while(rWindow.pollEvent(event))
    {
        /**Was the window closed?**/
        if(event.type == sf::Event::Closed)
            rWindow.close();

        /**Did a key-down event occur?**/
        if(event.type == Event::KeyPressed)
        {
            if(event.key.code == Keyboard::O)
                game.getUniverse().toggleDebugDraw();

            if(event.key.code == Keyboard::T)
            {
                m_tracking = !m_tracking;
            }

            /**CONTROL MENU VS GAME INTERACTION**/
            if(event.key.code == Keyboard::Escape)
            {
                sf::Packet falsePacket;
                falsePacket << false;

                sf::Packet truePacket;
                truePacket << true;

                if(not m_inGuiMode)
                {
                    Message guiModeOn("local_player", "setGuiMode", truePacket, 0, false);
                    Message show("main_menu", "setHidden", falsePacket, 0, false);
                    Message pause("universe", "setPause", truePacket, 0, false);

                    game.getCoreIO().recieve(guiModeOn);
                    game.getCoreIO().recieve(show);
                    game.getCoreIO().recieve(pause);
                }
                else
                {
                    Message guiModeOff("local_player", "setGuiMode", falsePacket, 0, false);
                    Message hide("main_menu", "setHidden", truePacket, 0, false);
                    Message unpause("universe", "setPause", falsePacket, 0, false);

                    game.getCoreIO().recieve(guiModeOff);
                    game.getCoreIO().recieve(hide);
                    game.getCoreIO().recieve(unpause);
                }
            }
        }



        if(m_inGuiMode)///IF WE ARE IN GUI MDOE SEND STUFF TO GUI
        {
            game.getOverlay().handleEvent(event);
        }
        else//We are in normal mode, send keypresses to game world!
        {
            ///event.key.code == Keyboard::Escape




            if(event.type == sf::Event::MouseWheelMoved)//control zooming of camera for a player
            {
                int change = -event.mouseWheel.delta;//negative because it's backwards

                if(change < 0)
                    m_camera.setZoom(m_camera.getZoom()*0.8);
                else
                    m_camera.setZoom(m_camera.getZoom()*1.2);
            }



        }
    }


    if(not m_inGuiMode)
    {
        const float speed = 0.02;
        if(Keyboard::isKeyPressed(m_inCfg.cameraUp))
            m_camera.move(b2Vec2(0,speed));
        if(Keyboard::isKeyPressed(m_inCfg.cameraDown))
            m_camera.move(b2Vec2(0,-speed));
        if(Keyboard::isKeyPressed(m_inCfg.cameraLeft))
            m_camera.move(b2Vec2(-speed,0));
        if(Keyboard::isKeyPressed(m_inCfg.cameraRight))
            m_camera.move(b2Vec2(speed,0));
    }


    /**If we are playing the game, we can intake constant presses**/
    if(not m_inGuiMode && pBody != NULL)
    {
        if(Keyboard::isKeyPressed(m_inCfg.up))
            directive(Directive::Up);
        if(Keyboard::isKeyPressed(m_inCfg.down))
            directive(Directive::Down);
        if(Keyboard::isKeyPressed(m_inCfg.left))
            directive(Directive::Left);
        if(Keyboard::isKeyPressed(m_inCfg.right))
            directive(Directive::Right);
        if(Keyboard::isKeyPressed(m_inCfg.rollCCW))
            directive(Directive::Left);
        if(Keyboard::isKeyPressed(m_inCfg.rollCW))
            directive(Directive::Right);



        if(m_tracking)
            m_camera.setPosition(pBody->GetPosition());
    }
}




void Player::input(std::string rCommand, sf::Packet rData)
{
    sf::Packet data(rData);
    if(rCommand == "toggleGuiMode")
    {
        toggleGuiMode(!m_inGuiMode);
    }
    else if(rCommand == "setGuiMode")
    {
        bool mode = false;
        data >> mode;
        toggleGuiMode(mode);
    }
}
