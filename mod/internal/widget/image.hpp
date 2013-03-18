/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean, Javier Caverni

*/

#ifndef _REDEMPTION_MOD_INTERNAL_WIDGET_IMAGE_HPP_
#define _REDEMPTION_MOD_INTERNAL_WIDGET_IMAGE_HPP_

#include "widget.hpp"
#include "mod_api.hpp"

struct widget_image : public Widget {
    Bitmap bmp;

    widget_image(mod_api * mod, int width, int height, int type, Widget * parent, int x, int y, const char* filename, uint8_t bpp)
    : Widget(mod, Rect(x, y, width, height), parent, type)
    , bmp(filename) 
    {
        TODO("we redefine image size after loading bmp, instead we should provide already loaded bmp to widget")
        this->rect = Rect(x, y, bmp.cx, bmp.cy);
    }

    ~widget_image() {
    }

    virtual void draw(const Rect & clip)
    {
        TODO("See why region clipping is not done the same way as everywhere else here")
        Rect image_screen_rect = this->to_screen_rect();
        Rect intersection = image_screen_rect.intersect(this->to_screen_rect(clip));
        const Region region = this->get_visible_region(this, this->parent, intersection);

        for (size_t ir = 0; ir < region.rects.size(); ir++){
            this->mod->draw(RDPMemBlt(0, image_screen_rect, 0xCC, 0, 0, 0), region.rects[ir], this->bmp);
        }
    }
};

#endif