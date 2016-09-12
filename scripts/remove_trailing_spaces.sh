#!/bin/bash

find . \( -name '*.hpp' -o -name '*.cpp' \) -print0 | xargs -i -r -0 sed -r -i 's/\s*$//' {}
