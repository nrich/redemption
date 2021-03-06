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
    Author(s): Christophe Grosjean, Raphael Zhou, Meng Tan
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestNtlm_context
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
#include "RDP/nla/ntlm/ntlm_context.hpp"
#include "RDP/nla/ntlm/ntlm.hpp"
#include "check_sig.hpp"

BOOST_AUTO_TEST_CASE(TestNtlmContext)
{


    NTLMContext context;
    // context.init();
    context.NTLMv2 = true;
    context.confidentiality = true;
    context.ntlm_set_negotiate_flags();
    context.set_tests();
    context.verbose = 0x400;
    // context.hardcoded_tests = true;

    // NtlmNegotiateFlags ntlm_nego_flag;

    // ntlm_nego_flag.flags = context.ConfigFlags;
    // ntlm_nego_flag.flags |= NTLMSSP_NEGOTIATE_56;  // W
    // ntlm_nego_flag.flags |= NTLMSSP_NEGOTIATE_VERSION;  // T
    // ntlm_nego_flag.flags |= NTLMSSP_NEGOTIATE_LM_KEY;  // G
    // ntlm_nego_flag.flags |= NTLMSSP_NEGOTIATE_SEAL;  // E
    // ntlm_nego_flag.flags |= NTLMSSP_NEGOTIATE_OEM;  // B

    // hexdump_c((uint8_t*)&ntlm_nego_flag.flags, 4);

    uint8_t nego_string[] =
        /* 0000 */ "\x4e\x54\x4c\x4d\x53\x53\x50\x00\x01\x00\x00\x00\xb7\x82\x08\xe2"
        /* 0010 */ "\x00\x00\x00\x00\x28\x00\x00\x00\x00\x00\x00\x00\x28\x00\x00\x00"
        /* 0020 */ "\x05\x01\x28\x0a\x00\x00\x00\x0f";

    uint8_t challenge_string[] =
        /* 0000 */ "\x4e\x54\x4c\x4d\x53\x53\x50\x00\x02\x00\x00\x00\x08\x00\x08\x00"
        /* 0010 */ "\x38\x00\x00\x00\x35\x82\x8a\xe2\x26\x6e\xcd\x75\xaa\x41\xe7\x6f"
        /* 0020 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x40\x00\x40\x00\x00\x00"
        /* 0030 */ "\x06\x01\xb0\x1d\x00\x00\x00\x0f\x57\x00\x49\x00\x4e\x00\x37\x00"
        /* 0040 */ "\x02\x00\x08\x00\x57\x00\x49\x00\x4e\x00\x37\x00\x01\x00\x08\x00"
        /* 0050 */ "\x57\x00\x49\x00\x4e\x00\x37\x00\x04\x00\x08\x00\x77\x00\x69\x00"
        /* 0060 */ "\x6e\x00\x37\x00\x03\x00\x08\x00\x77\x00\x69\x00\x6e\x00\x37\x00"
        /* 0070 */ "\x07\x00\x08\x00\xa9\x8d\x9b\x1a\x6c\xb0\xcb\x01\x00\x00\x00\x00";

    BStream s;
    s.out_copy_bytes(nego_string, sizeof(nego_string) - 1);
    s.mark_end();
    s.rewind();

    context.server = false;
    context.NEGOTIATE_MESSAGE.recv(s);

    s.reset();
    s.out_copy_bytes(challenge_string, sizeof(challenge_string) - 1);
    s.mark_end();
    s.rewind();
    context.CHALLENGE_MESSAGE.recv(s);

    const uint8_t password[] = {
        // 0x50, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        // 0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00
        0x70, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00
    };
    const uint8_t userName[] = {
        0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00,
        0x6e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00
    };
    // const uint8_t userUpper[] = {
    //     // 0x55, 0x00, 0x53, 0x00, 0x45, 0x00, 0x52, 0x00
    //     0x55, 0x00, 0x53, 0x00, 0x45, 0x00, 0x52, 0x00,
    //     0x4e, 0x00, 0x41, 0x00, 0x4d, 0x00, 0x45, 0x00
    // };
    const uint8_t userDomain[] = {
        // 0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00,
        // 0x69, 0x00, 0x6e, 0x00
        0x77, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x37, 0x00
    };



    context.ntlmv2_compute_response_from_challenge(password, sizeof(password),
                                                   userName, sizeof(userName),
                                                   userDomain, sizeof(userDomain));

    BStream & LmChallengeResponse = context.AUTHENTICATE_MESSAGE.LmChallengeResponse.Buffer;
    // LOG(LOG_INFO, "==== LmChallengeResponse =====");
    // hexdump_c(LmChallengeResponse.get_data(), LmChallengeResponse.size());
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x11\x1b\x69\x4b\xdb\x30\x53\x91\xef\x94\x8b\x20\x83\xbd\x07\x43" //..iK.0S.... ...C
/* 0010 */ "\xb8\x6c\xda\xa6\xf0\xf6\x30\x8d"                                 //.l....0.
,
                             LmChallengeResponse.get_data(),
                             LmChallengeResponse.size()),
                      0);
    BStream & NtChallengeResponse = context.AUTHENTICATE_MESSAGE.NtChallengeResponse.Buffer;
    // LOG(LOG_INFO, "==== NtChallengeResponse =====");
    // hexdump_c(NtChallengeResponse.get_data(), NtChallengeResponse.size());
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x54\x01\x2a\xc9\x4e\x20\x30\x7d\xed\x6a\xcf\xb8\x6b\xb0\x45\xc5" //T.*.N 0}.j..k.E.
/* 0010 */ "\x01\x01\x00\x00\x00\x00\x00\x00\x67\x95\x0e\x5a\x4e\x56\x76\xd6" //........g..ZNVv.
/* 0020 */ "\xb8\x6c\xda\xa6\xf0\xf6\x30\x8d\x00\x00\x00\x00\x02\x00\x08\x00" //.l....0.........
/* 0030 */ "\x57\x00\x49\x00\x4e\x00\x37\x00\x01\x00\x08\x00\x57\x00\x49\x00" //W.I.N.7.....W.I.
/* 0040 */ "\x4e\x00\x37\x00\x04\x00\x08\x00\x77\x00\x69\x00\x6e\x00\x37\x00" //N.7.....w.i.n.7.
/* 0050 */ "\x03\x00\x08\x00\x77\x00\x69\x00\x6e\x00\x37\x00\x07\x00\x08\x00" //....w.i.n.7.....
/* 0060 */ "\xa9\x8d\x9b\x1a\x6c\xb0\xcb\x01\x00\x00\x00\x00\x00\x00\x00\x00" //....l...........
,
                             NtChallengeResponse.get_data(),
                             NtChallengeResponse.size()),
                      0);

    // LOG(LOG_INFO, "==== SessionBaseKey =====");
    // hexdump_c(context.SessionBaseKey, 16);
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x1b\x76\xfd\xe3\x46\x77\x60\x04\x39\x7a\x47\x8a\x60\x92\x0c\x4c" //.v..Fw`.9zG.`..L
,
                             context.SessionBaseKey,
                             16),
                      0);

    context.ntlm_encrypt_random_session_key();

    // LOG(LOG_INFO, "==== EncryptedRandomSessionKey =====");
    // hexdump_c(context.EncryptedRandomSessionKey, 16);
    BOOST_CHECK_EQUAL(memcmp(// "\xb1\xd2\x45\x42\x0f\x37\x9a\x0e\xe0\xce\x77\x40\x10\x8a\xda\xba"
/* 0000 */ "\x5a\xcb\x6c\xba\x58\x07\xb5\xd4\xf4\x61\x65\xfb\xb0\x9a\xe7\xc6" //Z.l.X....ae.....
,
                             context.EncryptedRandomSessionKey,
                             16),
                      0);

    context.ntlm_generate_client_signing_key();
    // LOG(LOG_INFO, "==== ClientSigningKey =====");
    // hexdump_c(context.ClientSigningKey, 16);
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\xb6\x18\x61\x5b\xdb\x97\x6c\x62\xfd\xd5\x72\xab\x37\x24\xd1\x38" //..a[..lb..r.7$.8
,
                             context.ClientSigningKey,
                             16),
                      0);
    context.ntlm_generate_client_sealing_key();
    // LOG(LOG_INFO, "==== ClientSealingKey =====");
    // hexdump_c(context.ClientSealingKey, 16);
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x02\x46\xea\x18\xc8\xba\x71\xf3\xc1\x06\xb9\xf0\x54\x37\x44\x01" //.F....q.....T7D.
,
                             context.ClientSealingKey,
                             16),
                      0);
    context.ntlm_generate_server_signing_key();
    // LOG(LOG_INFO, "==== ServerSigningKey =====");
    // hexdump_c(context.ServerSigningKey, 16);
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x56\x66\xbd\xc3\x82\xda\xb7\x70\x08\x36\xb3\xed\xcd\x67\x8b\x5a" //Vf.....p.6...g.Z
,
                             context.ServerSigningKey,
                             16),
                      0);
    context.ntlm_generate_server_sealing_key();
    // LOG(LOG_INFO, "==== ServerSealingKey =====");
    // hexdump_c(context.ServerSealingKey, 16);
    BOOST_CHECK_EQUAL(memcmp(
/* 0000 */ "\x19\x3a\x3f\x24\x89\x27\xd3\x8b\x4b\xf5\x63\x2d\xa4\xc2\xb2\x78" //.:?$.'..K.c-...x
,
                             context.ServerSealingKey,
                             16),
                      0);
}
BOOST_AUTO_TEST_CASE(TestNTOWFv2)
{
    NTLMContext context;
    uint8_t buff[16];

    uint8_t password[] = "Password";
    uint8_t user[] = "User";
    uint8_t domain[] = "Domain";

    uint8_t upassword[(sizeof(password) - 1) * 2];
    uint8_t uuser[(sizeof(user) - 1) * 2];
    uint8_t udomain[(sizeof(domain) - 1) * 2];
    UTF8toUTF16(password, upassword, sizeof(upassword));
    UTF8toUTF16(user, uuser, sizeof(uuser));
    UTF8toUTF16(domain, udomain, sizeof(udomain));

    context.NTOWFv2(upassword, sizeof(upassword),
                    uuser, sizeof(uuser),
                    udomain, sizeof(udomain),
                    buff, sizeof(buff));
    BOOST_CHECK_EQUAL(memcmp("\x0c\x86\x8a\x40\x3b\xfd\x7a\x93"
                             "\xa3\x00\x1e\xf2\x2e\xf0\x2e\x3f",
                             buff,
                             16),
                      0);
}

