#pragma once
#ifndef _LAYERS_COMMENTCELL_HPP
#define _LAYERS_COMMENTCELL_HPP

#include <robtop/objects/gjcomment.hpp>

#include "tableviewcell.hpp"

class CommentCell : public TableViewCell {
public:
	void loadFromComment(GJComment*);
};

#endif
