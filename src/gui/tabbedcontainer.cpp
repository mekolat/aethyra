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

#include "button.h"
#include "tabbedcontainer.h"

#include "../utils/dtor.h"
#include "../utils/tostring.h"

TabbedContainer::TabbedContainer(int width, int padX, int buttonHeight,
                                 int height, int padY, int buttonsPerRow):
    mActiveContent(0),
    mWidth(width),
    mPadX(padX),
    mButtonHeight(buttonHeight),
    mHeight(height),
    mPadY(padY),
    mButtonsPerRow(buttonsPerRow),

    mButtonWidth((width - (buttonsPerRow - 1) * padX) / buttonsPerRow - padX)
{
}

TabbedContainer::~TabbedContainer()
{
    delete_all(mTabs);
    mTabs.clear();
    mContents.clear();
}

void TabbedContainer::addTab(gcn::Widget *widget, const std::string &caption)
{
    int tabNumber = mTabs.size();

    Button *tab = new Button(caption, toString(tabNumber), this);

    tab->setSize(mButtonWidth, mButtonHeight);
    add(tab, (mButtonWidth + mPadX) * (tabNumber % mButtonsPerRow),
             (mButtonHeight + mPadY) * (tabNumber / mButtonsPerRow));

    mTabs[caption] = tab;

    mContents.push_back(widget);
    widget->setPosition(0, mHeight);

    // If this is the first tab in this container, make it visible
    if (!mActiveContent) {
        mActiveContent = widget;
        add(mActiveContent);
        tab->setLogged(true);
    }

    mWidgets[widget] = caption;
}

void TabbedContainer::removeTab(const std::string &caption)
{
    gcn::ActionEvent actionEvent(this, "0");
    action(actionEvent);
    remove(mTabs[caption]);
    mTabs.erase(caption);
}

void TabbedContainer::logic()
{
    if (mActiveContent) {
        mActiveContent->setSize(
                getWidth() - 2 * mActiveContent->getFrameSize(),
                getHeight() - mHeight - 2 * mActiveContent->getFrameSize());
    }

    Container::logic();
}

void TabbedContainer::action(const gcn::ActionEvent &event)
{
    int tabNo;
    std::stringstream ss(event.getId());
    ss >> tabNo;

    gcn::Widget *newContent = mContents[tabNo];

    if (newContent) {
        if (mActiveContent) {
            // Unhighlight old tab
            ((Button*)mTabs[mWidgets[mActiveContent]])->setLogged(false);
            remove(mActiveContent);
        }
        mActiveContent = newContent;
        // Highlight new tab
        ((Button*)mTabs[mWidgets[mActiveContent]])->setLogged(true);
        add(newContent);
    }
}

void TabbedContainer::setOpaque(bool opaque)
{
    Container::setOpaque(opaque);
}

short TabbedContainer::getNumberOfTabs()
{
    return mTabs.size();
}

std::string TabbedContainer::getActiveWidget()
{
    return mWidgets[mActiveContent];
}
