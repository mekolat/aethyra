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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <guichan/color.hpp>
#include <guichan/graphics.hpp>

#include "../../core/image/image.h"

const short defaultScreenWidth = 800;
const short defaultScreenHeight = 600;

class SDL_Surface;

/**
 * 9 images defining a rectangle. 4 corners, 4 sides and a middle area. The
 * topology is as follows:
 *
 * <pre>
 *  !-----!-----------------!-----!
 *  !  0  !        1        !  2  !
 *  !-----!-----------------!-----!
 *  !  3  !        4        !  5  !
 *  !-----!-----------------!-----!
 *  !  6  !        7        !  8  !
 *  !-----!-----------------!-----!
 * </pre>
 *
 * Sections 0, 2, 6 and 8 will remain as is. 1, 3, 4, 5 and 7 will be
 * repeated to fit the size of the widget.
 */
struct ImageRect
{
    enum ImagePosition
    {
        UPPER_LEFT = 0,
        UPPER_CENTER = 1,
        UPPER_RIGHT = 2,
        LEFT = 3,
        CENTER = 4,
        RIGHT = 5,
        LOWER_LEFT = 6,
        LOWER_CENTER = 7,
        LOWER_RIGHT = 8
    };

    Image *grid[9];

    /**
     * Returns the minimum width for the ImageRect.
     */
    const int getMinWidth() const
    {
        return (grid[0]->getWidth() + grid[3]->getWidth() +
                grid[6]->getWidth());
    }

    /**
     * Returns the minimum height for the ImageRect.
     */
    const int getMinHeight() const
    {
        return (grid[0]->getHeight() + grid[3]->getHeight() +
                grid[6]->getHeight());
    }
};

/**
 * A central point of control for Graphics.
 */
class Graphics : public gcn::Graphics
{
    public:
        /**
         * Constructor.
         */
        Graphics();

        /**
         * Destructor.
         */
        virtual ~Graphics() {}

        /**
         * Sets the target SDL_Surface to draw to. The target can be any
         * SDL_Surface. This funtion also pushes a clip areas corresponding to
         * the dimension of the target.
         *
         * @param target the target to draw to.
         */
        virtual void setTarget(SDL_Surface* target) { mTarget = target; }

        /**
         * Gets the target SDL_Surface.
         *
         * @return the target SDL_Surface.
         */
        virtual SDL_Surface* getTarget() const { return mTarget; }

        /**
         * Try to create a window with the given settings.
         */
        virtual bool setVideoMode(int w, int h, int bpp, bool fs, bool hwaccel);

        /**
         * Set fullscreen mode.
         */
        virtual bool setFullscreen(bool fs);

        /**
         * Try to change the size of the window
         */
        bool resizeVideoMode(int w, int h);

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual bool drawImage(Image *image, int x, int y) = 0;

        /**
         * Overrides with our own drawing method.
         */
        virtual void drawImage(gcn::Image const *image, int srcX, int srcY,
                               int dstX, int dstY, int width, int height) = 0;

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual bool drawImage(Image *image, int srcX, int srcY, int dstX,
                               int dstY, int width, int height,
                               bool useColor = false) = 0;

        virtual void drawImagePattern(Image *image, int x, int y, int w, int h) = 0;

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(int x, int y, int w, int h, Image *topLeft,
                           Image *topRight, Image *bottomLeft,
                           Image *bottomRight, Image *top, Image *right,
                           Image *bottom, Image *left, Image *center);

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(int x, int y, int w, int h, const ImageRect &imgRect);

        /**
         * Updates the screen. This is done by either copying the buffer to the
         * screen or swapping pages.
         */
        virtual void updateScreen() = 0;

        /**
         * Returns the width of the screen.
         */
        int getWidth();

        /**
         * Returns the height of the screen.
         */
        int getHeight();

        const gcn::Color& getColor() const { return mColor; }

        virtual void setColor(const gcn::Color& color) = 0;

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        virtual SDL_Surface* getScreenshot() = 0;

        /**
         * Whether the graphics are available for drawing or not.
         */
        bool initialized() { return (mTarget != NULL); }

    protected:
        SDL_Surface *mTarget;
        gcn::Color mColor;
        int mWidth;
        int mHeight;
        int mBpp;
        bool mAlpha;
        bool mFullscreen;
        bool mHWAccel;
};

void saveScreenshot();

extern Graphics *graphics;

#endif
