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

#ifndef REDEMPTION_MOD_INTERNAL_TRANSPARENT_REPLAY_MOD_HPP
#define REDEMPTION_MOD_INTERNAL_TRANSPARENT_REPLAY_MOD_HPP

#include "in_file_transport.hpp"
#include "transparentplayer.hpp"
#include "internal_mod.hpp"

class TransparentReplayMod : public InternalMod {
private:
    std::string * auth_error_message;

    int               fd;
    InFileTransport   ift;
    TransparentPlayer player;

public:
    TransparentReplayMod( FrontAPI & front
                        , const char * replay_path
                        , uint16_t width
                        , uint16_t height
                        , std::string * auth_error_message
                        , Font const & font)
    : InternalMod(front, width, height, font)
    , auth_error_message(auth_error_message)
    , fd([&]() {
        const int fd = ::open(replay_path, O_RDWR);
        if (fd == -1) {
            throw Error(ERR_TRANSPORT_OPEN_FAILED);
        }
        return fd;
    }())
    , ift(this->fd)
    , player(&this->ift, &this->front)
    {}

    virtual ~TransparentReplayMod() {
        close(this->fd);
    }

    virtual void draw_event(time_t now) {
        try {
            if (!this->player.interpret_chunk()) {
                this->event.signal = /*BACK_EVENT_STOP*/BACK_EVENT_NEXT;
            }

            this->event.set(1);
        }
        catch (Error const & e) {
            if (e.id == ERR_TRANSPORT_OPEN_FAILED) {
                if (this->auth_error_message) {
                    *this->auth_error_message = "The recorded file is inaccessible or corrupted!";
                }

                this->event.signal = BACK_EVENT_NEXT;
                this->event.set(1);
            }
            else {
                throw;
            }
        }
    }
};

#endif  // #ifndef REDEMPTION_MOD_INTERNAL_TRANSPARENT_REPLAY_MOD_HPP
