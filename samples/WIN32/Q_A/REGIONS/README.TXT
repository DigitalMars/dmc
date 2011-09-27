 Sample: Using Region-Related API Functions

Summary:

The REGIONS sample demonstrates various region-related API
functions, and allows a user to create rectangular,
elliptic, and multi-polygon regions. In addition, hit-
testing a region and combining regions using the different
region-combination modes is demonstrated.

More Information:

When the program has started, create a region by choosing
one of the items in the Create submenu. At this point, items
in the Options submenu will be enabled, and hit-testing,
inversion, and other actions can be performed on the region.

When a second region is created, items in the Combine
submenu will be enabled. Choosing one of these items causes
CombineRgn to be called with the specified combine mode, and
the two regions are merged into one.

It is possible to create up to three regions at a time.
Items in the Options submenu always apply to the most
recently created (or combined) region. The Erase item
deletes all existing regions. Items in the Combine submenu
always apply to the two most recently created (or combined)
regions.

Additional reference words:

PtInRegion, CreateEllipticRgn, GetRgnBox, CreatePolygonRgn,
CreateRectRgn, SetRectRgn, OffsetRgn, FillRgn, FrameRgn
