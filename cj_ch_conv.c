
#include <cj/cj.h>
#include <cj/cj_ch_conv.h>


#ifdef WIN32

// multi-byte to wide character
CJEXTERNC cjstr_siz cj_conv_mc_to_wc(cjwc* dst, cjstr_siz dst_siz, const cjmc* src) {

	// cbMulitByte: can be -1, if the string is null-terminated
	// cchWideChar=0 to get length
		
	//MultiByteToWideChar does not null - terminate an output string 
	//if the input string length is explicitly specified without a terminating null character.
	// To null - terminate an output string for this function,
	// the application should pass in - 1 or explicitly count the terminating 
	// null character for the input string.

	// cbMulitByte = -1 used: src is null terminated string
	// conv_len : string_length + string terminator or 0 for failed to convert
	cjstr_siz conv_len = MultiByteToWideChar(CP_ACP, 0, src, -1, cjNULL, cjNULL);

	if (conv_len == 0) {
		// failed to convert
		if (dst)
			dst[0] = (cjwc)cjNULL;

		return 0;
	}

	if (!dst)
		return conv_len;

	if (dst_siz <= conv_len)
		conv_len = dst_siz; // with null terminator

	MultiByteToWideChar(CP_ACP, 0, src, -1, dst, conv_len);

	// -1 used, don't need appending null

	return conv_len;
}

// multi byte character to utf-8
CJEXTERNC cjstr_siz cj_conv_mc_to_u8c(cju8c* dst, cjstr_siz dst_siz, const cjmc* src) {

	cjstr_siz conv_wc_len = 0;
	cjstr_siz conv_u8c_len = 0;
	cjwc* wc_src = cjNULL;

	conv_wc_len = MultiByteToWideChar(CP_ACP, 0, src, -1, cjNULL, cjNULL);

	if (conv_wc_len == 0)
		goto exit_fail_conv;

	// included null terminiator
	wc_src = (cjwc*)cjmalloc(sizeof(cjwc) * conv_wc_len);

	if (!wc_src)
		goto exit_fail_conv;

	MultiByteToWideChar(CP_ACP, 0, src, -1, wc_src, conv_wc_len);

	// wide to utf8
	conv_u8c_len = WideCharToMultiByte(CP_UTF8, 0, wc_src, -1, cjNULL, 0, cjNULL, cjNULL);

	if (!dst) {
		cjmfree(wc_src);
		return conv_u8c_len;
	}

	if (!conv_u8c_len)
		goto exit_fail_conv;

	if (conv_u8c_len > dst_siz)
		conv_u8c_len = dst_siz;

	WideCharToMultiByte(CP_UTF8, 0, wc_src, -1, dst, conv_u8c_len, cjNULL, cjNULL);

	cjmfree(wc_src);

	return conv_u8c_len;


exit_fail_conv:
	cjmfree(wc_src);

	if (dst)
		dst[0] = (cju8c)cjNULL;

	return 0;
}


// multi byte character to utf8
CJEXTERNC cjmc* cj_conv_mc_to_u8c_mem(cjstr_siz* out_siz, const cjmc* src,
	cjstr_siz src_count, cjmalloc_pfn alloc_m, cjmfree_pfn free_m) {

	cjstr_siz conv_wc_len = 0;
	cjstr_siz conv_u8c_len = 0;
	cjwc* wc_src = cjNULL;
	cjmc* rst = cjNULL;
	 
	// convert count is not '-1', so returned length 'conv_wc_len' does not count null terminator
	conv_wc_len = MultiByteToWideChar(CP_ACP, 0, src, src_count, cjNULL, cjNULL);

	if (conv_wc_len == 0)
		goto exit_fail_conv;

	// src_count != -1: null terminator is does not included
	wc_src = CJMALLOC_P(alloc_m, sizeof(cjwc) * conv_wc_len);

	if (!wc_src)
		goto exit_fail_conv;

	MultiByteToWideChar(CP_ACP, 0, src, src_count, wc_src, conv_wc_len);

	// wide to utf8
	conv_u8c_len = WideCharToMultiByte(CP_UTF8, 0, wc_src, conv_wc_len, cjNULL, 0, cjNULL, cjNULL);

	if (!conv_u8c_len)
		goto exit_fail_conv;

	rst = CJMALLOC_P(alloc_m, sizeof(cjmc) * conv_u8c_len);

	WideCharToMultiByte(CP_UTF8, 0, wc_src, conv_wc_len, rst, conv_u8c_len, cjNULL, cjNULL);

	CJMFREE_P(free_m, wc_src);

	if (out_siz)
		*out_siz = conv_u8c_len;

	return rst;


exit_fail_conv:

	CJMFREE_P(free_m, wc_src);
 
	if (out_siz)
		*out_siz = 0;

	return rst;
}

