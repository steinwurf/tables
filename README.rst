tables
------

.. image:: https://travis-ci.org/steinwurf/tables.svg?branch=master
    :target: https://travis-ci.org/steinwurf/tables

tables is a C++ library which allows arbitrary data to be stored as rows in a
table with column headers.

Furthermore, tables allows the stored data to be converted into other data
formats.
Currently CSV and JSON are supported, but the library can be extended to support
additional formats if needed.

Build
-----

We use the ``waf`` build system to build the tables static library.
With some additional tools which may be found at waf_

.. waf: https://github.com/steinwurf/waf

If you already installed a C++14 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/tables.git

Configure and build the project::

    cd tables
    python waf configure
    python waf build

Run the unit tests::

    python waf --run_tests

You should now have the tables static library that you can link into your own
application.

License
-------

tables is available under the BSD license, see the LICENSE.rst file.
