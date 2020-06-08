

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

gh_file_utils.cpp

funcs to manage files.

--------------------------------------------------------------*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "gh_file_utils.h"

bool
gh_make_dir(gh_string_t the_pth, mode_t mod){
	int resp = mkdir(the_pth.c_str(), mod);
	return (resp == 0);
}

bool
gh_file_exists(gh_string_t th_pth){
	const char* fname = th_pth.c_str();
	
	bool ff_exists = (access(fname, F_OK) != -1);
	return ff_exists;
}

void
gh_copy_file(gh_string_t src, gh_string_t dst, gh_buffer_t& buff, const char* mod)
{
	long sz_dat = (long)buff.size();
	void* dat = (void*)buff.data();
	FILE*	src_ff = fopen(src.c_str(), "r");
	FILE*	dst_ff = fopen(dst.c_str(), mod);

	while(! feof(src_ff)) {
		size_t num_rd = fread(dat, 1, sz_dat, src_ff);
		if (num_rd) {
			fwrite(dat, 1, num_rd, dst_ff);
		}
	}
	fclose(src_ff);
	fclose(dst_ff);
}

void
gh_append_file(gh_string_t src, gh_string_t dst, gh_buffer_t& buff)
{
	gh_copy_file(src, dst, buff, "a");
}

bool
gh_path_create(gh_string_t n_pth){
	int eos = (int)gh_string_t::npos;
	int pos1 = n_pth.find(gh_path_sep_char);
	bool path_ok = true;
	while((pos1 == eos) || (pos1 < (int)n_pth.size())){
		if(pos1 == eos){
			pos1 = (int)n_pth.size();
		}

		gh_string_t nm_dir = n_pth.substr(0, pos1);
		if(nm_dir.size() > 0){
			path_ok = gh_make_dir(nm_dir, 0700);
		}

		if((pos1 + 1) < (int)n_pth.size()){
			pos1 = n_pth.find(gh_path_sep_char, pos1 + 1);
		} else {
			pos1 = (int)n_pth.size();
		}
	}

	return path_ok;
}

gh_string_t
gh_get_executable_path(){
	char exepath[GH_PATH_MAX] = {0};
	readlink("/proc/self/exe", exepath, sizeof(exepath) - 1);
	gh_string_t the_pth = exepath;
	return the_pth;
}

gh_string_t
gh_path_get_directory(gh_string_t the_pth, bool add_last_dir_sep){
	long pos = (long)the_pth.rfind(gh_path_sep_char);
	gh_string_t the_dir = the_pth.substr(0, pos);
	if(add_last_dir_sep){
		the_dir = the_dir + gh_path_sep_char;
	}
	return the_dir;
}

gh_string_t 
gh_path_get_name(gh_string_t the_pth){
	long eos = (long)gh_string_t::npos;
	long pos = (long)the_pth.rfind(gh_path_sep_char);
	if(pos == eos){
		return the_pth;
	}
	gh_string_t the_nm = the_pth.substr(pos + 1);
	return the_nm;
}

