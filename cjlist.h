
#ifndef _CJLIST_H_
#define _CJLIST_H_


#define cjlist_push_back(link, elem) { \
	cjassert(elem->_list_next_==0); \
	cjassert(elem->_list_prev_==0); \
\
	if(link) { \
	    elem->_list_prev_ = link; \
	    elem->_list_next_ = link->_list_next_; \
\
        if(elem->_list_next_) \
            elem->_list_next_->_list_prev_ = elem; \
\
        link->_list_next_ = elem; \
	} else { \
		link = elem; \
	} \
\
}


#define cjlist_push_front(link, elem) { \
	cjassert(elem->_list_next_==0); \
	cjassert(elem->_list_prev_==0); \
\
	if(link) { \
        elem->_list_next_ = link; \
	    elem->_list_prev_ = link->_list_prev_; \
\
	    if (elem->_list_prev_) \
		    elem->_list_prev_->_list_next_ = elem; \
\
	    link->_list_prev_ = elem; \
    } else { \
		link = elem; \
	} \
}


#define cjlist_remove(link) { \
	if(link) { \
        if (link->_list_prev_) \
		    link->_list_prev_->_list_next_ = link->_list_next_; \
\
	    if (link->_list_next_) \
            link->_list_next_->_list_prev_ = link->_list_prev_; \
\
        link->_list_prev_ = link->_list_next_ = 0; \
	} \
}


#endif
