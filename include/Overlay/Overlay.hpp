#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"

namespace leon
{
class Panel;
};

class Overlay
{
public:
    Overlay(const IOComponentData& rData);
    virtual ~Overlay();

    leon::Panel& getPanel(const std::string& rPanelName);
    void addPanel(std::tr1::shared_ptr<leon::Panel> spPanel);

    void handleEvent(sf::Event& rEvent);
    void loadMenus();
    tgui::Gui& getGui();

    void toggleMenu(bool show);//display menu, assume gui control, send pause game command

protected:
    void input(const std::string rCommand, sf::Packet rData);

private:
    bool m_menuShowing;//true if menu is showing

    IOComponent m_io;
    tgui::Gui m_gui;//the TGUI gui that handles most things
    std::vector<std::tr1::shared_ptr<leon::Panel> > m_panelList;//all the Panels that can get displayed, each item can be active or inactive, if its active, it gets displayed.
};

#endif // OVERLAY_HPP
