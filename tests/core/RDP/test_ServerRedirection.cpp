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
   Copyright (C) Wallix 2015
   Author(s): Christophe Grosjean, Jonathan Poelen,
              Meng Tan, Raphael Zhou

*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestServerRedirection
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
// #define LOGPRINT
#include "RDP/ServerRedirection.hpp"

BOOST_AUTO_TEST_CASE(TestServerRedirectionPDU)
{
    RedirectionInfo rinfo_init;
    rinfo_init.valid = true;
    rinfo_init.session_id = 3333;
    memcpy(rinfo_init.host, "machine.domaine.lan", sizeof("machine.domaine.lan"));
    rinfo_init.host_is_fqdn = true;
    memcpy(rinfo_init.username, "utilisateur", sizeof("utilisateur"));
    memcpy(rinfo_init.domain, "domaine", sizeof("domaine"));
    rinfo_init.smart_card_logon = true;

    ServerRedirectionPDU srv_redir_init;
    srv_redir_init.import_from_redirection_info(rinfo_init);

    srv_redir_init.log(LOG_INFO, "test server_redirection_pdu");

    BStream buffer;
    srv_redir_init.emit(buffer);
    buffer.rewind();

    ServerRedirectionPDU srv_redir_target;
    srv_redir_target.receive(buffer);

    srv_redir_target.log(LOG_INFO, "test 2 server_redirection_pdu");

    RedirectionInfo rinfo_target;
    srv_redir_target.export_to_redirection_info(rinfo_target);
    rinfo_target.log(LOG_INFO, "test 2 redirection_info");

    BOOST_CHECK_EQUAL(rinfo_init.valid, rinfo_target.valid);
    BOOST_CHECK_EQUAL(rinfo_init.session_id, rinfo_target.session_id);
    BOOST_CHECK_EQUAL(rinfo_init.host_is_fqdn, rinfo_target.host_is_fqdn);
    BOOST_CHECK_EQUAL(rinfo_init.smart_card_logon, rinfo_target.smart_card_logon);

    BOOST_CHECK_EQUAL(0, strcmp((char*)rinfo_init.host,
                                (char*)rinfo_target.host));
    BOOST_CHECK_EQUAL(0, strcmp((char*)rinfo_init.username,
                                (char*)rinfo_target.username));
    BOOST_CHECK_EQUAL(0, strcmp((char*)rinfo_init.domain,
                                (char*)rinfo_target.domain));
}
