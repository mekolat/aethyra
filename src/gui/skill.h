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
 *
 *  By ElvenProgrammer aka Eugenio Favalli (umperio@users.sourceforge.net)
 */

#ifdef WIN32
  #pragma warning (disable:4312)
#endif

#ifndef _SKILL_H
#define _SKILL_H

#include <allegro.h>
#include "../main.h"

//DIALOG skill_dialog[];

extern int n_skills;

struct SKILL {
	short id, lv, sp;
	SKILL *next;
};


void update_skill_dialog();
void add_skill(short id, short lv, short sp);
char *skill_list(int index, int *list_size);
int get_skill_id(int index);
SKILL *is_skill(int id);
void increaseStatus(void *dp3, int d1);
#endif
