#!/usr/bin/bash

set -u

NAME=
H_FILE=
C_FILE=
VERBOSE=0
SHOWONLY=0

function print_usage() {
  echo "Usage: mkct.llist [OPTIONS]...                                    " >&2
  echo "Generate a map from given key/object types in C                   " >&2
  echo "                                                                  " >&2
  echo "  --name=[NAME]           Set list name/prefix                    " >&2
  echo "  --key-type=[TYPE]       Set type of keys indexed by the map     " >&2
  echo "  --object-type=[TYPE]     Set type of values contained in the map " >&2
  echo "                                                                  " >&2
  echo "  --output-header=[PATH]  Set C header output file (.h)           " >&2
  echo "  --output-source=[PATH]  Set C source output file (.c)           " >&2
  echo "                                                                  " >&2
  echo "  --show-only             Don't produce output files, only show   " >&2
  echo "                            substitutions to be made              " >&2
  echo "  -v,--verbose            Set verbose output                      " >&2
  echo "  -h,--help               Show this usage and exit                " >&2
}

if [ "$#" -eq 0 ]; then
  print_usage
  exit 1
fi

while [ "$#" -gt 0 ]; do
  case "$1" in
    --name=*) NAME="${1#*=}"; shift 1;;
    --key-type=*) KEY_TYPE="${1#*=}"; shift 1;;
    --object-type=*) OBJECT_TYPE="${1#*=}"; shift 1;;
    --output-header=*) H_FILE="${1#*=}"; shift 1;;
    --output-source=*) C_FILE="${1#*=}"; shift 1;;

    --name|--key-type|--object-type|--output-header|--output-source) \
      echo "$1 requires an argument" >&2; print_usage; exit 1;;

    -v|--verbose) VERBOSE=1; shift 1;;
    -h|--help)    print_usage; exit 0;;
    --show-only)  SHOWONLY=1; shift 1;;

    -*) echo "unknown option: $1" >&2; print_usage; exit 1;;
    *) echo "unknown option: $1" >&2; print_usage; exit 1;;
  esac
done

# Minimum set parameters
if [ -z "$NAME" ]; then echo "no name specified! aborting." >&2; print_usage; exit 1; fi
if [ -z "$KEY_TYPE" ]; then echo "no key type specified! aborting." >&2; print_usage; exit 1; fi
if [ -z "$OBJECT_TYPE" ]; then echo "no value type specified! aborting." >&2; print_usage; exit 1; fi

# Overridable derived parameters
if [ -z "$H_FILE" ]; then H_FILE="$NAME.h"; fi
if [ -z "$C_FILE" ]; then C_FILE="$NAME.c"; fi

# Replace non alphanumeric characters with _
INCLUDE_GUARD="_${H_FILE//[^a-zA-Z0-9]/_}_"
INCLUDE_GUARD="${INCLUDE_GUARD^^}"

OBJMAP_STRUCT="$NAME"
OBJMAP_TYPE="${NAME}_t"
ENTRY_STRUCT="${NAME}_entry"
ENTRY_TYPE="${NAME}_entry_t"
OBJMAP_METHOD_INIT="${NAME}_init"
OBJMAP_METHOD_CLEAR="${NAME}_clear"
OBJMAP_METHOD_FIND="${NAME}_find"
OBJMAP_METHOD_CREATE="${NAME}_create"
OBJMAP_METHOD_DESTROY="${NAME}_destroy"

if [ $VERBOSE -eq 1 -o $SHOWONLY -eq 1 ]; then
  # Print a summary
  echo "H_FILE                : ${H_FILE}" >&2
  echo "C_FILE                : ${C_FILE}" >&2
  echo "INCLUDE_GUARD         : ${INCLUDE_GUARD}" >&2
  echo "KEY_TYPE              : ${KEY_TYPE}" >&2
  echo "OBJECT_TYPE           : ${OBJECT_TYPE}" >&2
  echo "OBJMAP_STRUCT         : ${OBJMAP_STRUCT}" >&2
  echo "OBJMAP_TYPE           : ${OBJMAP_TYPE}" >&2
  echo "ENTRY_STRUCT          : ${ENTRY_STRUCT}" >&2
  echo "ENTRY_TYPE            : ${ENTRY_TYPE}" >&2
  echo "OBJMAP_METHOD_INIT    : ${OBJMAP_METHOD_INIT}" >&2
  echo "OBJMAP_METHOD_CLEAR   : ${OBJMAP_METHOD_CLEAR}" >&2
  echo "OBJMAP_METHOD_FIND    : ${OBJMAP_METHOD_FIND}" >&2
  echo "OBJMAP_METHOD_CREATE  : ${OBJMAP_METHOD_CREATE}" >&2
  echo "OBJMAP_METHOD_DESTROY : ${OBJMAP_METHOD_DESTROY}" >&2
fi

# Stop here if just showing output
if [ $SHOWONLY -eq 1 ]; then exit 0; fi

read -r -d '' LICENSE << "EOF"
/* 
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org>
 */
EOF

# Why yes, this is an entire header file
read -r -d '' H_FILE_SRC << "EOF"
#include "objmap.h"
EOF

# Why yes, this is an entire C file
read -r -d '' C_FILE_SRC << "EOF"
#include "objmap.c"
EOF

REPLACE="\
s/INCLUDE_GUARD/$INCLUDE_GUARD/g;\
s/OBJMAP_STRUCT/$OBJMAP_STRUCT/g;\
s/OBJMAP_TYPE/$OBJMAP_TYPE/g;\
s/OBJMAP_METHOD_INIT/$OBJMAP_METHOD_INIT/g;\
s/OBJMAP_METHOD_CLEAR/$OBJMAP_METHOD_CLEAR/g;\
s/OBJMAP_METHOD_FIND/$OBJMAP_METHOD_FIND/g;\
s/OBJMAP_METHOD_CREATE/$OBJMAP_METHOD_CREATE/g;\
s/OBJMAP_METHOD_DESTROY/$OBJMAP_METHOD_DESTROY/g;\
s/ENTRY_STRUCT/$ENTRY_STRUCT/g;\
s/ENTRY_TYPE/$ENTRY_TYPE/g;\
s/KEY_TYPE/$KEY_TYPE/g;\
s/OBJECT_TYPE/$OBJECT_TYPE/g;\
s/H_FILE/$H_FILE/g;\
s/C_FILE/$C_FILE/g"

# Perform substitutions on header
H_FILE_SRC=$(echo "$H_FILE_SRC" | sed "$REPLACE")
if [ $VERBOSE -eq 1 ]; then echo "writing \`$H_FILE\`" >&2; fi
echo -e "$LICENSE\n\n$H_FILE_SRC" > $H_FILE

# Perform substitutions on source
C_FILE_SRC=$(echo "$C_FILE_SRC" | sed "$REPLACE")
if [ $VERBOSE -eq 1 ]; then echo "writing \`$C_FILE\`" >&2; fi
echo -e "$LICENSE\n\n$C_FILE_SRC" > $C_FILE

