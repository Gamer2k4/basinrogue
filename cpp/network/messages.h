#define MSG_SENDTILE    'l' // define a tile based on position in large bitmap
#define MSG_SENDSOUND   's' // define a sound effect
#define MSG_RESIZEWORLD 'd' // send the level size
#define MSG_ADDTILE     't' // send across one tile of the current level
#define MSG_CLEARTILE   'c' // erase one tile of the current level
#define MSG_SETCHARPOS  '@' // send across the current position of the @
#define MSG_CLEARLEVEL  'r' // clear the entire current level. Not used currently?
#define MSG_PLAYSOUND   'p' // play a sound
#define MSG_VIEWISREADY 'o' // when this is received, the map is ready to be displayed
#define MSG_SWAPBUFFERS 'f' // flash the double-buffering
#define MSG_SENDMESSAGE 'm' // send a text message to the player