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

#include "mw_button.h"

MWButton::MWButton(const std::string& caption):
    gcn::Button(caption)
{
    mouseDown = false;
    keyDown = false;
}

void MWButton::draw(gcn::Graphics* graphics) {
    gcn::Rectangle screenRect = getScreenDimension(this);
    int mode;
    int offset = 0;

    if (false /*disabled*/) {
        mode = 3;
    }
    else if (hasMouse() && mouseDown || keyDown) {
        mode = 2;
        offset = 1;
    }
    else if (hasMouse()) {
        mode = 1;
    }
    else {
        mode = 0;
    }

    draw_skinned_rect(gui_bitmap, &gui_skin.button.background[mode],
            screenRect.x, screenRect.y,
            screenRect.width, screenRect.height);

    int rtm = alfont_text_mode(-1);
    gui_text(gui_bitmap, getCaption().c_str(),
            screenRect.x + 2 + offset,
            screenRect.y + 4 + offset,
            gui_skin.button.textcolor[mode], FALSE);
    alfont_text_mode(rtm);
}

void MWButton::lostFocus() {
    mouseDown = false;
    keyDown = false;
}

void MWButton::mousePress(int x, int y, int button) {
    if (button == gcn::MouseInput::LEFT && hasMouse()) {
        mouseDown = true;
    }
}

void MWButton::mouseRelease(int x, int y, int button) {
    if (button == gcn::MouseInput::LEFT) {
        mouseDown = false;
    }
}

void MWButton::keyPress(const gcn::Key& key) {
    if (key.getValue() == gcn::Key::ENTER ||
        key.getValue() == gcn::Key::SPACE)
    {
        keyDown = true;
    }
    mouseDown = false;
}

void MWButton::keyRelease(const gcn::Key& key) {
    if ((key.getValue() == gcn::Key::ENTER ||
         key.getValue() == gcn::Key::SPACE) && keyDown)
    {
        keyDown = false;
        generateAction();
    }
}

