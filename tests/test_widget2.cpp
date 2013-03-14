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
 *   Copyright (C) Wallix 2013
 *   Author(s): Christophe Grosjean
 *
 */

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestWidgetNotify
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
#include <string>
#include <boost/lexical_cast.hpp>
#include <widget2/window.hpp>
#include <widget2/screen.hpp>
#include <widget2/edit.hpp>
#include <widget2/button.hpp>
#include <widget2/label.hpp>
#include <widget2/window_login.hpp>
#include <widget2/image.hpp>
#include <widget2/window_box.hpp>
#include "ssl_calls.hpp"
#include "RDP/RDPDrawable.hpp"
#include "png.hpp"
#include <stdio.h>
#include <font.hpp>

struct TestNotify : NotifyApi
{
    std::string s;

    virtual void notify(Widget * sender, NotifyApi::notify_event_t event)
    {
        BOOST_ASSERT(sender != 0);
        s += "event: ";
        s += boost::lexical_cast<std::string>(event);
        s += " -- id: ";
        s += boost::lexical_cast<std::string>(sender->id);
        s += ", type: ";
        s += boost::lexical_cast<std::string>(sender->type);
        s += '\n';
    }
};

struct TestDraw : ModApi
{
    RDPDrawable gd;
    Font font;

    TestDraw(uint16_t width = 1000, uint16_t height = 1000)
    : gd(width, height, true)
    , font(FIXTURES_PATH "/dejavu-sans-10.fv1")
    {}

    virtual void draw(const RDPOpaqueRect& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPScrBlt& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPDestBlt& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPPatBlt& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPMemBlt& cmd, const Rect& clip, const Bitmap& bmp)
    {
        gd.draw(cmd, clip, bmp);
    }

    virtual void draw(const RDPLineTo& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPGlyphIndex& cmd, const Rect& clip)
    {
        gd.draw(cmd, clip);
    }

    virtual void draw(const RDPBrushCache& cmd)
    {
        gd.draw(cmd);
    }

    virtual void draw(const RDPColCache& cmd)
    {
        gd.draw(cmd);
    }

    virtual void draw(const RDPGlyphCache& cmd)
    {
        gd.draw(cmd);
    }

    virtual void begin_update()
    {}

    virtual void end_update()
    {}

    class ContextText : public ModApi::ContextText
    {
    public:
        std::vector<Rect> rects;

        ContextText()
        : ModApi::ContextText()
        {}

        virtual void draw_in(ModApi* drawable, const Rect& rect, int16_t x, int16_t y, int16_t xclip, int16_t yclip, int color)
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
    };

    FontChar * get_font(uint32_t c)
    {
        return this->gd.get_font(this->font, c);
    }

    virtual ContextText* create_context_text(const char * s)
    {
        ContextText * ret = new ContextText;
        if (s[0] != 0) {
            uint32_t uni[128];
            size_t part_len = UTF8toUnicode(reinterpret_cast<const uint8_t *>(s), uni, sizeof(uni)/sizeof(uni[0]));
            ret->rects.reserve(part_len * 10);
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
                            ret->rects.push_back(Rect(ret->cx+x, y, 1,1));
                        }
                        oc >>= 1;
                    }
                }
                ret->cy = std::max<size_t>(ret->cy, font_item->height);
                ret->cx += font_item->width + 2;
            }
            if (part_len > 1)
                ret->cx -= 2;
        }
        return ret;
    }

    virtual void server_draw_text(int x, int y, const char* text, uint32_t fgcolor, uint32_t bgcolor, const Rect& clip)
    {
        this->gd.server_draw_text(x, y, text, fgcolor, bgcolor, clip, this->font);
    }

    virtual void text_metrics(const char* text, int& width, int& height)
    {
        height = 0;
        width = 0;
        uint32_t uni[256];
        size_t len_uni = UTF8toUnicode(reinterpret_cast<const uint8_t *>(text), uni, sizeof(uni)/sizeof(uni[0]));
        if (len_uni){
            for (size_t index = 0; index < len_uni; index++) {
                FontChar *font_item = this->get_font(uni[index]);
                width += font_item->width + 2;
                height = std::max(height, font_item->height);
            }
            if (len_uni > 1)
                width -= 2;
        }
    }
};