BOOST_AUTO_TEST_CASE(TestSetters)
{
    NTLMContext context;
    // context.init();

    uint8_t work[] = "Carpe Diem";

    uint8_t spn[] = "Sustine et abstine";

    BOOST_CHECK_EQUAL(context.Workstation.size(), 0);
    context.ntlm_SetContextWorkstation(work);
    BOOST_CHECK_EQUAL(context.Workstation.size(), (sizeof(work) - 1) * 2);
    BOOST_CHECK(memcmp(work, context.Workstation.get_data(), sizeof(work)));

    BOOST_CHECK_EQUAL(context.ServicePrincipalName.size(), 0);
    context.ntlm_SetContextServicePrincipalName(spn);
    BOOST_CHECK_EQUAL(context.ServicePrincipalName.size(), (sizeof(spn) - 1) * 2);
    BOOST_CHECK(memcmp(spn, context.ServicePrincipalName.get_data(), sizeof(spn)));

}


BOOST_AUTO_TEST_CASE(TestOutputs)
{
    LOG(LOG_INFO, "SebBuffer size : %u", sizeof(SecBuffer));
    LOG(LOG_INFO, "Array size : %u", sizeof(Array));
    LOG(LOG_INFO, "size_t size : %u", sizeof(size_t));
    LOG(LOG_INFO, "unsigned long size : %u", sizeof(unsigned long));
    LOG(LOG_INFO, "unsigned int size : %u", sizeof(unsigned int));
    LOG(LOG_INFO, "int size : %u", sizeof(int));
    LOG(LOG_INFO, "uint8_t size : %u", sizeof(uint8_t));
    LOG(LOG_INFO, "uint8_t* size : %u", sizeof(uint8_t*));
    uint8_t autobuffer[AUTOSIZE];
    LOG(LOG_INFO, "autobuffer size : %u", sizeof(autobuffer));


    uint8_t head[2];
    head[0] = 1;
    head[1] = 0xFF;
    size_t length = (head[0] << 8) | head[1];
    LOG(LOG_INFO, "size length : %x", length);

//     const char ccd_r[] = {
// // Client Core Data
// "\x01\xc0\xd8\x00\x04\x00\x08\x00" //..Duca..........
// /* 0090 */ "\x00\x04\x00\x03\x01\xca\x03\xaa\x0c\x04\x00\x00\x28\x0a\x00\x00" //............(...
// /* 00a0 */ "\x6d\x00\x74\x00\x61\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //m.t.a...........
// /* 00b0 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 00c0 */ "\x04\x00\x00\x00\x00\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00" //................
// /* 00d0 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 00e0 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 00f0 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 0100 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xca\x01\x00" //................
// /* 0110 */ "\x00\x00\x00\x00\x10\x00\x07\x00\x01\x00\x00\x00\x00\x00\x00\x00" //................
// /* 0120 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 0130 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 0140 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" //................
// /* 0150 */ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00" //................
//     };


//     const char ccd_f[] = {
// // Client Core Data
// "\x01\xc0\xd8\x00\x04\x00\x08\x00\x80\x07\xdb\x03" // ca.(............
// /*0090*/ "\x01\xca\x03\xaa\x09\x04\x00\x00\xb0\x1d\x00\x00\x57\x00\x49\x00" // ............W.I.
// /*00a0*/ "\x4e\x00\x58\x00\x50\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // N.X.P...........
// /*00b0*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00" // ................
// /*00c0*/ "\x00\x00\x00\x00\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*00d0*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*00e0*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*00f0*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*0100*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x01\xca\x01\x00\x00\x00\x00\x00" // ................
// /*0110*/ "\x18\x00\x0f\x00\x2f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ..../...........
// /*0120*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*0130*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*0140*/ "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // ................
// /*0150*/ "\x00\x00\x00\x00\x00\x00\x02\x00\x02\x00\x00\x00"
//     };

    // hexdump_c(ccd_r, sizeof(ccd_r) - 1);
    // hexdump_c(ccd_f, sizeof(ccd_f) - 1);

}


