
#include <cj/cj.h>

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>


CJEXTERNC void cjsch_trigger_destroy(cjsch_trigger* trig) {

	if (!trig)
		return;

	cjmfree(trig);
}
