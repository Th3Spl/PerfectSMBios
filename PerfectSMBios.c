#include "PerfectSMBios.h"


/* Config Table Finder */
static EFI_STATUS __fastcall PSMB_FindTable( unsigned long long SystemTable, unsigned long long* lookUpGUID, unsigned long long* out_info )
{
	unsigned int v2;
	unsigned int res;
	unsigned __int64 n_of_entries;
	__int64 config_table;
	__int64 v6;

	v2 = 0;
	res = ( unsigned int )-1073741275;																	/* STATUS_NOT_FOUND */
	n_of_entries = *( unsigned long long* )( ( unsigned long long )SystemTable + 104 );					/* Number of Table Entries ( 0x68 ) */
	if ( n_of_entries )
	{
		config_table = *( unsigned long long* )( ( unsigned long long )SystemTable + 112 );				/* EFI_CONFIGURATION_TABLE ptr ( 0x70 ) */
		v6 = 0i64;
		while ( *( unsigned long long* )( config_table + 24 * v6 ) != *lookUpGUID || *( unsigned long long* )( config_table + 24 * v6 + 8 ) != lookUpGUID[ 1 ] )
		{
			v6 = ++v2;
			if ( v2 >= n_of_entries )
				return res;
		}
		res = 0;
		*out_info = *( unsigned long long* )( config_table + 24 * v6 + 16 );
	}
	return res;
}


/* Getting the SMBios Table */
SMBIOS_TABLE_ENTRY_POINT* __fastcall PSMB_GetSMBiosTableEntry( EFI_SYSTEM_TABLE* SystemTable )
{
	/* Vars */
	unsigned long long smbios_addr = 0;

	/* Getting the table ( SMBiosTableGuid ) */
	if ( PSMB_FindTable(
		( unsigned long long )SystemTable,
		( unsigned long long* )&PerfectSMBios_SMBiosTableGuid,
		&smbios_addr) == 0 
	)
		return ( SMBIOS_TABLE_ENTRY_POINT* )smbios_addr;


	/* Cast and return */
	return ( SMBIOS_TABLE_ENTRY_POINT* )0;
}


/* Getting the SMBios3 Table */
SMBIOS_TABLE_3_0_ENTRY_POINT* __fastcall PSMB_GetSMBios3TableEntry( EFI_SYSTEM_TABLE* SystemTable )
{
	/* Vars */
	unsigned long long smbios_addr = 0;

	/* Getting the table ( SMBios3TableGuid ) */
	if ( PSMB_FindTable(
		( unsigned long long )SystemTable,
		( unsigned long long* ) & PerfectSMBios_SMBios3TableGuid,
		&smbios_addr ) == 0
	)
		return ( SMBIOS_TABLE_3_0_ENTRY_POINT* )smbios_addr;

	/* Cast and return */
	return ( SMBIOS_TABLE_3_0_ENTRY_POINT* )0;
}


/* Get Target Table */
void* __fastcall PSMB_GetTargetTable( PSMB_TableEnum table_type, void* table_addr, unsigned long long table_length )
{
	/* Vars */
	UINT8* table_iterator = ( UINT8* )table_addr;
	UINT8* table_end = ( UINT8* )table_addr + table_length;


	/* iterating */
	while ( table_iterator < table_end )
	{
		/* Getting the current header */
		SMBIOS_STRUCTURE* header = ( SMBIOS_STRUCTURE* )table_iterator;


		/* Checking if the target table is the one we need */
		if ( header->Type == table_type )
			return table_iterator;	/* succeeded */


		/* Getting the next structure */
		UINT8* next = table_iterator + header->Length;

		/* Skipping the double 0 termiated strings */
		while ( ( next + 1 ) < table_end && ( next[ 0 ] != 0 || next[ 1 ] != 0 ) ) next++; 

		/* Skipping the double zero terminator */
		next += 2;
		table_iterator = next;
	}


	/* failed */
	return NULL;
}


/* Getting an SMBios string ( by standard Print will use wide chars so u have to convert it ) */
CHAR8* __fastcall PSMB_GetSMBiosString( SMBIOS_STRUCTURE* Hdr, UINT8 str_idx )
{
	/* vars */
	CHAR8* str = ( CHAR8* )Hdr + Hdr->Length;

	/* check */
	if ( !Hdr || !str_idx || *str == 0 ) return 0;

	/* getting the target string */
	while ( --str_idx )
	{
		UINTN len = 0;
		while ( str && str[ len ] != '\0' ) {
			len++;
		}

		str += len + 1;
	}

	return str;
}


/* Generating a random number within bounds */
UINTN PSMB_GenRandNumber( UINTN min, UINTN max )
{
	if ( min > max ) return 0;

	UINT64 tsc = AsmReadTsc( ); /* use __rdtsc() in alternative */
	UINTN seed = ( UINT32 )( tsc ^ ( tsc >> 32 ) );

	UINT32 a = 1204565595;
	UINT32 c = 53214;
	UINT32 m = 0x7FFFFFFF;

	UINT32 rand = ( a * seed + c ) & m;
	UINT32 range = (UINT32)max - ( UINT32 )min + 1;
	return min + ( rand % range );
}


/* Generating a random ASCII character */
CHAR8 PSMB_GenRandASCII( )
{
	const char* dict = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	return dict[ PSMB_GenRandNumber( 0, 62 ) ];
}


/* Generating a random ASCII string */
CHAR8* PSMB_GenRandASCIIString( UINTN len )
{
	/* Allocating the memory for the string */
	CHAR8* string = AllocateZeroPool( len + 1 );
	if ( !string ) return 0;

	/* Filling*/
	for ( UINTN i = 0; i < len; i++ ) { string[ i ] = PSMB_GenRandASCII( ); }
	string[ len ] = '\0';

	return string;
}