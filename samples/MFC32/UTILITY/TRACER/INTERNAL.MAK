!if "$(PLATFORM)" == "M68K" || "$(PLATFORM)" == "MPPC"

!if "$(PLATFORM)" == "M68K"
!if "$(DEBUG)" == "1"
CFG=Tracer - Macintosh Debug
!else
CFG=Tracer - Macintosh Release
!endif
!else
!if "$(DEBUG)" == "1"
CFG=Tracer - Power Macintosh Debug
!else
CFG=Tracer - Power Macintosh Release
!endif
!endif

!include "tracer.mak"

!endif