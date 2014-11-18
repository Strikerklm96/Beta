#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include "stdafx.hpp"

enum class GraphicsLayer
{
    BackgroundFar = 0,//scenery
    BackgroundClose,//scenery

    ShipUnder,//fill background
    ShipMiddle,//fill (meter)
    ShipModules,//ship module

    ShipHull,//ship skin

    ShipAppendagesLower,//appendages
    ShipAppendagesUpper,//tippy top appendages

    Projectiles,//other
    Shield,//force fields

    HUD,//HUD SPRITES LIKE HEALTH AND AMMO
    Menus,//unknown what this is for
    maxLayers,
};

GraphicsLayer ChooseLayer(const std::string& rStr);

#endif // GRAPHICSLAYER_H
