
#include <cj/cj.h>
#include <cj/cjary.h>


CJEXTERNC cjary* cjary_new(cjary_idx capacity, cjary_idx elem_siz) {

    cjary* rst = cjmzalloc(sizeof(cjary));

    rst->elem_siz = elem_siz;
    rst->capacity = capacity;

    if (capacity > 0)
        rst->data = cjmzalloc(rst->elem_siz * capacity);

    return rst;
}

CJEXTERNC cjary* cjary_new_ptr(cjary_idx capacity) {
    cjary* rst;

    rst = cjary_new(capacity, sizeof(void*));
    rst->is_ptr_ary = cjtrue;

    return rst;
}

CJEXTERNC cjary* cjary_new_placed(cjary* ary, cjary_idx capacity, cjary_idx elem_siz) {

    if (!ary)
        return cjNULL;

    cjmemzero(ary, sizeof(cjary));

    ary->elem_siz = elem_siz;
    ary->capacity = capacity;
    ary->is_placed = cjtrue;

    if (capacity > 0)
        ary->data = cjmzalloc(ary->elem_siz * capacity);

    return ary;
}

CJEXTERNC cjary* cjary_new_ptr_placed(cjary* ary, cjary_idx capacity) {
    if (!ary)
        return cjNULL;

    cjary* rst;

    rst = cjary_new_placed(ary, capacity, sizeof(void*));
    rst->is_ptr_ary = cjtrue;

    return rst;
}

CJEXTERNC void cjary_del(cjary* ary) {
    if (!ary)
        return;

    cjmfree(ary->data);
    ary->data = cjNULL;

    if(!ary->is_placed)
        cjmfree(ary);
}

CJEXTERNC void cjary_del_placed(cjary* ary) {

    if (!ary)
        return;

    cjary_clear(ary);

    cjmfree(ary->data);
    ary->data = cjNULL;
}

CJEXTERNC cjary_idx cjary_siz(cjary* ary) {
    return ary->siz;
}

CJEXTERNC cjary_idx cjary_capacity(cjary* ary) {
    return ary->capacity;
}

CJEXTERNC cjary_idx cjary_resize(cjary* ary, cjary_idx new_capacity) {
    cjassert(ary);

    void* new_data = cjNULL;
    cjary_idx copy_cnt = 0;

    //todo: change to realloc
    if (new_capacity > 0) {
        new_data = cjmzalloc(ary->elem_siz * new_capacity);

        if(new_data) {
            if (ary->data && ary->siz > 0)
                copy_cnt = ary->siz;

            if (copy_cnt)
                cjmemcpy(new_data, ary->data, ary->elem_siz * copy_cnt);

        } else {
            return -1;
        }
    }

    cjmfree(ary->data);

    ary->data = new_data;

    ary->siz = copy_cnt;
    ary->capacity = new_capacity;

    return new_capacity;
}

CJEXTERNC void* cjary_get_ptr(cjary* ary, cjary_idx idx) {

    if (!ary->is_ptr_ary)
        return cjNULL;

    if (ary->data && idx < ary->capacity)
        return ((void**)ary->data)[idx];

    return cjNULL;
}

CJEXTERNC void* cjary_get_val(cjary* ary, cjary_idx idx, void* out) {

    if (ary->is_ptr_ary)
        return cjNULL;

    if (ary->data && idx < ary->capacity) {
        cjc* dst = ((char*)ary->data + (ary->elem_siz * idx));

        if (out)
            cjmemcpy(out, dst, ary->elem_siz);

        return (void*)dst;
    }

    return cjNULL;
}

CJEXTERNC cjbool cjary_set_ptr(cjary* ary, cjary_idx idx, void* p) {

    if (!ary->is_ptr_ary)
        return cjfalse;

    if (ary->data && idx < ary->siz) {
        ((void**)ary->data)[idx] = p;

        return cjtrue;
    }

    return cjfalse;
}

