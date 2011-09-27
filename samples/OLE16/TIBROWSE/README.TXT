---------------------------------------
OLE Automation Sample Program: TiBrowse
---------------------------------------

Tibrowse is a sample OLE Automation browser.  When started,
it asks the user to selected a type library (.tlb) file.
The browser then displays the contents of this file.




---------------------------
Shortcomings of this sample
---------------------------
1. This browser is not suitable for use by end users.  It is
deficient in many ways.  A few of them:
  - Users should never have to know that uuid's exist.  Thus,
		they should not be displayed in a browser.
  - Items that are restricted or that start with underscore
		should not be displayed in a browser.
  - Duplicates for propery get/set pairs should be removed
		since users think about properties, not about get/set
		pairs.
