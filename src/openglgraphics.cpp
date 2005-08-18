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

#ifdef USE_OPENGL

#include "openglgraphics.h"

#include <iostream>
#include <SDL.h>

#include "log.h"

#include "resources/image.h"

extern volatile int framesToDraw;

OpenGLGraphics::OpenGLGraphics():
    mAlpha(false), mTexture(false), mColorAlpha(false)
{
}

OpenGLGraphics::~OpenGLGraphics()
{
}

bool OpenGLGraphics::setVideoMode(int w, int h, int bpp, bool fs, bool hwaccel)
{
    int displayFlags = SDL_ANYFORMAT | SDL_OPENGL;

    mFullscreen = fs;
    mHWAccel = hwaccel;

    if (fs) {
        displayFlags |= SDL_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (!(mScreen = SDL_SetVideoMode(w, h, bpp, displayFlags))) {
        return false;
    }

    // Setup OpenGL
    glViewport(0, 0, w, h);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    int gotDoubleBuffer;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &gotDoubleBuffer);
    logger->log("Using OpenGL %s double buffering.",
            (gotDoubleBuffer ? "with" : "without"));

    return true;
}

bool OpenGLGraphics::drawImage(Image *image, int srcX, int srcY,
        int dstX, int dstY, int width, int height)
{
    srcX += image->bounds.x;
    srcY += image->bounds.y;

    // Find OpenGL texture coordinates
    float texX1 = srcX / (float)image->texWidth;
    float texY1 = srcY / (float)image->texHeight;
    float texX2 = (srcX + width) / (float)image->texWidth;
    float texY2 = (srcY + height) / (float)image->texHeight;

    glColor4f(1.0f, 1.0f, 1.0f, image->alpha);
    glBindTexture(GL_TEXTURE_2D, image->glimage);

    if (!mTexture) {
        glEnable(GL_TEXTURE_2D);
        mTexture = true;
    };

    // Check if blending already is enabled
    if (!mAlpha)
    {
        glEnable(GL_BLEND);
        mAlpha = true;
    }

    // Draw a textured quad -- the image
    glBegin(GL_QUADS);
    glTexCoord2f(texX1, texY1);
    glVertex3i(dstX, dstY, 0);

    glTexCoord2f(texX2, texY1);
    glVertex3i(dstX + width, dstY, 0);

    glTexCoord2f(texX2, texY2);
    glVertex3i(dstX + width, dstY + height, 0);

    glTexCoord2f(texX1, texY2);
    glVertex3i(dstX, dstY + height, 0);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    return true;
}

void OpenGLGraphics::updateScreen()
{
    glFlush();
    glFinish();
    SDL_GL_SwapBuffers();

    // Decrement frame counter when using framerate limiting
    if (framesToDraw > 1) framesToDraw--;

    // Wait while we're not allowed to draw next frame yet
    while (framesToDraw == 1)
    {
        SDL_Delay(10);
    }
}

void OpenGLGraphics::_beginDraw()
{
    glPushAttrib(
            GL_COLOR_BUFFER_BIT |
            GL_CURRENT_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_ENABLE_BIT |
            GL_FOG_BIT |
            GL_LIGHTING_BIT |
            GL_LINE_BIT |
            GL_POINT_BIT |
            GL_POLYGON_BIT |
            GL_SCISSOR_BIT |
            GL_STENCIL_BUFFER_BIT    |
            GL_TEXTURE_BIT |
            GL_TRANSFORM_BIT
            );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, (double)mScreen->w, (double)mScreen->h, 0.0, -1.0, 1.0);

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_SCISSOR_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    pushClipArea(gcn::Rectangle(0, 0, mScreen->w, mScreen->h));
}

void OpenGLGraphics::_endDraw()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();

    popClipArea();
}

bool OpenGLGraphics::pushClipArea(gcn::Rectangle area)
{
    bool result = gcn::Graphics::pushClipArea(area);

    glScissor(mClipStack.top().x,
            mScreen->h - mClipStack.top().y - mClipStack.top().height,
            mClipStack.top().width,
            mClipStack.top().height);

    return result;
}

void OpenGLGraphics::popClipArea()
{
    gcn::Graphics::popClipArea();

    if (mClipStack.empty())
    {
        return;
    }

    glScissor(mClipStack.top().x,
            mScreen->h - mClipStack.top().y - mClipStack.top().height,
            mClipStack.top().width,
            mClipStack.top().height);
}

void OpenGLGraphics::setColor(const gcn::Color& color)
{
    mColor = color;
    glColor4f(color.r/255.0,
            color.g/255.0,
            color.b/255.0,
            color.a/255.0);

    mColorAlpha = (color.a != 255);
}

