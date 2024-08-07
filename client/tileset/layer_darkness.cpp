/*__            ___                 ***************************************
/   \          /   \         Copyright (c) 2021-2023 Freeciv21 contributors.
\_   \        /  __/                         This file is part of Freeciv21.
 _\   \      /  /__     Freeciv21 is free software: you can redistribute it
 \___  \____/   __/    and/or modify it under the terms of the GNU  General
     \_       _/          Public License  as published by the Free Software
       | @ @  \_               Foundation, either version 3 of the  License,
       |                              or (at your option) any later version.
     _/     /\                  You should have received  a copy of the GNU
    /o)  (o/\ \_                General Public License along with Freeciv21.
    \_____/ /                     If not, see https://www.gnu.org/licenses/.
      \____/        ********************************************************/

#include "layer_darkness.h"

#include "climap.h"
#include "game.h"
#include "map.h"
#include "sprite_g.h"
#include "tilespec.h"

namespace freeciv {

layer_darkness::layer_darkness(struct tileset *ts, darkness_style style)
    : freeciv::layer(ts, LAYER_DARKNESS), m_style(style), m_sprites{}
{
}

void layer_darkness::load_sprites()
{
  switch (m_style) {
  case freeciv::DARKNESS_NONE:
    // Nothing.
    break;
  case freeciv::DARKNESS_ISORECT: {
    // Isometric: take a single tx.darkness tile and split it into 4.
    QPixmap *darkness =
        load_sprite(tileset(), QStringLiteral("tx.darkness"));
    const int ntw = tileset_tile_width(tileset()),
              nth = tileset_tile_height(tileset());
    int offsets[4][2] = {
        {ntw / 2, 0}, {0, nth / 2}, {ntw / 2, nth / 2}, {0, 0}};

    if (!darkness) {
      tileset_error(tileset(), LOG_FATAL, _("Sprite tx.darkness missing."));
    }
    for (int i = 0; i < 4; i++) {
      const auto sprite = std::unique_ptr<QPixmap>(
          crop_sprite(darkness, offsets[i][0], offsets[i][1], ntw / 2,
                      nth / 2, nullptr, 0, 0));
      set_sprite(i, *sprite);
    }
  } break;
  case freeciv::DARKNESS_CARD_SINGLE:
    for (int i = 0; i < tileset_num_cardinal_dirs(tileset()); i++) {
      enum direction8 dir = tileset_cardinal_dirs(tileset())[i];

      auto buffer =
          QStringLiteral("tx.darkness_%1").arg(dir_get_tileset_name(dir));

      const auto sprite = load_sprite(tileset(), buffer);
      if (sprite) {
        set_sprite(i, *sprite);
      } else {
        tileset_error(tileset(), LOG_FATAL,
                      _("Sprite for tag '%s' missing."),
                      qUtf8Printable(buffer));
      }
    }
    break;
  case freeciv::DARKNESS_CARD_FULL:
    for (int i = 1; tileset_num_index_cardinals(tileset()); i++) {
      auto buffer = QStringLiteral("tx.darkness_%1")
                        .arg(cardinal_index_str(tileset(), i));

      const auto sprite = load_sprite(tileset(), buffer);
      if (sprite) {
        set_sprite(i, *sprite);
      } else {
        tileset_error(tileset(), LOG_FATAL,
                      _("Sprite for tag '%s' missing."),
                      qUtf8Printable(buffer));
      }
    }
    break;
  case freeciv::DARKNESS_CORNER:
    // Handled in LAYER_FOG.
    break;
  };
}

std::vector<drawn_sprite>
layer_darkness::fill_sprite_array(const tile *ptile, const tile_edge *pedge,
                                  const tile_corner *pcorner,
                                  const unit *punit) const
{
  Q_UNUSED(pedge);
  Q_UNUSED(pcorner);

  if (!ptile) {
    return {};
  }

  // Don't draw darkness when the solid background is used
  if (solid_background(ptile, punit, tile_city(ptile))) {
    return {};
  }

  auto sprites = std::vector<drawn_sprite>();

  int i, tileno;
  struct tile *adjc_tile;

  const auto is_unknown = [&](direction8 dir) {
    return ((adjc_tile = mapstep(&(wld.map), ptile, (dir)))
            && client_tile_get_known(adjc_tile) == TILE_UNKNOWN);
  };

  switch (m_style) {
  case DARKNESS_NONE:
    break;
  case DARKNESS_ISORECT:
    for (i = 0; i < 4; i++) {
      const int W = tileset_tile_width(tileset()),
                H = tileset_tile_height(tileset());
      int offsets[4][2] = {{W / 2, 0}, {0, H / 2}, {W / 2, H / 2}, {0, 0}};

      if (is_unknown(DIR4_TO_DIR8[i])) {
        sprites.emplace_back(tileset(), &m_sprites[i], true, offsets[i][0],
                             offsets[i][1]);
      }
    }
    break;
  case DARKNESS_CARD_SINGLE:
    for (i = 0; i < tileset_num_cardinal_dirs(tileset()); i++) {
      if (is_unknown(tileset_cardinal_dirs(tileset())[i])) {
        sprites.emplace_back(tileset(), &m_sprites[i]);
      }
    }
    break;
  case DARKNESS_CARD_FULL:
    /* We're looking to find the INDEX_NSEW for the directions that
     * are unknown.  We want to mark unknown tiles so that an unreal
     * tile will be given the same marking as our current tile - that
     * way we won't get the "unknown" dither along the edge of the
     * map. */
    tileno = 0;
    for (i = 0; i < tileset_num_cardinal_dirs(tileset()); i++) {
      if (is_unknown(tileset_cardinal_dirs(tileset())[i])) {
        tileno |= 1 << i;
      }
    }

    if (tileno != 0) {
      sprites.emplace_back(tileset(), &m_sprites[tileno]);
    }
    break;
  case DARKNESS_CORNER:
    // Handled in LAYER_FOG.
    break;
  };

  return sprites;
}

} // namespace freeciv
