
#ifndef _CJ_CH_CONV_H_
#define _CJ_CH_CONV_H_


// wide character (unicode-16) convert library

// ret: writtern character count in buffer
//  > 0: failed to convert
//  > converted string count : because src is null terminated string,
//    so returned string count that include null terminator

// multi-byte to wide character
CJEXTERNC cjstr_siz cj_conv_mc_to_wc(cjwc* dst, cjstr_siz dst_siz, const cjmc* src);

// convert multi-byte as utf8 and write 
CJEXTERNC cjstr_siz cj_conv_mc_to_u8c(cju8c* dst, cjstr_siz dst_siz, const cjmc* src);

// if src_count is not -1, out_siz is not counted null terminiator 
CJEXTERNC cjmc* cj_conv_mc_to_u8c_mem(cjstr_siz* out_siz, const cjmc* src,
	cjstr_siz src_count, cjmalloc_pfn alloc_m, cjmfree_pfn free_m);


// utf8 to multi-byte
// if dst is null, only converted length is returned
CJEXTERNC cjstr_siz cj_conv_u8c_to_mc(cjmc* dst, cjstr_siz dst_siz, const cju8c* src);

// if src_count is not -1, out_siz is not counted null terminiator 
CJEXTERNC cjmc* cj_conv_u8c_to_mc_mem(cjstr_siz* out_siz, const cjmc* src,
	cjstr_siz src_count, cjmalloc_pfn alloc_m, cjmfree_pfn free_m);


// wide character to multi-byte
CJEXTERNC cjstr_siz cj_conv_wc_to_mc(cjmc* dst, cjstr_siz dst_siz, const cjwc* src);

// wide character to utf-8
CJEXTERNC cjstr_siz cj_conv_wc_to_u8c(cju8c* dst, cjstr_siz dst_siz, const cjwc* src);

// utf-8 to wide character
CJEXTERNC cjstr_siz cj_conv_u8c_to_wc(cjwc* dst, cjstr_siz dst_siz, const cju8c* src);


#endif