BOOST_AUTO_TEST_CASE(TestNtlmScenario)
{

    NTLMContext client_context;
    NTLMContext server_context;
    client_context.verbose = 0x400;
    server_context.verbose = 0x400;
    const uint8_t password[] = {
        0x50, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00,
        0x70, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00
    };
    const uint8_t userName[] = {
        0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00,
        0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00,
        0x6e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00
    };
    // const uint8_t userUpper[] = {
    //     0x55, 0x00, 0x53, 0x00, 0x45, 0x00, 0x52, 0x00,
    //     0x55, 0x00, 0x53, 0x00, 0x45, 0x00, 0x52, 0x00,
    //     0x4e, 0x00, 0x41, 0x00, 0x4d, 0x00, 0x45, 0x00
    // };
    const uint8_t userDomain[] = {
        0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00,
        0x69, 0x00, 0x6e, 0x00,
        0x77, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x37, 0x00
    };

    const uint8_t workstation[] = {
        0x57, 0x00, 0x49, 0x00, 0x4e, 0x00, 0x58, 0x00, 0x50, 0x00
    };

    // Initialization
    BStream client_to_server;
    BStream server_to_client;

    bool result;

    // client_context.init();
    // server_context.init();

    client_context.server = false;
    server_context.server = true;


    // CLIENT BUILDS NEGOTIATE
    client_context.ntlm_set_negotiate_flags();
    client_context.NEGOTIATE_MESSAGE.negoFlags.flags = client_context.NegotiateFlags;
    if (client_context.NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
        client_context.NEGOTIATE_MESSAGE.version.ntlm_get_version_info();

    // send NEGOTIATE MESSAGE
    client_context.NEGOTIATE_MESSAGE.emit(client_to_server);
    client_to_server.rewind();
    server_context.NEGOTIATE_MESSAGE.recv(client_to_server);

    // SERVER RECV NEGOTIATE AND BUILD CHALLENGE
    result = server_context.ntlm_check_nego();
    BOOST_CHECK(result);
    server_context.ntlm_generate_server_challenge();
    memcpy(server_context.ServerChallenge, server_context.CHALLENGE_MESSAGE.serverChallenge, 8);
    server_context.ntlm_generate_timestamp();
    server_context.ntlm_construct_challenge_target_info();

    server_context.CHALLENGE_MESSAGE.negoFlags.flags = server_context.NegotiateFlags;
    if (server_context.NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
        server_context.CHALLENGE_MESSAGE.version.ntlm_get_version_info();

    // send CHALLENGE MESSAGE
    server_context.CHALLENGE_MESSAGE.emit(server_to_client);
    server_to_client.rewind();
    client_context.CHALLENGE_MESSAGE.recv(server_to_client);

    // CLIENT RECV CHALLENGE AND BUILD AUTHENTICATE

    client_context.ntlmv2_compute_response_from_challenge(password, sizeof(password),
                                                          userName, sizeof(userName),
                                                          userDomain, sizeof(userDomain));
    client_context.ntlm_encrypt_random_session_key();
    client_context.ntlm_generate_client_signing_key();
    client_context.ntlm_generate_client_sealing_key();
    client_context.ntlm_generate_server_signing_key();
    client_context.ntlm_generate_server_sealing_key();
    client_context.AUTHENTICATE_MESSAGE.negoFlags.flags = client_context.NegotiateFlags;

    uint32_t flag = client_context.AUTHENTICATE_MESSAGE.negoFlags.flags;
    if (flag & NTLMSSP_NEGOTIATE_VERSION)
        client_context.AUTHENTICATE_MESSAGE.version.ntlm_get_version_info();

    if (flag & NTLMSSP_NEGOTIATE_WORKSTATION_SUPPLIED) {
        BStream & workstationbuff = client_context.AUTHENTICATE_MESSAGE.Workstation.Buffer;
        workstationbuff.reset();
        workstationbuff.out_copy_bytes(workstation, sizeof(workstation));
        workstationbuff.mark_end();
    }

    flag |= NTLMSSP_NEGOTIATE_DOMAIN_SUPPLIED;
    BStream & domain = client_context.AUTHENTICATE_MESSAGE.DomainName.Buffer;
    domain.reset();
    domain.out_copy_bytes(userDomain, sizeof(userDomain));
    domain.mark_end();

    BStream & user = client_context.AUTHENTICATE_MESSAGE.UserName.Buffer;

    user.reset();
    user.out_copy_bytes(userName, sizeof(userName));
    user.mark_end();

    client_context.AUTHENTICATE_MESSAGE.version.ntlm_get_version_info();


    // send AUTHENTICATE MESSAGE
    client_to_server.reset();
    client_context.AUTHENTICATE_MESSAGE.emit(client_to_server);
    client_to_server.rewind();
    server_context.AUTHENTICATE_MESSAGE.recv(client_to_server);

    // SERVER PROCEED RESPONSE CHECKING
    uint8_t hash[16] = {};
    server_context.hash_password(password, sizeof(password), hash);

    result = server_context.ntlm_check_nt_response_from_authenticate(hash, 16);
    BOOST_CHECK(result);
    result = server_context.ntlm_check_lm_response_from_authenticate(hash, 16);
    BOOST_CHECK(result);
    // SERVER COMPUTE SHARED KEY WITH CLIENT
    server_context.ntlm_compute_session_base_key(hash, 16);
    server_context.ntlm_decrypt_exported_session_key();

    server_context.ntlm_generate_client_signing_key();
    server_context.ntlm_generate_client_sealing_key();
    server_context.ntlm_generate_server_signing_key();
    server_context.ntlm_generate_server_sealing_key();

    // CHECK SHARED KEY ARE EQUAL BETWEEN SERVER AND CLIENT
    // LOG(LOG_INFO, "===== SESSION BASE KEY =====");
    // hexdump_c(server_context.SessionBaseKey, 16);
    // hexdump_c(client_context.SessionBaseKey, 16);
    BOOST_CHECK(!memcmp(server_context.SessionBaseKey,
                        client_context.SessionBaseKey,
                        16));

    // LOG(LOG_INFO, "===== EXPORTED SESSION KEY =====");
    // hexdump_c(server_context.ExportedSessionKey, 16);
    // hexdump_c(client_context.ExportedSessionKey, 16);
    BOOST_CHECK(!memcmp(server_context.ExportedSessionKey,
                        client_context.ExportedSessionKey,
                        16));

    // LOG(LOG_INFO, "===== CLIENT SIGNING KEY =====");
    // hexdump_c(server_context.ClientSigningKey, 16);
    // hexdump_c(client_context.ClientSigningKey, 16);
    BOOST_CHECK(!memcmp(server_context.ClientSigningKey,
                        client_context.ClientSigningKey,
                        16));

    // LOG(LOG_INFO, "===== CLIENT SEALING KEY =====");
    // hexdump_c(server_context.ClientSealingKey, 16);
    // hexdump_c(client_context.ClientSealingKey, 16);
    BOOST_CHECK(!memcmp(server_context.ClientSealingKey,
                        client_context.ClientSealingKey,
                        16));

    // LOG(LOG_INFO, "===== SERVER SIGNING KEY =====");
    // hexdump_c(server_context.ServerSigningKey, 16);
    // hexdump_c(client_context.ServerSigningKey, 16);
    BOOST_CHECK(!memcmp(server_context.ServerSigningKey,
                        client_context.ServerSigningKey,
                        16));

    // LOG(LOG_INFO, "===== SERVER SEALING KEY =====");
    // hexdump_c(server_context.ServerSealingKey, 16);
    // hexdump_c(client_context.ServerSealingKey, 16);
    BOOST_CHECK(!memcmp(server_context.ServerSealingKey,
                        client_context.ServerSealingKey,
                        16));
}


BOOST_AUTO_TEST_CASE(TestNtlmScenario2)
{

    NTLMContext client_context;
    NTLMContext server_context;

    const uint8_t password[] = {
        0x50, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00,
        0x70, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00,
        0x77, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x64, 0x00
    };
    uint8_t userName[] = {
        0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x72, 0x00,
        0x6e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00,
        0x6e, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00
    };
    const uint8_t userDomain[] = {
        0x44, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x61, 0x00,
        0x69, 0x00, 0x6e, 0x00,
        0x77, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x37, 0x00
    };

    const uint8_t workstation[] = {
        0x57, 0x00, 0x49, 0x00, 0x4e, 0x00, 0x58, 0x00, 0x50, 0x00
    };

    // Initialization
    BStream client_to_server;
    BStream server_to_client;

    // client_context.init();
    // server_context.init();

    client_context.server = false;
    server_context.server = true;


    // CLIENT BUILDS NEGOTIATE
    client_context.ntlm_client_build_negotiate();

    // send NEGOTIATE MESSAGE
    client_context.NEGOTIATE_MESSAGE.emit(client_to_server);

    client_context.SavedNegotiateMessage.init(client_to_server.size());
    memcpy(client_context.SavedNegotiateMessage.get_data(),
           client_to_server.get_data(), client_to_server.size());
    client_to_server.rewind();

    server_context.NEGOTIATE_MESSAGE.recv(client_to_server);
    server_context.SavedNegotiateMessage.init(client_to_server.size());
    memcpy(server_context.SavedNegotiateMessage.get_data(),
           client_to_server.get_data(), client_to_server.size());
    // SERVER RECV NEGOTIATE AND BUILD CHALLENGE
    server_context.ntlm_server_build_challenge();

    // send CHALLENGE MESSAGE
    server_context.CHALLENGE_MESSAGE.emit(server_to_client);
    server_context.SavedChallengeMessage.init(server_to_client.size());
    memcpy(server_context.SavedChallengeMessage.get_data(),
           server_to_client.get_data(), server_to_client.size());

    server_to_client.rewind();
    client_context.CHALLENGE_MESSAGE.recv(server_to_client);
    client_context.SavedChallengeMessage.init(server_to_client.size());
    memcpy(client_context.SavedChallengeMessage.get_data(),
           server_to_client.get_data(), server_to_client.size());
    // CLIENT RECV CHALLENGE AND BUILD AUTHENTICATE

    client_context.ntlm_client_build_authenticate(password, sizeof(password),
                                                  userName, sizeof(userName),
                                                  userDomain, sizeof(userDomain),
                                                  workstation, sizeof(workstation));

    // send AUTHENTICATE MESSAGE
    client_to_server.reset();
    if (client_context.UseMIC) {
            client_context.AUTHENTICATE_MESSAGE.ignore_mic = true;
            client_context.AUTHENTICATE_MESSAGE.emit(client_to_server);
            client_context.AUTHENTICATE_MESSAGE.ignore_mic = false;

            client_context.SavedAuthenticateMessage.init(client_to_server.size());
            memcpy(client_context.SavedAuthenticateMessage.get_data(), client_to_server.get_data(),
                   client_to_server.size());
            client_context.ntlm_compute_MIC();
            memcpy(client_context.AUTHENTICATE_MESSAGE.MIC, client_context.MessageIntegrityCheck, 16);
    }
    client_to_server.reset();
    client_context.AUTHENTICATE_MESSAGE.emit(client_to_server);
    client_to_server.rewind();
    server_context.AUTHENTICATE_MESSAGE.recv(client_to_server);
    if (server_context.AUTHENTICATE_MESSAGE.has_mic) {
        server_context.UseMIC = true;
        memset(client_to_server.get_data() +
               server_context.AUTHENTICATE_MESSAGE.PayloadOffset, 0, 16);
        server_context.SavedAuthenticateMessage.init(client_to_server.size());
        memcpy(server_context.SavedAuthenticateMessage.get_data(),
               client_to_server.get_data(), client_to_server.size());
    }

    // SERVER PROCEED RESPONSE CHECKING
    uint8_t hash[16] = {};
    server_context.hash_password(password, sizeof(password), hash);
    server_context.ntlm_server_proceed_authenticate(hash);

    // CHECK SHARED KEY ARE EQUAL BETWEEN SERVER AND CLIENT
    // LOG(LOG_INFO, "===== SESSION BASE KEY =====");
    // hexdump_c(server_context.SessionBaseKey, 16);
    // hexdump_c(client_context.SessionBaseKey, 16);
    BOOST_CHECK(!memcmp(server_context.SessionBaseKey,
                        client_context.SessionBaseKey,
                        16));

    // LOG(LOG_INFO, "===== EXPORTED SESSION KEY =====");
    // hexdump_c(server_context.ExportedSessionKey, 16);
    // hexdump_c(client_context.ExportedSessionKey, 16);
    BOOST_CHECK(!memcmp(server_context.ExportedSessionKey,
                        client_context.ExportedSessionKey,
                        16));

    // LOG(LOG_INFO, "===== CLIENT SIGNING KEY =====");
    // hexdump_c(server_context.ClientSigningKey, 16);
    // hexdump_c(client_context.ClientSigningKey, 16);
    BOOST_CHECK(!memcmp(server_context.ClientSigningKey,
                        client_context.ClientSigningKey,
                        16));

    // LOG(LOG_INFO, "===== CLIENT SEALING KEY =====");
    // hexdump_c(server_context.ClientSealingKey, 16);
    // hexdump_c(client_context.ClientSealingKey, 16);
    BOOST_CHECK(!memcmp(server_context.ClientSealingKey,
                        client_context.ClientSealingKey,
                        16));

    // LOG(LOG_INFO, "===== SERVER SIGNING KEY =====");
    // hexdump_c(server_context.ServerSigningKey, 16);
    // hexdump_c(client_context.ServerSigningKey, 16);
    BOOST_CHECK(!memcmp(server_context.ServerSigningKey,
                        client_context.ServerSigningKey,
                        16));

    // LOG(LOG_INFO, "===== SERVER SEALING KEY =====");
    // hexdump_c(server_context.ServerSealingKey, 16);
    // hexdump_c(client_context.ServerSealingKey, 16);
    BOOST_CHECK(!memcmp(server_context.ServerSealingKey,
                        client_context.ServerSealingKey,
                        16));

    // LOG(LOG_INFO, "===== Message Integrity Check =====");
    // hexdump_c(client_context.MessageIntegrityCheck, 16);
    // hexdump_c(server_context.MessageIntegrityCheck, 16);
    BOOST_CHECK(!memcmp(client_context.MessageIntegrityCheck,
                        server_context.MessageIntegrityCheck,
                        16));

}




BOOST_AUTO_TEST_CASE(TestWrittersReaders)
{
    NTLMContext context_write;
    context_write.NegotiateFlags |= NTLMSSP_NEGOTIATE_WORKSTATION_SUPPLIED;
    context_write.NegotiateFlags |= NTLMSSP_NEGOTIATE_DOMAIN_SUPPLIED;
    NTLMContext context_read;
    context_read.verbose = 0x400;
    context_write.verbose = 0x400;
    context_read.server = true;
    SEC_STATUS status;
    SecBuffer nego;
    status = context_write.write_negotiate(&nego);
    BOOST_CHECK_EQUAL(status, SEC_I_CONTINUE_NEEDED);
    BOOST_CHECK_EQUAL(context_write.state, NTLM_STATE_CHALLENGE);
    status = context_read.read_negotiate(&nego);
    BOOST_CHECK_EQUAL(status, SEC_I_CONTINUE_NEEDED);
    BOOST_CHECK_EQUAL(context_read.state, NTLM_STATE_CHALLENGE);


    SecBuffer chal;
    status = context_write.write_challenge(&chal);
    BOOST_CHECK_EQUAL(status, SEC_I_CONTINUE_NEEDED);
    BOOST_CHECK_EQUAL(context_write.state, NTLM_STATE_AUTHENTICATE);
    status = context_read.read_challenge(&chal);
    BOOST_CHECK_EQUAL(status, SEC_I_CONTINUE_NEEDED);
    BOOST_CHECK_EQUAL(context_read.state, NTLM_STATE_AUTHENTICATE);

    SecBuffer auth;
    status = context_write.write_authenticate(&auth);
    BOOST_CHECK_EQUAL(status, SEC_I_COMPLETE_NEEDED);
    BOOST_CHECK_EQUAL(context_write.state, NTLM_STATE_FINAL);
    status = context_read.read_authenticate(&auth);
    BOOST_CHECK_EQUAL(status, SEC_E_LOGON_DENIED);
}
