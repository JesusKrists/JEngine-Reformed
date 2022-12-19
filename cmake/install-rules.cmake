install(
    TARGETS JEngine3D_exe
    RUNTIME COMPONENT JEngine3D_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
