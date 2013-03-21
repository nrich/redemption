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

#include "widget_rect.hpp"

class WidgetLabel : public WidgetRect
{
public:
    static const size_t buffer_size = 256;

    char buffer[buffer_size];

#if 0
    class ContextText
    {
    public:
        size_t cx;
        size_t cy;
        std::vector<Rect> rects;

        ContextText()
        : cx(0)
        , cy(0)
        {}

        void init_context_text(const char * s)
        {
            if (s[0] != 0) {
                uint32_t uni[128];
                size_t part_len = UTF8toUnicode(reinterpthis_cast<const uint8_t *>(s), uni, sizeof(uni)/sizeof(uni[0]));
                this->rects.reserve(part_len * 10);
                for (size_t index = 0; index < part_len; index++) {
                    FontChar *font_item = this->get_font(uni[index]);
                    int i = 0;
                    for (int y = 0 ; y < font_item->height; y++){
                        unsigned char oc = 1<<7;
                        for (int x = 0; x < font_item->width; x++){
                            if (!oc) {
                                oc = 1 << 7;
                                ++i;
                            }
                            if (font_item->data[i + y] & oc) {
                                this->rects.push_back(Rect(this->cx+x, y, 1,1));
                            }
                            oc >>= 1;
                        }
                    }
                    this->cy = std::max<size_t>(this->cy, font_item->height);
                    this->cx += font_item->width + 2;
                }
                if (part_len > 1)
                    this->cx -= 2;
            }
        }

        void draw_in(ModApi* drawable, const Rect& rect, int16_t x, int16_t y, int16_t xclip, int16_t yclip, int color)
        {
            Rect clip(
                std::max<int16_t>(rect.x + x, xclip),
                std::max<int16_t>(rect.y + y, yclip),
                std::min<int>(rect.x, xclip) + rect.cx,
                std::min<int>(rect.y, yclip) + rect.cy
            );

            if (clip.isempty()) {
                return ;
            }
            for (size_t i = 0; i < this->rects.size(); ++i) {
                Rect rectd = rect.intersect(this->rects[i]);
                if (!rectd.isempty()) {
                    drawable->draw(
                        RDPOpaqueRect(rectd.offset(x, y), color),
                        clip
                    );
                }
            }
        }
    } context_text;
#endif
    int x_text;
    int y_text;
    int fg_color;

public:
    WidgetLabel(ModApi* drawable, const Rect& rect, Widget* parent, NotifyApi* notifier, const char * text, int id = 0, int bgcolor = BLACK, int fgcolor = WHITE, int xtext = 0, int ytext = 0)
    : WidgetRect(drawable, rect, parent, notifier, id, bgcolor)
    , x_text(xtext)
    , y_text(ytext)
    , fg_color(fgcolor)
    {
        this->set_text(text);
    }

    virtual ~WidgetLabel()
    {
    }

    void set_text(const char * text)
    {
        this->buffer[0] = 0;
        //this->context_text.rects.clear();
        if (text) {
            const size_t max = buffer_size - 1;
            memcpy(buffer, text, max);
            this->buffer[max] = 0;
            //if (this->drawable) {
            //    this->context_text.init_context_text(this->buffer);
            //}
        }
    }

    const char * get_text() const
    {
        return this->buffer;
    }

    virtual void draw(const Rect& clip)
    {
        this->WidgetRect::draw(clip);
        screen_position s = this->position_in_screen(clip);
        int dx = s.clip.x < 0 ? s.clip.x : 0;
        int dy = s.clip.y < 0 ? s.clip.y : 0;
        this->drawable->server_draw_text(this->x_text + s.x,
                                         this->y_text + s.y,
                                         this->get_text(),
                                         this->fg_color,
                                         Rect(s.clip.x - dx,
                                              s.clip.y - dy,
                                              s.clip.cx + dx,
                                              s.clip.cy + dy
                                             )
                                        );
    }

#if 0
    virtual void draw(const Rect& rect, int16_t x, int16_t y, int16_t xclip, int16_t yclip)
    {
        this->Widget::draw(rect, x, y, xclip, yclip);
        this->context_text.draw_in(
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
#endif
};

#endif