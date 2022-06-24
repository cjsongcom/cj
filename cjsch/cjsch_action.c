
#include <cj/cj.h>

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_action.h>


CJEXTERNC void cjsch_action_destroy(cjsch_action* action) {

	if (!action)
		return;

	cjmfree(action);
}

