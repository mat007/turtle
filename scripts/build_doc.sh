#!/bin/bash

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Execute from repo root folder or set PROJECT_DIR
# Requires BOOST_ROOT to be set

set -eu

copy()
{
    for dir; do true; done
    mkdir -p "$dir"
    cp "$@"
}

PROJECT_DIR="${PROJECT_DIR:-$(pwd)}"

export BOOSTBOOK_DIR="${PROJECT_DIR}/bin/turtle/boostbook"
copy -r "$BOOST_ROOT"/tools/boostbook/xsl "$BOOSTBOOK_DIR"
copy -r "$BOOST_ROOT"/tools/boostbook/dtd "$BOOSTBOOK_DIR"
copy -r build/boostbook/* "$BOOSTBOOK_DIR"
copy "$BOOST_ROOT"/doc/src/boostbook.css doc/html
copy "$BOOST_ROOT"/doc/src/images/*.png doc/html/images
copy "$BOOST_ROOT"/doc/src/images/callouts/*.png doc/html/images/callouts

cd "$BOOST_ROOT"
./b2 "$PROJECT_DIR/doc" -q "$@"