struct TestWidget
{
    WidgetScreen screen;
    Window win;
    WidgetLabel w1;
    WidgetEdit w2;
    WidgetButton w3;

    TestWidget(TestDraw * drawable=0, TestNotify * notify=0)
    : screen(drawable, 1000, 1000, notify)
    , win(drawable, Rect(30,30, 800, 600), &screen, notify, "Fenêtre 1")
    , w1(drawable, Rect(10, 40, 10, 10), &win, notify, "", 1)
    , w2(drawable, Rect(50, 40, 120, 45), &win, notify, "plop", 4, 2)
    , w3(drawable, Rect(100, 400, 10, 10), &win, notify, "", 3)
    {
        win.has_focus = true;
        w2.has_focus = true;
        screen.bg_color = 100;
        win.bg_color = 1000;
        win.titlebar.bg_color = 5326;
        w1.bg_color = 10000;
        w2.bg_color = 100000;
        w3.bg_color = 1000000;
    }
};


void save_to_png(TestDraw & drawable, const char * filename)
{
    std::FILE * file = fopen(filename, "w+");
    dump_png24(file, drawable.gd.drawable.data, drawable.gd.drawable.width,
               drawable.gd.drawable.height, drawable.gd.drawable.rowsize);
    fclose(file);
}

inline bool check_sig(const uint8_t* data, std::size_t height, uint32_t len,
                      char * message, const char * shasig)
{
    uint8_t sig[20];
    SslSha1 sha1;
    for (size_t y = 0; y < (size_t)height; y++){
        sha1.update(data + y * len, len);
    }
    sha1.final(sig);

    if (memcmp(shasig, sig, 20)){
        sprintf(message, "Expected signature: \""
        "\\x%.2x\\x%.2x\\x%.2x\\x%.2x"
        "\\x%.2x\\x%.2x\\x%.2x\\x%.2x"
        "\\x%.2x\\x%.2x\\x%.2x\\x%.2x"
        "\\x%.2x\\x%.2x\\x%.2x\\x%.2x"
        "\\x%.2x\\x%.2x\\x%.2x\\x%.2x\"",
        sig[ 0], sig[ 1], sig[ 2], sig[ 3],
        sig[ 4], sig[ 5], sig[ 6], sig[ 7],
        sig[ 8], sig[ 9], sig[10], sig[11],
        sig[12], sig[13], sig[14], sig[15],
        sig[16], sig[17], sig[18], sig[19]);
        return false;
    }
    return true;
}

inline bool check_sig(Drawable & data, char * message, const char * shasig)
{
    return check_sig(data.data, data.height, data.rowsize, message, shasig);
}

BOOST_AUTO_TEST_CASE(TraceWidgetAtPos)
{
    TestWidget w;
    BOOST_CHECK(&w.w1 == w.w1.widget_at_pos(15,45));
    BOOST_CHECK(0 == w.screen.widget_at_pos(5, 20));
    BOOST_CHECK(&w.w1 == w.screen.widget_at_pos(45, 70));
    BOOST_CHECK(&w.w2 == w.screen.widget_at_pos(89, 70));
    BOOST_CHECK(&w.w3 == w.screen.widget_at_pos(133, 437));
}

