/*
 *  Aethyra
 *  Copyright (C) 2004  The Mana World Development Team
 *
 *  This file is part of Aethyra based on original code
 *  from The Mana World.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MAIN_H
#define MAIN_H

#include <string>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#elif defined WIN32
#include "winver.h"
#elif defined __APPLE__
#define PACKAGE_VERSION "0.0.29.1"
#endif

/*
 * Client different States
 */
enum {
    EXIT_STATE,
    LOADDATA_STATE,
    START_STATE,
    LOGIN_STATE,
    ACCOUNT_STATE,
    REGISTER_STATE,
    CHAR_CONNECT_STATE,
    CHAR_SERVER_STATE,
    CHAR_SELECT_STATE,
    CHAR_NEW_STATE,
    CHAR_DEL_STATE,
    GAME_STATE,
    ERROR_STATE,
    UPDATE_STATE,
    CONNECTING_STATE
};

extern char n_server, n_character;
extern unsigned char state;
extern std::string errorMessage;

#endif
