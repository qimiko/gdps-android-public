#pragma once
#ifndef _ENUMS_GAMEOBJECTTYPE_HPP
#define _ENUMS_GAMEOBJECTTYPE_HPP

enum class GameObjectType {
    UnknownObject = 0,
    BasicObject = 1,
    Dangerous = 2,
    ReverseGravityPortal = 3,
    DefaultGravityPortal = 4,
    ShipVehiclePortal = 5,
    CubeVehiclePortal = 6,
    NonSolidBlock = 7,
    PulsingDecoration = 8,
    YellowPad = 9,
    PinkPad = 10,
    BluePad = 11,
    YellowRing = 12,
    PinkRing = 13,
    BlueRing = 14,
    MirrorPortal = 15,
    UnMirrorPortal = 16,
    BallVehiclePortal = 17,
    NormalSizePortal = 18,
    MiniSizePortal = 19,
    UfoVehiclePortal = 20,
    SpeedPortalObject = 21,
    BreakableBlock = 22,
    SecretCoin = 23,
    EnableDualPortal = 24,
    DisableDualPortal = 25,
    SlopeObject = 26,
    WaveVehiclePortal = 27,
};

#endif