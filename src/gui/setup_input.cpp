/*
 *  Aethyra
 *  Copyright (C) 2007  The Mana World Development Team
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

#include <SDL_keyboard.h>

#include "ok_dialog.h"
#include "setup_input.h"

#include "../configuration.h"

#include "../bindings/guichan/keyboardconfig.h"
#include "../bindings/guichan/layouthelper.h"

#include "../bindings/guichan/models/keylistmodel.h"

#include "../bindings/guichan/widgets/button.h"
#include "../bindings/guichan/widgets/checkbox.h"
#include "../bindings/guichan/widgets/label.h"
#include "../bindings/guichan/widgets/listbox.h"
#include "../bindings/guichan/widgets/scrollarea.h"

#include "../bindings/sdl/joystick.h"

#include "../utils/gettext.h"
#include "../utils/stringutils.h"

extern Joystick *joystick;

Setup_Input::Setup_Input():
    mKeyListModel(new KeyListModel()),
    mCalibrateLabel(new Label(_("Press the button to start calibration"))),
    mJoystickCheckbox(new CheckBox(_("Enable joystick"))),
    mKeyList(new ListBox(mKeyListModel)),
    mCalibrateButton(new Button(_("Calibrate"), "calibrate", this)),
    mKeySetting(false)
{
    keyboard.setSetupKeyboard(this);
    setName(_("Input"));

    refreshKeys();

    mJoystickCheckbox->setSelected(config.getValue("joystickEnabled", false));

    mJoystickCheckbox->addActionListener(this);

    mKeyList->addActionListener(this);

    ScrollArea *scrollArea = new ScrollArea(mKeyList);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mAssignKeyButton = new Button(_("Assign"), "assign", this);
    mAssignKeyButton->addActionListener(this);
    mAssignKeyButton->setEnabled(false);

    mMakeDefaultButton = new Button(_("Default"), "makeDefault", this);
    mMakeDefaultButton->addActionListener(this);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, scrollArea, 4, 7).setPadding(2);
    place(0, 7, mMakeDefaultButton);
    place(3, 7, mAssignKeyButton);
    place(0, 8, mJoystickCheckbox, 3);
    place(0, 9, mCalibrateLabel, 4);
    place(0, 10, mCalibrateButton);

    setDimension(gcn::Rectangle(0, 0, 325, 280));
}

Setup_Input::~Setup_Input()
{
    delete mKeyList;
    delete mKeyListModel;

    delete mAssignKeyButton;
    delete mMakeDefaultButton;
}

void Setup_Input::apply()
{
    config.setValue("joystickEnabled", mJoystickCheckbox->isSelected());

    if (joystick)
        joystick->setEnabled(mJoystickCheckbox->isSelected());

    keyUnresolved();

    if (keyboard.hasConflicts())
    {
        new OkDialog(_("Key Conflict(s) Detected."),
                     _("Resolve them, or gameplay may result in strange "
                       "behavior."));
    }
    keyboard.setEnabled(true);
    keyboard.store();
}

void Setup_Input::cancel()
{
    mJoystickCheckbox->setSelected(config.getValue("joystickEnabled", false));

    keyUnresolved();

    keyboard.retrieve();
    keyboard.setEnabled(true);

    refreshKeys();
}

void Setup_Input::action(const gcn::ActionEvent &event)
{
    if (event.getSource() == mKeyList)
    {
        if (!mKeySetting)
            mAssignKeyButton->setEnabled(true);
    }
    else if (event.getId() == "assign")
    {
        mKeySetting = true;
        mAssignKeyButton->setEnabled(false);
        keyboard.setEnabled(false);
        int i(mKeyList->getSelected());
        keyboard.setNewKeyIndex(i);
        mKeyListModel->setElementAt(i, keyboard.getKeyCaption(i) + ": ?");
    }
    else if (event.getId() == "makeDefault")
    {
        keyboard.makeDefault();
        refreshKeys();
    }

    if (!joystick)
        return;

    if (event.getSource() == mJoystickCheckbox)
    {
        joystick->setEnabled(mJoystickCheckbox->isSelected());
    }
    else
    {
        if (joystick->isCalibrating())
        {
            mCalibrateButton->setCaption(_("Calibrate"));
            mCalibrateLabel->setCaption
                (_("Press the button to start calibration"));
            joystick->finishCalibration();
        }
        else
        {
            mCalibrateButton->setCaption(_("Stop"));
            mCalibrateLabel->setCaption(_("Rotate the stick"));
            joystick->startCalibration();
        }
    }
}

void Setup_Input::refreshAssignedKey(int index)
{
    std::string caption;
    char *temp = SDL_GetKeyName((SDLKey) keyboard.getKeyValue(index));
    caption = keyboard.getKeyCaption(index) + ": " + toString(temp);
    mKeyListModel->setElementAt(index, caption);
}

void Setup_Input::newKeyCallback(int index)
{
    mKeySetting = false;
    refreshAssignedKey(index);
    mAssignKeyButton->setEnabled(true);
}

void Setup_Input::refreshKeys()
{
    for (int i = 0; i < keyboard.KEY_TOTAL; i++)
    {
        refreshAssignedKey(i);
    }
}

void Setup_Input::keyUnresolved()
{
    if (mKeySetting)
    {
        newKeyCallback(keyboard.getNewKeyIndex());
        keyboard.setNewKeyIndex(keyboard.KEY_NO_VALUE);
    }
}