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

    HUD,
    Menus,
    maxLayers,
};

#endif // GRAPHICSLAYER_H
