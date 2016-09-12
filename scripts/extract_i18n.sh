#!/bin/bash
DOMAIN=display3r
shopt -s nullglob

xgettext -k_ *.{c,h} --from-code='utf-8' -o $DOMAIN.pot
