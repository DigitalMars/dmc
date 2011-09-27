
TARGETOS=WINNT

!include <ntwin32.mak>

opengllibs	    = opengl32.lib glu32.lib glaux.lib

EXES	= accanti.exe  \
          accnot.exe   \
          accpersp.exe \
          accum.exe    \
          aim.exe      \
          alpha.exe    \
          alpha3d.exe  \
          anti.exe     \
          antindx.exe  \
          antipndx.exe \
          antipnt.exe  \
          antipoly.exe \
          bezcurve.exe \
          bezmesh.exe  \
          bezsurf.exe  \
          checker.exe  \
          checker2.exe \
          chess.exe    \
          clip.exe     \
          colormat.exe \
          cone.exe     \
          cube.exe     \
          curve.exe    \
          depthcue.exe \
          disk.exe     \
          dof.exe      \
          dofnot.exe   \
          double.exe   \
          drawf.exe    \
          feedback.exe \
          fog.exe      \
          fogindex.exe \
          font.exe     \
          light.exe    \
          linelist.exe \
          lines.exe    \
          list.exe     \
          list2.exe    \
          maplight.exe \
          material.exe \
          mipmap.exe   \
          model.exe    \
          movelght.exe \
          nurbs.exe    \
          pickdpth.exe \
          pickline.exe \
          picksqr.exe  \
          plane.exe    \
          planet.exe   \
          planetup.exe \
          polys.exe    \
          robot.exe    \
          scclrlt.exe  \
          scene.exe    \
          scenebmb.exe \
          sceneflt.exe \
          select.exe   \
          simple.exe   \
          smooth.exe   \
          sphere.exe   \
          stencil.exe  \
          stroke.exe   \
          surface.exe  \
          tea.exe      \
          teaamb.exe   \
          teapots.exe  \
          texgen.exe   \
          texsurf.exe  \
          trim.exe

all: $(EXES)

.c.exe:
    $(cc) $(cflags) $(cdebug) -DSILENT_WARN $(cvars) $<
    $(link) $(linkdebug) $(guiflags) -subsystem:windows -entry:mainCRTStartup -out:$*.exe $*.obj $(opengllibs) $(guilibs)
