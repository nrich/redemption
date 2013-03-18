/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2012
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen
 */

#if !defined(REDEMPTION_MOD_WIDGET2_LABEL_HPP)
#define REDEMPTION_MOD_WIDGET2_LABEL_HPP

#include "widget.hpp"

class WidgetLabel : public Widget
{
public:
    static const size_t buffer_size = 256;

    char buffer[buffer_size];
    ModApi::ContextText * context_text;
    int x_text;
    int y_text;

public:
    WidgetLabel(ModApi* drawable, const Rect& rect, Widget* parent, NotifyApi* notifier, const char * text, int id = 0, int xtext = 0, int ytext = 0)
    : Widget(drawable, rect, parent, Widget::TYPE_LABEL, notifier, id)
    , context_text(0)
    , x_text(xtext)
    , y_text(ytext)
    {
        this->set_text(text);
    }

    virtual ~WidgetLabel()
    {
        delete context_text;
    }

    void set_text(const char * text)
    {
        delete this->context_text;
        this->context_text = 0;
        this->buffer[0] = 0;
        if (text) {
            const size_t max = buffer_size - 1;
            memcpy(buffer, text, max);
            this->buffer[max] = 0;
            if (this->drawable) {
                this->context_text = this->drawable->create_context_text(this->buffer);
            }
        }
    }

    const char * get_text() const
    {
        return this->buffer;
    }

    virtual void draw(const Rect& rect, int16_t x, int16_t y, int16_t xclip, int16_t yclip)
    {
        this->Widget::draw(rect, x, y, xclip, yclip);
        if (this->context_text) {
            this->context_text->draw_in(
                this->drawable,
                Rect(rect.x,
                     rect.y,
                     rect.cx - this->x_text,
                     rect.cy - this->y_text),
                x + this->x_text,
                y + this->y_text,
                xclip + this->x_text,
                yclip + this->y_text,
                ~this->bg_color
            );
        }
    }
};

#endif