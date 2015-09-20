
/**
 * @file elf.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Nov  2 17:03:04 2013
 * @brief ELF format.
 *
 * ELF format.
 */

#include <strings.h>
#include <string.h>

#include "common/notify.h"
#include "common/bits.h"
#include "elf/elf.h"
#include "elf/formats.h"

// strdup is defined in the string.h header but included only for glibc >=2.12.
// the following declaration bypasses the version declaration
char *strdup( const char * );


/**
 * Swaps relocation entry
 * @param rel the relocation entry to swap
 * @param width the size of the target processor word in bits
 * @param e the target endianness
 * @return the address of the swapped bytes (same as input)
 */
byte * __Elf_Rel_flip_endianness( byte * rel, uint width, endianness e ) {

    printf("%p\n",rel);
    if ( e == get_host_endianness() ) return rel;

    if ( width == 32 ) {
        Elf32_Rel *e32 = (Elf32_Rel*)rel;

        FLIP_ENDIANNESS( e32->r_offset );
        FLIP_ENDIANNESS( e32->r_info );
    }

    if ( width == 64 ) {
        Elf64_Rel *e64 = (Elf64_Rel*)rel;

        FLIP_ENDIANNESS( e64->r_offset );
        FLIP_ENDIANNESS( e64->r_info );
    }

    return rel;
}

/**
 * Swaps symbol entry
 * @param sym the symbol entry to swap
 * @param width the size of the target processor word in bits
 * @param e the target endianness
 * @return the address of the swapped bytes (same as input)
 */

// static makes this function unknown outside this compilation unit (i.e., this file)
static byte * __Elf_Sym_flip_endianness( byte * sym, uint width, endianness e ) {
    if ( e == get_host_endianness() ) return sym;

    if ( width == 32 ) {
        Elf32_Sym *e32 = (Elf32_Sym*)sym;

        FLIP_ENDIANNESS( e32->st_name );
        FLIP_ENDIANNESS( e32->st_value );
        FLIP_ENDIANNESS( e32->st_size );
        FLIP_ENDIANNESS( e32->st_shndx );
    }

    if ( width == 64 ) {
        Elf64_Sym *e64 = (Elf64_Sym*)sym;

        FLIP_ENDIANNESS( e64->st_name );
        FLIP_ENDIANNESS( e64->st_value );
        FLIP_ENDIANNESS( e64->st_size );
        FLIP_ENDIANNESS( e64->st_shndx );
    }

    return sym;
}


/**
 * Swaps section table
 * @param shdr the section table to swap
 * @param width the size of the target processor word in bits
 * @param e the target endianness
 * @return the address of the swapped section table (same as input)
 */

// static makes this function unknown outside this compilation unit (i.e., this file)
static byte * __Elf_Shdr_flip_endianness( byte * shdr, uint width, endianness e ) {
    if ( e == get_host_endianness() ) return shdr;

    if ( width == 32 ) {
        Elf32_Shdr *e32 = (Elf32_Shdr*)shdr;

        FLIP_ENDIANNESS( e32->sh_name );
        FLIP_ENDIANNESS( e32->sh_type );
        FLIP_ENDIANNESS( e32->sh_flags );
        FLIP_ENDIANNESS( e32->sh_addr );
        FLIP_ENDIANNESS( e32->sh_offset );
        FLIP_ENDIANNESS( e32->sh_size );
        FLIP_ENDIANNESS( e32->sh_info );
        FLIP_ENDIANNESS( e32->sh_addralign );
        FLIP_ENDIANNESS( e32->sh_entsize );
    }

    if ( width == 64 ) {
        Elf64_Shdr *e64 = (Elf64_Shdr*)shdr;

        FLIP_ENDIANNESS( e64->sh_name );
        FLIP_ENDIANNESS( e64->sh_type );
        FLIP_ENDIANNESS( e64->sh_flags );
        FLIP_ENDIANNESS( e64->sh_addr );
        FLIP_ENDIANNESS( e64->sh_offset );
        FLIP_ENDIANNESS( e64->sh_size );
        FLIP_ENDIANNESS( e64->sh_info );
        FLIP_ENDIANNESS( e64->sh_addralign );
        FLIP_ENDIANNESS( e64->sh_entsize );
    }

    return shdr;
}

