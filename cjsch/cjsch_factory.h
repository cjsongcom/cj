
#ifndef _CJSCH_FACTORY_H_
#define _CJSCH_FACTORY_H_


typedef struct cjsch_trigger_t cjsch_trigger;
typedef struct cjsch_action_t cjsch_action;

enum {
	CJ_MAX_CJSCH_FACTORY_DEF_HASH_BUCKET = 64
};


typedef struct cjsch_factory_prm_t {
	void* prm;
	cji hash_bucket_siz;

} cjsch_factory_prm;


typedef struct cjsch_factory_t {

	// cjsch_trigger
	cjhash							triggers;

	// cjsch_action
	cjhash							actions;

	cjsch_factory_prm*				prm;

} cjsch_factory;


CJEXTERNC cjbool cjsch_factory_create(cjsch_factory** out, cjsch_factory_prm* prm);
CJEXTERNC void cjsch_factory_destroy(cjsch_factory* factory);

CJEXTERNC cjbool cjsch_factory_update(cjsch_factory* factory, cjtime_ms curtime);

CJEXTERNC cjbool cjsch_factory_register_trigger(cjsch_factory* factory, cjsch_trigger* trigger, 
	const cjmc* trigger_name);

CJEXTERNC cjbool cjsch_factory_register_action(cjsch_factory* factory, cjsch_action* action,
	const cjmc* action_name);

CJEXTERNC cjbool cjsch_factory_find_trigger(cjsch_trigger** out_trigger, 
	cjsch_factory* factory, const cjmc* trigger_name);

CJEXTERNC cjbool cjsch_factory_find_action(cjsch_action** out_action, 
	cjsch_factory* factory, const cjmc* action_name);


#endif
