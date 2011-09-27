Mazelords is a 3-d maze game. The objective is to get as high a score as
possible by zapping other players, and by destroying automated drones which
wander around the maze. When there are no other players, zapping drones still
makes an interesting game in itself.

Mazelords is a networked or non-networked game, depending upon how it is
invoked. In both versions, the player is placed in a room of sanctuary in a
palace. In this room, players may not shoot, and drones cannot hurt players.
Once outside of its 3x3 confines, however, anything goes. Your task is to hunt
down drones and other players and 'zap' them. Players will be teleported back
to sanctuary, and drones will be teleported to a random location in the maze.

Drones appear as grey robots, and the number and speed of drones is
configurable under the Options.  20 drones, speed 2 is a playable setting.

The controls to maze lords are simple:

  UpArrow:    Move one square forward
  LeftArrow:  Turn left, stay in same square
  RightArrow: Turn right, stay in same square
  DownArrow:  Move one square backwards
  Space:      Zap whatever is in line of sight
  X: exit the game.
  W: Whisper something. Anyone within the specified # of squares of you will
     hear your message
  S: Shout something. Same as whisper, except the default distance people
     will be able to hear you at is different.

To play, ideally you should be running on a 486 or MIPS with lots of memory
and sound enabled (ie a soundblaster or jazzsnd). The game should work on
other systems. Note that if you don't have sound, you can still play, as
the important sounds will be changed to speaker beeps.


Upon invoking Maze, you have several options:
    FILE->NewGame: Starts a new game. Make sure you have configured the game
                   under the OPTIONS menu unless you want the defaults and want
                   to play Locally rather than over the network.
    FILE->PauseGame: This pauses the movement of all drones, but you will
                     still be a sitting duck if someone shoots at you. This will
                     eventually be changed.
    FILE->StopGame:  Resets the game completely.
    FILE->Exit: exits you from the game.

    OPTIONS->Networked/Local:  This determines whether you will be playing
             against other people on your network domain or not. It's fine to
             run NETWORK'd when there is noone else playing. Default is LOCAL.
    OPTIONS->PlayerSettings: This brings up a dialog with two list boxes. The
                             first shows a list of bitmaps you can choose from
                             to be your character. Currently there is a fish
                             and a smiley face, but the number of choices will
                             be growing.  You can also choose a home maze.
                             this isn't used right now, but soon your home maze
                             will be add to a random part of the existing maze,
                             and you will be able to teleport there at will.
    OPTIONS->Drones: This brings up the drones configuration dialog. Select
                     the number of drones you want and the speed you want them
                     to go, ie # of moves per second. Move algorithm is
                     currently ignored, as all drones use the same semi-random
                     algorithm.


The display consists of the following

o   A 3-d view of the maze. This has a few drawing bugs in terms of correctness,
    but unless you're watching for them they generally don't make a difference.
    If you have doubts about a wall, you can verify its existing on the

o   Overhead view: this is a view of two squares to each direction from
    where you are at. You can't see other players on the overhead map, but
    you can see a small triangle in the center, which indicates which way
    you are facing.  This window is an ABSOLUTE view, ie up is ALWAYS north on
    the window, regardless of which direction you're facing in the maze.

o   Text window: This gives information about what is happening in the game,
    such as who you have zapped or who zaps you, and notification when
    players enter or leave the game. You will also see messages whispered or
    shouted by other players if they are close enough.

o   Score window.  This shows all players active in your domain if you're
    networked. The display includes the bitmap being used by the player, their
    score, and a directional indicator to show the ABSOLUTE direction you need
    to go to reach the square that player is in. These are updated every
    time they change.

Have fun with the game! Any comments or suggestions should be sent to
JohnMil@microsoft.com.

Note that MazeLords is still in progress. It is cross-compilable for Windows 3.1,
but won't run under Windows 3.1 yet.  Also, there are miscellaneous glitches
which will be ironed out for the retail release of the SDK.
