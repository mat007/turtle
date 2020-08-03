#!/bin/sh

# Copyright (C) 2015 Mathieu Champlon
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

set -eux

# Need to set BOOST_ROOT and run from the source root directory
PROJECT_DIR="$(pwd)"

cd "$BOOST_ROOT"
./b2 "$PROJECT_DIR/test" -q "$@"

cd "$PROJECT_DIR"
scripts/build_doc.sh "$@"

cd "$BOOST_ROOT"
./b2 "$PROJECT_DIR/doc//mock_examples" -q "$@"