/**
 * Swaps ELF header
 * @param ehdr the header content to swap
 * @param width the size of the target processor word in bits
 * @param e the target endianness
 * @return the address of the swapped header content (same as input)
 */

// static makes this function unknown outside this compilation unit (i.e., this file)
static byte * __Elf_Ehdr_flip_endianness( byte * ehdr, uint width, endianness e ) {

    if ( e == get_host_endianness() ) return ehdr;

    if ( width == 32 ) {
        Elf32_Ehdr *e32 = (Elf32_Ehdr*)ehdr;

        FLIP_ENDIANNESS( e32->e_type );
        FLIP_ENDIANNESS( e32->e_machine );
        FLIP_ENDIANNESS( e32->e_version );
        FLIP_ENDIANNESS( e32->e_entry );
        FLIP_ENDIANNESS( e32->e_phoff );
        FLIP_ENDIANNESS( e32->e_shoff );
        FLIP_ENDIANNESS( e32->e_flags );
        FLIP_ENDIANNESS( e32->e_ehsize );
        FLIP_ENDIANNESS( e32->e_phentsize );
        FLIP_ENDIANNESS( e32->e_phnum );
        FLIP_ENDIANNESS( e32->e_shentsize );
        FLIP_ENDIANNESS( e32->e_shnum );
        FLIP_ENDIANNESS( e32->e_shstrndx );
    }

    if ( width == 64 ) {
        Elf64_Ehdr *e64 = (Elf64_Ehdr*)ehdr;

        FLIP_ENDIANNESS( e64->e_type );
        FLIP_ENDIANNESS( e64->e_machine );
        FLIP_ENDIANNESS( e64->e_version );
        FLIP_ENDIANNESS( e64->e_entry );
        FLIP_ENDIANNESS( e64->e_phoff );
        FLIP_ENDIANNESS( e64->e_shoff );
        FLIP_ENDIANNESS( e64->e_flags );
        FLIP_ENDIANNESS( e64->e_ehsize );
        FLIP_ENDIANNESS( e64->e_phentsize );
        FLIP_ENDIANNESS( e64->e_phnum );
        FLIP_ENDIANNESS( e64->e_shentsize );
        FLIP_ENDIANNESS( e64->e_shnum );
        FLIP_ENDIANNESS( e64->e_shstrndx );
    }

    return ehdr;
}


/**
 * Extracts the ELF header from ELF file
 * @param fp the pointer to the opened ELF file
 * @return the address of the raw content of the header in case of success, NULL otherwise
 */

