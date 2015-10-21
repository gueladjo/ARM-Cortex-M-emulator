/**
 * @file notify.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Thu Mar 22 17:22:55 2012
 * @brief Notification management.
 *
 * Notification management.
 */

#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include <stdlib.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#define FOR_ERRORS      0x00
#define FOR_WARNINGS    0x01
#define FOR_INFOS       0x02

#define STYLE_OFF       0x00
#define STYLE_BOLD      0x01
#define STYLE_USCORE    0x04
#define STYLE_BLINK     0x05
#define STYLE_REVERSE   0x07
#define STYLE_CONCEALED 0x08

#define COLOR_BLACK       30
#define COLOR_RED         31
#define COLOR_GREEN       32
#define COLOR_YELLOW      33
#define COLOR_BLUE        34
#define COLOR_MAGENTA     35
#define COLOR_CYAN        36
#define COLOR_WHITE       37

#define STYLE_ERROR       STYLE_BLINK
#define STYLE_WARNING     STYLE_BOLD
#define STYLE_INFO        STYLE_BOLD

#define COLOR_ERROR       COLOR_RED
#define COLOR_WARNING     COLOR_YELLOW
#define COLOR_INFO        COLOR_GREEN

#define STYLE(purpose)						\
  (purpose == FOR_ERRORS ? STYLE_ERROR :			\
   (purpose == FOR_WARNINGS ? STYLE_WARNING : STYLE_INFO ))

#define COLOR(purpose)							\
  (purpose == FOR_ERRORS ? COLOR_ERROR :				\
   (purpose == FOR_WARNINGS ? COLOR_WARNING : COLOR_INFO ))

#define ON(stream) stream
#define RESET_COLORS(on_stream)			\
  fprintf( on_stream, "%c[%dm", 0x1B, 0 )
#define SET_COLORS(purpose, on_stream)		\
  fprintf( on_stream, "%c[%d;%dm", 0x1B,	\
	   STYLE(purpose), COLOR(purpose) )

#define ERROR_MSG(...) do {							\
    fprintf( stderr, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE );	\
    fprintf( stderr, "[ ERROR :: %s:%s:%d] ",				\
	     __FILE__, __FUNCTION__, __LINE__ );			\
    SET_COLORS(FOR_ERRORS, ON(stderr));					\
    fprintf( stderr, __VA_ARGS__ );					\
    fprintf( stderr, ".\n" );						\
    RESET_COLORS(ON(stderr));						\
    exit( EXIT_FAILURE );						\
} while( 0 )

#define WARNING_MSG(...) do {						\
    fprintf( stderr, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE );	\
    fprintf( stderr, "[WARNING:: %s:%s:%d] ",				\
	     __FILE__, __FUNCTION__, __LINE__ );			\
    SET_COLORS(FOR_WARNINGS, ON(stderr));				\
    fprintf( stderr, __VA_ARGS__ );					\
    fprintf( stderr, ".\n" );						\
    RESET_COLORS(ON(stderr));						\
} while( 0 )

#ifdef VERBOSE
#define INFO_MSG(...) do {							\
    fprintf( stderr, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE );	\
    fprintf( stderr, "[ INFO  :: %s:%s:%d] ",				\
	     __FILE__, __FUNCTION__, __LINE__ );			\
    SET_COLORS(FOR_INFOS, ON(stderr));					\
    fprintf( stderr, __VA_ARGS__ );					\
    fprintf( stderr, ".\n" );						\
    RESET_COLORS(ON(stderr));						\
} while( 0 )
#else
#define INFO_MSG(...)
#endif

#ifdef DEBUG
#define DEBUG_MSG(...) do {							\
    fprintf( stderr, "%c[%d;%dm", 0x1B, STYLE_BOLD, COLOR_BLUE );	\
    fprintf( stderr, "[ DEBUG :: %s:%s:%d] ",				\
	     __FILE__, __FUNCTION__, __LINE__ );			\
    SET_COLORS(FOR_INFOS, ON(stderr));					\
    fprintf( stderr, __VA_ARGS__ );					\
    fprintf( stderr, ".\n" );						\
    RESET_COLORS(ON(stderr));						\
} while( 0 )
#else
#define DEBUG_MSG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NOTIFY_H_ */