BOOST_AUTO_TEST_CASE(TraceWidgetFocus)
{
    TestNotify notify;
    TestWidget w(0, &notify);

    BOOST_CHECK(!w.w1.has_focus && w.w2.has_focus && !w.w3.has_focus);
    {
        Widget* wevent = w.screen.widget_at_pos(133, 437);
        wevent->has_focus = true;
        wevent->notify(wevent->id, FOCUS_BEGIN);
    }
    BOOST_CHECK(!w.w1.has_focus && !w.w2.has_focus && w.w3.has_focus);
    {
        Widget* wevent = w.screen.widget_at_pos(45, 70);
        wevent->has_focus = true;
        wevent->notify(wevent->id, FOCUS_BEGIN);
    }
    BOOST_CHECK(w.w1.has_focus && !w.w2.has_focus && !w.w3.has_focus);
    {
        Widget* wevent = w.screen.widget_at_pos(89, 70);
        wevent->has_focus = true;
        wevent->notify(wevent->id, FOCUS_BEGIN);
    }
    BOOST_CHECK(!w.w1.has_focus && w.w2.has_focus && !w.w3.has_focus);

    BOOST_CHECK(notify.s ==
        "event: 0 -- id: 3, type: 8\n" //FOCUS_END
        "event: 1 -- id: 2, type: 12\n" //FOCUS_BEGIN
        "event: 0 -- id: 1, type: 14\n" //FOCUS_END
        "event: 1 -- id: 3, type: 8\n" //FOCUS_BEGIN
        "event: 0 -- id: 2, type: 12\n" //FOCUS_END
        "event: 1 -- id: 1, type: 14\n" //FOCUS_BEGIN
    );
}

BOOST_AUTO_TEST_CASE(TraceWidgetDraw)
{
    TestDraw drawable;
    TestWidget w(&drawable);
    Widget wid(&drawable, Rect(700, 500, 200, 200), &w.win, Widget::TYPE_BUTTON, 0);
    wid.bg_color = 10000000;

    w.screen.send_event(WM_DRAW, 0, 0, 0);
    //or w.screen.refresh(w.screen.rect);

    //save_to_png(drawable, "/tmp/a.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\x1b\xe0\x4c\x5c\x0c\x50\x2a\x4d\x97\x0a"
        "\x0b\x13\x87\x90\xed\xa2\xad\x3b\x10\x75")){
        BOOST_CHECK_MESSAGE(false, message);
    }
}

BOOST_AUTO_TEST_CASE(TraceWidgetEdit)
{
    TestNotify notify;
    TestDraw drawable;
    TestWidget w(&drawable, &notify);

    w.screen.send_event(WM_DRAW, 0, 0, 0);
    Keymap2 keymap;
    keymap.push_kevent(Keymap2::KEVENT_KEY);
    keymap.push_char('a');
    w.screen.send_event(KEYDOWN, 0, 0, &keymap);
    BOOST_CHECK(notify.s ==
      "event: 11 -- id: 2, type: 12\n");

    //save_to_png(drawable, "/tmp/b.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\xfb\x3b\xb8\x94\x35\x80\x99\x64\xe1\xa3"
        "\xfd\x4a\xfe\xdf\x79\xe6\x93\x14\x2e\x5f")){
        BOOST_CHECK_MESSAGE(false, message);
    }
}

BOOST_AUTO_TEST_CASE(TraceWindowLogin)
{
    TestNotify notify;
    TestDraw drawable;
    WidgetScreen screen(&drawable, 1000, 1000, 0);
    WindowLogin win(&drawable, 50, 50, &screen, &notify);
    win.bg_color = 10000000;
    win.titlebar.bg_color = 322425;

    screen.refresh(screen.rect);

    win.submit.send_event(CLIC_BUTTON1_DOWN, 0, 0, 0);
    win.submit.send_event(CLIC_BUTTON1_UP, 0, 0, 0);
    BOOST_CHECK(notify.s ==
        "event: 12 -- id: 0, type: 1\n");

    //save_to_png(drawable, "/tmp/c.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\x5b\x94\x8d\x26\xe9\x23\x04\xbb\xa4\xf8"
        "\x9f\xf1\x61\xd3\x66\x39\x0e\x02\x6b\x0d")){
        BOOST_CHECK_MESSAGE(false, message);
    }
}

BOOST_AUTO_TEST_CASE(TraceDrawText)
{
    TestDraw drawable(100,100);
    WidgetScreen screen(&drawable, 100, 100, 0);
    screen.refresh(screen.rect);

    drawable.server_draw_text(10,10, "plop", WHITE, 55555, Rect(0,0,27,20));
    drawable.server_draw_text(10,30, "une phrase", BLACK, 55555, screen.rect);
    drawable.server_draw_text(-10,50, "une phrase", BLACK, 55555, screen.rect);

    //save_to_png(drawable, "/tmp/d.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\xe4\x6b\x5f\x8f\xb4\xc2\xda\x61\x28\xd6"
        "\x9f\x9e\x9b\x4f\xb4\x0c\xb9\xaa\x1a\xcf")){
        BOOST_CHECK_MESSAGE(false, message);
    }
}

