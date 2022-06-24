
#ifndef _CJSCH_MANAGER_H_
#define _CJSCH_MANAGER_H_


typedef struct cjsch_factory_t			cjsch_factory;

typedef struct cjsch_manager_t {

	// cjsch_trigger_inst
	cjary				trigger_insts;

	// cjsch_action_inst
	cjary				action_insts;

	cjsch_factory*		sch_factory;

} cjsch_manager;


typedef struct cjsch_mananger_prm_t {
	void* user_prm;

} cjsch_mananger_prm;


CJEXTERNC cjbool cjsch_man_create(cjsch_manager** out_man, cjsch_mananger_prm* prm);
CJEXTERNC void cjsch_man_destroy(cjsch_manager* man);

CJEXTERNC cjbool cjsch_man_update(cjsch_manager* man, cjtime_ms curtime, 
	cjtime_ms* out_elapsed);

CJEXTERNC cjbool cjsch_man_action(cjsch_manager* man, cjtime_ms curtime, 
	cjtime_ms* out_elapsed);


#endif