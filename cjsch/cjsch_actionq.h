
#ifndef _CJSCH_ACTIONQ_H_
#define _CJSCH_ACTIONQ_H_


typedef struct cjsch_actionq_t {

	cjsch_acmd*					_top;


} cjsch_actionq;


cjbool cjsch_actionq_push(cjsch_actionq* q, cjsch_acmd_id id, cjsch_acmd_prm* prm);



#endif

