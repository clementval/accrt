add_executable(
  basic
  basic.cpp
)
  
target_link_libraries(basic openacc cuda)
target_include_directories(basic
  PRIVATE
  ${OPENACC_INCLUDE_DIR}
)