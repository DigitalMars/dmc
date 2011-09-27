sc -c %1 -ms -I..\include
lib \dm\lib\sds -+%1;
sc -c %1 -mm -I..\include
lib \dm\lib\sdm -+%1;
sc -c %1 -mc -I..\include
lib \dm\lib\sdc -+%1;
sc -c %1 -ml -I..\include
lib \dm\lib\sdl -+%1;

