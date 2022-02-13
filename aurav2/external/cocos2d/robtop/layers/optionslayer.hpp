#pragma once
#ifndef _LAYERS_OPTIONSLAYER_HPP
#define _LAYERS_OPTIONSLAYER_HPP

#include "gjdropdownlayer.hpp"

class OptionsLayer : public GJDropDownLayer {
public:
    static OptionsLayer* create();
};

#endif
