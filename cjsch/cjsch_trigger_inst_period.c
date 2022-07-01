
#include <cj/cj.h>

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_trigger_inst.h>

#include <cj/cjsch/cjsch_trigger_inst_period.h>



static cjbool _cjsch_check_period(cjsch_trigger_period* period) {

	cjassert(period);

	switch (period->type) {

	case CJ_PERIOD_NONE:
		return cjtrue;

	case CJ_PERIOD_ONCE:
		return cjtrue;

	case CJ_PERIOD_EVERY_DAY: {
		period->dat.every_day.per_day = 
	}
	break;

	case CJ_PERIOD_EVERY_WEEK: {

	}


	case CJ_PERIOD_EVERY_MONTH:




	}



	return cjtrue;
}


CJEXTERNC cjbool cjsch_trigger_inst_check_period(cjsch_trigger_inst* inst) {

	cjassert(inst);

	_cjsch_check_period(&inst->period);


	return cjtrue;
}