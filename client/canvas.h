/**************************************************************************
 Copyright (c) 1996-2023 Freeciv21 and Freeciv contributors. This file is
 __    __          part of Freeciv21. Freeciv21 is free software: you can
/ \\..// \    redistribute it and/or modify it under the terms of the GNU
  ( oo )        General Public License  as published by the Free Software
   \__/         Foundation, either version 3 of the License,  or (at your
                      option) any later version. You should have received
    a copy of the GNU General Public License along with Freeciv21. If not,
                  see https://www.gnu.org/licenses/.
**************************************************************************/

#pragma once
#include "support.h"
// Qt
#include <QImage>
#include <QPixmap>

// Text drawing functions
enum client_font {
  FONT_CITY_NAME,
  FONT_CITY_PROD,
  FONT_REQTREE_TEXT,
  FONT_COUNT
};

void pixmap_copy(QPixmap *dest, const QPixmap *src, int src_x, int src_y,
                 int dest_x, int dest_y, int width, int height);
QRect zealous_crop_rect(QImage &p);
QFont get_font(client_font font);
QPixmap crop_sprite(const QPixmap *sprite);
