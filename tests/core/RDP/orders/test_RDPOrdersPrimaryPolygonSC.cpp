/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  Product name: redemption, a FLOSS RDP proxy
  Copyright (C) Wallix 2013
  Author(s): Christophe Grosjean, Raphael Zhou, Jonathan Poelen,
  Meng Tan

  Unit test to RDP Orders coder/decoder
  Using lib boost functions for testing
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestOrderPolygonSC
#include <boost/test/auto_unit_test.hpp>

#define LOGPRINT

#include "RDP/orders/RDPOrdersPrimaryPolygonSC.hpp"

#include "test_orders.hpp"

BOOST_AUTO_TEST_CASE(TestPolygonSCEmpty)
{
    using namespace RDP;
    BStream stream(1000);
    RDPOrderCommon state_common(POLYGONSC, Rect(700, 200, 100, 200));
    RDPPolygonSC state_Polygon;

    BOOST_CHECK_EQUAL(0, (stream.get_offset()));

    RDPOrderCommon newcommon(POLYGONSC, Rect(0, 400, 800, 76));
    RDPPolygonSC().emit(stream, newcommon, state_common, state_Polygon);

    uint8_t datas[7] = {
        SMALL | BOUNDS | STANDARD | DELTA,
        0x83,
        0x00,
        0x00,
        0x90,
        0x01,
        0x4C };
    check_datas(stream.get_offset(), stream.get_data(), 7, datas, "polygonsc draw 01");

    stream.mark_end(); stream.p = stream.get_data();

    RDPOrderCommon common_cmd = state_common;
    uint8_t control = stream.in_uint8();
    BOOST_CHECK_EQUAL(true, !!(control & STANDARD));
    RDPPrimaryOrderHeader header = common_cmd.receive(stream, control);

    BOOST_CHECK_EQUAL((uint8_t)POLYGONSC, common_cmd.order);
    BOOST_CHECK_EQUAL(0, common_cmd.clip.x);
    BOOST_CHECK_EQUAL(400, common_cmd.clip.y);
    BOOST_CHECK_EQUAL(800, common_cmd.clip.cx);
    BOOST_CHECK_EQUAL(76, common_cmd.clip.cy);

    RDPPolygonSC cmd;
    cmd.receive(stream, header);

    check<RDPPolygonSC>(common_cmd, cmd,
                        RDPOrderCommon(POLYGONSC, Rect(0, 400, 800, 76)),
                        RDPPolygonSC(),
                        "polygonsc draw 01");
}
BOOST_AUTO_TEST_CASE(TestPolygonSC)
{
    using namespace RDP;
    BStream stream(1000);

    RDPOrderCommon state_common(0, Rect(0, 0, 0, 0));
    RDPPolygonSC state_polygonSC;
    RDPOrderCommon newcommon(POLYGONSC, Rect(0, 0, 1024, 768));

    BStream deltaPoints(1024);

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(20);

    deltaPoints.out_sint16_le(160);
    deltaPoints.out_sint16_le(0);

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(-30);

    deltaPoints.out_sint16_le(50);
    deltaPoints.out_sint16_le(50);

    deltaPoints.out_sint16_le(-50);
    deltaPoints.out_sint16_le(50);

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(-30);

    deltaPoints.out_sint16_le(-160);
    deltaPoints.out_sint16_le(0);

    deltaPoints.mark_end();
    deltaPoints.rewind();

    RDPPolygonSC polygonSC(158, 230, 0x0D, 0, 0x000000, 7, deltaPoints);


    polygonSC.emit(stream, newcommon, state_common, state_polygonSC);

    BOOST_CHECK_EQUAL((uint8_t)POLYGONSC, newcommon.order);
    BOOST_CHECK_EQUAL(Rect(0, 0, 0, 0), newcommon.clip);

    uint8_t datas[] = {
        CHANGE | STANDARD,
        POLYGONSC,
        0x67,
        0x9E, 0x00, 0xE6, 0x00, // xStart = 158, yStart = 203
        0x0D,                   // bRop2
        0x07,                   // NumDeltaEntries
        0x0D,                   // cbData
        0x98, 0x24, 0x14, 0x80, 0xA0, 0x62, 0x32, 0x32,
        0x4E, 0x32, 0x62, 0xFF, 0x60
    };
    check_datas(stream.p-stream.get_data(), stream.get_data(), sizeof(datas), datas, "PolygonSC 1");

    stream.mark_end(); stream.p = stream.get_data();

    RDPOrderCommon common_cmd = state_common;
    uint8_t control = stream.in_uint8();
    BOOST_CHECK_EQUAL(true, !!(control & STANDARD));
    RDPPrimaryOrderHeader header = common_cmd.receive(stream, control);

    BOOST_CHECK_EQUAL((uint8_t)0x09, header.control);
    BOOST_CHECK_EQUAL((uint32_t)0x67, header.fields);
    BOOST_CHECK_EQUAL((uint8_t)POLYGONSC, common_cmd.order);
    BOOST_CHECK_EQUAL(Rect(0, 0, 0, 0), common_cmd.clip);

    RDPPolygonSC cmd = state_polygonSC;
    cmd.receive(stream, header);

    deltaPoints.reset();

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(20);

    deltaPoints.out_sint16_le(160);
    deltaPoints.out_sint16_le(0);

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(-30);

    deltaPoints.out_sint16_le(50);
    deltaPoints.out_sint16_le(50);

    deltaPoints.out_sint16_le(-50);
    deltaPoints.out_sint16_le(50);

    deltaPoints.out_sint16_le(0);
    deltaPoints.out_sint16_le(-30);

    deltaPoints.out_sint16_le(-160);
    deltaPoints.out_sint16_le(0);

    deltaPoints.mark_end();
    deltaPoints.rewind();

    check<RDPPolygonSC>(common_cmd, cmd,
                        RDPOrderCommon(POLYGONSC, Rect(0, 0, 0, 0)),
                        RDPPolygonSC(158, 230, 0x0D, 0, 0x000000, 7, deltaPoints),
                        "PolygonSC 1");
}
