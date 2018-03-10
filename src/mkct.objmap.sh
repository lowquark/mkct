#!/usr/bin/bash

set -u

H_FILE=
C_FILE=

# TODO: Style, force overwrite, verbosity, output files, etc.
while [ "$#" -gt 0 ]; do
  case "$1" in
    -n) OBJMAP_NAME="$2"; shift 2;;
    -k) KEY_TYPEDEF="$2"; shift 2;;
    -v) VALUE_TYPEDEF="$2"; shift 2;;

    --name=*) OBJMAP_NAME="${1#*=}"; shift 1;;
    --key-type=*) KEY_TYPEDEF="${1#*=}"; shift 1;;
    --value-type=*) VALUE_TYPEDEF="${1#*=}"; shift 1;;
    --name|--key-type|--value-type) echo "$1 requires an argument" >&2; exit 1;;

    -*) echo "unknown option: $1" >&2; exit 1;;
    *) echo "unknown option: $1" >&2; exit 1;;
  esac
done

# Minimum set parameters
if [ -z "$OBJMAP_NAME" ]; then echo "no name specified! aborting." >&2; exit 1; fi
if [ -z "$KEY_TYPEDEF" ]; then echo "no key type specified! aborting." >&2; exit 1; fi
if [ -z "$VALUE_TYPEDEF" ]; then echo "no value type specified! aborting." >&2; exit 1; fi

# Derived parameters
INCLUDE_GUARD="${OBJMAP_NAME^^}_H"
OBJMAP_STRUCT="$OBJMAP_NAME"
OBJMAP_TYPEDEF="${OBJMAP_NAME}_t"
OBJMAP_METHOD_INIT="${OBJMAP_NAME}_init"
OBJMAP_METHOD_CLEAR="${OBJMAP_NAME}_clear"
OBJMAP_METHOD_FIND="${OBJMAP_NAME}_find"
OBJMAP_METHOD_CREATE="${OBJMAP_NAME}_create"
OBJMAP_METHOD_DESTROY="${OBJMAP_NAME}_destroy"
ENTRY_STRUCT="${OBJMAP_NAME}_entry"
ENTRY_TYPEDEF="${OBJMAP_NAME}_entry_t"

# Overridable derived parameters
if [ -z "$H_FILE" ]; then H_FILE="$OBJMAP_NAME.h"; fi
if [ -z "$C_FILE" ]; then C_FILE="$OBJMAP_NAME.c"; fi

# Print a summary
#echo "Header file    : ${H_FILE}" >&2
#echo "Source file    : ${C_FILE}" >&2
#echo "INCLUDE_GUARD  : $INCLUDE_GUARD" >&2
#echo "OBJMAP_STRUCT  : $OBJMAP_STRUCT" >&2
#echo "OBJMAP_TYPEDEF : $OBJMAP_TYPEDEF" >&2
#echo "ENTRY_STRUCT   : $ENTRY_STRUCT" >&2
#echo "ENTRY_TYPEDEF  : $ENTRY_TYPEDEF" >&2

# Don't overwrite
#if [ -f $H_FILE ]; then echo "\"$H_FILE\" already exists! aborting." >&2; exit 1; fi
#if [ -f $C_FILE ]; then echo "\"$C_FILE\" already exists! aborting." >&2; exit 1; fi

read -r -d '' UNLICENSE << "EOF"
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
s/OBJMAP_TYPEDEF/$OBJMAP_TYPEDEF/g;\
s/OBJMAP_METHOD_INIT/$OBJMAP_METHOD_INIT/g;\
s/OBJMAP_METHOD_CLEAR/$OBJMAP_METHOD_CLEAR/g;\
s/OBJMAP_METHOD_FIND/$OBJMAP_METHOD_FIND/g;\
s/OBJMAP_METHOD_CREATE/$OBJMAP_METHOD_CREATE/g;\
s/OBJMAP_METHOD_DESTROY/$OBJMAP_METHOD_DESTROY/g;\
s/ENTRY_STRUCT/$ENTRY_STRUCT/g;\
s/ENTRY_TYPEDEF/$ENTRY_TYPEDEF/g;\
s/KEY_TYPEDEF/$KEY_TYPEDEF/g;\
s/VALUE_TYPEDEF/$VALUE_TYPEDEF/g;\
s/H_FILE/$H_FILE/g;\
s/C_FILE/$C_FILE/g"

# Perform substitutions on header
H_FILE_SRC=$(echo "$H_FILE_SRC" | sed "$REPLACE")
#echo "writing \`$H_FILE\`" >&2
echo -e "$UNLICENSE\n\n$H_FILE_SRC" > $H_FILE

# Perform substitutions on source
C_FILE_SRC=$(echo "$C_FILE_SRC" | sed "$REPLACE")
#echo "writing \`$C_FILE\`" >&2
echo -e "$UNLICENSE\n\n$C_FILE_SRC" > $C_FILE

