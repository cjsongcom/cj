
#include <cj/cj.h>
#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_action.h>

#include <cj/cjsch/cjsch_factory.h>


CJEXTERNC void _cjsch_fact_rm_trigger(void* _pair) {
	cjhash_pair* pair = _pair;

	cjhash_mfree(pair->key);
	cjsch_trigger_destroy(pair->val);
}

CJEXTERNC void _cjsch_fact_rm_action(void* _pair) {
	cjhash_pair* pair = _pair;

	cjhash_mfree(pair->key);
	cjsch_action_destroy(pair->val);
}

CJEXTERNC cjbool cjsch_factory_create(cjsch_factory** out, cjsch_factory_prm* prm) {

	cjsch_factory* fact 
		= cjmzalloc(sizeof(cjsch_factory));

	cjhash_siz bucket_siz = 0;

	fact->prm = prm;

	if (prm) {
		bucket_siz = prm->hash_bucket_siz;
	}

	if (bucket_siz == 0)
		bucket_siz = CJ_MAX_CJSCH_FACTORY_DEF_HASH_BUCKET;

	cjhash_new_str_ptr_placed(&fact->triggers, bucket_siz);
	cjhash_new_str_ptr_placed(&fact->actions, bucket_siz);

	cjhash_set_func(&fact->triggers, cjNULL, cjNULL, _cjsch_fact_rm_trigger);
	cjhash_set_func(&fact->actions, cjNULL, cjNULL, _cjsch_fact_rm_action);

	*out = fact;

	return cjtrue;
}

CJEXTERNC void cjsch_factory_destroy(cjsch_factory* factory) {

	if (!factory)
		return;

	cjhash_del(&factory->triggers);
	cjhash_del(&factory->actions);

	cjmfree(factory);
}

CJEXTERNC cjbool cjsch_factory_update(cjsch_factory* factory, cjtime_ms curtime) {

	cjassert(factory);

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_register_trigger(cjsch_factory* factory, cjsch_trigger* trigger,
	const cjmc* trigger_name) {

	cjassert(factory);
	cjassert(trigger);
	cjassert(trigger_name);

	cjhash_pair* pair = cjhash_find(&factory->triggers, trigger_name);

	if (pair)
		return cjfalse;

	if (!cjhash_push_str_ptr(&factory->triggers, trigger_name, trigger))
		return cjfalse;

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_register_action(cjsch_factory* factory, cjsch_action* action,
	const cjmc* action_name) {

	cjassert(factory);
	cjassert(action);
	cjassert(action_name);

	cjhash_pair* pair = cjhash_find(&factory->actions, action_name);

	if (pair)
		return cjfalse;

	if (!cjhash_push_str_ptr(&factory->actions, action_name, action))
		return cjfalse;

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_find_trigger(cjsch_trigger** out_trigger,
	cjsch_factory* factory, const cjmc* trigger_name) {

	cjassert(out_trigger);
	cjassert(factory);
	cjassert(trigger_name);

	cjhash_pair* pair = cjhash_find(&factory->triggers, trigger_name);

	if (!pair)
		return cjfalse;

	if (out_trigger)
		*out_trigger = pair->val;

	return cjtrue;
}

CJEXTERNC cjbool cjsch_factory_find_action(cjsch_action** out_action,
	cjsch_factory* factory, const cjmc* action_name) {

	cjassert(out_action);
	cjassert(factory);
	cjassert(action_name);

	cjhash_pair* pair = cjhash_find(&factory->actions, action_name);

	if (!pair)
		return cjfalse;

	if (out_action)
		*out_action = pair->val;

	return cjtrue;
}

