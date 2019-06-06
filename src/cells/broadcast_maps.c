

/*************************************************************

This file is part of messaging-cells.

messaging-cells is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

messaging-cells is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://messaging-cells.github.io/

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------*/
/*! \file broadcast_maps.c 

\brief Standard parallel broadcast maps (they are also used a parallel loading maps).

\include broadcast_maps.c 

------------------------------------------------------------*/

#include "global.h"

mc_dbg_e3_leaf(15)
mc_dbg_e3_leaf(14)
mc_dbg_e3_leaf(13)
mc_dbg_e3_leaf(12)
mc_dbg_e3_leaf(11)
mc_dbg_e3_leaf(10)
mc_dbg_e3_leaf(9)
mc_dbg_e3_leaf(8)
mc_dbg_e3_leaf(7)
mc_dbg_e3_leaf(6)
mc_dbg_e3_leaf(5)
mc_dbg_e3_leaf(4)
mc_dbg_e3_leaf(3)
mc_dbg_e3_leaf(2)
mc_dbg_e3_leaf(1)

mc_dbg_e3_tree(0, 
	mc_dbg_e3_node(1),
	mc_dbg_e3_node(2),
	mc_dbg_e3_node(3),
	mc_dbg_e3_node(4),
	mc_dbg_e3_node(5),
	mc_dbg_e3_node(6),
	mc_dbg_e3_node(7),
	mc_dbg_e3_node(8),
	mc_dbg_e3_node(9),
	mc_dbg_e3_node(10),
	mc_dbg_e3_node(11),
	mc_dbg_e3_node(12),
	mc_dbg_e3_node(13),
	mc_dbg_e3_node(14),
	mc_dbg_e3_node(15)
)


mc_std_e3_leaf(7)
mc_std_e3_leaf(8)
mc_std_e3_leaf(9)
mc_std_e3_leaf(10)
mc_std_e3_leaf(11)
mc_std_e3_leaf(12)
mc_std_e3_leaf(13)
mc_std_e3_leaf(14)
mc_std_e3_leaf(15)

mc_std_e3_tree(3, 
	mc_std_e3_node(7),
	mc_std_e3_node(8)
)

mc_std_e3_tree(4, 
	mc_std_e3_node(9),
	mc_std_e3_node(10)
)

mc_std_e3_tree(5, 
	mc_std_e3_node(11),
	mc_std_e3_node(12)
)

mc_std_e3_tree(6, 
	mc_std_e3_node(13),
	mc_std_e3_node(14),
	mc_std_e3_node(15)
)

mc_std_e3_tree(1, 
	mc_std_e3_node(3),
	mc_std_e3_node(4)
)

mc_std_e3_tree(2, 
	mc_std_e3_node(5),
	mc_std_e3_node(6)
)

mc_std_e3_tree(0, 
	mc_std_e3_node(1),
	mc_std_e3_node(2)
)

mc_load_map_st*
mc_get_first_load_map(){
	return mc_std_e3_node(0);
}