byte * __elf_get_ehdr( FILE * fp ) {
    byte       *ehdr=NULL;
    byte        ident[ EI_NIDENT ];
    endianness  endian;

    if ( NULL == fp ) {
        WARNING_MSG( "No file to read from" );
        return NULL;
    }

    rewind( fp );

    if ( fread( ident, EI_NIDENT, 1, fp ) < 1 ) {
        WARNING_MSG( "Corrupted ELF file" );
        return NULL;
    }

    rewind( fp );
    endian = ident[EI_DATA]==ELFDATA2LSB?LSB:MSB;

    switch ( ident[ EI_CLASS ] ) {
    case ELFCLASS32 :
        ehdr = malloc( sizeof( Elf32_Ehdr ) );
        if ( fread( ehdr, sizeof( Elf32_Ehdr ), 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            return NULL;
        }
        __Elf_Ehdr_flip_endianness( ehdr, 32, endian );
        return ehdr;
    case ELFCLASS64 :
        ehdr = malloc( sizeof( Elf64_Ehdr ) );
        if ( fread( ehdr, sizeof( Elf64_Ehdr ), 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            return NULL;
        }
        __Elf_Ehdr_flip_endianness( ehdr, 64, endian );
        return ehdr;
    default :
        WARNING_MSG( "Invalid ELF class (must be 32 or 64)" );
        return NULL;
    }

    return ehdr;
}

/**
 * Extracts section name table from ELF file
 * @param fp INPUT the pointer to the opened ELF file
 * @param size OUTPUT the size in bytes of the extracted content
 * @return the address of the raw content of the section name table in case of success, NULL otherwise
 */
byte *elf_extract_section_names( FILE *fp, uint *size ) {
    byte *sstab  = NULL;
    byte *ehdr   = __elf_get_ehdr( fp );
    uint  endian = 0 ;
    uint  width  = 0 ;

    if ( NULL == ehdr ) {
        WARNING_MSG( "Can't read ELF file" );
        return NULL;
    }

    endian = ENDIANNESS_FROM_EHDR( ehdr );
    width  = WIDTH_FROM_EHDR( ehdr );
    *size  = 1;

    if ( endian != ELFDATA2LSB && endian != ELFDATA2MSB ) {
        WARNING_MSG( "Wrong endianness" );
        free( ehdr );
        return NULL;
    }

    if ( width != 32 && width != 64 ) {
        WARNING_MSG( "Wrong width" );
        free( ehdr );
        return NULL;
    }

    if ( width == 32 ) {
        Elf32_Ehdr *e = (Elf32_Ehdr*)ehdr;

        if ( e->e_shstrndx == SHN_UNDEF ) {
            WARNING_MSG( "ELF file has no section names table" );
            return NULL;
        }

        if ( 0 == e->e_shnum ) {
            WARNING_MSG( "File has no section header table" );
            free( ehdr );
            return NULL;
        }

        fseek( fp, e->e_shstrndx*e->e_shentsize+e->e_shoff, SEEK_SET );
    }

    if ( width == 64 ) {
        Elf64_Ehdr *e = (Elf64_Ehdr*)ehdr;

        if ( e->e_shstrndx == SHN_UNDEF ) {
            WARNING_MSG( "ELF file has no section names table" );
            free( ehdr );
            return NULL;


            if ( 0 == e->e_shnum ) {
                WARNING_MSG( "File has no section header table" );
                free( ehdr );
                return NULL;
            }

            fseek( fp, e->e_shstrndx*e->e_shentsize+e->e_shoff, SEEK_SET );
        } /* CHACKE */
    }
    if ( width == 32 ) {
        Elf32_Shdr shdr;
        endianness e = endian==ELFDATA2LSB?LSB:MSB;

        if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            free( ehdr );
            return NULL;
        }

        __Elf_Shdr_flip_endianness( (byte*)&shdr, 32, e );

        *size = shdr.sh_size;

        sstab = malloc( *size );

        if ( NULL == sstab ) {
            WARNING_MSG( "Can't allocate memory for section names table" );
            free( ehdr );
            return NULL;
        }

        fseek( fp, shdr.sh_offset, SEEK_SET );

        if ( fread( sstab, *size, 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            free( ehdr );
            return NULL;
        }
    }
    else {
        Elf64_Shdr shdr;
        endianness e = endian == ELFDATA2LSB?LSB:MSB;

        if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            free( ehdr );
            return NULL;
        }

        __Elf_Shdr_flip_endianness( (byte*)&shdr, 64, e );

        *size = shdr.sh_size;

        sstab = malloc( *size );

        if ( NULL == sstab ) {
            WARNING_MSG( "Can't allocate memory for section names table" );
            free( ehdr );
            return NULL;
        }

        fseek( fp, shdr.sh_offset, SEEK_SET );
        if ( fread( sstab, *size, 1, fp ) < 1 ) {
            WARNING_MSG( "Corrupted ELF file" );
            free( sstab );
            free( ehdr );
            return NULL;
        }
    }
    free( ehdr );

    return sstab;
}


/**
 * Extracts the content of a section using its section name.
 * @param ehdr INPUT the ELF header data
 * @param fp INPUT the pointer to the opened ELF file
 * @param name INPUT the name of the section to extract
 * @param secsz OUTPUT the size in bytes of the extracted content
 * @param file_offset OUTPUT the offset in bit from the beginning of the file where the raw content of the section is stored
 * @return the address of the allocated memory space in which the raw content of the section is stored in case of success, NULL otherwise
 */

byte *elf_extract_scn_by_name( byte *ehdr, FILE *fp, char *name, uint *secsz, uint *file_offset ) {
    uint        sstabsz    = 0;
    byte       *sstab      = elf_extract_section_names( fp, &sstabsz );
    uint        eendian    = ENDIANNESS_FROM_EHDR( ehdr );
    endianness  endian     = eendian==ELFDATA2LSB?LSB:MSB;
    uint        width      = WIDTH_FROM_EHDR( ehdr );
    byte       *section    = NULL;

    if ( NULL == sstab ) {
        WARNING_MSG( "Unable to read section names table" );
        return NULL;
    }

    if ( width == 32 ) {
        Elf32_Ehdr *e      = (Elf32_Ehdr*)ehdr;
        Elf32_Shdr  shdr;
        uint        i;

        for ( i= 0; i< e->e_shnum; i++ ) {
            fseek( fp, e->e_shoff+i*e->e_shentsize, SEEK_SET );

            if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
                WARNING_MSG( "Corrupted ELF file: i == %u %u %u", i, e->e_shoff, e->e_shentsize );
                return NULL;
            }

            __Elf_Shdr_flip_endianness( (byte*)&shdr, 32, endian );
            if ( 0 == strncmp( (char*)&sstab[shdr.sh_name], name, strlen(name) ) ) {
                break;
            }
        }

        if ( i == e->e_shnum ) {
            free( sstab );
            return NULL;
        }

        section = calloc(shdr.sh_size, sizeof(*section)); //calloc since .bss data should be set to 0

        if ( NULL == section ) {
            WARNING_MSG( "Unable to allocate memory for section %s", name );
            return NULL;
        }

        *secsz = shdr.sh_size;
        if ( file_offset )
            *file_offset = shdr.sh_offset;

        if ( SHT_NOBITS != shdr.sh_type ) {
            fseek( fp, shdr.sh_offset, SEEK_SET );
            if ( shdr.sh_size > 0 && fread( section, shdr.sh_size, 1, fp ) < 1 ) {
                WARNING_MSG( "Corrupted ELF file" );
                return NULL;
            }
        }
    }
    else {
        Elf64_Ehdr *e      = (Elf64_Ehdr*)ehdr;
        Elf64_Shdr  shdr;
        uint        i;

        for ( i= 0; i< e->e_shnum; i++ ) {
            fseek( fp, e->e_shoff+i*e->e_shentsize, SEEK_SET );

            if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
                WARNING_MSG( "Corrupted ELF file" );
                return NULL;
            }

            __Elf_Shdr_flip_endianness( (byte*)&shdr, 64, endian );

            if ( !strncasecmp( (char*)&sstab[shdr.sh_name], name, strlen(name) ) ) {
                break;
            }
        }

        if ( i == e->e_shnum ) {
            WARNING_MSG( "No section by that name" );
            free( sstab );
            return NULL;
        }

        section = malloc( shdr.sh_size );

        if ( NULL == section ) {
            WARNING_MSG( "Unable to allocate memory for section %s", name );
            return NULL;
        }

        *secsz = shdr.sh_size;
        if ( file_offset )
            *file_offset = shdr.sh_offset;

        if ( SHT_NOBITS != shdr.sh_type ) {
            fseek( fp, shdr.sh_offset, SEEK_SET );
            if ( fread( section, shdr.sh_size, 1, fp ) < 1 ) {
                WARNING_MSG( "Corrupted ELF file" );
                return NULL;
            }
        }
    }

    free( sstab );

    return section;
}


