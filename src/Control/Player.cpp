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
}
Player::~Player()
{

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

        }
    }




    /**If we are playing the game, we can intake constant presses**/
    if(not m_inGuiMode)
    {
        ///ARE CERTAIN KEYS PRESSED DOWN AT THIS MOMENT

    }
}




void Player::input(const std::string& rCommand, const sf::Packet& rData)
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
