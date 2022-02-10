# Notes

This is a collection of notes related to reverse-engineering World.

Index ranges in "vtext.dat":

*   0001 - 1000 : Messages
*   1001 - 2000 : Room descriptions
*   2001 - 3000 : Room short names
*   3001 - 4000 : Moveable object short names
*   4001 - 5000 : Object messages (not globally unique; starts at 4001 for every object)
*   5001 - 6000 : Fixed object short names
*   9999        : Sentinel for end of file.
