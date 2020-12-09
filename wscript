#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'tables'
VERSION = '8.0.1'


def build(bld):

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_TABLES_VERSION="{}"'.format(VERSION))

    bld.recurse('src/tables')

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse('test')
        bld.recurse('examples/creating_printing_table')

