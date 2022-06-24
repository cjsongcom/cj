function(src_cj_get_files out_lst rel_path)

  set(__src_file_lst__, "")

  #file(GLOB_RECURSE
  file(GLOB __src_file_lst__
    LIST_DIRECTORIES true 	
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"  
	"${CMAKE_CURRENT_LIST_DIR}/${rel_path}/*.c" "${CMAKE_CURRENT_LIST_DIR}/${rel_path}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/${rel_path}/cjtui/*.c" "${CMAKE_CURRENT_LIST_DIR}/${rel_path}/cjtui/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/${rel_path}/cjsch/*.c" "${CMAKE_CURRENT_LIST_DIR}/${rel_path}/cjsch/*.cpp"
    )

  if(CJB_DBG_SHOW_SRC_FILES)
     message("*SRC LIST: ${rel_path}")
     foreach(f ${__src_file_lst__})
       message(">>>>  ${f}")
     endforeach()
  endif()

  set(${out_lst} ${__src_file_lst__} PARENT_SCOPE)

endfunction()