/**
 * Extracts the content of the table of strings
 * @param ehdr INPUT the ELF header data
 * @param fp INPUT the pointer to the opened ELF file
 * @param name INPUT the name of the section to extract
 * @param secsz OUTPUT the size in bytes of the extracted content
 * @return the address of the allocated memory space in which the raw content of the section is stored in case of success, NULL otherwise
 */
byte *elf_extract_string_table( byte *ehdr, FILE *fp, char *name, uint *sz ) {
    return elf_extract_scn_by_name( ehdr, fp, name, sz, NULL );
}

/**
 * Prints the string table on the standard output
 * @param stab the raw string table content
 * @param sz the size in bytes of the string table
 */
void  print_string_table( byte *stab, unsigned int sz ) {
    unsigned int i = 1;

    while ( i< sz ) {
        printf( "String[%03u]: %s\n", i, (char*)&stab[i] );
        i += strlen( (char*)&stab[i] ) + 1;
    }

    return;
}


/**
 * Extracts the section header table
 * @param ehdr the ELF header data
 * @param fp the pointer to the opened ELF file
 * @return the address at which the raw data is stored in case of success, NULL otherwise
 */

byte *elf_extract_section_header_table( byte * ehdr, FILE * fp ) {
    uint        sz      = 0;
    uint        width   = WIDTH_FROM_EHDR( ehdr );
    uint        eendian = ENDIANNESS_FROM_EHDR( ehdr );

    if ( 32 == width ) {
        Elf32_Ehdr *e     = (Elf32_Ehdr*)ehdr;
        Elf32_Shdr *s     = NULL;
        Elf32_Shdr *shtab = NULL;
        uint i;

        if ( 0 == e->e_shnum  ) {
            WARNING_MSG( "No section header table to extract" );
            return NULL;
        }

        sz = e->e_shnum*sizeof(*s);

        shtab = malloc( sz );


        fseek( fp, e->e_shoff, SEEK_SET );
        fread( shtab, e->e_shnum, sizeof(*s), fp );

        for ( i= 0; i< e->e_shnum; i++ ) {
            __Elf_Shdr_flip_endianness( (byte*)&shtab[i], 32, eendian );
        }

        return (byte*)shtab;
    }
    else {
        Elf32_Shdr *shtab = NULL;
        Elf64_Ehdr *e = (Elf64_Ehdr*)ehdr;

        if ( 0 < e->e_shnum  ) {
            WARNING_MSG( "No section header table to extract" );
            return NULL;
        }

        sz = e->e_shnum*e->e_shentsize;

        shtab = malloc( sz );


        fseek( fp, e->e_shoff, SEEK_SET );
        fread( shtab, e->e_shnum, e->e_shentsize, fp );

        return (byte*)shtab;
    }
}


