
#ifndef _CJSCH_MANAGER_H_
#define _CJSCH_MANAGER_H_

typedef struct cjsch_factory_t			cjsch_factory;

typedef struct cjsch_trigger_inst_t		cjsch_trigger_inst;
typedef struct cjsch_action_inst_t		cjsch_action_inst;


typedef enum cjsch_man_err_t {

	CJSCH_MAN_ERR_OK = 0,
	CJSCH_MAN_ERR_FAIL = 1,

} cjsch_man_err;


typedef struct cjsch_manager_t {

	// cjsch_trigger_inst
	cjsch_trigger_inst*				trigger_insts;

	// cjsch_action_inst
	cjsch_action_inst*				action_insts;

	cjsch_factory*					sch_factory;

	void*							user_prm;

	cjsch_man_err					last_err;

} cjsch_manager;


typedef struct cjsch_manager_prm_t {

	void* user_prm;
	cjsch_factory* sch_factory;

} cjsch_manager_prm;


CJEXTERNC cjbool cjsch_man_create(cjsch_manager** out_man, cjsch_manager_prm* prm);
CJEXTERNC void cjsch_man_destroy(cjsch_manager* man);

CJEXTERNC cjbool cjsch_man_update(cjsch_manager* man, cjtime_ms curtime, 
	cjtime_ms* out_elapsed);

CJEXTERNC cjbool cjsch_man_action(cjsch_manager* man, cjtime_ms curtime, 
	cjtime_ms* out_elapsed);

CJEXTERNC cjbool cjsch_man_create_trigger(cjsch_manager* man, const cjmc* trigger_name, 
	cjsch_man_err* out_err);


#endif