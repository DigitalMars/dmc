@if exist generic.err del generic.err > nul
@start /w hcrtf -x generic.hpj
@if exist generic.err type generic.err
