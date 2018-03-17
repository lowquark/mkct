#!/usr/bin/bash

set -u

NAME=list
VALUE_TYPE=int
H_FILE=
C_FILE=
OUTPUT_TYPE='overview'

function print() {
  echo "$1" >&2
}

function print_usage() {
  print "Usage: mkct.list [OPTIONS]...                                        "
  print "Generate a circular, linked list implementation with the given type  "
  print "                                                                     "
  print "  --name=[NAME]            Set list name/prefix                      "
  print "  --value-type=[TYPE]      Set type of values contained in the list  "
  print "                                                                     "
  print "  --header-file=[FILENAME] Set header file to [FILENAME]             "
  print "                             Defaults to [NAME].h                    "
  print "  --source-file=[FILENAME] Set source file to [FILENAME]             "
  print "                             Defaults to [NAME].c                    "
  print "                                                                     "
  print "  --overview               Output API/Overview   (default)           "
  print "  --header                 Output C header file                      "
  print "  --source                 Output C source file                      "
  print "                                                                     "
  print "  -h,--help                Show this usage and exit                  "
  print "                                                                     "
}

function fail() {
  print "error: $1"
  print ""
  exit 1
}

function fail_badusage() {
  print "error: $1"
  print ""
  print_usage
  exit 1
}

while [ "$#" -gt 0 ]; do
  case "$1" in
    --name=*)       NAME="${1#*=}";       shift 1 ;;
    --value-type=*) VALUE_TYPE="${1#*=}"; shift 1 ;;

    --header-file=*) H_FILE="${1#*=}"; shift 1 ;;
    --source-file=*) C_FILE="${1#*=}"; shift 1 ;;

    --name|--value-type|--header-file|--source-file)
      fail_badusage "$1 requires an argument" ;;

    --overview) OUTPUT_TYPE='overview'; shift 1 ;;
    --header)   OUTPUT_TYPE='header';   shift 1 ;;
    --source)   OUTPUT_TYPE='source';   shift 1 ;;

    -h|--help) print_usage; exit 0 ;;

    -*) fail_badusage "unknown option: $1" ;;
    *)  fail_badusage "unknown option: $1" ;;
  esac
done

if [ -z $H_FILE ]; then H_FILE="$NAME.h"; fi
if [ -z $C_FILE ]; then C_FILE="$NAME.c"; fi

case "$OUTPUT_TYPE" in
  overview)
read -r -d '' OUTPUT << "EOF"
{{list.overview.h}}
EOF
    ;;
  header)
read -r -d '' OUTPUT << "EOF"
{{list.h}}
EOF
    ;;
  source)
read -r -d '' OUTPUT << "EOF"
{{list.c}}
EOF
    ;;
  *)
    fail 'bad output type'
    ;;
esac

# Replace non alphanumeric characters with _
INCLUDE_GUARD="_${H_FILE//[^a-zA-Z0-9]/_}_"
INCLUDE_GUARD="${INCLUDE_GUARD^^}"

REPLACE="\
s/INCLUDE_GUARD/${INCLUDE_GUARD}/g;\
s/VALUE_TYPE/${VALUE_TYPE}/g;\
s/LIST_STRUCT/${NAME}/g;\
s/LIST_TYPE/${NAME}_t/g;\
s/NODE_STRUCT/${NAME}_node/g;\
s/NODE_TYPE/${NAME}_node_t/g;\
s/LIST_METHOD_INIT/${NAME}_init/g;\
s/LIST_METHOD_CLEAR/${NAME}_clear/g;\
s/LIST_METHOD_ERASE/${NAME}_erase/g;\
s/LIST_METHOD_PUSHBACK/${NAME}_pushback/g;\
s/LIST_METHOD_PUSHFRONT/${NAME}_pushfront/g;\
s/LIST_METHOD_INSERTBEFORE/${NAME}_insertbefore/g;\
s/LIST_METHOD_INSERTAFTER/${NAME}_insertafter/g;\
s/LIST_METHOD_FIRST/${NAME}_first/g;\
s/LIST_METHOD_LAST/${NAME}_last/g;\
s/LIST_METHOD_NEXT/${NAME}_next/g;\
s/LIST_METHOD_PREV/${NAME}_prev/g;\
s/LIST_METHOD_VALUE/${NAME}_value/g;\
s/H_FILE/${H_FILE////\\/}/g;\
s/C_FILE/${C_FILE////\\/}/g"

# Perform substitutions and print
echo "$OUTPUT" | sed "$REPLACE"

