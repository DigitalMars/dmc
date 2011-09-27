Module Name:

    mfmt.c

Abstract:

    This program is designed to show how to access a physical floppy
    disk using the Win32 API set.

    This program has two major functions.

        - It can be used to display the geometry of a disk

            mfmt -g a:

        - It can be used to produce a disk image, or to write a disk
          image to a floppy.

            mfmt -c a: bootdisk         - produce a disk image of a:

            mfmt -c bootdisk a:         - make a: identical to bootdisk image

    This program is very very simple. Minimal error checking is done. It is
    meant to provide an example of how to:

        - Open a physical disk

        - Read a disk's geometry

        - Perform a low level format operation

        - read and write physical sectors