/**
 * Extracts the symbol table
 * @param ehdr INPUT the ELF header data
 * @param fp INPUT the pointer to the opened ELF file
 * @param name INPUT the name of the section
 * @param nsymbol OUTPUT the number of symbols in the table
 * @return the address at which the raw symbol table is stored in case of success, NULL otherwise
 */


byte *elf_extract_symbol_table( byte *ehdr, FILE *fp, char *name, unsigned int *nsymbol ) {
    uint        sz         = 0;
    byte       *symtab     = elf_extract_scn_by_name( ehdr, fp, name, &sz, NULL );
    uint        width      = WIDTH_FROM_EHDR( ehdr );

    if ( NULL == symtab ) {
        WARNING_MSG( "No symbol table found" );
        return NULL;
    }

    if ( width == 32 ) {
        *nsymbol = sz/sizeof(Elf32_Sym);
        return symtab;
    }

    if ( width == 64 ) {
        *nsymbol = sz/sizeof(Elf64_Sym);
        return symtab;
    }

    return symtab;
}


/**
 * Checks whether a given file is an ELF file
 * @param fp the pointer to the opened file
 * @return 0 if not an ELF file, a non null value otherwise
 */

int assert_elf_file( FILE *fp ) {

    if ( NULL != fp ) {
        Elf32_Ehdr ehdr;

        rewind( fp );

        if ( fread( &ehdr, sizeof(ehdr), 1, fp ) < 1 ) {
            WARNING_MSG( "Can't read ELF file header" );
            return 0;
        }

        return ( ehdr.e_ident[EI_MAG0] == 0x7f &&
                 ehdr.e_ident[EI_MAG1] == 'E'  &&
                 ehdr.e_ident[EI_MAG2] == 'L'  &&
                 ehdr.e_ident[EI_MAG3] == 'F'     );
    }

    return 0;
}

