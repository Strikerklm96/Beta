#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "stdafx.hpp"

namespace leon
{
class Panel;
};

class Overlay
{
public:
    Overlay();
    virtual ~Overlay();

    leon::Panel& getPanel(const std::string& rPanelName);
    void addPanel(std::tr1::shared_ptr<leon::Panel> spPanel);

    void handleEvent(sf::Event& rEvent);
    void loadMenus();
    tgui::Gui& getGui();

protected:
private:
    tgui::Gui m_gui;//the TGUI gui that handles most things
    std::vector<std::tr1::shared_ptr<leon::Panel> > m_panelList;//all the Panels that can get displayed, each item can be active or inactive, if its active, it gets displayed.
};

#endif // OVERLAY_HPP
