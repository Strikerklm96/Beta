#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

enum class GraphicsLayer
{
    BackgroundFar = 0,//scenery
    BackgroundClose,//scenery

    ShipUnder,//fill background
    ShipMiddle,//fill (meter)
    GModules,//ship module

    ShipHull,//ship skin

    ShipAppendagesLower,//appendages
    ShipAppendagesUpper,//tippy top appendages

    Projectiles,//other
    Shield,//force fields

    HUD,//HUD SPRITES LIKE HEALTH AND AMMO
    Menus,//unknown what this is for
    maxLayers,
};

#endif // GRAPHICSLAYER_H