/**
 * Gives the details of the architecture targeted by a given ELF file
 * @param fp INPUT the pointer to the opened ELF file
 * @param machine OUTPUT the family of processors this code is intended to (@see formats.h)
 * @param endianness OUTPUT the order with which bytes or a word are written/read in memory (@see types.h)
 * @param width OUTPUT the size of the target processor word in bits
 * @return 0 in case of success, a non null value otherwise
 */


int elf_get_arch_info( FILE *fp, uint *machine, endianness *endianness, uint *width ) {

    if ( NULL != fp ) {
        Elf32_Ehdr ehdr;

        rewind( fp );

        if ( fread( &ehdr, sizeof(ehdr), 1, fp ) < 1 ) {
            WARNING_MSG( "Can't read ELF file header" );
            return 1;
        }

        switch( ehdr.e_ident[EI_DATA] ) {
        case ELFDATA2LSB :
            *endianness = LSB;
            break;
        case ELFDATA2MSB :
            *endianness = MSB;
            break;
        default :
            *endianness = UNDEF;
            break;
        }

        switch( ehdr.e_ident[EI_CLASS] ) {
        case ELFCLASS32 :
            *width = 32;
            break;
        case ELFCLASS64 :
            *width = 64;
            break;
        default :
            *width = 0;
            break;
        }

        __Elf_Ehdr_flip_endianness( (byte*)&ehdr, *width, *endianness );

        switch( ehdr.e_machine ) {
        case EM_NONE :
        case EM_M32 :
            *machine = MACH_T_UNDEF;
            break;
        case EM_SPARC :
            *machine = MACH_T_SPARC;
            break;
        case EM_386 :
            *machine = MACH_T_I386;
            break;
        case EM_68K :
            *machine = MACH_T_M68K;
            break;
        case EM_88K :
        case EM_860 :
            *machine = MACH_T_UNDEF;
            break;
        case EM_MIPS :
            *machine = MACH_T_MIPS;
            break;
        case EM_ARM :
            *machine = MACH_T_ARM;
            break;
        default :
            *machine = MACH_T_UNDEF;
            break;
        }

        return 0;
    }

    return 1;
}

/**
 * Extracts the section table
 * @param fp INPUT the pointer to the opened ELF file
 * @param width the size of the target processor word in bits
 * @param scntab OUTPUT the memory space in with the section table must be stored
 * @return 0 in case of success, a non null value otherwise
 */

int elf_load_scntab( FILE *fp, unsigned int width, scntab *scntab ) {
    if ( 32 == width ) {
        Elf32_Ehdr *ehdr     = (Elf32_Ehdr*)__elf_get_ehdr( fp );
        uint        nscns    = NULL!=ehdr?ehdr->e_shnum:0;
        Elf32_Shdr *elf_shdr = (Elf32_Shdr*)elf_extract_section_header_table( (byte*)ehdr, fp );

        if ( NULL == elf_shdr ) {
            WARNING_MSG( "Unable to extract section header table" );
            free( ehdr );
            return 1;
        }
        else {
            uint  i;
            uint  sz;
            char *scnname = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".shstrtab", &sz, NULL );

            if ( NULL == scnname ) {
                WARNING_MSG( "Unable to extract section name table" );
                free( ehdr );
                free( elf_shdr );
                return 1;
            }

            *scntab = new_scntab( nscns );

            for ( i= 0; i< nscns; i++ ) {
                scntab->scn[i].name = strdup( &scnname[elf_shdr[i].sh_name] );
                scntab->scn[i].addr._32 = elf_shdr[i].sh_addr;
            }

            free( ehdr );
            free( elf_shdr );
            free( scnname );

            /*
            print_scntab(scntab );
            */
            return 0;
        } /* has shdr */
    } /* 32 == width */

    return 0;
}



