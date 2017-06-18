
/*------------------------------------------------------------

bjh_stack_trace.h

func to print a stack trace.

--------------------------------------------------------------*/

#ifndef MC_STACK_TRACE_H
#define MC_STACK_TRACE_H

#include "string_hst.h"

bjh_string_t	bjh_get_stack_trace( const bjh_string_t & file, int line );

#define STACK_STR bjh_get_stack_trace(__FILE__, __LINE__)


#endif		// MC_STACK_TRACE_H


