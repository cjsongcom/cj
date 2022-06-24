
#ifndef _CJSCH_FACTORY_H_
#define _CJSCH_FACTORY_H_


typedef struct cjsch_factory_t {
	cjary				loaded_triggers;
	cjary				loaded_actions;




} cjsch_factory;


typedef struct cjsch_factory_prm_t {
	cjary				loaded_triggers;
	cjary				loaded_actions;

} cjsch_factory_prm;


CJEXTERNC cjbool cjsch_factory_create(cjsch_factory** out, cjsch_factory_prm* prm);
CJEXTERNC void cjsch_factory_destroy(cjsch_factory* factory);

CJEXTERNC cjbool cjsch_factory_update(cjsch_factory* factory, cjtime_ms curtime);

CJEXTERNC cjbool cjsch_factory_add_trigger(cjsch_factory* factory, cjsch_trigger* trigger, 
	const cjmc* trigger_name);
CJEXTERNC cjbool cjsch_factory_add_action(cjsch_factory* factory, cjsch_action* action,
	const cjmc* action_name);



#endif
