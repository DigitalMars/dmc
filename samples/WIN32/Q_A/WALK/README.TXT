 Sample: Walking a Directory Tree

Summary:

A demonstration of how to recursively find all
subdirectories under the current working directory is
available in a sample file named WALK.

The WALK sample can be found in the \Q_A\SAMPLES\WALK
directory.

More Information:

Starting with the current working directory, a call is made
to the Walk function which will find all subdirectories in
the current working directory. When a subdirectory is found,
the current working directory is changed to this
subdirectory and another, recursive call is made to Walk,
which again will find all subdirectories in this new current
working directory. Once all subdirectories for the current
working directory have been found, the current working
directory is changed up one level (..). When the original
current working directory is re-entered, then the recursive
process stops.

Additional reference words:

FindFirstFile, GetCurrentDirectory, SetCurrentDirectory
FindNextFile, GetFileAttributes
