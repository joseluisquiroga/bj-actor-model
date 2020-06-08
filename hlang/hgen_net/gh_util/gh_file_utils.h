

/*************************************************************

This file is part of hgen_net.

hgen_net is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

hgen_net is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with hgen_net.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2020. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/hgen_net

hgen_net is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

gh_file_utils.h

func to manage files.

NOTE: In May 2020 gcc the ubuntu version is 7.5.0 
(NOT the current GNU version which is 10.1) 
and the c++ std::filesystem library is still experimental.
So in order to keep the program simple to compile for everyone
the library is not used.

--------------------------------------------------------------*/

#ifndef GH_FILE_UTILS_H
#define GH_FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <array>

#include "gh_dbg_util.h"

#define gh_string_t std::string

typedef std::array<int8_t, 10000> gh_buffer_t;

#define GH_PATH_MAX 500  // should be PATH_MAX as in limits.h

#define gh_path_sep "/"
#define gh_path_sep_char '/'

bool gh_make_dir(gh_string_t the_pth, mode_t mod);
bool gh_file_exists(gh_string_t th_pth);
void gh_copy_file(gh_string_t src, gh_string_t dst, gh_buffer_t& buff, const char* mod = "w");
void gh_append_file(gh_string_t src, gh_string_t dst, gh_buffer_t& buff);
bool gh_path_create(gh_string_t n_pth);
gh_string_t gh_get_executable_path();
gh_string_t gh_path_get_directory(gh_string_t the_pth, bool add_last_dir_sep);
gh_string_t gh_path_get_name(gh_string_t the_pth);



#endif		// GH_FILE_UTILS_H


