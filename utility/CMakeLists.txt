#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

add_library(HEADER_ONLY_TARGET STATIC
    easylogging++.h
)

set_target_properties(HEADER_ONLY_TARGET PROPERTIES LINKER_LANGUAGE CXX)
