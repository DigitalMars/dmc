# IOS.MAK September 16, 1993
# Copyright (C) 1985-1993 by Digital Mars
# All Rights Reserved
# makefile includes for IO streams library
# $Revision: 1.1.1.1 $


!if "$(OS)" == "MSDOS"
OSDIR = DOS16
OS_CHAR = D
WINFLAG =
EXFLAG =
!else if "$(OS)" == "DOS386"
OSDIR = DOS32
OS_CHAR = D
WINFLAG =
EXFLAG =
!else if "$(OS)" == "_WINDOWS"
OSDIR = WIN16
OS_CHAR = W
!if "$(MODEL)" == "L" || "$(MODEL)" == "C"
WINFLAG = -W1w
!else
WINFLAG = -W1
!endif
EXFLAG = -4
!else if "$(OS)" == "__NT__"
OSDIR = WIN32
OS_CHAR = N
WINFLAG =
EXFLAG = -D_MT -5
!endif

D=S$(OS_CHAR)$(MODEL)

ALL:   create.dir ..\..\LIB\IOSOLD$(OS_CHAR)$(MODEL).LIB



IOSFB_LIB   = $D\iosfb1.OBJ  $D\iosfb2.OBJ  $D\iosfb3.OBJ  $D\iosfb4.OBJ  $D\iosfb5.OBJ  \
              $D\iosfb6.OBJ  $D\iosfb7.OBJ  $D\iosfb8.OBJ  $D\iosfb9.OBJ  $D\iosfb10.OBJ \
              $D\iosfb11.OBJ $D\iosfb12.OBJ $D\iosfb13.OBJ $D\iosfb14.OBJ $D\iosfb15.OBJ \
              $D\iosfb16.OBJ $D\iosfb17.OBJ $D\iosfb18.OBJ $D\iosfb19.OBJ $D\iosfb20.OBJ
              
IOSFS_LIB   = $D\iosfs1.OBJ  $D\iosfs2.OBJ  $D\iosfs3.OBJ  $D\iosfs4.OBJ  $D\iosfs5.OBJ  \
              $D\iosfs6.OBJ  $D\iosfs7.OBJ  $D\iosfs8.OBJ  $D\iosfs9.OBJ  $D\iosfs10.OBJ \
              $D\iosfs11.OBJ $D\iosfs12.OBJ $D\iosfs13.OBJ $D\iosfs14.OBJ $D\iosfs15.OBJ \
              $D\iosfs16.OBJ $D\iosfs17.OBJ $D\iosfs18.OBJ $D\iosfs19.OBJ
              
IOSIOS_LIB  = $D\iosios1.OBJ $D\iosios2.OBJ $D\iosios3.OBJ $D\iosios4.OBJ $D\iosios5.OBJ \
              $D\iosios6.OBJ $D\iosios7.OBJ $D\iosios8.OBJ $D\iosios9.OBJ              
              
IOSIOST_LIB = $D\iosiost1.OBJ $D\iosiost2.OBJ $D\iosiost3.OBJ

IOSIOWA_LIB = $D\iosiowa1.OBJ $D\iosiowa2.OBJ $D\iosiowa3.OBJ $D\iosiowa4.OBJ

IOSIS_LIB   = $D\iosis1.OBJ  $D\iosis2.OBJ  $D\iosis3.OBJ  $D\iosis4.OBJ  $D\iosis5.OBJ  \
              $D\iosis6.OBJ  $D\iosis7.OBJ  $D\iosis8.OBJ  $D\iosis9.OBJ  $D\iosis10.OBJ \
              $D\iosis11.OBJ $D\iosis12.OBJ $D\iosis13.OBJ $D\iosis14.OBJ $D\iosis15.OBJ \
              $D\iosis16.OBJ $D\iosis17.OBJ $D\iosis18.OBJ $D\iosis19.OBJ $D\iosis20.OBJ \
              $D\iosis21.OBJ $D\iosis22.OBJ $D\iosis23.OBJ $D\iosis24.OBJ $D\iosis25.OBJ \
              $D\iosisdfa.OBJ
              
IOSIWA_LIB  = $D\iosiwa1.OBJ $D\iosiwa2.OBJ $D\iosiwa3.OBJ $D\iosiwa4.OBJ

IOSMAN_LIB  = $D\iosman1.OBJ $D\iosman2.OBJ $D\iosman3.OBJ $D\iosman4.OBJ $D\iosman5.OBJ \
              $D\iosman6.OBJ $D\iostfx.OBJ

