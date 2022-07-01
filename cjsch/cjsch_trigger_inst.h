
#ifndef _CJSCH_TRIGGER_INST_H_
#define _CJSCH_TRIGGER_INST_H_

typedef struct cjsch_trigger_t cjsch_trigger;
typedef struct cjsch_trigger_inst_t cjsch_trigger_inst;


typedef struct cjsch_trigger_inst_t {
    cjsch_trigger_inst*                 _list_next_;
    cjsch_trigger_inst*                 _list_prev_;

	cjsch_trigger*						_src; // readonly
	
	cjsch_trigger_start_type			st_type;
	cjdatetime							st_datetime;

    cjbool								expire_enable;
    cjdatetime							expire_datetime;

	cjsch_trigger_period				period;
	cjsch_trigger_advance				advance;

} cjsch_trigger_inst;


#endif
