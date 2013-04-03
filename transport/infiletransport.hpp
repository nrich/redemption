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
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean

   Transport layer abstraction
*/

#ifndef _REDEMPTION_TRANSPORT_INFILETRANSPORT_HPP_
#define _REDEMPTION_TRANSPORT_INFILETRANSPORT_HPP_

#include "transport.hpp"
#include "rio/rio.h"

class InFileTransport : public Transport {
    public:
    RIO rio;
    uint32_t verbose;

    InFileTransport(int fd, unsigned verbose = 0)
        : verbose(verbose) 
    {
        RIO_ERROR status = rio_init_infile(&this->rio, fd);
        if (status != RIO_ERROR_OK){
            LOG(LOG_ERR, "rio infile initialisation failed (%u)", status);
            throw Error(ERR_TRANSPORT);
        }
    }

    virtual ~InFileTransport() 
    {
        rio_clear(&this->rio);
    }

    using Transport::recv;
    virtual void recv(char ** pbuffer, size_t len) throw (Error)
    {
        ssize_t res = rio_recv(&this->rio, *pbuffer, len);
        if (res <= 0){
            throw Error(ERR_TRANSPORT_READ_FAILED, errno);
        }
        *pbuffer += res;
        if (res != (ssize_t)len){
            throw Error(ERR_TRANSPORT_NO_MORE_DATA, errno);
        }
    }

    using Transport::send;
    virtual void send(const char * const buffer, size_t len) throw (Error) {
        throw Error(ERR_TRANSPORT_INPUT_ONLY_USED_FOR_RECV, 0);
    }
};

#endif