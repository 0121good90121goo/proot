/****************************************/
/* The person responsible for writing this (in other words */
/* the one who you'd blame) is Adam. He has no idea what   */
/* he's doing. Thank you. -Adam.	 	           */

#include <stdio.h>

#include "arch.h"
#include "attribute.h"
#include "path/path.h"
#include "syscall/syscall.h"
#include "syscall/sysnum.h"
#include "tracee/mem.h"
#include "tracee/tracee.h"
#include "extension/extension.h"

int Android_callback(Extension *extension, ExtensionEvent event, 
		intptr_t data1, intptr_t data2 UNUSED) 
{

	switch (event) {
		case TRANSLATED_PATH: {
					      printf("INTERCEPTED TOUCHDOWN\n");
				      }
		break;
		
		default: fprintf(stderr, "Unknown event %d\n", event);
			 break;
	}

return 0;
}

