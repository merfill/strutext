# Copyright &copy; 2013, Vladimir Lapshin.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
# \brief Search Log4cplus library.
# This module defines
#  Log4cplus_FOUND, if false, do not try to link to Log4cplus
#  Log4cplus_LIBRARY
#  Log4cplus_INCLUDE_DIR, where to find log4cplus.hpp

find_path(Log4cplus_INCLUDE_DIR log4cplus/logger.h
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES include
)

find_library(Log4cplus_LIBRARY
  NAMES Log4cplus log4cplus
  PATH_SUFFIXES lib64/ lib/
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

if(Log4cplus_INCLUDE_DIR-NOTFOUND OR Log4cplus_LIBRARY-NOTFOUND)
  set(Log4cplus_FOUND FALSE)
else()
  set(Log4cplus_FOUND TRUE)
endif()

mark_as_advanced(
  Log4cplus_INCLUDE_DIR
  Log4cplus_LIBRARIES
)