IOSOS_LIB   = $D\iosos1.OBJ  $D\iosos2.OBJ  $D\iosos3.OBJ              $D\iosos5.OBJ  \
              $D\iosos6.OBJ  $D\iosos7.OBJ  $D\iosos8.OBJ  $D\iosos9.OBJ  $D\iosos10.OBJ \
              $D\iosos11.OBJ $D\iosos12.OBJ $D\iosos13.OBJ $D\iosos14.OBJ $D\iosos15.OBJ \
              $D\iosos16.OBJ $D\iosos17.OBJ $D\iosos18.OBJ $D\iosos19.OBJ $D\iosos20.OBJ \
              $D\iosos21.OBJ $D\iosos22.OBJ $D\iosos23.OBJ $D\iosos24.OBJ $D\iosos25.OBJ \
              $D\iosos26.OBJ $D\iosos27.OBJ $D\iosos28.OBJ $D\iosos29.OBJ $D\iosos30.OBJ \
              $D\iosos31.OBJ $D\iosos32.OBJ $D\iosos33.OBJ $D\iososfp.OBJ
              
IOSOWA_LIB  = $D\iosowa1.OBJ $D\iosowa2.OBJ $D\iosowa3.OBJ $D\iosowa4.OBJ

IOSSB_LIB   = $D\iossb1.OBJ  $D\iossb2.OBJ  $D\iossb3.OBJ  $D\iossb4.OBJ  $D\iossb5.OBJ  \
              $D\iossb6.OBJ  $D\iossb7.OBJ  $D\iossb8.OBJ  $D\iossb9.OBJ  $D\iossb10.OBJ \
              $D\iossb11.OBJ $D\iossb12.OBJ $D\iossb13.OBJ $D\iossb14.OBJ
              
IOSSTD_LIB  = $D\iosstd1.OBJ $D\iosstd2.OBJ

            
IOSOLD_LIB  = $(IOSFB_LIB)   $(IOSFS_LIB)   $(IOSIOS_LIB) $(IOSIOST_LIB) \
              $(IOSIOWA_LIB) $(IOSIS_LIB)   $(IOSIWA_LIB) $(IOSMAN_LIB)  \
              $(IOSOS_LIB)   $(IOSOWA_LIB)  $(IOSSB_LIB)  $(IOSSTD_LIB)  \
              $D\IOS2CD.OBJ    $D\IOS2CN.OBJ    $D\IOS2CX.OBJ   $D\IOSIS2C.OBJ    \
              $D\IOSISFLT.OBJ  $D\IOSOS2C.OBJ   $D\IOSST1.OBJ   $D\IOSST2.OBJ     \
              $D\IOSST3.OBJ    $D\IOSST4.OBJ    $D\IOSST5.OBJ   $D\STDIOBUF.OBJ   \
              $D\STRSBUF.OBJ   $D\STRSTRM.OBJ
              
create.dir:
	@-if not exist $D\*.* mkdir $D

clean:
    -del $(IOSOLD_LIB)              

..\..\LIB\IOSOLD$(OS_CHAR)$(MODEL).LIB : $(IOSOLD_LIB)
   IF EXIST ..\..\LIB\IOSOLD$(OS_CHAR)$(MODEL).LIB DEL ..\..\LIB\IOSOLD$(OS_CHAR)$(MODEL).LIB
   LIB /C /NOI /PAGESIZE:16 @<<
     ..\..\LIB\IOSOLD$(OS_CHAR)$(MODEL)
     +$(IOSOLD_LIB: =&^
     +)
     ;
<<


{..\IOSOLD}.CPP{$D}.OBJ:
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSFB_LIB)   : ..\IOSOLD\IOSFB.CPP      
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s
    
$(IOSFS_LIB)   : ..\IOSOLD\IOSFS.CPP      
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSIOS_LIB)  : ..\IOSOLD\IOSIOS.CPP     
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSIOST_LIB) : ..\IOSOLD\IOSIOST.CPP    
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSIOWA_LIB) : ..\IOSOLD\IOSIOWA.CPP    
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSIS_LIB)   : ..\IOSOLD\IOSIS.CPP      
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSIWA_LIB)  : ..\IOSOLD\IOSIWA.CPP     
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSMAN_LIB)  : ..\IOSOLD\IOSMAN.CPP     
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s

$(IOSOS_LIB)   : ..\IOSOLD\IOSOS.CPP      
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSOWA_LIB)  : ..\IOSOLD\IOSOWA.CPP     
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                     

$(IOSSB_LIB)   : ..\IOSOLD\IOSSB.CPP      
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s                  

$(IOSSTD_LIB)  : ..\IOSOLD\IOSSTD.CPP     
  SC -w12 -Ae -Ar -c -o -D$(OS)=1 -m$(MODEL) -wx -r -Nc -NL -DA$(@B) -I..\INCLUDE -I..\..\INCLUDE -o$* $(WINFLAG) $(EXFLAG) %s

