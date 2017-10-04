#ifndef PIESCAPE2_GAME_UTIL_H
#define PIESCAPE2_GAME_UTIL_H

/*
 * Direction definition:
 *
 *        OpenGL           game grid       game w or h
 * N -> towards min Z -> towards max y  -> getHeight()
 * S -> towards max Z -> towards min y  -> getHeight()
 * E -> towards max X -> towards max x  -> getWidth()
 * W -> towards min X -> towards min x  -> getWidth()
 * */
//#define N 0
//#define S 1
//#define E 2
//#define W 3
typedef enum Direction { N, S, E, W } Direction;

#define OPPOSITE_DIRECTION(DIR) ((DIR)==N ? S : ((DIR)==S ? N : ((DIR)==E ? W : ((DIR)==W ? E : fatal_int("invalid dir %d", DIR)))))

#define DIRECTION_FIRST N
#define DIRECTION_LAST W

#endif //PIESCAPE2_GAME_UTIL_H