void OpenGLGraphics::drawImage(const gcn::Image* image, int srcX, int srcY,
        int dstX, int dstY, int width, int height)
{
    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;

    // The following code finds the real width and height of the texture.
    // OpenGL only supports texture sizes that are powers of two
    int realImageWidth = 1;
    int realImageHeight = 1;
    while (realImageWidth < image->getWidth())
    {
        realImageWidth *= 2;
    }
    while (realImageHeight < image->getHeight())
    {
        realImageHeight *= 2;
    }

    // Find OpenGL texture coordinates
    float texX1 = srcX / (float)realImageWidth;
    float texY1 = srcY / (float)realImageHeight;
    float texX2 = (srcX+width) / (float)realImageWidth;
    float texY2 = (srcY+height) / (float)realImageHeight;

    // Please dont look too closely at the next line, it is not pretty.
    // It uses the image data as a pointer to a GLuint
    glBindTexture(GL_TEXTURE_2D, *((GLuint *)(image->_getData())));

    if (!mTexture) {
        glEnable(GL_TEXTURE_2D);
        mTexture = true;
    };

    // Check if blending already is enabled
    if (!mAlpha)
    {
        glEnable(GL_BLEND);
        mAlpha = true;
    }

    // Draw a textured quad -- the image
    glBegin(GL_QUADS);
    glTexCoord2f(texX1, texY1);
    glVertex3i(dstX, dstY, 0);

    glTexCoord2f(texX1, texY2);
    glVertex3i(dstX, dstY + height, 0);

    glTexCoord2f(texX2, texY2);
    glVertex3i(dstX + width, dstY + height, 0);

    glTexCoord2f(texX2, texY1);
    glVertex3i(dstX + width, dstY, 0);
    glEnd();
}

void OpenGLGraphics::drawPoint(int x, int y)
{
    x += mClipStack.top().xOffset;
    y += mClipStack.top().yOffset;

    if (mAlpha && !mColorAlpha) {
        glDisable(GL_BLEND);
        mAlpha = false;
    }

    glDisable(GL_TEXTURE_2D);
    mTexture = false;

    glBegin(GL_POINTS);
    glVertex3i(x, y, 0);
    glEnd();
}

void OpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    x1 += mClipStack.top().xOffset;
    y1 += mClipStack.top().yOffset;
    x2 += mClipStack.top().xOffset;
    y2 += mClipStack.top().yOffset;

    if (mAlpha && !mColorAlpha) {
        glDisable(GL_BLEND);
        mAlpha = false;
    }

    glDisable(GL_TEXTURE_2D);
    mTexture = false;

    glBegin(GL_LINES);
    glVertex3f(x1+0.5f, y1+0.5f, 0);
    glVertex3f(x2+0.5f, y2+0.5f, 0);
    glEnd();

    glBegin(GL_POINTS);
    glVertex3f(x2+0.5f, y2+0.5f, 0);
    glEnd();
}

void OpenGLGraphics::drawRectangle(const gcn::Rectangle& rectangle)
{
    if (mAlpha && !mColorAlpha) {
        glDisable(GL_BLEND);
        mAlpha = false;
    }

    glDisable(GL_TEXTURE_2D);
    mTexture = false;

    glBegin(GL_LINE_LOOP);
    glVertex3f(rectangle.x + mClipStack.top().xOffset + 0.5f,
            rectangle.y + mClipStack.top().yOffset + 0.5f, 0);
    glVertex3f(rectangle.x + rectangle.width - 0.5f + mClipStack.top().xOffset,
            rectangle.y + mClipStack.top().yOffset + 0.5f, 0);
    glVertex3f(rectangle.x + rectangle.width - 0.5f + mClipStack.top().xOffset,
            rectangle.y + rectangle.height + mClipStack.top().yOffset - 0.5f, 0);
    glVertex3f(rectangle.x + mClipStack.top().xOffset + 0.5f,
            rectangle.y + rectangle.height + mClipStack.top().yOffset - 0.5f, 0);
    glEnd();
}

void OpenGLGraphics::fillRectangle(const gcn::Rectangle& rectangle)
{
    if (mAlpha && !mColorAlpha) {
        glDisable(GL_BLEND);
        mAlpha = false;
    }

    glDisable(GL_TEXTURE_2D);
    mTexture = false;

    glBegin(GL_QUADS);
    glVertex3i(rectangle.x + mClipStack.top().xOffset,
            rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset,
            rectangle.y + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + rectangle.width + mClipStack.top().xOffset,
            rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glVertex3i(rectangle.x + mClipStack.top().xOffset,
            rectangle.y + rectangle.height + mClipStack.top().yOffset, 0);
    glEnd();
}

void OpenGLGraphics::setTargetPlane(int width, int height)
{
}

#endif // USE_OPENGL