CJEXTERNC cjbool cjary_set_val(cjary* ary, cjary_idx idx, void* val) {

    if (ary->is_ptr_ary)
        return cjfalse;

    if (ary->data && idx < ary->siz) {
        cjmemcpy(((cjc*)ary->data + (ary->elem_siz * idx)), val, ary->elem_siz);

        return cjtrue;
    } 

    return cjfalse;
}

CJEXTERNC cjbool cjary_push_ptr(cjary* ary, void* p)     {

    if (!ary->is_ptr_ary)
        return cjfalse;

    // check maximum capacity
    if (ary->data) {
        if (ary->capacity == ary->siz) {
            if (-1 == cjary_resize(ary, ary->capacity * 2))
                return cjfalse;
        }

    } else {
        if (-1 == cjary_resize(ary, 2))
            return cjfalse;
    }

    ((void**)ary->data)[ary->siz] = p;

    ary->siz++;

    return cjtrue;
}

CJEXTERNC cjbool cjary_push_val(cjary* ary, void* val) {

    if (ary->is_ptr_ary)
        return cjfalse;

    // check maximum capacity
    if (ary->data) {
        if (ary->capacity == ary->siz) {
            if (-1 == cjary_resize(ary, ary->capacity * 2))
                return cjfalse;
        }

    } else {
        if (-1 == cjary_resize(ary, 2))
            return cjfalse;
    }

    cjmemcpy(((char*)ary->data + (ary->elem_siz * ary->siz)), val, ary->elem_siz);
    ary->siz++;

    return cjtrue;
}

CJEXTERNC cjbool cjary_erase(cjary* ary, cjary_idx idx) {

    if (!ary->siz)
        return cjfalse;

    if (ary->data && idx < ary->siz) {

        cjsiz_t mov_cnt = ary->siz - (idx + 1);

        if (mov_cnt > 0) {
            cjmemmove((cjc*)ary->data + (ary->elem_siz * idx),
                (cjc*)ary->data + (ary->elem_siz * (idx + 1)), ary->elem_siz*mov_cnt);
        }

        // clean last element
        cjmemzero((cjc*)ary->data + (ary->elem_siz * (ary->siz-1)), ary->elem_siz);

        ary->siz--;

        return cjtrue;
    }

    return cjfalse;
}

CJEXTERNC void cjary_clear(cjary* ary) {
    ary->siz = 0;
}

CJEXTERNC void cjary_clear_zero(cjary* ary)     {
    ary->siz = 0;
    cjmemzero(ary->data, ary->elem_siz * ary->capacity);
}

CJEXTERNC cjbool cjary_empty(cjary* ary) {

    if (ary->siz)
        return cjtrue;

    return cjfalse;
}








//
//
//static void vector_resize(vector* v, int capacity) {
//#ifdef DEBUG_ON
//    printf("vector_resize: %d to %d\n", v->capacity, capacity);
//#endif
//
//}
//
//void vector_add(vector* v, void* item) {
//    if (v->capacity == v->total)
//        vector_resize(v, v->capacity * 2);
//    v->items[v->total++] = item;
//}
//
//void vector_set(vector* v, int index, void* item) {
//    if (index >= 0 && index < v->total)
//        v->items[index] = item;
//}
//
//void* vector_get(vector* v, int index) {
//    if (index >= 0 && index < v->total)
//        return v->items[index];
//    return NULL;
//}
//
//void vector_delete(vector* v, int index) {
//    if (index < 0 || index >= v->total)
//        return;
//
//    v->items[index] = NULL;
//    int i;
//    for (i = 0; i < v->total - 1; i++) {
//        v->items[i] = v->items[i + 1];
//        v->items[i + 1] = NULL;
//    }
//
//    v->total--;
//
//    if (v->total > 0 && v->total == v->capacity / 4)
//        vector_resize(v, v->capacity / 2);
//}
//
//void vector_free(vector* v) {
//    free(v->items);
//}

