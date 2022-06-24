
#ifndef _CJSCH_ACTION_H_
#define _CJSCH_ACTION_H_

#include <cj/cjsch/cjsch_def.h>


typedef struct cjsch_action_setting_t {

	cji							rev;
	cjbool						instant_run_able;

} cjsch_action_setting;


typedef enum cjsch_action_actor_type_t {
	CJSCH_ACTION_ACTOR_TYPE_NONE,
	CJSCH_ACTION_ACTOR_TYPE_PROGRAM,
	CJSCH_ACTION_ACTOR_TYPE_MAIL,
	CJSCH_ACTION_ACTOR_TYPE_MESSAGE,
	CJSCH_ACTION_ACTOR_TYPE_FUNCTOR,

	CJSCH_ACTION_ACTOR_TYPE_MAX

} cjsch_action_actor_type;


typedef struct cjsch_action_actor_t {

	cjsch_action_actor_type		type;

	union {
		struct {
			cjbool enable;
			cjmc   path[CJ_MAX_CJSCH_ACTOR_PROG_PATH];
			cjmc   work_path[CJ_MAX_CJSCH_ACTOR_PROG_PATH];
			cjmc   arg[CJ_MAX_CJSCH_ACTOR_PROG_ARG];
		} prog;

		struct {
			cjbool enable;
			cjmc   mail_addr[CJ_MAX_CJSCH_ACTOR_MAIL_ADDR];
		} mail;

		struct {
			cjbool enable;
			cjmc   text[CJ_MAX_CJSCH_ACTOR_MSG_TEXT];
			void* prm;
			cjmsiz_t prm_siz;
		} msg;

		struct {
			cjbool enable;
			cjmc   class[CJ_MAX_CJSCH_CLASS_NAME];
			cjmc   arg[CJ_MAX_CJSCH_ACTOR_FUNCTOR_ARG];
			void* prm;
			cjmsiz_t prm_siz;
		} functor;

	} dat;

} cjsch_action_actor;


typedef struct cjsch_action_t {
    cjsch_ch                        name[CJ_MAX_CJSCH_ACTION_NAME];
    cjsch_ch                        class[CJ_MAX_CJSCH_CLASS_NAME];

    cji                             rev;
    cjbool                          enable;

    cjsch_action_setting            setting;
    cjsch_action_actor              actor;

} cjsch_action;


CJEXTERNC void cjsch_action_destroy(cjsch_action* action);


#endif


/*
  actions:
    - name: action00
      id : web_crolling_stock
      rev: 1
      enable: 1
      setting:
        rev: 1
        instant_run_able: 1

      # none, program , mail, message, functor
      actor_type: program
      actor:
        program:
          enable: 1
          path: ""
          arg: ""
          work_path: ""
        mail:
          enable: 1
          address: test@cjsong.com
        message:
          enable: 1
          text: "hello"
        functor:
          enable: 1
          class: cjsch_action_xxx
*/


