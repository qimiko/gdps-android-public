#pragma once
#ifndef _LAYERS_GJSCORECELL_HPP
#define _LAYERS_GJSCORECELL_HPP

#include <robtop/objects/gjuserscore.hpp>

#include "tableviewcell.hpp"

class GJScoreCell : public TableViewCell {
public:
    void loadFromScore(GJUserScore*);
};

#endif
