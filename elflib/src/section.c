
/**
 * @file section.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 14:58:09 2013
 * @brief Section handling.
 *
 * Section handling.
 */

#include "common/notify.h"
#include "elf/section.h"


/**
 * Creates a new table of sections and performs the necessary memory allocations
 * @param size  the number of sections of this table
 * @return the new allocated table of sections
 */

scntab new_scntab( uint size ) {
    scntab scntab;

    scntab.size = size;

    scntab.scn  = malloc( scntab.size*sizeof( *(scntab.scn) ) );

    if ( NULL == scntab.scn ) {
        WARNING_MSG( "Unable to allocate section table" );
    }

    return scntab;
}

/**
 * Destroys a table of sections by making all the necessary free operations
 * @param the table of sections to free
 */
void   del_scntab( scntab scntab ) {
    if ( NULL != scntab.scn ) {
        uint i;

        for ( i= 0; i< scntab.size; i++ ) {
            if ( NULL != scntab.scn[i].name ) {
                free( scntab.scn[i].name );
            }
        }

        free( scntab.scn );
    }

    return;
}


/**
* prints the content of a table of sections on the standard output
* @param the table of sections
*/
void   print_scntab( scntab scntab ) {

    if ( NULL != scntab.scn ) {
        uint i;

        printf( "Listing %u sections:\n", scntab.size );

        for ( i= 0; i< scntab.size; i++ ) {
            printf( "Scn[%02u]: %s\n", i, scntab.scn[i].name );
        }
    }

    return;
}

