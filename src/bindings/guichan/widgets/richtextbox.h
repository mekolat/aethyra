/*
 *  Aethyra
 *  Copyright (C) 2004  The Mana World Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *
 *  This file is part of Aethyra.
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

#ifndef RICHTEXTBOX_H
#define RICHTEXTBOX_H

#include <list>
#include <vector>

#include <guichan/mouselistener.hpp>
#include <guichan/widgetlistener.hpp>
#include <guichan/widget.hpp>

class LinkHandler;

struct HYPERLINK {
    int x1, x2, y1, y2;     /**< Where link is placed */
    std::string link;
    std::string caption;
};

/**
 * A simple browser box able to handle links and forward events to the
 * parent conteiner.
 */
class RichTextBox : public gcn::Widget, public gcn::MouseListener,
                    public gcn::WidgetListener
{
    public:
        /**
         * Constructor.
         */
        RichTextBox(unsigned int mode = AUTO_SIZE, bool opaque = true);

        /**
         * Destructor.
         */
        ~RichTextBox();

        /**
         * Sets the handler for links.
         */
        void setLinkHandler(LinkHandler *linkHandler);

        /**
         * Sets the RichTextBox opacity.
         */
        void setOpaque(bool opaque);

        /**
         * Sets the Highlight mode for links.
         */
        void setHighlightMode(unsigned int highMode);

        /**
         * Sets the maximum numbers of rows in the browser box. 0 = no limit.
         */
        void setMaxRow(int max) {mMaxRows = max; };

        /**
         * Disable links & user defined colors to be used in chat input.
         */
        void disableLinksAndUserColors();

        /**
         * Adds a text row to the browser.
         */
        void addRow(const std::string &row);

        /**
         * Remove all rows.
         */
        void clearRows();

        /**
         * Handles mouse actions.
         */
        void mousePressed(gcn::MouseEvent &event);
        void mouseMoved(gcn::MouseEvent &event);

        /**
         * After a resize, calculateTextLayout must be called
         * before (or at the start of) the next draw.
         */
        void widgetResized(const gcn::Event &event);

        /**
         * Draws the browser box.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Parses the raw text, updates and positions all the
         * Links and LaidOutParts.
         */
        void calculateTextLayout();

        /**
         * Forces the RichTextBox to recalculate its lines, since the font
         * changed.
         */
        void fontChanged();

        /**
         * Escapes color text strings, so that they aren't parsed in the
         * RichTextBox.
         */
        void sanitizeText(std::string &text);

        /**
         * RichTextBox modes.
         */
        enum {
            AUTO_SIZE,
            AUTO_WRAP       /**< Maybe it needs a fix or to be redone. */
        };

        /**
         * RichTextBox colors.
         *
         * NOTES (by Javila):
         *  - color values is "0x" prefix followed by HTML color style.
         *  - we can add up to 10 different colors: [0..9].
         *  - not all colors will be fine with all backgrounds due transparent
         *    windows and widgets. So, I think it's better keep RichTextBox
         *    opaque (white background) by default.
         */
        enum {
            BLACK = 0x000000,       /**< Color 0 */
            RED = 0xff0000,         /**< Color 1 */
            GREEN = 0x009000,       /**< Color 2 */
            BLUE = 0x0000ff,        /**< Color 3 */
            ORANGE = 0xe0980e,      /**< Color 4 */
            YELLOW = 0xf1dc27,      /**< Color 5 */
            PINK = 0xff00d8,        /**< Color 6 */
            PURPLE = 0x8415e2,      /**< Color 7 */
            GRAY = 0x919191,        /**< Color 8 */
            BROWN = 0x8e4c17        /**< Color 9 */
        };

        /**
         * Highlight modes for links.
         * This can be used for a bitmask.
         */
        enum {
            UNDERLINE  = 1,
            BACKGROUND = 2
        };

        virtual void logic();

    protected:
        /*
         * mTextRows contains the raw text, before any layout or
         * link-recognition operations.
         *
         * mQueuedRows contains the rows added since the last logic update.
         * Rows are cached in advance, rather than operating on them immediately
         * in order to try to avoid degenerating into a O(n!) algorithm when
         * adding rows, and instead have a series of O(n^2) readjustments.
         */
        typedef std::list<std::string> TextRows;
        typedef TextRows::iterator TextRowIterator;
        TextRows mTextRows;
        TextRows mQueuedRows;

        /**
         * A result of parsing mTextRows which has been
         * positioned and is ready to draw.  Due to line-wrapping
         * and color changes, a TextRow may become several
         * LaidOutParts.
         *
         * ORDINARY_TEXT is text that is all on one line, and can
         * be drawn with a single call to graphics->setColor and
         * font->drawString.
         */
        class LaidOutPart {
            public:
                enum PartType
                {
                    ORDINARY_TEXT,      /**< Single line of text */
                    HORIZONTAL_RULE     /**< Separator - similar to an HTML [hr] element */
                };
                PartType type;
                std::string text;
                int x;
                int y;
                gcn::Color color;

                LaidOutPart(std::string t, int xcoord, int ycoord, gcn::Color c) :
                    type(ORDINARY_TEXT),
                    text(t),
                    x(xcoord),
                    y(ycoord),
                    color(c)
                    {}

                LaidOutPart(PartType pt, std::string t, int xcoord, int ycoord, gcn::Color c) :
                    type (pt),
                    text(t),
                    x(xcoord),
                    y(ycoord),
                    color(c)
                    {}
        };
        typedef std::list<LaidOutPart> LaidOutText;
        typedef LaidOutText::iterator LaidOutTextIterator;
        LaidOutText mLaidOutText;

        /**
         * Whether calculateTextLayout has been called since the
         * last event that requires it.
         */
        bool mLaidOutTextValid;

        typedef std::vector<HYPERLINK> Links;
        typedef Links::iterator LinkIterator;
        Links mLinks;

        LinkHandler *mLinkHandler;
        unsigned int mMode;
        unsigned int mHighMode;
        bool mOpaque;
        bool mUseLinksAndUserColors;
        int mSelectedLink;
        unsigned int mMaxRows;
};

#endif

