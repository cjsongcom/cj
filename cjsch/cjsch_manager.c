
#include <cj/cj.h>

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_trigger_inst.h>
#include <cj/cjsch/cjsch_action.h>
#include <cj/cjsch/cjsch_action_inst.h>
#include <cj/cjsch/cjsch_factory.h>

#include <cj/cjsch/cjsch_manager.h>


CJEXTERNC cjbool cjsch_man_create(cjsch_manager** out_man, cjsch_manager_prm* prm){
		
	if (!prm)
		return cjfalse;

	cjsch_manager* man = cjmzalloc(sizeof(cjsch_manager));

	man->user_prm = prm->user_prm;
	man->sch_factory = prm->sch_factory;

	*out_man = man;

	return cjtrue;
}

CJEXTERNC void cjsch_man_destroy(cjsch_manager* man) {

	if (!man)
		return;

	cjmfree(man);
}

CJEXTERNC cjbool cjsch_man_create_trigger(cjsch_manager* man, const cjmc* trigger_name,
	cjsch_man_err* out_err) {
	
	cjassert(man);
	cjassert(trigger_name);
	cjassert(man->sch_factory);

	cjsch_trigger* trigger = cjNULL;

	if (!cjsch_factory_find_trigger(&trigger, man->sch_factory, trigger_name))
		return cjfalse;

	cjsch_trigger_inst* inst 
		= cjmzalloc(sizeof(cjsch_trigger_inst));

	inst->_src = trigger;

	inst->st_type = trigger->st_type;
	inst->st_datetime = trigger->st_datetime;

	inst->expire_enable = trigger->expire_enable;
	inst->expire_datetime = trigger->expire_datetime;

	inst->period = trigger->period;
	inst->advance = trigger->advance;

	cjlist_push_back(man->trigger_insts, inst);


	return cjtrue;
}

