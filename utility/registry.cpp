/***********************************************************************
 Freeciv - Copyright (C) 1996 - A Kjeldberg, L Gregersen, P Unold
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/

#ifdef HAVE_CONFIG_H
#include <fc_config.h>
#endif

#include "registry.h"

/*********************************************************************/ /**
   Create a section file from a file.  Returns NULL on error.
 *************************************************************************/
struct section_file *secfile_load(const char *filename,
                                  bool allow_duplicates)
{
  return secfile_load_section(filename, NULL, allow_duplicates);
}
