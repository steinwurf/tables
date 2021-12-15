tables
------

|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Linux CMake| |Windows CMake| |MacOS CMake| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

.. |Linux make-specs| image:: https://github.com/steinwurf/tables/actions/workflows/linux_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/linux_mkspecs.yml
   
.. |Windows make-specs| image:: https://github.com/steinwurf/tables/actions/workflows/windows_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/windows_mkspecs.yml

.. |MacOS make-specs| image:: https://github.com/steinwurf/tables/actions/workflows/macos_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/macos_mkspecs.yml

.. |Clang Format| image:: https://github.com/steinwurf/tables/actions/workflows/clang-format.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/clang-format.yml

.. |No Assertions| image:: https://github.com/steinwurf/tables/actions/workflows/nodebug.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/nodebug.yml

.. |Valgrind| image:: https://github.com/steinwurf/tables/actions/workflows/valgrind.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/valgrind.yml

.. |Cppcheck| image:: https://github.com/steinwurf/tables/actions/workflows/cppcheck.yml/badge.svg
   :target: https://github.com/steinwurf/tables/actions/workflows/cppcheck.yml

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
