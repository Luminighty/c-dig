#ifndef WORK_H
#define WORK_H

#include <stdint.h>
#include <stdbool.h>


typedef uint32_t WorkUnit;

typedef enum {
	WORK_STATUS_NONE,
	WORK_STATUS_ACTIVE,
	WORK_STATUS_FINISHED,
} WorkStatus;


typedef struct {
	WorkStatus status;
	WorkUnit remaining;
	WorkUnit total;
} Work;


void work_print(Work* work);

static inline void work_start(Work* work, WorkUnit work_amount) {
	work->total = work_amount;
	work->remaining = work_amount;
	work->status = WORK_STATUS_ACTIVE;
}


static inline bool work_progress(Work* work, WorkUnit progress) {
	if (work->status != WORK_STATUS_ACTIVE)
		return false;
	if (work->remaining <= progress) {
		work->remaining = 0;
		work->status = WORK_STATUS_FINISHED;
		return true;
	} else {
		work->remaining -= progress;
		return false;
	}
}

// NOTE: Consider adding finish/abort if you need to know the difference as a state
static inline void work_reset(Work* work) {
	work->status = WORK_STATUS_NONE;
}

// Resets the work if it's invalid
#define work_validate(work, is_valid) if (!(is_valid)) work_reset(work)
#define work_is_inactive(work) ((work)->status != WORK_STATUS_ACTIVE)

// Returns true if the work has been finished
static inline bool work_full_update(Work* work, bool precondition, WorkUnit total, WorkUnit progress) {
	if (work_is_inactive(work)) {
		if (precondition) {
			work_start(work, total);
		} else {
			return false;
		}
	}
	if (!work_progress(work, progress)) {
		work_validate(work, precondition);
		return false;
	}
	work_reset(work);
	return true;
}

#endif // WORK_H
