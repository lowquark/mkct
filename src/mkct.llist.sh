#!/usr/bin/bash

set -u

NAME=
H_FILE=
C_FILE=
VERBOSE=0
SHOWONLY=0

function print_usage() {
  echo "Usage: mkct.llist [OPTIONS]...                                 " >&2
  echo "Generate a circular linked list for a given type in C          " >&2
  echo "                                                               " >&2
  echo "  --name=[NAME]           Set list name/prefix                 " >&2
  echo "  --value-type=[TYPE]     Set type of values contained in list " >&2
  echo "                                                               " >&2
  echo "  --output-header=[PATH]  Set C header output file (.h)        " >&2
  echo "  --output-source=[PATH]  Set C source output file (.c)        " >&2
  echo "                                                               " >&2
  echo "  --show-only             Don't produce output files, only show" >&2
  echo "                            substitutions to be made           " >&2
  echo "  -v,--verbose            Set verbose output                   " >&2
  echo "  -h,--help               Show this usage and exit             " >&2
}

if [ "$#" -eq 0 ]; then
  print_usage
  exit 1
fi

while [ "$#" -gt 0 ]; do
  case "$1" in
    --name=*) NAME="${1#*=}"; shift 1;;
    --value-type=*) VALUE_TYPE="${1#*=}"; shift 1;;
    --output-header=*) H_FILE="${1#*=}"; shift 1;;
    --output-source=*) C_FILE="${1#*=}"; shift 1;;

    --name|--value-type|--output-header|--output-source) \
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
if [ -z "$VALUE_TYPE" ]; then echo "no value type specified! aborting." >&2; print_usage; exit 1; fi

# Overridable derived parameters
if [ -z "$H_FILE" ]; then H_FILE="$NAME.h"; fi
if [ -z "$C_FILE" ]; then C_FILE="$NAME.c"; fi

# Replace non alphanumeric characters with _
INCLUDE_GUARD="_${H_FILE//[^a-zA-Z0-9]/_}_"
INCLUDE_GUARD="${INCLUDE_GUARD^^}"

LIST_STRUCT="${NAME}"
LIST_TYPE="${NAME}_t"
NODE_STRUCT="${NAME}_node"
NODE_TYPE="${NAME}_node_t"
LIST_METHOD_INIT="${NAME}_init"
LIST_METHOD_CLEAR="${NAME}_clear"
LIST_METHOD_ERASE="${NAME}_erase"
LIST_METHOD_PUSHFRONT="${NAME}_pushfront"
LIST_METHOD_PUSHBACK="${NAME}_pushback"
LIST_METHOD_INSERTBEFORE="${NAME}_insertbefore"
LIST_METHOD_INSERTAFTER="${NAME}_insertafter"
LIST_METHOD_FIRST="${NAME}_first"
LIST_METHOD_LAST="${NAME}_last"
LIST_METHOD_NEXT="${NAME}_next"
LIST_METHOD_PREV="${NAME}_prev"

if [ $VERBOSE -eq 1 -o $SHOWONLY -eq 1 ]; then
  # Print a summary
  echo "H_FILE                   : ${H_FILE}" >&2
  echo "C_FILE                   : ${C_FILE}" >&2
  echo "INCLUDE_GUARD            : ${INCLUDE_GUARD}" >&2
  echo "VALUE_TYPE               : ${LIST_TYPE}" >&2
  echo "LIST_STRUCT              : ${LIST_STRUCT}" >&2
  echo "LIST_TYPE                : ${LIST_TYPE}" >&2
  echo "NODE_STRUCT              : ${NODE_STRUCT}" >&2
  echo "NODE_TYPE                : ${NODE_TYPE}" >&2
  echo "LIST_METHOD_INIT         : ${LIST_METHOD_INIT}" >&2
  echo "LIST_METHOD_CLEAR        : ${LIST_METHOD_CLEAR}" >&2
  echo "LIST_METHOD_ERASE        : ${LIST_METHOD_ERASE}" >&2
  echo "LIST_METHOD_PUSHFRONT    : ${LIST_METHOD_PUSHFRONT}" >&2
  echo "LIST_METHOD_PUSHBACK     : ${LIST_METHOD_PUSHBACK}" >&2
  echo "LIST_METHOD_INSERTBEFORE : ${LIST_METHOD_INSERTBEFORE}" >&2
  echo "LIST_METHOD_INSERTAFTER  : ${LIST_METHOD_INSERTAFTER}" >&2
  echo "LIST_METHOD_FIRST        : ${LIST_METHOD_FIRST}" >&2
  echo "LIST_METHOD_LAST         : ${LIST_METHOD_LAST}" >&2
  echo "LIST_METHOD_NEXT         : ${LIST_METHOD_NEXT}" >&2
  echo "LIST_METHOD_PREV         : ${LIST_METHOD_PREV}" >&2
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
#include "llist.h"
EOF

# Why yes, this is an entire C file
read -r -d '' C_FILE_SRC << "EOF"
#include "llist.c"
EOF

# Big sed expression
REPLACE="\
s/INCLUDE_GUARD/$INCLUDE_GUARD/g;\
s/LIST_STRUCT/$LIST_STRUCT/g;\
s/LIST_TYPE/$LIST_TYPE/g;\
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
s/NODE_TYPE/$NODE_TYPE/g;\
s/VALUE_TYPE/$VALUE_TYPE/g;\
s/H_FILE/${H_FILE////\\/}/g;\
s/C_FILE/${C_FILE////\\/}/g"

# Perform substitutions on header
H_FILE_SRC=$(echo "$H_FILE_SRC" | sed "$REPLACE")
if [ $VERBOSE -eq 1 ]; then echo "writing \`$H_FILE\`" >&2; fi
echo -e "$LICENSE\n\n$H_FILE_SRC" > $H_FILE

# Perform substitutions on source
C_FILE_SRC=$(echo "$C_FILE_SRC" | sed "$REPLACE")
if [ $VERBOSE -eq 1 ]; then echo "writing \`$C_FILE\`" >&2; fi
echo -e "$LICENSE\n\n$C_FILE_SRC" > $C_FILE

