include_guard()

macro(disable_static_analysis target)
  set_property(TARGET ${target} PROPERTY CXX_INCLUDE_WHAT_YOU_USE "")
  set_property(TARGET ${target} PROPERTY C_INCLUDE_WHAT_YOU_USE "")
  #
  set_property(TARGET ${target} PROPERTY CXX_CLANG_TIDY "")
  set_property(TARGET ${target} PROPERTY C_CLANG_TIDY "")
  #
  set_property(TARGET ${target} PROPERTY CXX_CPPCHECK "")
  set_property(TARGET ${target} PROPERTY C_CPPCHECK "")
endmacro()