CJEXTERNC cjstr_siz cj_conv_u8c_to_mc(cjmc* dst, cjstr_siz dst_siz, const cju8c* src) {

	cjstr_siz conv_wc_len = 0;
	cjstr_siz conv_mc_len = 0;
	cjwc* wc_src = cjNULL;

	// utf8 to wide		
	conv_wc_len = MultiByteToWideChar(CP_UTF8, 0, src, -1, 0, 0);

	if(!conv_wc_len)
		goto exit_fail_conv;

	// included null terminiator
	wc_src = (cjwc*)cjmalloc(sizeof(cjwc) * conv_wc_len);

	if (!wc_src)
		goto exit_fail_conv;

	MultiByteToWideChar(CP_UTF8, 0, src, -1, wc_src, conv_wc_len);

	// wide to multibyte
	conv_mc_len = WideCharToMultiByte(CP_ACP, 0, wc_src, -1, cjNULL, 0, cjNULL, cjNULL);

	if (!dst) {
		cjmfree(wc_src);
		return conv_mc_len;
	}
	 
	if (!conv_mc_len)
		goto exit_fail_conv;

	if (conv_mc_len > dst_siz)
		conv_mc_len = dst_siz;

	WideCharToMultiByte(CP_ACP, 0, wc_src, -1, dst, conv_mc_len, cjNULL, cjNULL);

	cjmfree(wc_src);

	return conv_mc_len;


exit_fail_conv:

	if (wc_src)
		cjmfree(wc_src);

	if (dst)
		dst[0] = (cjmc)cjNULL;

	return 0;
}


CJEXTERNC cjmc* cj_conv_u8c_to_mc_mem(cjstr_siz* out_siz, const cjmc* src,
	cjstr_siz src_count, cjmalloc_pfn alloc_m, cjmfree_pfn free_m) {

	cjstr_siz conv_wc_len = 0;
	cjstr_siz conv_mc_len = 0;
	cjwc* wc_src = cjNULL;
	cjmc* rst = cjNULL;

	// utf8 to wide		
	// convert count is not '-1', so returned length 'conv_wc_len' does not count null terminator
	conv_wc_len = MultiByteToWideChar(CP_UTF8, 0, src, src_count, cjNULL, cjNULL);

	if (!conv_wc_len)
		goto exit_fail_conv;

	// src_count != -1: null terminator is does not included
	wc_src = CJMALLOC_P(alloc_m, sizeof(cjwc) * conv_wc_len);

	if (!wc_src)
		goto exit_fail_conv;

	MultiByteToWideChar(CP_UTF8, 0, src, src_count, wc_src, conv_wc_len);

	// wide to multibyte
	conv_mc_len = WideCharToMultiByte(CP_ACP, 0, wc_src, conv_wc_len, cjNULL, 0, cjNULL, cjNULL);

	if (!conv_mc_len)
		goto exit_fail_conv;

	rst = CJMALLOC_P(alloc_m, sizeof(cjmc) * conv_mc_len);

	WideCharToMultiByte(CP_ACP, 0, wc_src, conv_wc_len, rst, conv_mc_len, cjNULL, cjNULL);

	CJMFREE_P(free_m, wc_src);

	if (out_siz)
		*out_siz = conv_mc_len;

	return rst;


exit_fail_conv:

	CJMFREE_P(free_m, wc_src);

	if (out_siz)
		*out_siz = 0;

	return 0;
}


// wide character to multi-byte
CJEXTERNC cjstr_siz cj_conv_wc_to_mc(cjmc* dst, cjstr_siz dst_siz, const cjwc* src) {

	// cchWideChar: can be -1, if the string is null-terminated
	// cbMultiByte=0 to get length
	cjstr_siz conv_len = WideCharToMultiByte(CP_ACP, 0, src, -1, cjNULL, 0, cjNULL, cjNULL);

	if (conv_len == 0) {
		// failed to convert
		if (dst)
			dst[0] = (cjmc)cjNULL;

		return 0;
	}

	if (!dst)
		return conv_len;

	if (dst_siz <= conv_len)
		conv_len = dst_siz;

	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, conv_len, cjNULL, cjNULL);

	return conv_len;
}

// wide character to utf-8
CJEXTERNC cjstr_siz cj_conv_wc_to_u8c(cju8c* dst, cjstr_siz dst_siz, const cjwc* src) {

	// cchWideChar: can be -1, if the string is null-terminated
	// cbMultiByte=0 to get length
	cjstr_siz conv_len = WideCharToMultiByte(CP_UTF8, 0, src, -1, cjNULL, 0, cjNULL, cjNULL);

	if (conv_len == 0) {
		// failed to convert
		if (dst)
			dst[0] = (cju8c)cjNULL;

		return 0;
	}

	if (!dst)
		return conv_len;

	if (dst_siz <= conv_len)
		conv_len = dst_siz;

	WideCharToMultiByte(CP_UTF8, 0, src, -1, dst, conv_len, cjNULL, cjNULL);

	return conv_len;
}

// utf-8 to wide character
CJEXTERNC cjstr_siz cj_conv_u8c_to_wc(cjwc* dst, cjstr_siz dst_siz, const cju8c* src) {

	// cbMulitByte: can be -1, if the string is null-terminated
	// cchWideChar=0 to get length
	cjstr_siz conv_len = MultiByteToWideChar(CP_UTF8, 0, src, -1, cjNULL, cjNULL);

	if (conv_len == 0) {
		// failed to convert
		if (dst)
			dst[0] = (cjwc)cjNULL;

		return 0;
	}

	if (!dst)
		return conv_len;

	if (dst_siz <= conv_len)
		conv_len = dst_siz;

	MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, conv_len);

	return conv_len;
}



#else 


// multi-byte to wide character
CJEXTERN cjwc* cj_conv_mc_to_wc(cjwc* dst, cjsiz_t dst_siz, const cjmc* src) {
#error "not impl"
}

// wide character to multi-byte
CJEXTERN cjmc* cj_conv_wc_to_mc(cjmc* dst, cjsiz_t dst_siz, const cjwc* src) {
#error "not impl"
}

// wide character to utf-8
CJEXTERN cjwc* cj_conv_wc_to_u8c(cju8c* dst, cjsiz_t dst_siz, const cjwc* src) {
#error "not impl"
}


#endif







