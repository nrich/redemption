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

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestZLibRaii
#include <boost/test/auto_unit_test.hpp>

#include "zlib.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE(TestZRaiiDeflateEnd)
{
    z_stream zstrm;
    zstrm.zalloc = 0;
    zstrm.zfree = 0;
    zstrm.opaque = 0;
    BOOST_REQUIRE_EQUAL(Z_OK, deflateInit(&zstrm, Z_DEFAULT_COMPRESSION));
    {
        ZRaiiDeflateEnd end(zstrm);
    }
    BOOST_REQUIRE(Z_OK != deflateEnd(&zstrm));
}

BOOST_AUTO_TEST_CASE(TestZRaiiInflateEnd)
{
    z_stream zstrm;
    zstrm.zalloc = 0;
    zstrm.zfree = 0;
    zstrm.opaque = 0;
    BOOST_REQUIRE_EQUAL(Z_OK, inflateInit(&zstrm));
    {
        ZRaiiInflateEnd end(zstrm);
    }
    BOOST_REQUIRE(Z_OK != deflateEnd(&zstrm));
}