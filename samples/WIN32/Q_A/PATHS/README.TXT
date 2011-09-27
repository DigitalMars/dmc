 Sample: Using Graphic Paths Demonstration

Summary:

The PATHS sample demonstrates the use of paths for drawing,
filling, and clipping. The program draws six different
figures in the window and labels each one. Each figure is
based on the same path re-created six times. The six figures
are the result of calling the following Windows functions
(with the poly fill mode in parentheses):

   StrokePath
   FillPath
   StrokeAndFill(Winding)
   StrokeAndFill(Alternate)
   SelectClipPath(Winding)
   SelectClipPath(Alternate)

Note:

Although Windows 95 supports paths, this sample relies 
heavily on the AngleArc() API which is not implemented in
Windows 95.
