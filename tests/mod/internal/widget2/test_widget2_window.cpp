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
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen,
 *              Meng Tan
 */

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestWidgetWindow
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
#include "log.hpp"

#include "internal/widget2/widget2_rect.hpp"
#include "internal/widget2/widget2_window.hpp"
#include "internal/widget2/screen.hpp"
#include "png.hpp"
#include "ssl_calls.hpp"
#include "RDP/RDPDrawable.hpp"
#include "check_sig.hpp"

#include "fake_draw.hpp"

BOOST_AUTO_TEST_CASE(TraceWidgetWindow)
{
    TestDraw drawable(800, 600);
    NotifyApi * notifier = NULL;
    WidgetScreen parent(drawable, 800, 600);
    int id = 0;

    Window window(drawable, Rect(30,40,500,400), parent, notifier, "Window 1");
    window.resize_titlebar();
    WidgetRect wrect1(drawable, Rect(0,window.titlebar.cy()+0,100,100),
                      window, notifier, id++, YELLOW);
    WidgetRect wrect2(drawable, Rect(0,window.titlebar.cy()+100,100,100),
                      window, notifier, id++, RED);
    WidgetRect wrect3(drawable, Rect(100,window.titlebar.cy()+100,100,100),
                      window, notifier, id++, BLUE);
    WidgetRect wrect4(drawable, Rect(300,300,100,100),
                      window, notifier, id++, GREEN);
    WidgetRect wrect5(drawable, Rect(700,-50,100,100),
                      window, notifier, id++, WHITE);
    WidgetRect wrect6(drawable, Rect(-50,550,100,100),
                      window, notifier, id++, GREY);
    window.add_widget(&wrect1);
    window.add_widget(&wrect2);
    window.add_widget(&wrect3);
    window.add_widget(&wrect4);
    window.add_widget(&wrect5);
    window.add_widget(&wrect6);

    // ask to widget to redraw at position 150,500 and of size 800x600
    window.rdp_input_invalidate(Rect(150 + window.dx(),
                                     150 + window.dy(),
                                     window.cx(),
                                     window.cy()));

    //drawable.save_to_png("/tmp/window.png");

    char message[1024];
    if (!check_sig(drawable.gd.drawable, message,
        "\xd4\xd9\xca\x9c\xf9\x83\xaa\xa1\x6f\xac"
        "\xaa\x1d\x71\x5a\xfd\x22\x95\x0d\x81\x13"
        )){
        BOOST_CHECK_MESSAGE(false, message);
    }

    // ask to widget to redraw at position 0,500 and of size 100x100
    window.rdp_input_invalidate(Rect(0 + window.dx(),
                                     500 + window.dy(),
                                     100,
                                     100));

    //drawable.save_to_png("/tmp/window2.png");

    if (!check_sig(drawable.gd.drawable, message,
        "\xd4\xd9\xca\x9c\xf9\x83\xaa\xa1\x6f\xac"
        "\xaa\x1d\x71\x5a\xfd\x22\x95\x0d\x81\x13"
        )){
        BOOST_CHECK_MESSAGE(false, message);
    }

    // ask to widget to redraw at it's current position
    window.rdp_input_invalidate(window.rect);

    //drawable.save_to_png("/tmp/window3.png");

    if (!check_sig(drawable.gd.drawable, message,
        "\x09\x31\xc2\xa0\xae\x78\x5a\x00\x69\x3e"
        "\xfe\x5e\x80\xb9\x3d\xbe\x52\x3f\x99\x1e"
        )){
        BOOST_CHECK_MESSAGE(false, message);
    }
    window.clear();
}

BOOST_AUTO_TEST_CASE(EventWidgetWindow)
{
    struct TestNotify : NotifyApi
    {
        Widget2* sender;
        notify_event_t event;

        TestNotify()
        : sender(0)
        , event(0)
        {}

        virtual void notify(Widget2* sender, notify_event_t event)
        {
            this->sender = sender;
            this->event = event;
        }
    } notifier;

    TestDraw drawable(800, 600);

    WidgetScreen parent(drawable, 800, 600);

    Window window(drawable, Rect(30,40,500,400), parent, &notifier, "Window 1");
    window.button_close.rdp_input_mouse(MOUSE_FLAG_BUTTON1|MOUSE_FLAG_DOWN, 0, 0, NULL);
    BOOST_CHECK(notifier.event == 0);
    BOOST_CHECK(notifier.sender == 0);
    window.button_close.rdp_input_mouse(MOUSE_FLAG_BUTTON1, 0, 0, NULL);
    window.clear();
}