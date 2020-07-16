#!/bin/sh

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

set -eux

export BOOST="$BOOST_ROOT"
PROJECT_DIR="$(pwd)"

cd "$BOOST"
./b2 "$PROJECT_DIR/test" -q "$@"

cd "$PROJECT_DIR"
scripts/build_doc.sh "$@"

cd "$BOOST"
./b2 "$PROJECT_DIR/doc//mock_examples" -q "$@"
