
#ifndef _CJSCH_ACTION_CMD_H_
#define _CJSCH_ACTION_CMD_H_


typedef enum cjsch_acmd_id_t {

	CJSCH_ACMD_NONE,
	CJSCH_ACMD_CREATE_PROGRAM,

} cjsch_acmd_id;


typedef struct cjsch_acmd_t cjsch_acmd;

typedef struct cjsch_acmd_t {
	cjsch_acmd*				_next;
	cjsch_acmd*				_prev;

	cjsch_acmd_id			cmd_id;

} cjsch_acmd;


typedef struct cjsch_acmd_prm_t {
	void* prm;

} cjsch_acmd_prm;

#endif
