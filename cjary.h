
#ifndef _CJARY_H_
#define _CJARY_H_

// https://www.sanfoundry.com/c-program-implement-vector/


typedef cji                 cjary_idx;


typedef struct cjary_t {
    void*                       data;
    cji                         elem_siz;

    cjary_idx                   siz;
    cjary_idx                   capacity;

    cjbool                      is_ptr_ary;
    cjbool                      is_placed;

} cjary;


CJEXTERNC cjary* cjary_new(cjary_idx capacity, cjary_idx elem_siz);
CJEXTERNC cjary* cjary_new_ptr(cjary_idx capacity);

CJEXTERNC cjary* cjary_new_placed(cjary* ary, cjary_idx capacity, cjary_idx elem_siz);
CJEXTERNC cjary* cjary_new_ptr_placed(cjary* ary, cjary_idx capacity);


CJEXTERNC void cjary_del(cjary* ary);
CJEXTERNC void cjary_del_placed(cjary* ary);

CJEXTERNC cjary_idx cjary_siz(cjary* ary);
CJEXTERNC cjary_idx cjary_capacity(cjary* ary);

CJEXTERNC cjary_idx cjary_resize(cjary* ary, cjary_idx new_capacity);

CJEXTERNC void* cjary_get_ptr(cjary* ary, cjary_idx idx);
CJEXTERNC void* cjary_get_val(cjary* ary, cjary_idx idx, void* out);

CJEXTERNC cjbool cjary_set_ptr(cjary* ary, cjary_idx idx, void* p);
CJEXTERNC cjbool cjary_set_val(cjary* ary, cjary_idx idx, void* val);

CJEXTERNC cjbool cjary_push_ptr(cjary* ary, void* p);
CJEXTERNC cjbool cjary_push_val(cjary* ary, void* val);

//CJEXTERNC void cjary_insert_ptr(cjary* ary, void* p);
//CJEXTERNC void cjary_insert_val(cjary* ary, void* val);

CJEXTERNC cjbool cjary_erase(cjary* ary, cjary_idx idx);

CJEXTERNC void cjary_clear(cjary* ary);
CJEXTERNC void cjary_clear_zero(cjary* ary);

CJEXTERNC cjbool cjary_empty(cjary* ary);


//
//int vector_total(vector*);
//static void vector_resize(vector*, int);
//void vector_add(vector*, void*);
//void vector_set(vector*, int, void*);
//void* vector_get(vector*, int);
//void vector_delete(vector*, int);
//void vector_free(vector*);
//
//
//
//
//
//
//
//
//#ifndef VECTOR_H
//#define VECTOR_H
//
//#define VECTOR_INIT_CAPACITY 4
//
//#define VECTOR_INIT(vec) vector vec; vector_init(&vec)
//#define VECTOR_ADD(vec, item) vector_add(&vec, (void *) item)
//#define VECTOR_SET(vec, id, item) vector_set(&vec, id, (void *) item)
//#define VECTOR_GET(vec, type, id) (type) vector_get(&vec, id)
//#define VECTOR_DELETE(vec, id) vector_delete(&vec, id)
//#define VECTOR_TOTAL(vec) vector_total(&vec)
//#define VECTOR_FREE(vec) vector_free(&vec)
//
//
//void vector_init(vector*);
//int vector_total(vector*);
//static void vector_resize(vector*, int);
//void vector_add(vector*, void*);
//void vector_set(vector*, int, void*);
//void* vector_get(vector*, int);
//void vector_delete(vector*, int);
//void vector_free(vector*);
//
//#endif
//
//
//
//int main(void) {
//    int i;
//
//    vector v;
//    vector_init(&v);
//
//    vector_add(&v, "Bonjour");
//    vector_add(&v, "tout");
//    vector_add(&v, "le");
//    vector_add(&v, "monde");
//
//    for (i = 0; i < vector_total(&v); i++)
//        printf("%s ", (char*)vector_get(&v, i));
//    printf("\n");
//
//    vector_delete(&v, 3);
//    vector_delete(&v, 2);
//    vector_delete(&v, 1);
//
//    vector_set(&v, 0, "Hello");
//    vector_add(&v, "World");
//
//    for (i = 0; i < vector_total(&v); i++)
//        printf("%s ", (char*)vector_get(&v, i));
//    printf("\n");
//
//    vector_free(&v);
//}



#endif
