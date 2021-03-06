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

#ifndef NET_CHARSERVERHANDLER_H
#define NET_CHARSERVERHANDLER_H

#include "messagehandler.h"

#include "../../core/utils/lockedarray.h"

class LocalPlayer;

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler : public MessageHandler
{
    public:
        CharServerHandler();

        void handleMessage(MessageIn *msg);

        void login(LockedArray<LocalPlayer*> *charInfo);

    protected:
        LockedArray<LocalPlayer*> *mCharInfo;

        LocalPlayer* readPlayerData(MessageIn &msg, int &slot);
};

#endif
