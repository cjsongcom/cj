
#include <cj/cj.h>
#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_action.h>

#include <cj/cjsch/cjsch_factory.h>


CJEXTERNC cjbool cjsch_factory_create(cjsch_factory** out, cjsch_factory_prm* prm) {

	return cjtrue;
}

CJEXTERNC void cjsch_factory_destroy(cjsch_factory* factory) {

}

CJEXTERNC cjbool cjsch_factory_update(cjsch_factory* factory, cjtime_ms curtime) {

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_add_trigger(cjsch_factory* factory, cjsch_trigger* trigger,
	const cjmc* trigger_name) {

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_add_action(cjsch_factory* factory, cjsch_action* action,
	const cjmc* action_name) {

	return cjtrue;
}