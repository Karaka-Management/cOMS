/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_UI_H
#define TOS_GPUAPI_OPENGL_UI_H

#include "../../stdlib/Types.h"

#include "../../ui/UIButton.h"
#include "../../ui/UIWindow.h"

#include <gl/GL.h>

void render_button(UIButton* btn)
{
    // Draw the background rectangle (button)
    glColor3f(btn->layout_default->color_background.r, btn->layout_default->color_background.g, btn->layout_default->color_background.b);
    glBegin(GL_QUADS);
    glVertex2f(btn->layout_default->x, btn->layout_default->y);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y + btn->layout_default->height);
    glVertex2f(btn->layout_default->x, btn->layout_default->y + btn->layout_default->height);
    glEnd();

    // Draw the border
    glColor3f(btn->layout_default->border_color.r, btn->layout_default->border_color.g, btn->layout_default->border_color.b);
    // Bottom border
    glLineWidth(btn->layout_default->border_width[0]);
    glBegin(GL_LINES);
    glVertex2f(btn->layout_default->x, btn->layout_default->y);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y);
    glEnd();

    // Right border
    glLineWidth(btn->layout_default->border_width[1]);
    glBegin(GL_LINES);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y + btn->layout_default->height);
    glEnd();

    // Top border
    glLineWidth(btn->layout_default->border_width[2]);
    glBegin(GL_LINES);
    glVertex2f(btn->layout_default->x + btn->layout_default->width, btn->layout_default->y + btn->layout_default->height);
    glVertex2f(btn->layout_default->x, btn->layout_default->y + btn->layout_default->height);
    glEnd();

    // Left border
    glLineWidth(btn->layout_default->border_width[3]);
    glBegin(GL_LINES);
    glVertex2f(btn->layout_default->x, btn->layout_default->y + btn->layout_default->height);
    glVertex2f(btn->layout_default->x, btn->layout_default->y);
    glEnd();
}

#endif