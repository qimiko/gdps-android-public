#pragma once
#ifndef _LAYERS_LEVELCELL_HPP
#define _LAYERS_LEVELCELL_HPP

#include <robtop/objects/gjgamelevel.hpp>

#include "tableviewcell.hpp"

class LevelCell : public TableViewCell {
public:
    void updateBGColor(int);
    void loadCustomLevelCell();
    void loadFromLevel(GJGameLevel*);
};

#endif
