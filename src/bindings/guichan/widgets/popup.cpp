/*
 *  Aethyra
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "popup.h"

#include "../graphics.h"
#include "../skin.h"

#include "../../../core/configuration.h"
#include "../../../core/log.h"

Popup::Popup(const std::string &name, const std::string &skin,
             gcn::Container *parent):
    mParentContainer(parent),
    mPopupName(name),
    mDefaultSkinPath(skin),
    mMaxWidth(graphics->getWidth()),
    mMaxHeight(graphics->getHeight())
{
    logger->log("Popup::Popup(\"%s\")", name.c_str());

    if (!skinLoader)
        skinLoader = new SkinLoader();

    setPadding(3);

    // Loads the skin
    mSkin = skinLoader->load(skin, mDefaultSkinPath);

    mMinWidth = mSkin->getMinWidth();
    mMinHeight = mSkin->getMinHeight();

    if (mParentContainer)
        mParentContainer->add(this);

    // Popups are invisible by default
    setVisible(false);
}

Popup::~Popup()
{
    logger->log("Popup::~Popup(\"%s\")", mPopupName.c_str());

    savePopupConfiguration();
    setParentContainer(NULL);

    mSkin->instances--;
}

void Popup::setParentContainer(gcn::Container *container)
{
    if (mParentContainer)
        mParentContainer->remove(this);

    mParentContainer = container;

    if (mParentContainer)
        mParentContainer->add(this);
}

void Popup::loadPopupConfiguration()
{
    if (mPopupName.empty())
        return;

    const std::string &name = mPopupName;
    const std::string &skinName = config.getValue(name + "Skin",
                                                  mSkin->getFilePath());

    if (skinName.compare(mSkin->getFilePath()) != 0)
    {
        mSkin->instances--;
        mSkin = skinLoader->load(skinName, mDefaultSkinPath);

        setMinWidth(mSkin->getMinWidth());
        setMinHeight(mSkin->getMinHeight());
    }
}

void Popup::savePopupConfiguration()
{
    if (mPopupName.empty())
        return;

    const std::string &name = mPopupName;

    // Saves the skin path in a config file (which allows for skins to be
    // changed from the default path)
    config.setValue(name + "Skin", mSkin->getFilePath());
}

void Popup::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics*>(graphics);

    g->drawImageRect(0, 0, getWidth(), getHeight(), mSkin->getBorder());

    drawChildren(graphics);
}

gcn::Rectangle Popup::getChildrenArea()
{
    return gcn::Rectangle(getPadding(), 0, getWidth() - getPadding() * 2,
                          getHeight() - getPadding() * 2);
}

void Popup::setContentSize(int width, int height)
{
    width += 2 * getPadding();
    height += 2 * getPadding();

    if (getMinWidth() > width)
        width = getMinWidth();
    else if (getMaxWidth() < width)
        width = getMaxWidth();
    if (getMinHeight() > height)
        height = getMinHeight();
    else if (getMaxHeight() < height)
        height = getMaxHeight();

    setSize(width, height);
}

void Popup::setLocationRelativeTo(gcn::Widget *widget)
{
    int wx, wy;
    int x, y;

    widget->getAbsolutePosition(wx, wy);
    getAbsolutePosition(x, y);

    setPosition(getX() + (wx + (widget->getWidth() - getWidth()) / 2 - x),
                getY() + (wy + (widget->getHeight() - getHeight()) / 2 - y));
}

void Popup::setMinWidth(int width)
{
    mMinWidth = width > mSkin->getMinWidth() ? width : mSkin->getMinWidth();
}

void Popup::setMinHeight(int height)
{
    mMinHeight = height > mSkin->getMinHeight() ? height : mSkin->getMinHeight();
}

void Popup::setMaxWidth(int width)
{
    mMaxWidth = width;
}

void Popup::setMaxHeight(int height)
{
    mMaxHeight = height;
}

