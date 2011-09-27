#include <stdio.h>
#include <sound.h>
#include <stdlib.h>

int main()
{
	int pitch = 0;
	char c;

	printf("Press z to exit...");
	while((c = getch()) != 'z'){
	   switch (c){
		case 'a' : pitch = 262; break;
		case 'w' : pitch = 277; break;
		case 's' : pitch = 294; break;
		case 'e' : pitch = 311; break;	
		case 'd' : pitch = 330; break;
		case 'f' : pitch = 349; break;
		case 't' : pitch = 370; break;	
		case 'g' : pitch = 392; break;
		case 'y' : pitch = 415; break;		
		case 'h' : pitch = 440; break;
		case 'u' : pitch = 466; break;	
		case 'j' : pitch = 494; break;
		case 'k' : pitch = 2 * 262; break;
		default: pitch = 0; break;
	        }
	   if (pitch != 0) sound_beep((int)(1000000L / pitch ));
	   printf("%c ", c);
	   }
	return EXIT_SUCCESS;
}
