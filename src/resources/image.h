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
 *  $Id$
 */

#ifndef _TMW_IMAGE_H
#define _TMW_IMAGE_H

#include "resource.h"
#include <SDL.h>

#ifdef USE_OPENGL
#include <SDL_opengl.h>
#endif

// This flag causes image alpha channel to be preserved, otherwise masking is
// used.
#define IMG_ALPHA  1


// Forward declarations
class SubImage;
class ScaledImage;

/**
 * Defines a class for loading and storing images.
 */
class Image : public Resource
{
    public:
        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Loads an image.
         *
         * @param filePath The path to the image file to load.
         * @param flags    These flags allow controlling the way the image is
         *                 loaded. Currently only IMG_ALPHA is supported, which
         *                 causes alpha layer to be preserved.
         *
         * @return <code>NULL</code> if the an error occurred, a
         * valid pointer otherwise.
         */
        static Image *load(const std::string &filePath, int flags);

        /**
         * Loads an image from a buffer in memory.
         *
         * @param buffer     The memory buffer containing the image data.
         * @param bufferSize The size of the memory buffer in bytes.
         *
         * @return <code>NULL</code> if the an error occurred, a valid pointer
         *         otherwise.
         */
        static Image *load(void* buffer, unsigned int bufferSize);

        /**
         * Creates a new empty image with given height and width.
         */
        static Image *create(int width, int height);

        /**
         * Frees the resources created by SDL.
         */
        virtual void unload();

        /**
         * Returns the width of the image.
         */
        virtual int getWidth() const;

        /**
         * Returns the height of the image.
         */
        virtual int getHeight() const;

        /**
         * Creates a new image with the desired clipping rectangle.
         * @return <code>NULL</code> if creation failed and a valid
         * object otherwise.
         */
        virtual Image* getSubImage(int x, int y, int width, int height);

        /**
         * Blits the image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual bool draw(SDL_Surface *screen,
                int srcX, int srcY,
                int dstX, int dstY,
                int width, int height);

        /**
         * Blits the image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual bool draw(SDL_Surface *screen, int x, int y);

        /**
         * Does a pattern fill on the given area.
         */
        virtual void drawPattern(
                SDL_Surface *screen, int x, int y, int w, int h);

        /**
         * Sets the alpha value of this image.
         */
        void setAlpha(float alpha);

        /**
         * Returns the alpha value of this image.
         */
        float getAlpha();

        /**
         * Fills the image with given color.
         */
        void fillWithColor(
                unsigned char red, unsigned char green, unsigned blue);


    protected:
        /**
         * Constructor.
         */
#ifdef USE_OPENGL
        Image(GLuint image,
                int width, int height,
                int texWidth, int texHeight);
#else
        Image(SDL_Surface *image);
#endif

#ifdef USE_OPENGL
        GLuint image;
        int width, height;
        int texWidth, texHeight;
#else
        SDL_Surface *image;
#endif
        float alpha;
};

/**
 * A clipped version of a larger image.
 */
class SubImage : public Image
{
    public:
        /**
         * Constructor.
         */
        //SubImage(SDL_Surface *timage, int x, int y, int width, int height);
#ifndef USE_OPENGL
        SubImage(Image *parent, SDL_Surface *image,
                int x, int y, int width, int height);
#else
        SubImage(Image *parent, GLuint image, int x, int y,
                int width, int height, int texWidth, int textHeight);
#endif

        /**
         * Destructor.
         */
        ~SubImage();

        /**
         * Returns the width of the image.
         */
        int getWidth() const;

        /**
         * Returns the height of the image.
         */
        int getHeight() const;

        /**
         * Creates a new image with the desired clipping rectangle.
         * @return <code>NULL</code> if creation failed and a valid
         * object otherwise.
         */
        Image* getSubImage(int x, int y, int width, int height);

        /**
         * Draws this image.
         */
        bool draw(SDL_Surface *screen, int srcX, int srcY,
                int dstX, int dstY, int width, int height);

        /**
         * Draws the clipped image onto the screen.
         * @return <code>true</code> if drawing was succesful
         * <code>false</code> otherwise.
         */
        bool draw(SDL_Surface *screen, int x, int y);

    private:
        Image *parent;
        SDL_Rect rect;
};

#endif
