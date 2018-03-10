#!/usr/bin/bash

set -u

H_FILE=
C_FILE=

# TODO: Style, force overwrite, verbosity, output files, etc.
while [ "$#" -gt 0 ]; do
  case "$1" in
    -n) LIST_NAME="$2"; shift 2;;
    -v) VALUE_TYPEDEF="$2"; shift 2;;

    --name=*) LIST_NAME="${1#*=}"; shift 1;;
    --value-type=*) VALUE_TYPEDEF="${1#*=}"; shift 1;;
    --name|--value-type) echo "$1 requires an argument" >&2; exit 1;;

    -*) echo "unknown option: $1" >&2; exit 1;;
    *) echo "unknown option: $1" >&2; exit 1;;
  esac
done

# Minimum set parameters
if [ -z "$LIST_NAME" ]; then echo "no name specified! aborting." >&2; exit 1; fi
if [ -z "$VALUE_TYPEDEF" ]; then echo "no value type specified! aborting." >&2; exit 1; fi

# Derived parameters
INCLUDE_GUARD="${LIST_NAME^^}_H"
LIST_STRUCT="$LIST_NAME"
LIST_TYPEDEF="${LIST_NAME}_t"
LIST_METHOD_INIT="${LIST_NAME}_init"
LIST_METHOD_CLEAR="${LIST_NAME}_clear"
LIST_METHOD_ERASE="${LIST_NAME}_erase"
LIST_METHOD_PUSHFRONT="${LIST_NAME}_pushfront"
LIST_METHOD_PUSHBACK="${LIST_NAME}_pushback"
LIST_METHOD_INSERTBEFORE="${LIST_NAME}_insertbefore"
LIST_METHOD_INSERTAFTER="${LIST_NAME}_insertafter"
LIST_METHOD_FIRST="${LIST_NAME}_first"
LIST_METHOD_LAST="${LIST_NAME}_last"
LIST_METHOD_NEXT="${LIST_NAME}_next"
LIST_METHOD_PREV="${LIST_NAME}_prev"
NODE_STRUCT="${LIST_NAME}_node"
NODE_TYPEDEF="${LIST_NAME}_node_t"

# Overridable derived parameters
if [ -z "$H_FILE" ]; then H_FILE="$LIST_NAME.h"; fi
if [ -z "$C_FILE" ]; then C_FILE="$LIST_NAME.c"; fi

# Print a summary
echo "Header file   : ${H_FILE}" >&2
echo "Source file   : ${C_FILE}" >&2
echo "INCLUDE_GUARD : $INCLUDE_GUARD" >&2
echo "LIST_STRUCT   : $LIST_STRUCT" >&2
echo "LIST_TYPEDEF  : $LIST_TYPEDEF" >&2
echo "NODE_STRUCT   : $NODE_STRUCT" >&2
echo "NODE_TYPEDEF  : $NODE_TYPEDEF" >&2

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
#include "llist.h"
EOF

# Why yes, this is an entire C file
read -r -d '' C_FILE_SRC << "EOF"
#include "llist.c"
EOF

REPLACE="\
s/INCLUDE_GUARD/$INCLUDE_GUARD/g;\
s/LIST_STRUCT/$LIST_STRUCT/g;\
s/LIST_TYPEDEF/$LIST_TYPEDEF/g;\
s/LIST_METHOD_INIT/$LIST_METHOD_INIT/g;\
s/LIST_METHOD_CLEAR/$LIST_METHOD_CLEAR/g;\
s/LIST_METHOD_ERASE/$LIST_METHOD_ERASE/g;\
s/LIST_METHOD_PUSHBACK/$LIST_METHOD_PUSHBACK/g;\
s/LIST_METHOD_PUSHFRONT/$LIST_METHOD_PUSHFRONT/g;\
s/LIST_METHOD_INSERTAFTER/$LIST_METHOD_INSERTAFTER/g;\
s/LIST_METHOD_INSERTBEFORE/$LIST_METHOD_INSERTBEFORE/g;\
s/LIST_METHOD_FIRST/$LIST_METHOD_FIRST/g;\
s/LIST_METHOD_LAST/$LIST_METHOD_LAST/g;\
s/LIST_METHOD_NEXT/$LIST_METHOD_NEXT/g;\
s/LIST_METHOD_PREV/$LIST_METHOD_PREV/g;\
s/NODE_STRUCT/$NODE_STRUCT/g;\
s/NODE_TYPEDEF/$NODE_TYPEDEF/g;\
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

