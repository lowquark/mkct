#!/usr/bin/bash

set -u

NAME=objmap
KEY_TYPE=int
OBJECT_TYPE=int
H_FILE=
C_FILE=
OUTPUT_TYPE='overview'

function print() {
  echo "$1" >&2
}

function print_usage() {
  print "Usage: mkct.objmap [OPTIONS]...                                      "
  print "Generate a key/object map implementation with the given types        "
  print "                                                                     "
  print "  --name=[NAME]            Set list name/prefix                      "
  print "  --key-type=[TYPE]        Set type of keys indexed by the map       "
  print "  --object-type=[TYPE]     Set type of objects contained in the map  "
  print "                                                                     "
  print "  --header-file=[FILENAME] Set header file to [FILENAME]             "
  print "                             Defaults to [NAME].h                    "
  print "  --source-file=[FILENAME] Set source file to [FILENAME]             "
  print "                             Defaults to [NAME].c                    "
  print "                                                                     "
  print "  --overview               Output API/Overview   (default)           "
  print "  --header                 Generate C header file                    "
  print "  --source                 Generate C source file                    "
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
    --key-type=*)   KEY_TYPE="${1#*=}";   shift 1 ;;
    --object-type=*) OBJECT_TYPE="${1#*=}"; shift 1 ;;

    --header-file=*) H_FILE="${1#*=}"; shift 1 ;;
    --source-file=*) C_FILE="${1#*=}"; shift 1 ;;

    --name|--key-type|--object-type|--header-file|--source-file)
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
{{objmap.overview.h}}
EOF
    ;;
  header)
read -r -d '' OUTPUT << "EOF"
{{objmap.h}}
EOF
    ;;
  source)
read -r -d '' OUTPUT << "EOF"
{{objmap.c}}
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
s/KEY_TYPE/${KEY_TYPE}/g;\
s/OBJECT_TYPE/${OBJECT_TYPE}/g;\
s/OBJMAP_STRUCT/${NAME}/g;\
s/OBJMAP_TYPE/${NAME}_t/g;\
s/ENTRY_STRUCT/${NAME}_entry/g;\
s/ENTRY_TYPE/${NAME}_entry_t/g;\
s/SIZE_TYPE/${NAME}_size_t/g;\
s/OBJMAP_METHOD_INIT/${NAME}_init/g;\
s/OBJMAP_METHOD_CLEAR/${NAME}_clear/g;\
s/OBJMAP_METHOD_CREATE/${NAME}_create/g;\
s/OBJMAP_METHOD_DESTROY/${NAME}_destroy/g;\
s/OBJMAP_METHOD_FIND/${NAME}_find/g;\
s/OBJMAP_METHOD_SIZE/${NAME}_size/g;\
s/H_FILE/${H_FILE////\\/}/g;\
s/C_FILE/${C_FILE////\\/}/g"

# Perform substitutions and print
echo "$OUTPUT" | sed "$REPLACE"

