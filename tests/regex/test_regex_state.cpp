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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   Unit test for bitmap class, compression performance

*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestDfaRegexAutomate
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL


#include "regex_state.hpp"

using namespace re;

inline char_int multi_char(const char * c)
{
    return re::utf8_consumer(c).bumpc();
}

BOOST_AUTO_TEST_CASE(TestRegexCheck)
{
    re::utf8_consumer empty_cons("");
    {
        State st(RANGE, multi_char("Þ"), multi_char("Þ"));
        BOOST_CHECK(st.check(multi_char("Þ"), empty_cons));
        BOOST_CHECK( ! st.check('a', empty_cons));
    }
    {
        State st(RANGE, 0, -1u);
        BOOST_CHECK(st.check(multi_char("Þ"), empty_cons));
        BOOST_CHECK(st.check('a', empty_cons));
        BOOST_CHECK(st.check('\1', empty_cons));
    }
    {
        State st(RANGE, 'e','g');
        BOOST_CHECK(st.check('e', empty_cons));
        BOOST_CHECK(st.check('f', empty_cons));
        BOOST_CHECK(st.check('g', empty_cons));
        BOOST_CHECK( ! st.check('d', empty_cons));
        BOOST_CHECK( ! st.check('h', empty_cons));
    }
    {
        State st(SEQUENCE);
        char_int seq[] = {'a','b','c',0};
        utf8_consumer consumer("abc");
        st.data.sequence.s = seq;
        BOOST_CHECK_EQUAL(st.check(consumer.bumpc(), consumer), 3);
        consumer.str("abcd");
        BOOST_CHECK_EQUAL(st.check(consumer.bumpc(), consumer), 3);
        st.data.sequence.s = 0;
    }
}


