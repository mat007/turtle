#!/bin/sh

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

copy()
{
    for dir; do true; done
    mkdir -p $dir
    cp $@
}

set -ex

export BOOST=$BOOST_ROOT

cd ../test
$BOOST/b2 -q "$@"
cd ../build

export BOOSTBOOK_DIR=../bin/turtle/boostbook
copy -r "$BOOST"/tools/boostbook/xsl $BOOSTBOOK_DIR
copy -r "$BOOST"/tools/boostbook/dtd $BOOSTBOOK_DIR
copy -r boostbook/* $BOOSTBOOK_DIR
copy "$BOOST"/doc/src/boostbook.css ../doc/html
copy "$BOOST"/doc/src/images/*.png ../doc/html/images
copy "$BOOST"/doc/src/images/callouts/*.png ../doc/html/images/callouts
cd ../doc
$BOOST/b2 -q "$@"
cd ../build
