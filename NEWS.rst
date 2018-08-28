News for tables
===============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* tbd

7.0.2
-----
* Patch: Fix const qualifier warning with g++ 8.2.

7.0.1
-----
* Patch: Added virtual constructor to format.

7.0.0
-----
* Major: Upgrade to waf-tools 4
* Major: Upgrade to boost 3
* Minor: Upgrade to gtest 4

6.0.0
-----
* Major: Upgrade to waf-tools 3
* Major: Upgrade to boost 2
* Minor: Upgrade to gtest 3
* Minor: Added buildbot.py for coverage reports.
* Minor: Added a simple example showing how to create and print a table in
  different formats. Added by Juan Cabrera.

5.1.2
-----
* Patch: Fix conversion warnings with 64-bit MSVC.

5.1.1
-----
* Patch: Fix version define.

5.1.0
-----
* Minor: Added version define.

5.0.0
-----
* Major: Made single argument constructors explicit
* Minor: Fixed style issues
* Minor: Update to waf 1.8.0-pre1
* Minor: Made python files comply with pep8
* Patch: Add missing iostream include in format.hpp

4.1.0
-----
* Minor: Improved vector nesting support.

4.0.0
-----
* Major: Modified the API for specifying default values.
* Minor: Improved unit test for table merging.

3.0.1
-----
* Bug: Bumped version number in wscript.

3.0.0
-----
* Major: Modified the API used for tables.

2.0.0
-----
* Major: Deleted pylist and pydict.
* Minor: Adding a method to ``column`` to check if it has a fill value.
* Bug: Fixed Python and JSON output.
* Bug: Fixed Windows builds.

1.0.0
-----
* Initial release.
