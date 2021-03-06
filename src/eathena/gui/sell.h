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

#ifndef SELL_H
#define SELL_H

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

#include "../../bindings/guichan/widgets/window.h"

class Item;
class ShopListBox;
class ShopListModel;

/**
 * The sell dialog.
 *
 * \ingroup Interface
 */
class SellDialog : public Window, gcn::ActionListener, gcn::SelectionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        SellDialog();

        /**
         * Destructor
         */
        virtual ~SellDialog();

        /**
         * Resets the dialog, clearing inventory.
         */
        void reset();

        /**
         * Adds an item to the inventory.
         */
        void addItem(const Item *item, int price);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Returns the number of items in the shop inventory.
         */
        int getNumberOfElements();

        /**
         * Updates labels according to selected item.
         *
         * @see SelectionListener::selectionChanged
         */
        void valueChanged(const gcn::SelectionEvent &event);

        /**
         * Gives Player's Money amount
         */
        void setMoney(int amount);

        /**
         * Focus on the shop item list on dialog focus.
         */
        void requestFocus();

        /**
         * Closes the Buy Window, as well as resetting the current npc.
         */
        void close();

        /**
         * Resets the listbox selection on shown events.
         */
        void widgetShown(const gcn::Event& event);

        void fontChanged();
    private:
        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        gcn::Button *mSellButton;
        gcn::Button *mQuitButton;
        gcn::Button *mAddMaxButton;
        ShopListBox *mShopItemList;
        gcn::ScrollArea *mScrollArea;
        gcn::Label *mMoneyLabel;
        gcn::Label *mItemDescLabel;
        gcn::Label *mItemEffectLabel;
        gcn::Label *mQuantityLabel;
        gcn::Slider *mSlider;

        ShopListModel *mShopListModel;
        int mPlayerMoney;

        int mMaxItems;
        int mAmountItems;
};

extern SellDialog *sellDialog;

#endif
