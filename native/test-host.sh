#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT HUP INT TERM
python3 "$root/tools/test_normalize_icky_c.py"
python3 "$root/tools/normalize_icky_c.py" \
    "$root/icky/seifert.c" "$work/seifert.c"
${CC:-cc} -std=c11 -Wall -Wextra -Werror -pedantic \
    -I "$root/native" \
    "$work/seifert.c" "$root/android/seifert_view.c" \
    "$root/native/test_seifert.c" \
    -lm -o "$work/test_seifert"
"$work/test_seifert"
