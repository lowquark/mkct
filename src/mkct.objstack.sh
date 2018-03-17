#!/usr/bin/bash

set -u

NAME=stack
OBJECT_TYPE=int
H_FILE=
C_FILE=
OUTPUT_TYPE='overview'

function print() {
  echo "$1" >&2
}

function print_usage() {
  print "Usage: mkct.stack [OPTIONS]...                                        "
  print "Generate a dynamically-sized stack implementation with the given type "
  print "                                                                      "
  print "  --name=[NAME]            Set stack name/prefix                      "
  print "  --object-type=[TYPE]      Set type of objects contained in the stack"
  print "                                                                      "
  print "  --header-file=[FILENAME] Set header file to [FILENAME]              "
  print "                             Defaults to [NAME].h                     "
  print "  --source-file=[FILENAME] Set source file to [FILENAME]              "
  print "                             Defaults to [NAME].c                     "
  print "                                                                      "
  print "  --overview               Output API/Overview   (default)            "
  print "  --header                 Output C header file                       "
  print "  --source                 Output C source file                       "
  print "                                                                      "
  print "  -h,--help                Show this usage and exit                   "
  print "                                                                      "
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
    --object-type=*) OBJECT_TYPE="${1#*=}"; shift 1 ;;

    --header-file=*) H_FILE="${1#*=}"; shift 1 ;;
    --source-file=*) C_FILE="${1#*=}"; shift 1 ;;

    --name|--object-type|--header-file|--source-file)
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
{{objstack.overview.h}}
EOF
    ;;
  header)
read -r -d '' OUTPUT << "EOF"
{{objstack.h}}
EOF
    ;;
  source)
read -r -d '' OUTPUT << "EOF"
{{objstack.c}}
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
s/OBJECT_TYPE/${OBJECT_TYPE}/g;\
s/OBJSTACK_STRUCT/${NAME}/g;\
s/OBJSTACK_TYPE/${NAME}_t/g;\
s/SIZE_TYPE/${NAME}_size_t/g;\
s/OBJSTACK_METHOD_INIT/${NAME}_init/g;\
s/OBJSTACK_METHOD_CLEAR/${NAME}_clear/g;\
s/OBJSTACK_METHOD_PUSH/${NAME}_push/g;\
s/OBJSTACK_METHOD_POP/${NAME}_pop/g;\
s/OBJSTACK_METHOD_PEEK/${NAME}_peek/g;\
s/OBJSTACK_METHOD_AT/${NAME}_at/g;\
s/OBJSTACK_METHOD_SIZE/${NAME}_size/g;\
s/H_FILE/${H_FILE////\\/}/g;\
s/C_FILE/${C_FILE////\\/}/g"

# Perform substitutions and print
echo "$OUTPUT" | sed "$REPLACE"

