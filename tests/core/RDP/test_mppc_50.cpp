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
    Copyright (C) Wallix 2013
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestMPPC50
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL

#include "RDP/mppc_50.hpp"

BOOST_AUTO_TEST_CASE(TestRDP50BlukCompression2)
{
    #include "../../fixtures/test_mppc_2.hpp"

    rdp_mppc_50_enc mppc_enc;


    BOOST_CHECK_EQUAL(sizeof(historyBuffer),     RDP_50_HIST_BUF_LEN);
    BOOST_CHECK_EQUAL(sizeof(outputBufferPlus),  RDP_50_HIST_BUF_LEN + 64);
    BOOST_CHECK_EQUAL(sizeof(hash_table),        rdp_mppc_50_enc::hash_table_manager::get_table_size());
    BOOST_CHECK_EQUAL(sizeof(uncompressed_data), 4037);
    BOOST_CHECK_EQUAL(sizeof(compressed_data),   3015);


    memcpy(mppc_enc.historyBuffer,           historyBuffer,    RDP_50_HIST_BUF_LEN);
    memcpy(mppc_enc.outputBufferPlus,        outputBufferPlus, RDP_50_HIST_BUF_LEN + 64);
    mppc_enc.historyOffset = 61499;
    mppc_enc.bytes_in_opb  = 2834;
    mppc_enc.flags         = 33;
    mppc_enc.flagsHold     = 0;
    mppc_enc.first_pkt     = false;
    memcpy(mppc_enc.hash_tab_mgr.hash_table, hash_table,       rdp_mppc_50_enc::hash_table_manager::get_table_size());

    uint8_t  compressionFlags;
    uint16_t datalen;

    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    BOOST_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));
    BOOST_CHECK_EQUAL(3015,  datalen);
    BOOST_CHECK_EQUAL(0,     memcmp( compressed_data, mppc_enc.outputBuffer
                                   , mppc_enc.bytes_in_opb));
}

BOOST_AUTO_TEST_CASE(TestRDP50BlukCompression3)
{
    #include "../../fixtures/test_mppc_3.hpp"

    rdp_mppc_50_enc mppc_enc;


    BOOST_CHECK_EQUAL(sizeof(historyBuffer),     RDP_50_HIST_BUF_LEN);
    BOOST_CHECK_EQUAL(sizeof(outputBufferPlus),  RDP_50_HIST_BUF_LEN + 64);
    BOOST_CHECK_EQUAL(sizeof(hash_table),        rdp_mppc_50_enc::hash_table_manager::get_table_size());
    BOOST_CHECK_EQUAL(sizeof(uncompressed_data), 12851);
    BOOST_CHECK_EQUAL(sizeof(compressed_data),   8893);


    memcpy(mppc_enc.historyBuffer,           historyBuffer,    RDP_50_HIST_BUF_LEN);
    memcpy(mppc_enc.outputBufferPlus,        outputBufferPlus, RDP_50_HIST_BUF_LEN + 64);
    mppc_enc.historyOffset = 0;
    mppc_enc.bytes_in_opb  = 0;
    mppc_enc.flags         = 0;
    mppc_enc.flagsHold     = 0;
    mppc_enc.first_pkt     = 1;
    memcpy(mppc_enc.hash_tab_mgr.hash_table, hash_table,       rdp_mppc_50_enc::hash_table_manager::get_table_size());

    uint8_t  compressionFlags;
    uint16_t datalen;

    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    BOOST_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));
    BOOST_CHECK_EQUAL(8893,  datalen);
    BOOST_CHECK_EQUAL(0,     memcmp( compressed_data, mppc_enc.outputBuffer
                                   , mppc_enc.bytes_in_opb));
}

BOOST_AUTO_TEST_CASE(TestRDP50BlukDecompression5)
{
    #include "../../fixtures/test_mppc_5.hpp"

    rdp_mppc_50_dec mppc_dec;


    BOOST_CHECK_EQUAL(sizeof(compressed_data),   2053);
    BOOST_CHECK_EQUAL(sizeof(uncompressed_data), 3790);


    memcpy(mppc_dec.history_buf, historyBuffer, RDP_50_HIST_BUF_LEN);
    mppc_dec.history_buf_end = mppc_dec.history_buf + 65535;
    mppc_dec.history_ptr     = mppc_dec.history_buf + 54626;

    uint8_t  compressionFlags = 0x21;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(compressed_data, sizeof(compressed_data), compressionFlags, rdata, rlen);

    BOOST_CHECK_EQUAL(3790, rlen);
    BOOST_CHECK_EQUAL(0,   memcmp(uncompressed_data, rdata, rlen));
}
