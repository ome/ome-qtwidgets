# OME-QtWidgets

OME-QtWidgets is a simple set of widgets and a viewer for simple image
rendering using OME Files C++.


Purpose
-------

Simple image rendering with OpenGL and Qt5 using OME Files C++ to read
image pixel data and metadata.

Supported formats
-----------------

OME-TIFF using the OME metadata model.

For users
---------

[Many software
packages](http://www.openmicroscopy.org/site/support/bio-formats/users/index.html)
use OME-Files to read and write microscopy formats.


For developers
--------------

You can use OME-Files C++ to easily support OME-TIFF in your software.


More information
----------------

For more information, see the [OME-Files web
site](http://www.openmicroscopy.org/site/products/bio-formats).


Pull request testing
--------------------

We welcome pull requests from anyone, but ask that you please verify the
following before submitting a pull request:

 * verify that the branch merges cleanly into ```develop```
 * verify that the branch compiles with the ```clean jars tools``` Ant targets
 * verify that the branch compiles using Maven
 * verify that the branch does not use syntax or API specific to Java 1.8+
 * run the unit tests (```ant test```) and correct any failures
 * test at least one file in each affected format, using the ```showinf```
   command
 * internal developers only: [run the data
   tests](http://www.openmicroscopy.org/site/support/bio-formats/developers/commit-testing.html)
   against directories corresponding to the affected format(s)
 * make sure that your commits contain the correct authorship information and,
   if necessary, a signed-off-by line
 * make sure that the commit messages or pull request comment contains
   sufficient information for the reviewer(s) to understand what problem was
   fixed and how to test it
