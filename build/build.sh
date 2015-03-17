#!/bin/sh

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

run()
{
    echo $@
    $@
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}

copy()
{
    for dir; do true; done
    run mkdir -p $dir
    run cp $@
}

run export BOOST=$BOOST_ROOT

run cd ../test
run $BOOST/b2 -q "$@"
run cd ../build

run export BOOSTBOOK_DIR=../bin/turtle/boostbook
run copy -r "$BOOST"/tools/boostbook/xsl $BOOSTBOOK_DIR
run copy -r "$BOOST"/tools/boostbook/dtd $BOOSTBOOK_DIR
run copy -r boostbook/* $BOOSTBOOK_DIR
run copy "$BOOST"/doc/src/boostbook.css ../doc/html
run copy "$BOOST"/doc/src/images/*.png ../doc/html/images
run copy "$BOOST"/doc/src/images/callouts/*.png ../doc/html/images/callouts
run cd ../doc
run $BOOST/b2 -q "$@"
run cd ../build
