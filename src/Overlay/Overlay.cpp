#include "Overlay.hpp"

#include "Globals.hpp"
#include "Panel.hpp"
#include "Picture.hpp"
#include "Button.hpp"
#include "Courier.hpp"
#include "EditBox.hpp"
#include "Chatbox.hpp"

using namespace std;

Overlay::Overlay() : m_gui(game.getWindow())
{
    m_gui.setGlobalFont("TGUI/fonts/DejaVuSans.ttf");
    /**If we call loadMenus now, we try and access this very Overlay object before it has been returned to game**/
}
Overlay::~Overlay()
{
    cout << "\nOverlay Destroying...";
}
void Overlay::addPanel(std::tr1::shared_ptr<leon::Panel> spPanel)
{
    m_panelList.push_back(spPanel);
}
void Overlay::handleEvent(sf::Event& rEvent)
{
    m_gui.handleEvent(rEvent);
}
tgui::Gui& Overlay::getGui()
{
    return m_gui;
}







void Overlay::loadMenus()
{


    /**MAIN MENU**/
    /**=========**/
    sf::Packet voidPacket;

    leon::PanelData mainMenuData;
    mainMenuData.ioComp.name = "main_menu";
    mainMenuData.backgroundTex = "core/screen_1.png";
    mainMenuData.screenCoords = sf::Vector2f(0, 0);
    mainMenuData.size = sf::Vector2f(1920,1080);
    leon::Panel* pMain_menu = new leon::Panel(game.getOverlay().getGui(), mainMenuData);
    /**====TITLE====**/
    leon::PictureData pictureData;
    pictureData.texName = "core/main_menu_logo.png";
    pictureData.screenCoords = sf::Vector2f(20, 20);
    pictureData.size = sf::Vector2f(847,104);
    leon::WidgetBase* picture = new leon::Picture(*pMain_menu->getPanelPtr(), pictureData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(picture));
    /**====RESUME====**/
    leon::ButtonData resumeButtonData;
    resumeButtonData.size = sf::Vector2f(150,50);
    resumeButtonData.buttonText = "Resume";
    resumeButtonData.screenCoords = sf::Vector2f(20, 300);
    sf::Packet falsePacket;
    falsePacket << false;
    sf::Packet truePacket;
    truePacket << true;
    Courier resumeMessage1;
    resumeMessage1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    resumeMessage1.message.reset("main_menu", "setHidden", truePacket, 0, false);
    Courier resumeMessage2;
    resumeMessage2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    sf::Packet statePacket;
    statePacket << false;
    resumeMessage2.message.reset("local_player", "setGuiMode", falsePacket, 0, false);
    Courier resumeMessage3;
    resumeMessage3.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    resumeMessage3.message.reset("universe", "setPause", falsePacket, 0, false);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage1);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage2);
    resumeButtonData.ioComp.courierList.push_back(resumeMessage3);
    leon::WidgetBase* pResume = new leon::Button(*pMain_menu->getPanelPtr(), resumeButtonData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pResume));
    /**====HOW TO PLAY====**/
    leon::ButtonData htpButData;
    htpButData.size = sf::Vector2f(275,50);
    htpButData.buttonText = "Multiplayer";
    htpButData.screenCoords = sf::Vector2f(20, 400);
    Courier htpMessage;
    htpMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    htpMessage.message.reset("multiplayer_connect", "toggleHidden", voidPacket, 0, false);
    htpButData.ioComp.courierList.push_back(htpMessage);
    leon::WidgetBase* pHTP = new leon::Button(*pMain_menu->getPanelPtr(), htpButData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pHTP));
    /**====EXIT====**/
    leon::ButtonData exitButtonData;
    exitButtonData.size = sf::Vector2f(100,50);
    exitButtonData.buttonText = "Exit";
    exitButtonData.screenCoords = sf::Vector2f(20, 600);
    Courier buttonMessage;
    buttonMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    buttonMessage.message.reset("game", "exit", voidPacket, 0, false);
    exitButtonData.ioComp.courierList.push_back(buttonMessage);
    leon::WidgetBase* pExit = new leon::Button(*pMain_menu->getPanelPtr(), exitButtonData);
    pMain_menu->add(tr1::shared_ptr<leon::WidgetBase>(pExit));

    game.getOverlay().addPanel(tr1::shared_ptr<leon::Panel>(pMain_menu));
    /**=========**/
    /**MAIN MENU**/


    /**MULTIPLAYER**/
    sf::Vector2f multPanelSize = sf::Vector2f(640,480);
    leon::PanelData multiplayerConnect;
    multiplayerConnect.ioComp.name = "multiplayer_connect";
    multiplayerConnect.startHidden = true;
    multiplayerConnect.backgroundColor = sf::Color::Black;
    multiplayerConnect.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-multPanelSize.x/2,game.getWindow().getSize().y/2-multPanelSize.y/2);
    multiplayerConnect.size = sf::Vector2f(multPanelSize.x,multPanelSize.y);
    leon::Panel* pMultMenu = new leon::Panel(*pMain_menu->getPanelPtr(), multiplayerConnect);

    /**JOIN**/
    leon::ButtonData joinButt;
    joinButt.ioComp.name = "join_button";
    joinButt.size = sf::Vector2f(100,50);
    joinButt.buttonText = "Join";
    joinButt.screenCoords = sf::Vector2f(5, 5);
    Courier joinMess;
    joinMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    joinMess.message.reset("networkboss", "join", voidPacket, 0, false);
    joinButt.ioComp.courierList.push_back(joinMess);
    pMultMenu->add(tr1::shared_ptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), joinButt)));
    /**HOST**/
    leon::ButtonData hostButt;
    hostButt.size = sf::Vector2f(100,50);
    hostButt.buttonText = "Host";
    hostButt.screenCoords = sf::Vector2f(110,5);
    Courier hostMess;
    hostMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    hostMess.message.reset("networkboss", "host", voidPacket, 0, false);
    hostButt.ioComp.courierList.push_back(hostMess);
    pMultMenu->add(tr1::shared_ptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), hostButt)));
    /**PORT**/
    leon::EditBoxData port;
    port.size = sf::Vector2f(125,50);
    port.startingText = "5050";
    port.screenCoords = sf::Vector2f(215,5);
    Courier portMess;
    portMess.condition.reset(EventType::TextChanged, 0, 'd', true);
    portMess.message.reset("networkboss", "joinPort", voidPacket, 0, true);
    port.ioComp.courierList.push_back(portMess);
    pMultMenu->add(tr1::shared_ptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), port)));
    /**IP**/
    leon::EditBoxData ipAdd;
    ipAdd.ioComp.name = "ipaddress_editbox";
    ipAdd.size = sf::Vector2f(335,50);
    ipAdd.startingText = "IP Address";
    ipAdd.screenCoords = sf::Vector2f(5,60);
    Courier ipAddMess;
    ipAddMess.condition.reset(EventType::TextChanged, 0, 'd', true);
    ipAddMess.message.reset("networkboss", "joinIP", voidPacket, 0, true);
    ipAdd.ioComp.courierList.push_back(ipAddMess);
    pMultMenu->add(tr1::shared_ptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), ipAdd)));
    /**MULTIPLAYER**/




    /**LOBBY**/
    sf::Vector2f lobbyPanelSize = sf::Vector2f(640,480);
    leon::PanelData lobbyPanel;
    lobbyPanel.ioComp.name = "lobby";
    lobbyPanel.startHidden = true;
    lobbyPanel.backgroundColor = sf::Color::Black;
    lobbyPanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-lobbyPanelSize.x/2,game.getWindow().getSize().y/2-lobbyPanelSize.y/2);
    lobbyPanel.size = sf::Vector2f(lobbyPanelSize.x,lobbyPanelSize.y);
    leon::Panel* pLobby = new leon::Panel(*pMain_menu->getPanelPtr(), lobbyPanel);

    /**DISCONNECT**/
    leon::ButtonData disconnect;
    disconnect.ioComp.name = "lobby_disconnect";
    disconnect.size = sf::Vector2f(250,50);
    disconnect.buttonText = "Disconnect";
    disconnect.screenCoords = sf::Vector2f(lobbyPanelSize.x-(disconnect.size.x+5), lobbyPanelSize.y-(disconnect.size.y+5));
    Courier disconnectMess1;
    disconnectMess1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    disconnectMess1.message.reset("lobby", "toggleHidden", voidPacket, 0, false);
    Courier disconnectMess2;
    disconnectMess2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    disconnectMess2.message.reset("networkboss", "localOnly", voidPacket, 0, false);

    disconnect.ioComp.courierList.push_back(disconnectMess1);
    disconnect.ioComp.courierList.push_back(disconnectMess2);


    pLobby->add(tr1::shared_ptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), disconnect)));

    /**CHATBOX**/
    leon::ChatboxData chatbox;
    chatbox.ioComp.name = "lobby_chatbox";
    chatbox.size = sf::Vector2f(250,200);
    chatbox.screenCoords = sf::Vector2f(3,7);

    pLobby->add(tr1::shared_ptr<leon::WidgetBase>(new leon::Chatbox(*pLobby->getPanelPtr(), chatbox)));
    /**LOBBY**/





    /**MESSAGE**/
    sf::Vector2f closePanelSize = sf::Vector2f(640,480);
    leon::PanelData messagePanel;
    messagePanel.ioComp.name = "message_panel";
    messagePanel.startHidden = true;
    messagePanel.backgroundColor = sf::Color::Blue;
    messagePanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-closePanelSize.x/2,game.getWindow().getSize().y/2-closePanelSize.y/2);
    messagePanel.size = sf::Vector2f(closePanelSize.x,closePanelSize.y);
    leon::Panel* pMessBox = new leon::Panel(game.getOverlay().getGui(), messagePanel);
    /**====OK====**/
    leon::ButtonData closeMessBox;
    closeMessBox.size = sf::Vector2f(50,50);
    closeMessBox.buttonText = "OK";
    closeMessBox.screenCoords = sf::Vector2f(closePanelSize.x/2-50/2, closePanelSize.y-(50+5));
    Courier closeMess;
    closeMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
    closeMess.message.reset("message_panel", "toggleHidden", voidPacket, 0, false);
    closeMessBox.ioComp.courierList.push_back(closeMess);
    leon::WidgetBase* pClose = new leon::Button(*pMessBox->getPanelPtr(), closeMessBox);
    pMessBox->add(tr1::shared_ptr<leon::WidgetBase>(pClose));
    game.getOverlay().addPanel(tr1::shared_ptr<leon::Panel>(pMessBox));

}