/**
 * load the symbol table into an symtab structure.
 * Also extract section names from shstrtab when needed
 * @param fp INPUT the pointer to the opened ELF file
 * @param width INPUT the size of the target processor word in bits
 * @param endian INPUT the byte order of the targeted architecture of the ELF file
 * @param symtab OUTPUT the memory space in with the data must be stored
 * @return 0 in case of success, a non null value otherwise
 */

int elf_load_symtab( FILE *fp,  unsigned int width, unsigned int endian,  stab *symtab ) {

    // remove all previous allocation
    if (NULL == symtab)
        ERROR_MSG( "Cannot load symbols in an unallocated memory space" );
    del_stab(*symtab);


    if ( 32 == width ) {
        uint        nsyms   = 0;
        Elf32_Ehdr *ehdr    = (Elf32_Ehdr*)__elf_get_ehdr( fp );
        Elf32_Sym  *elf_sym = (Elf32_Sym*)elf_extract_symbol_table( (byte*)ehdr, fp, ".symtab", &nsyms );
        uint        nscns    = NULL!=ehdr?ehdr->e_shnum:0;
        if ( 0 == nsyms ) {
            *symtab = new_stab( 0 );
            free( ehdr );
            free( elf_sym );
            return 0;
        }


        uint   i;
        uint   namesz = 0;
        uint   sz     = 0;
        char *scnname = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".shstrtab", &sz, NULL );

        if ( NULL == scnname ) {
            WARNING_MSG( "Unable to extract section name table" );
            free( ehdr );
            free( elf_sym );
            return 1;
        }

        char  *snames = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".strtab", &namesz, NULL );
        if ( NULL == snames ) {
            WARNING_MSG( "Unable to read ELF symbol names" );
            free( scnname );
            free( ehdr );
            free( elf_sym );
            return 1;
        }

        Elf32_Shdr *elf_shdr = (Elf32_Shdr*)elf_extract_section_header_table( (byte*)ehdr, fp );
        if ( NULL == elf_shdr ) {
            WARNING_MSG( "Unable to extract section header table" );
            free( snames );

            free( scnname );
            free( ehdr );
            free( elf_sym );
            return 1;
        }

        *symtab = new_stab( nsyms );

        for ( i= 0; i< nsyms; i++ ) {
            sym_type t;
            uint     shndx;


            __Elf_Sym_flip_endianness( (byte*)&elf_sym[i], 32, endian );

            shndx = elf_sym[i].st_shndx;

            switch( ELF32_ST_TYPE( elf_sym[i].st_info ) ) {
            case STT_NOTYPE :
                t = notype;
                break;
            case STT_OBJECT :
                t = object;
                break;
            case STT_FUNC :
                t = function;
                break;
            case STT_SECTION :
                t = section;
                break;
            case STT_FILE :
                t = file;
                break;
            }


//                printf( "ST_NAME[%02u] info %d: %u : %s \n", i,t, elf_sym[i].st_name, &snames[elf_sym[i].st_name] );


            //if section, then the name of the section is in the shstrtab
            if ( section == t ) {

                if (shndx < nscns) {
                    symtab->sym[i] = new_sym32(&scnname[elf_shdr[shndx].sh_name], elf_sym[i].st_value, elf_sym[i].st_size, t, shndx );
                }
                else {
                    ERROR_MSG("Section number %d unknown for symbol %d, symbol not loaded",shndx,i);
                    free( snames );
                    free( scnname );
                    free( ehdr );
                    free( elf_sym );
                    free( elf_shdr);
                    return 1;
                }
            }
            else {
                symtab->sym[i] = new_sym32( &snames[elf_sym[i].st_name], elf_sym[i].st_value, elf_sym[i].st_size, t, shndx );

            }


        }
        free( snames );
        free( scnname );
        free( elf_shdr);
        /* non-empty symtab */
        free( ehdr );
        free( elf_sym );
    } /* 32 == width */
    return 0;
}

