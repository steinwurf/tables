#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'tables'
VERSION = '5.1.1'

def options(opt):

    opt.load('wurf_common_tools')


def configure(conf):

    import waflib.extras.wurf_dependency_bundle as bundle
    import waflib.extras.wurf_dependency_resolve as resolve

    # waf-tools must be the first dependency
    bundle.add_dependency(conf, resolve.ResolveGitMajorVersion(
        name='waf-tools',
        git_repository='github.com/steinwurf/waf-tools.git',
        major_version=2))

    bundle.add_dependency(conf, resolve.ResolveGitMajorVersion(
        name='boost',
        git_repository='github.com/steinwurf/boost.git',
        major_version=1))

    if conf.is_toplevel():

        # Internal dependencies
        bundle.add_dependency(conf, resolve.ResolveGitMajorVersion(
            name='gtest',
            git_repository='github.com/steinwurf/gtest.git',
            major_version=2))

        # Download and recurse all dependencies
        conf.load("wurf_common_tools")


def build(bld):

    if bld.is_toplevel():

        bld.load("wurf_common_tools")

        bld.recurse('test')

    bld.recurse('src/tables')

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_TABLES_VERSION="{}"'.format(VERSION))
