if exist ihello.h del ihello.h
if exist hello.tlb del hello.tlb
win mktyplib /nocpp /h ihello.h /o hello.log /tlb hello.tlb hello.odl
