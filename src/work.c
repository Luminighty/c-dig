#include "work.h"
#include <stdio.h>

void work_print(Work* work) {
	printf("Work{ %d/%d, ", work->remaining, work->total);
	switch (work->status) {
	case WORK_STATUS_NONE:
		printf("NONE"); break;
	case WORK_STATUS_ACTIVE:
		printf("ACTIVE"); break;
	case WORK_STATUS_FINISHED:
		printf("FINISHED"); break;
	default:
		printf("UNKNOWN"); break;
	}
	printf(" }");
}