// void show_focus(std::ostream& out, Widget * w)
// {
//     out << "w->id: " << w->id << ' ' << w->has_focus << '\n';
//     if (w->type & Widget::TYPE_WND) {
//         WidgetComposite * win = static_cast<WidgetComposite*>(w);
//         for (size_t i = 0; i < win->child_list.size(); ++i) {
//             show_focus(out, win->child_list[i]);
//         }
//     }
// }

BOOST_AUTO_TEST_CASE(WidgetFocusTrace)
{
    TestNotify notify;
    WidgetScreen screen(0, 1000, 1000, &notify);
    Window win(0, Rect(30,30, 50, 50), &screen, &notify, "Fenêtre 1",1);
    WidgetLabel w1(0, Rect(10, 40, 10, 10), &win, &notify, "text", 2);
    Window win2(0, Rect(100,100, 800, 600), &win, &notify, "Fenêtre 1",3);
    WidgetEdit w2(0, Rect(50, 40, 120, 45), &win2, &notify, "plop", 4, 4);
    WidgetButton w3(0, Rect(100, 400, 10, 10), &win2, &notify, "submit", 5);
    win.has_focus = true;
    w1.has_focus = true;

    Keymap2 keymap;
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    keymap.push_kevent(Keymap2::KEVENT_TAB);
    screen.send_event(KEYDOWN, 0, 0, &keymap);
    //std::ostringstream oss;
    //show_focus(oss, &screen);

    BOOST_CHECK(notify.s ==
        "event: 0 -- id: 4, type: 12\n"
        "event: 1 -- id: 2, type: 14\n"
        "event: 0 -- id: 5, type: 8\n"
        "event: 1 -- id: 4, type: 12\n"
        "event: 0 -- id: 2, type: 14\n"
        "event: 1 -- id: 3, type: 1\n"
        "event: 0 -- id: 4, type: 12\n"
        "event: 1 -- id: 5, type: 8\n"
        "event: 1 -- id: 2, type: 14\n"
        "event: 0 -- id: 5, type: 8\n"
        "event: 1 -- id: 4, type: 12\n"
        "event: 0 -- id: 2, type: 14\n"
        "event: 1 -- id: 3, type: 1\n"
    );
}

BOOST_AUTO_TEST_CASE(TraceDrawImage)
{
    TestDraw drawable(1000,1000);
    WidgetScreen screen(&drawable, 1000, 1000, 0);
    screen.bg_color = 12212;
    WidgetImage image1(&drawable, 0, 0, FIXTURES_PATH"/logo-redemption.bmp", &screen, 0);
    WidgetImage image2(&drawable, 0, 100, FIXTURES_PATH"/logo-redemption.bmp", &screen, 0);
    WidgetImage image3(&drawable, -100, 500, FIXTURES_PATH"/logo-redemption.bmp", &screen, 0);
    screen.refresh(screen.rect);

    //save_to_png(drawable, "/tmp/e.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\x4a\xbe\xa7\xe2\x7a\xdf\x0f\xc9\xf3\x6d"
        "\x48\x94\xb5\x01\xaa\xa2\x84\xb6\xb0\x22")){
            BOOST_CHECK_MESSAGE(false, message);
    }
}

BOOST_AUTO_TEST_CASE(TraceWindowBox)
{
    TestDraw drawable;
    WidgetScreen screen(&drawable, 500, 500, 0);
    WindowBox win(&drawable, Rect(50, 50, 300, 400), &screen, 0, "WindowBox");
    win.bg_color = 10000000;
    win.titlebar.bg_color = 322425;

    screen.refresh(screen.rect);

    //save_to_png(drawable, "/tmp/f.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\x6f\x2c\x85\xd6\xa5\xc9\x72\xb3\x0a\x0b"
        "\x2d\x40\x2e\x68\x17\xbc\x17\x18\xbc\xb9")){
        BOOST_CHECK_MESSAGE(false, message);
    }
}