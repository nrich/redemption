/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2015
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestOrderAlternateSecondaryWindowing
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
//#define LOGPRINT

#include "RDP/orders/AlternateSecondaryWindowing.hpp"

BOOST_AUTO_TEST_CASE(TestWindowingCommon)
{
/*
    const uint32_t WindowId = 1000;
    WindowInformationCommonHeader wch_s(WINDOW_ORDER_TYPE_WINDOW, WindowId);

    uint8_t data[256];
    FixedSizeStream stream(data, sizeof(data));

    wch_s.emit_begin(stream);
    stream.mark_end();
    wch_s.emit_end();

    stream.rewind();

    WindowInformationCommonHeader wch_r;
    wch_r.receive(stream);

    BOOST_CHECK_EQUAL(wch_r.FieldsPresentFlags(), WINDOW_ORDER_TYPE_WINDOW);
    BOOST_CHECK_EQUAL(wch_r.WindowId(),           WindowId);
*/

    RDP::RAIL::NewOrExistingWindow new_or_existing_window;
}