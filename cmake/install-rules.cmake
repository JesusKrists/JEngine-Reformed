install(TARGETS JEngine-Reformed_exe RUNTIME COMPONENT JEngine-Reformed_Runtime)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
