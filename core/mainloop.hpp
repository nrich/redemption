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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni, Raphael Zhou
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   main include "ile
*/

#ifndef _REDEMPTION_CORE_MAINLOOP_HPP_
#define _REDEMPTION_CORE_MAINLOOP_HPP_

class Inifile;
class crypto_key_holder;

int g_is_term(void);
void redemption_main_loop(Inifile & ini, unsigned uid, unsigned gid, crypto_key_holder & cryptoKeyHldr);
void redemption_new_session();

#endif
