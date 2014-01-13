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
#define BOOST_TEST_MODULE TestNtlm
#include <boost/test/auto_unit_test.hpp>

#define LOGPRINT
#include "log.hpp"
#include "RDP/nla/ntlm/ntlm.hpp"
#include "check_sig.hpp"

BOOST_AUTO_TEST_CASE(TestAcquireCredentials)
{
    Ntlm_SecurityFunctionTable table;
    SEC_STATUS status;
    uint8_t name[] = "Ménélas";
    uint8_t dom[] = "Sparte";
    uint8_t pass[] = "Hélène";
    SEC_WINNT_AUTH_IDENTITY id;
    id.SetAuthIdentityFromUtf8(name, dom, pass);
    CredHandle credentials;
    TimeStamp expiration;

    // status = table.FreeCredentialsHandle(&credentials);
    // BOOST_CHECK_EQUAL(status, SEC_E_INVALID_HANDLE);

    // If AcquireCredential succeed, do not forget to free credential handle !
    status = table.AcquireCredentialsHandle(NULL, NTLMSP_NAME, SECPKG_CRED_OUTBOUND, NULL,
                                            &id, NULL, NULL,
                                            &credentials, &expiration);

    BOOST_CHECK_EQUAL(status, SEC_E_OK);
    CREDENTIALS * creds = (CREDENTIALS*)credentials.SecureHandleGetLowerPointer();
    BOOST_CHECK(!memcmp("\x4d\x00\xe9\x00\x6e\x00\xe9\x00\x6c\x00\x61\x00\x73\x00",
                        creds->identity.User.get_data(),
                        creds->identity.User.size()));
    BOOST_CHECK(!memcmp("\x53\x00\x70\x00\x61\x00\x72\x00\x74\x00\x65\x00",
                        creds->identity.Domain.get_data(),
                        creds->identity.Domain.size()));
    BOOST_CHECK(!memcmp("\x48\x00\xe9\x00\x6c\x00\xe8\x00\x6e\x00\x65\x00",
                        creds->identity.Password.get_data(),
                        creds->identity.Password.size()));
    status = table.FreeCredentialsHandle(&credentials);
    BOOST_CHECK_EQUAL(status, SEC_E_OK);

}

BOOST_AUTO_TEST_CASE(TestInitialize)
{
    Ntlm_SecurityFunctionTable table;
    SEC_STATUS status;
    uint8_t name[] = "Ménélas";
    uint8_t dom[] = "Sparte";
    uint8_t pass[] = "Hélène";
    SEC_WINNT_AUTH_IDENTITY id;
    id.SetAuthIdentityFromUtf8(name, dom, pass);
    CredHandle credentials;
    TimeStamp expiration;

    // status = table.FreeCredentialsHandle(&credentials);
    // BOOST_CHECK_EQUAL(status, SEC_E_INVALID_HANDLE);

    // If AcquireCredential succeed, do not forget to free credential handle !
    status = table.AcquireCredentialsHandle(NULL, NTLMSP_NAME, SECPKG_CRED_OUTBOUND, NULL,
                                            &id, NULL, NULL,
                                            &credentials, &expiration);
    BOOST_CHECK_EQUAL(status, SEC_E_OK);

    CREDENTIALS * creds = (CREDENTIALS*)credentials.SecureHandleGetLowerPointer();
    BOOST_CHECK(!memcmp("\x4d\x00\xe9\x00\x6e\x00\xe9\x00\x6c\x00\x61\x00\x73\x00",
                        creds->identity.User.get_data(),
                        creds->identity.User.size()));
    BOOST_CHECK(!memcmp("\x53\x00\x70\x00\x61\x00\x72\x00\x74\x00\x65\x00",
                        creds->identity.Domain.get_data(),
                        creds->identity.Domain.size()));
    BOOST_CHECK(!memcmp("\x48\x00\xe9\x00\x6c\x00\xe8\x00\x6e\x00\x65\x00",
                        creds->identity.Password.get_data(),
                        creds->identity.Password.size()));

    SecPkgInfo packageInfo = {};
    status = table.QuerySecurityPackageInfo(NTLMSP_NAME, &packageInfo);
    BOOST_CHECK_EQUAL(status, SEC_E_OK);

    CtxtHandle context;
    SecBuffer input_buffer;
    SecBuffer output_buffer;
    SecBufferDesc input_buffer_desc;
    SecBufferDesc output_buffer_desc;
    input_buffer.setzero();
    output_buffer.setzero();
    output_buffer_desc.ulVersion = SECBUFFER_VERSION;
    output_buffer_desc.cBuffers = 1;
    output_buffer_desc.pBuffers = &output_buffer;
    output_buffer.BufferType = SECBUFFER_TOKEN;
    output_buffer.Buffer.init(packageInfo.cbMaxToken);
    (void)input_buffer_desc;
    unsigned long pfContextAttr;
    unsigned long fContextReq = 0;
    fContextReq = ISC_REQ_MUTUAL_AUTH | ISC_REQ_CONFIDENTIALITY | ISC_REQ_USE_SESSION_KEY;

    // first call, no input buffer, no context
    status = table.InitializeSecurityContext(&credentials,
                                             NULL, // context
                                             NULL, // TargetName
                                             fContextReq, 0, SECURITY_NATIVE_DREP,
                                             NULL, // input buffer desc
                                             0, &context, // context (NTLMContext)
                                             &output_buffer_desc, // output buffer desc
                                             &pfContextAttr, &expiration);


    BOOST_CHECK_EQUAL(status, SEC_I_CONTINUE_NEEDED);

    BOOST_CHECK_EQUAL(output_buffer.Buffer.size(), 40);
    // hexdump_c(output_buffer.Buffer.get_data(), 40);







    // clear handles
    status = table.FreeContextBuffer(&context);
    BOOST_CHECK_EQUAL(status, SEC_E_OK);
    status = table.FreeCredentialsHandle(&credentials);
    BOOST_CHECK_EQUAL(status, SEC_E_OK);

}