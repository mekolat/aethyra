/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
  #pragma warning (disable:4312)
#endif

#ifndef _CHAR_SEL_SERVER_H
#define _CHAR_SEL_SERVER_H

#include <allegro.h>

#include "../main.h"
#include "../net/network.h"
#include "gui.h"

/**
 * The action listener for the server select dialog.
 *
 * \ingroup GUI
 */
class ServerSelectListener : public gcn::ActionListener {
    public:
        void action(const std::string& eventId);
};

/**
 * The list model for the server list.
 *
 * \ingroup GUI
 */
class ServerListModel : public gcn::ListModel {
    public:
        int getNumberOfElements();
        std::string getElementAt(int i);
};

void char_server();
void server_char_server(int serverIndex);
char *server_list(int index, int *size);

#endif
