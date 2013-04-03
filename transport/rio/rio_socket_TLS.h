/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARIO *ICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2013
   Author(s): Christophe Grosjean

   new Socket RedTransport class

*/

#ifndef _REDEMPTION_LIBS_RIO_SOCKET_TLS_H_
#define _REDEMPTION_LIBS_RIO_SOCKET_TLS_H_

#include "rio.h"
#include "netutils.hpp"
#include </usr/include/openssl/ssl.h>
#include </usr/include/openssl/err.h>

extern "C" {

    struct RIOSocketTLS {
        int sck;
        bool tls;
        SSL * ssl;
    };

    /* This method does not allocate space for object itself, 
        but initialize it's properties
        and allocate and initialize it's subfields if necessary
    */
    static inline RIO_ERROR rio_m_RIOSocketTLS_constructor(RIOSocketTLS * self, int sck)
    {
        self->tls = false;
        self->ssl = NULL;
        self->sck = sck;
        return RIO_ERROR_OK;
    }

    /* This method deallocate any space used for subfields if any
    */
    static inline RIO_ERROR rio_m_RIOSocketTLS_destructor(RIOSocketTLS * self)
    {
        free(self->ssl);
        self->ssl = 0;
        return RIO_ERROR_CLOSED;
    }

    static inline size_t rio_m_RIOSocketTLS_recv_tls(RIOSocketTLS * self, void * data, size_t len)
    {
        char * pbuffer = (char*)data;
        size_t remaining_len = len;
        while (remaining_len > 0) {
            ssize_t rcvd = ::SSL_read(self->ssl, pbuffer, remaining_len);
            unsigned long error = SSL_get_error(self->ssl, rcvd);
            switch (error) {
                case SSL_ERROR_NONE:
                    pbuffer += rcvd;
                    remaining_len -= rcvd;
                    break;

                case SSL_ERROR_WANT_READ:
                    LOG(LOG_INFO, "recv_tls WANT READ");
                    continue;

                case SSL_ERROR_WANT_WRITE:
                    LOG(LOG_INFO, "recv_tls WANT WRITE");
                    continue;

                case SSL_ERROR_WANT_CONNECT:
                    LOG(LOG_INFO, "recv_tls WANT CONNECT");
                    continue;

                case SSL_ERROR_WANT_ACCEPT:
                    LOG(LOG_INFO, "recv_tls WANT ACCEPT");
                    continue;

                case SSL_ERROR_WANT_X509_LOOKUP:
                    LOG(LOG_INFO, "recv_tls WANT X509 LOOKUP");
                    continue;

                case SSL_ERROR_ZERO_RETURN:
                    return remaining_len - len;
                default:
                {
                    uint32_t errcount = 0;
                    errcount++;
                    LOG(LOG_INFO, "%s", ERR_error_string(error, NULL));
                    while ((error = ERR_get_error()) != 0){
                        errcount++;
                        LOG(LOG_INFO, "%s", ERR_error_string(error, NULL));
                    }
                    TODO("if recv fail with partial read we should return the amount of data received, "
                         "close socket and store some delayed error value that will be sent back next call")
                    TODO("replace this with actual error management, EOF is not even an option for sockets")
                    TODO("Manage actual errors, check possible values")
                    rio_m_RIOSocketTLS_destructor(self);
                    return (RIO_ERROR)-RIO_ERROR_ANY;
                }
                break;
            }
        }
        return len;
    }


    /* This method receive len bytes of data into buffer
       target buffer *MUST* be large enough to contains len data
       returns len actually received (may be 0),
       or negative value to signal some error.
       If an error occurs after reading some data, the return buffer
       has been changed but an error is returned anyway
       and an error returned on subsequent call.
    */
    static inline ssize_t rio_m_RIOSocketTLS_recv(RIOSocketTLS * self, void * data, size_t len)
    {
        if (self->tls) {
            return rio_m_RIOSocketTLS_recv_tls(self, data, len);
        }
        char * pbuffer = (char*)data;
        size_t remaining_len = len;

        while (remaining_len > 0) {
            ssize_t res = ::recv(self->sck, pbuffer, remaining_len, 0);
            switch (res) {
                case -1: /* error, maybe EAGAIN */
                    if (try_again(errno)) {
                        fd_set fds;
                        struct timeval time = { 0, 100000 };
                        FD_ZERO(&fds);
                        FD_SET(self->sck, &fds);
                        select(self->sck + 1, &fds, NULL, NULL, &time);
                        continue;
                    }
                    TODO("if recv fail with partial read we should return the amount of data received, "
                         "close socket and store some delayed error value that will be sent back next call")
                    TODO("replace this with actual error management, EOF is not even an option for sockets")
                    rio_m_RIOSocketTLS_destructor(self);
                    return -RIO_ERROR_EOF;
                case 0: /* no data received, socket closed */
                    rio_m_RIOSocketTLS_destructor(self);
                    return -RIO_ERROR_EOF;
                default: /* some data received */
                    pbuffer += res;
                    remaining_len -= res;
                break;
            }
        }
        return len;
    }

    static inline ssize_t rio_m_RIOSocketTLS_send_tls(RIOSocketTLS * self, const void * data, size_t len)
    {
        const char * const buffer = (const char * const)data;
        size_t remaining_len = len;
        size_t offset = 0;
        while (remaining_len > 0){
            int ret = SSL_write(self->ssl, buffer + offset, remaining_len);

            unsigned long error = SSL_get_error(self->ssl, ret);
            switch (error)
            {
                case SSL_ERROR_NONE:
                    remaining_len -= ret;
                    offset += ret;
                    break;

                case SSL_ERROR_WANT_READ:
                    LOG(LOG_INFO, "send_tls WANT READ");
                    continue;

                case SSL_ERROR_WANT_WRITE:
                    LOG(LOG_INFO, "send_tls WANT WRITE");
                    continue;

                default:
                {
                    LOG(LOG_INFO, "Failure in SSL library");
                    uint32_t errcount = 0;
                    errcount++;
                    LOG(LOG_INFO, "%s", ERR_error_string(error, NULL));
                    while ((error = ERR_get_error()) != 0){
                        errcount++;
                        LOG(LOG_INFO, "%s", ERR_error_string(error, NULL));
                    }
                    rio_m_RIOSocketTLS_destructor(self);
                    return (RIO_ERROR)-RIO_ERROR_ANY;
                }
            }
        }
        return len;
    }

    /* This method send len bytes of data from buffer to current transport
       buffer must actually contains the amount of data requested to send.
       returns len actually sent (may be 0),
       or negative value to signal some error.
       If an error occurs after sending some data the amount sent will be returned
       and an error returned on subsequent call.
    */
    static inline ssize_t rio_m_RIOSocketTLS_send(RIOSocketTLS * self, const void * data, size_t len)
    {
        if (self->tls){
            return rio_m_RIOSocketTLS_send_tls(self, data, len);
        }
        size_t total = 0;
        while (total < len) {
            ssize_t sent = ::send(self->sck, &(((uint8_t*)data)[total]), len - total, 0);
            switch (sent){
            case -1:
                if (try_again(errno)) {
                    fd_set wfds;
                    struct timeval time = { 0, 10000 };
                    FD_ZERO(&wfds);
                    FD_SET(self->sck, &wfds);
                    select(self->sck + 1, NULL, &wfds, NULL, &time);
                    continue;
                }
                rio_m_RIOSocketTLS_destructor(self);
                return -RIO_ERROR_EOF;
            case 0:
                rio_m_RIOSocketTLS_destructor(self);
                return -RIO_ERROR_EOF;
            default:
                total = total + sent;
            }
        }
        return len;
    }


    // X509_NAME_print_ex() prints a human readable version of nm to BIO out. 
    // Each line (for multiline formats) is indented by indent spaces.
    // The output format can be extensively customised by use of the flags parameter.

    static inline char* crypto_print_name(X509_NAME* name)
    {
        char* buffer = NULL;
        BIO* outBIO = BIO_new(BIO_s_mem());
    
        if (X509_NAME_print_ex(outBIO, name, 0, XN_FLAG_ONELINE) > 0)
        {
            unsigned long size = BIO_number_written(outBIO);
            buffer = (char*)malloc(size + 1);
            memset(buffer, 0, size + 1);
            BIO_read(outBIO, buffer, size);
        }
        BIO_free(outBIO);
        return buffer;
    }

    static inline char* crypto_cert_fingerprint(X509* xcert)
    {
        uint32_t fp_len;
        uint8_t fp[EVP_MAX_MD_SIZE];

        X509_digest(xcert, EVP_sha1(), fp, &fp_len);

        char * fp_buffer = (char*) malloc(3 * fp_len);
        memset(fp_buffer, 0, 3 * fp_len);

        char * p = fp_buffer;

        int i = 0;
        for (i = 0; i < (int) (fp_len - 1); i++)
        {
            sprintf(p, "%02x:", fp[i]);
            p = &fp_buffer[(i + 1) * 3];
        }
        sprintf(p, "%02x", fp[i]);

        return fp_buffer;
    }


    static inline RIO_ERROR rio_m_RIOSocketTLS_enable_TLS_client(RIOSocketTLS * self)
    {
        LOG(LOG_INFO, "RIO *::enable_tls()");

        // -------- Start of system wide SSL_Ctx option ------------------------------

        
        // ERR_load_crypto_strings() registers the error strings for all libcrypto 
        // functions. SSL_load_error_strings() does the same, but also registers the
        // libssl error strings.

        // One of these functions should be called before generating textual error 
        // messages. However, this is not required when memory usage is an issue.

        // ERR_free_strings() frees all previously loaded error strings. 
        
        SSL_load_error_strings();

        // SSL_library_init() registers the available SSL/TLS ciphers and digests.
        // OpenSSL_add_ssl_algorithms() and SSLeay_add_ssl_algorithms() are synonyms 
        // for SSL_library_init(). 
        
        // - SSL_library_init() must be called before any other action takes place. 
        // - SSL_library_init() is not reentrant. 
        // - SSL_library_init() always returns "1", so it is safe to discard the return
        // value.
        
        // Note: OpenSSL 0.9.8o and 1.0.0a and later added SHA2 algorithms to 
        // SSL_library_init(). Applications which need to use SHA2 in earlier versions
        // of OpenSSL should call OpenSSL_add_all_algorithms() as well. 
        
        SSL_library_init();

        // SSL_CTX_new - create a new SSL_CTX object as framework for TLS/SSL enabled functions
        // ------------------------------------------------------------------------------------
        
        // SSL_CTX_new() creates a new SSL_CTX object as framework to establish TLS/SSL enabled
        // connections. 
        
        // The SSL_CTX data structure is the global context structure which is created by a server
        // or client *once* per program life-time and which holds mainly default values for the SSL
        // structures which are later created for the connections.
        
        // Various options regarding certificates, algorithms, etc. can be set in this object.

        // SSL_CTX_new() receive a data structure of type SSL_METHOD (SSL Method), which is 
        // a dispatch structure describing the internal ssl library methods/functions which 
        // implement the various protocol versions (SSLv1, SSLv2 and TLSv1). An SSL_METHOD
        // is necessary to create an SSL_CTX.

        // The SSL_CTX object uses method as connection method. The methods exist in a generic
        // type (for client and server use), a server only type, and a client only type. method
        // can be of several types (server, client, generic, support SSLv2, TLSv1, TLSv1.1, etc.)
        
        // TLSv1_client_method(void): A TLS/SSL connection established with this methods will 
        // only understand the TLSv1 protocol. A client will send out TLSv1 client hello messages
        // and will indicate that it only understands TLSv1.
        
        SSL_CTX* ctx = SSL_CTX_new(TLSv1_client_method());

        /*
         * This is necessary, because the Microsoft TLS implementation is not perfect.
         * SSL_OP_ALL enables a couple of workarounds for buggy TLS implementations,
         * but the most important workaround being SSL_OP_TLS_BLOCK_PADDING_BUG.
         * As the size of the encrypted payload may give hints about its contents,
         * block padding is normally used, but the Microsoft TLS implementation
         * won't recognize it and will disconnect you after sending a TLS alert.
         */
         
        // SSL_CTX_set_options() adds the options set via bitmask in options to ctx. 
        // Options already set before are not cleared! 
        
         // During a handshake, the option settings of the SSL object are used. When
         // a new SSL object is created from a context using SSL_new(), the current 
         // option setting is copied. Changes to ctx do not affect already created 
         // SSL objects. SSL_clear() does not affect the settings.

         // The following bug workaround options are available:

         // SSL_OP_MICROSOFT_SESS_ID_BUG

         // www.microsoft.com - when talking SSLv2, if session-id reuse is performed,
         // the session-id passed back in the server-finished message is different 
         // from the one decided upon.
         
         // SSL_OP_NETSCAPE_CHALLENGE_BUG

         // Netscape-Commerce/1.12, when talking SSLv2, accepts a 32 byte challenge 
         // but then appears to only use 16 bytes when generating the encryption keys. 
         // Using 16 bytes is ok but it should be ok to use 32. According to the SSLv3
         // spec, one should use 32 bytes for the challenge when operating in SSLv2/v3
         // compatibility mode, but as mentioned above, this breaks this server so 
         // 16 bytes is the way to go.

         // SSL_OP_NETSCAPE_REUSE_CIPHER_CHANGE_BUG

         // As of OpenSSL 0.9.8q and 1.0.0c, this option has no effect.
        
        // SSL_OP_SSLREF2_REUSE_CERT_TYPE_BUG

        //  ...

        // SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER

        // ...

        // SSL_OP_MSIE_SSLV2_RSA_PADDING

        // As of OpenSSL 0.9.7h and 0.9.8a, this option has no effect.

        // SSL_OP_SSLEAY_080_CLIENT_DH_BUG
        // ...

        // SSL_OP_TLS_D5_BUG
        //    ...

        // SSL_OP_TLS_BLOCK_PADDING_BUG
        //   ...

        // SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS

        // Disables a countermeasure against a SSL 3.0/TLS 1.0 protocol vulnerability
        // affecting CBC ciphers, which cannot be handled by some broken SSL implementations.
        // This option has no effect for connections using other ciphers.

        // SSL_OP_ALL
        // All of the above bug workarounds.

        // It is usually safe to use SSL_OP_ALL to enable the bug workaround options if
        // compatibility with somewhat broken implementations is desired.

        // The following modifying options are available:

        // SSL_OP_TLS_ROLLBACK_BUG

        // Disable version rollback attack detection.

        // During the client key exchange, the client must send the same information about
        // acceptable SSL/TLS protocol levels as during the first hello. Some clients violate
        // this rule by adapting to the server's answer. (Example: the client sends a SSLv2
        // hello and accepts up to SSLv3.1=TLSv1, the server only understands up to SSLv3.
        // In this case the client must still use the same SSLv3.1=TLSv1 announcement. Some
        // clients step down to SSLv3 with respect to the server's answer and violate the 
        // version rollback protection.)

        // SSL_OP_SINGLE_DH_USE

        // Always create a new key when using temporary/ephemeral DH parameters (see
        // SSL_CTX_set_tmp_dh_callback(3)). This option must be used to prevent small subgroup
        // attacks, when the DH parameters were not generated using ``strong'' primes (e.g. 
        // when using DSA-parameters, see dhparam(1)). If ``strong'' primes were used, it is
        // not strictly necessary to generate a new DH key during each handshake but it is
        // also recommended. SSL_OP_SINGLE_DH_USE should therefore be enabled whenever 
        // temporary/ephemeral DH parameters are used.
        
        // SSL_OP_EPHEMERAL_RSA

        // Always use ephemeral (temporary) RSA key when doing RSA operations (see 
        // SSL_CTX_set_tmp_rsa_callback(3)). According to the specifications this is only done,
        // when a RSA key can only be used for signature operations (namely under export ciphers
        // with restricted RSA keylength). By setting this option, ephemeral RSA keys are always
        // used. This option breaks compatibility with the SSL/TLS specifications and may lead
        // to interoperability problems with clients and should therefore never be used. Ciphers
        // with EDH (ephemeral Diffie-Hellman) key exchange should be used instead.

        // SSL_OP_CIPHER_SERVER_PREFERENCE

        // When choosing a cipher, use the server's preferences instead of the client preferences.
        // When not set, the SSL server will always follow the clients preferences. When set, the
        // SSLv3/TLSv1 server will choose following its own preferences. Because of the different
        // protocol, for SSLv2 the server will send its list of preferences to the client and the
        // client chooses.

        // SSL_OP_PKCS1_CHECK_1
        //  ...

        // SSL_OP_PKCS1_CHECK_2
        //  ...

        // SSL_OP_NETSCAPE_CA_DN_BUG
        // If we accept a netscape connection, demand a client cert, have a non-self-signed CA
        // which does not have its CA in netscape, and the browser has a cert, it will crash/hang.
        // Works for 3.x and 4.xbeta

        // SSL_OP_NETSCAPE_DEMO_CIPHER_CHANGE_BUG
        //    ...

        // SSL_OP_NO_SSLv2
        // Do not use the SSLv2 protocol.

        // SSL_OP_NO_SSLv3
        // Do not use the SSLv3 protocol.

        // SSL_OP_NO_TLSv1

        // Do not use the TLSv1 protocol.
        // SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION

        // When performing renegotiation as a server, always start a new session (i.e., session
        // resumption requests are only accepted in the initial handshake). This option is not
        // needed for clients.

        // SSL_OP_NO_TICKET
        // Normally clients and servers will, where possible, transparently make use of RFC4507bis
        // tickets for stateless session resumption.

        // If this option is set this functionality is disabled and tickets will not be used by
        // clients or servers.

        // SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION

        // Allow legacy insecure renegotiation between OpenSSL and unpatched clients or servers.
        // See the SECURE RENEGOTIATION section for more details.

        // SSL_OP_LEGACY_SERVER_CONNECT
        // Allow legacy insecure renegotiation between OpenSSL and unpatched servers only: this option
        // is currently set by default. See the SECURE RENEGOTIATION section for more details.
        
        LOG(LOG_INFO, "RIO *::SSL_CTX_set_options()");
        SSL_CTX_set_options(ctx, SSL_OP_ALL);
        
        // -------- End of system wide SSL_Ctx option ----------------------------------
        
        // --------Start of session specific init code ---------------------------------
        
        // SSL_new() creates a new SSL structure which is needed to hold the data for a TLS/SSL 
        // connection. The new structure inherits the settings of the underlying context ctx:
        // - connection method (SSLv2/v3/TLSv1),
        // - options, 
        // - verification settings, 
        // - timeout settings. 

        // return value: NULL: The creation of a new SSL structure failed. Check the error stack 
        // to find out the reason.
        TODO("add error management")
        self->ssl = SSL_new(ctx);

        TODO("I should probably not be doing that here ? Is it really necessary")
        int flags = fcntl(self->sck, F_GETFL);
        fcntl(self->sck, F_SETFL, flags & ~(O_NONBLOCK));

        // SSL_set_fd - connect the SSL object with a file descriptor
        // ==========================================================
        
        // SSL_set_fd() sets the file descriptor fd as the input/output facility for the TLS/SSL (encrypted)
        // side of ssl. fd will typically be the socket file descriptor of a network connection.

        // When performing the operation, a socket BIO is automatically created to interface between the ssl
        // and fd. The BIO and hence the SSL engine inherit the behaviour of fd. If fd is non-blocking, the ssl
        // will also have non-blocking behaviour.

        // If there was already a BIO connected to ssl, BIO_free() will be called (for both the reading and 
        // writing side, if different).

        // SSL_set_rfd() and SSL_set_wfd() perform the respective action, but only for the read channel or the
        //  write channel, which can be set independently. 

        // The following return values can occur:
        // 0 : The operation failed. Check the error stack to find out why.
        // 1 : The operation succeeded.

        TODO("add error maangement")
        SSL_set_fd(self->ssl, self->sck);
        
        LOG(LOG_INFO, "RIO *::SSL_connect()");
    again:
        // SSL_connect - initiate the TLS/SSL handshake with an TLS/SSL server 
        // -------------------------------------------------------------------
        
        // SSL_connect() initiates the TLS/SSL handshake with a server. The 
        // communication channel must already have been set and assigned to the
        // ssl by setting an underlying BIO. 
        
        // The behaviour of SSL_connect() depends on the underlying BIO.

        // If the underlying BIO is blocking, SSL_connect() will only return once
        // the handshake has been finished or an error occurred.

        // If the underlying BIO is non-blocking, SSL_connect() will also return 
        // when the underlying BIO could not satisfy the needs of SSL_connect()
        // to continue the handshake, indicating the problem by the return value -1.
        // In this case a call to SSL_get_error() with the return value of SSL_connect()
        // will yield SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE. The calling process
        // then must repeat the call after taking appropriate action to satisfy the needs
        // of SSL_connect(). The action depends on the underlying BIO. When using a 
        // non-blocking socket, nothing is to be done, but select() can be used to check
        // for the required condition. When using a buffering BIO, like a BIO pair,
        // data must be written into or retrieved out of the BIO before being able to
        // continue. 
        
        // RETURN VALUES, The following return values can occur:
        // - 0 : The TLS/SSL handshake was not successful but was shut down controlled
        // and by the specifications of the TLS/SSL protocol. Call SSL_get_error()
        // with the return value ret to find out the reason.
        // - 1 : The TLS/SSL handshake was successfully completed, a TLS/SSL connection
        // has been established.
        // - <0 : The TLS/SSL handshake was not successful, because a fatal error occurred
        // either at the protocol level or a connection failure occurred. The shutdown
        // was not clean. It can also occur if action is need to continue the operation
        // for non-blocking BIOs. Call SSL_get_error() with the return value ret to find
        // out the reason
        
        int connection_status = SSL_connect(self->ssl);

        if (connection_status <= 0)
        {
            unsigned long error;

            switch (SSL_get_error(self->ssl, connection_status))
            {
                case SSL_ERROR_ZERO_RETURN:
                    LOG(LOG_INFO, "Server closed TLS connection\n");
                    LOG(LOG_INFO, "tls::tls_print_error SSL_ERROR_ZERO_RETURN done\n");
                    rio_m_RIOSocketTLS_destructor(self);
                    return RIO_ERROR_TLS_CONNECT_FAILED;

                case SSL_ERROR_WANT_READ:
                    LOG(LOG_INFO, "SSL_ERROR_WANT_READ\n");
                    LOG(LOG_INFO, "tls::tls_print_error SSL_ERROR_WANT_READ done\n");
                    goto again;

                case SSL_ERROR_WANT_WRITE:
                    LOG(LOG_INFO, "SSL_ERROR_WANT_WRITE\n");
                    LOG(LOG_INFO, "tls::tls_print_error SSL_ERROR_WANT_WRITE done\n");
                    goto again;

                case SSL_ERROR_SYSCALL:
                    LOG(LOG_INFO, "I/O error\n");
                    while ((error = ERR_get_error()) != 0)
                        LOG(LOG_INFO, "%s\n", ERR_error_string(error, NULL));
                    LOG(LOG_INFO, "tls::tls_print_error SSL_ERROR_SYSCLASS done\n");
                    rio_m_RIOSocketTLS_destructor(self);
                    return RIO_ERROR_TLS_CONNECT_FAILED;

                case SSL_ERROR_SSL:
                    LOG(LOG_INFO, "Failure in SSL library (protocol error?)\n");
                    while ((error = ERR_get_error()) != 0)
                        LOG(LOG_INFO, "%s\n", ERR_error_string(error, NULL));
                    LOG(LOG_INFO, "tls::tls_print_error SSL_ERROR_SSL done\n");
                    rio_m_RIOSocketTLS_destructor(self);
                    return RIO_ERROR_TLS_CONNECT_FAILED;

                default:
                    LOG(LOG_INFO, "Unknown error\n");
                    while ((error = ERR_get_error()) != 0){
                        LOG(LOG_INFO, "%s\n", ERR_error_string(error, NULL));
                    }
                    LOG(LOG_INFO, "tls::tls_print_error %s [%u]", strerror(errno), errno);
                    LOG(LOG_INFO, "tls::tls_print_error Unknown error done\n");
                    rio_m_RIOSocketTLS_destructor(self);
                    return RIO_ERROR_TLS_CONNECT_FAILED;
            }
        }

        LOG(LOG_INFO, "RIO *::SSL_get_peer_certificate()");
        
        // SSL_get_peer_certificate - get the X509 certificate of the peer
        // ---------------------------------------------------------------
        
        // SSL_get_peer_certificate() returns a pointer to the X509 certificate
        // the peer presented. If the peer did not present a certificate, NULL
        // is returned. 
        
        // Due to the protocol definition, a TLS/SSL server will always send a
        // certificate, if present. A client will only send a certificate when
        // explicitly requested to do so by the server (see SSL_CTX_set_verify(3)).
        // If an anonymous cipher is used, no certificates are sent.

        // That a certificate is returned does not indicate information about the
        // verification state, use SSL_get_verify_result(3) to check the verification
        // state.

        // The reference count of the X509 object is incremented by one, so that 
        // it will not be destroyed when the session containing the peer certificate
        // is freed. The X509 object must be explicitly freed using X509_free().
        
        // RETURN VALUES The following return values can occur:

        // NULL : no certificate was presented by the peer or no connection was established.
        // Pointer to an X509 certificate : the return value points to the certificate 
        // presented by the peer.
        
        X509 * px509 = SSL_get_peer_certificate(self->ssl);
        if (!px509) {
            LOG(LOG_INFO, "RIO *::crypto_cert_get_public_key: SSL_get_peer_certificate() failed");
            rio_m_RIOSocketTLS_destructor(self);
            return RIO_ERROR_TLS_CONNECT_FAILED;
        }
        
        // SSL_get_verify_result - get result of peer certificate verification 
        // -------------------------------------------------------------------
        // SSL_get_verify_result() returns the result of the verification of the X509 certificate
        // presented by the peer, if any. 
        
        // SSL_get_verify_result() can only return one error code while the verification of 
        // a certificate can fail because of many reasons at the same time. Only the last 
        // verification error that occurred during the processing is available from 
        // SSL_get_verify_result().

        // The verification result is part of the established session and is restored when
        // a session is reused. 

        // bug: If no peer certificate was presented, the returned result code is X509_V_OK.
        // This is because no verification error occurred, it does however not indicate
        // success. SSL_get_verify_result() is only useful in connection 
        // with SSL_get_peer_certificate(3). 

        // RETURN VALUES The following return values can currently occur:

        // X509_V_OK: The verification succeeded or no peer certificate was presented.
        // Any other value: Documented in verify(1).


        //  A X.509 certificate is a structured grouping of information about an individual, 
        // a device, or anything one can imagine. A X.509 CRL (certificate revocation list) 
        // is a tool to help determine if a certificate is still valid. The exact definition
        // of those can be found in the X.509 document from ITU-T, or in RFC3280 from PKIX.
        // In OpenSSL, the type X509 is used to express such a certificate, and the type 
        // X509_CRL is used to express a CRL.

        // A related structure is a certificate request, defined in PKCS#10 from RSA Security,
        // Inc, also reflected in RFC2896. In OpenSSL, the type X509_REQ is used to express
        // such a certificate request.

        // To handle some complex parts of a certificate, there are the types X509_NAME 
        // (to express a certificate name), X509_ATTRIBUTE (to express a certificate attributes),
        // X509_EXTENSION (to express a certificate extension) and a few more.

        // Finally, there's the supertype X509_INFO, which can contain a CRL, a certificate
        // and a corresponding private key. 


        LOG(LOG_INFO, "RIO *::X509_get_pubkey()");
        // extract the public key
        EVP_PKEY* pkey = X509_get_pubkey(px509);
        if (!pkey)
        {
            LOG(LOG_INFO, "RIO *::crypto_cert_get_public_key: X509_get_pubkey() failed");
            rio_m_RIOSocketTLS_destructor(self);
            return RIO_ERROR_TLS_CONNECT_FAILED;
        }

        LOG(LOG_INFO, "RIO *::i2d_PublicKey()");
        
        // i2d_X509() encodes the structure pointed to by x into DER format. 
        // If out is not NULL is writes the DER encoded data to the buffer at *out,
        // and increments it to point after the data just written. 
        // If the return value is negative an error occurred, otherwise it returns
        // the length of the encoded data. 
        
        // export the public key to DER format
        int public_key_length = i2d_PublicKey(pkey, NULL);
        uint8_t * public_key_data = (uint8_t *)malloc(public_key_length);
        LOG(LOG_INFO, "RIO *::i2d_PublicKey()");
        uint8_t * tmp = public_key_data;
        i2d_PublicKey(pkey, &tmp);

        ;
        X509* xcert = px509;
            X509_STORE* cert_ctx = X509_STORE_new();
        
            // OpenSSL_add_all_algorithms(3SSL)
            // --------------------------------

            // OpenSSL keeps an internal table of digest algorithms and ciphers. It uses this table 
            // to lookup ciphers via functions such as EVP_get_cipher_byname().

           // OpenSSL_add_all_digests() adds all digest algorithms to the table.

           // OpenSSL_add_all_algorithms() adds all algorithms to the table (digests and ciphers).

           // OpenSSL_add_all_ciphers() adds all encryption algorithms to the table including password
           // based encryption algorithms.

           // EVP_cleanup() removes all ciphers and digests from the table.

            OpenSSL_add_all_algorithms();
            
            X509_LOOKUP* lookup = X509_STORE_add_lookup(cert_ctx, X509_LOOKUP_file());
                         lookup = X509_STORE_add_lookup(cert_ctx, X509_LOOKUP_hash_dir());

                                  X509_LOOKUP_add_dir(lookup, NULL, X509_FILETYPE_DEFAULT);
//                                  X509_LOOKUP_add_dir(lookup, certificate_store_path, X509_FILETYPE_ASN1);

                X509_STORE_CTX* csc = X509_STORE_CTX_new();
                                          X509_STORE_set_flags(cert_ctx, 0);
                                          X509_STORE_CTX_init(csc, cert_ctx, xcert, 0);
                                          X509_verify_cert(csc);
                                      X509_STORE_CTX_free(csc);
                              
        X509_STORE_free(cert_ctx);

//        int index = X509_NAME_get_index_by_NID(subject_name, NID_commonName, -1);
//        X509_NAME_ENTRY *entry = X509_NAME_get_entry(subject_name, index);
//        ASN1_STRING * entry_data = X509_NAME_ENTRY_get_data(entry);
//        void * subject_alt_names = X509_get_ext_d2i(xcert, NID_subject_alt_name, 0, 0);

       X509_NAME * issuer_name = X509_get_issuer_name(xcert);
       char * issuer = crypto_print_name(issuer_name);
       LOG(LOG_INFO, "TLS::X509::issuer=%s", issuer);

       X509_NAME * subject_name = X509_get_subject_name(xcert);
       char * subject = crypto_print_name(subject_name);
       LOG(LOG_INFO, "TLS::X509::subject=%s", subject);

       char * fingerprint = crypto_cert_fingerprint(xcert);
       LOG(LOG_INFO, "TLS::X509::fingerprint=%s", fingerprint);

       X509_free(px509);

       self->tls = true;
       LOG(LOG_INFO, "RIO *::enable_tls() done");
       return RIO_ERROR_OK;
    }
    
    static inline RIO_ERROR rio_m_RIOSocketTLS_get_status(RIOSocketTLS * self)
    {
        TODO("when we will keep error value needed for recv we should return the stored error status")
        return RIO_ERROR_OK;
    }

};

#endif
