#!/bin/sh

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

copy()
{
    for dir; do true; done
    mkdir -p "$dir"
    cp "$@"
}

set -eux

export BOOST="$BOOST_ROOT"
PROJECT_DIR="$(pwd)"

cd "$BOOST"
./b2 "$PROJECT_DIR/test" -q "$@"

cd "$PROJECT_DIR"
export BOOSTBOOK_DIR="$PROJECT_DIR/bin/turtle/boostbook"
copy -r "$BOOST"/tools/boostbook/xsl "$BOOSTBOOK_DIR"
copy -r "$BOOST"/tools/boostbook/dtd "$BOOSTBOOK_DIR"
copy -r build/boostbook/* "$BOOSTBOOK_DIR"
copy "$BOOST"/doc/src/boostbook.css doc/html
copy "$BOOST"/doc/src/images/*.png doc/html/images
copy "$BOOST"/doc/src/images/callouts/*.png doc/html/images/callouts

cd "$BOOST"
./b2 "$PROJECT_DIR/doc" -q "$@"
