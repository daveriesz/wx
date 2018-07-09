
#include <stdio.h>

#include "dprjson.h"

int main(int argc, char **argv)
{
	dprJson *dj;
//	printf("Hello.\n");
	if(argv[1] == NULL) { return 1; }
	dj = dj_load_from_file(argv[1]);
//	dj_dump(dj, stdout);
	if(argv[2] != NULL)
	{
		djValue *dv = dj_get_value(dj, argv[2]);
		printf("%s: %s\n", argv[2], dj_value_to_string(dv));
//		dj_dump_value(dv, stdout); printf("\n");
//		printf("received: %p\n", dv);
	}
	dj_delete(dj);
	return 0;
}

