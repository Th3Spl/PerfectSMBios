//
// Basic UEFI Libraries
//
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ShellLib.h>
#include ".\PerfectSMBios\PerfectSMBios.h"

/* UEFI Boilerplate */
extern CONST UINT32 _gUefiDriverRevision = 1;
CHAR8 *gEfiCallerBaseName = "PerfectSMBios";



//
// UEFI Unload | Won't be called
//
EFI_STATUS EFIAPI UefiUnload ( IN EFI_HANDLE ImageHandle ) { ASSERT(FALSE); }



//
// Calling the UEFI Main
//
EFI_STATUS EFIAPI UefiMain ( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable )
{
    /* Vars */
    EFI_STATUS  status      = EFI_SUCCESS;
    void*       table_base  = NULL;
    UINTN       table_len   = 0;


    /* Logs */
    SystemTable->ConOut->ClearScreen( SystemTable->ConOut );
    Print( L"(+) Welcome to PerfectSMBios:\n" );

    
    /* Getting the table pointer */
    SMBIOS_TABLE_ENTRY_POINT* entry2_0 = PSMB_GetSMBiosTableEntry( SystemTable );
    if ( entry2_0 )
    {
        table_base = ( void* )( ( UINTN )entry2_0->TableAddress );
        table_len = entry2_0->TableLength;
    }
    else
    {
        /* If it's version 3 this will work otherwise it's unsupported */
        SMBIOS_TABLE_3_0_ENTRY_POINT* entry3_0 = PSMB_GetSMBios3TableEntry( SystemTable );
        if ( !entry3_0 ) return EFI_UNSUPPORTED;

        /* Saving the table base */
        table_base = ( void* )( ( UINTN )entry3_0->TableAddress );
        table_len = entry3_0->TableMaximumSize;
    }
    

    /* Logs */
    Print( L"(+) Table: 0x%p\n", table_base );

    
    /* Getting the table 1 */
    SMBIOS_TABLE_TYPE1* sys_info = PSMB_GetTargetTable( PSMB_SystemInformation, table_base, table_len );


    /* Getting the manufacturer string */
    CHAR8* ascii_manufacturer = PSMB_GetSMBiosString( &sys_info->Hdr, sys_info->Manufacturer );
    if ( !ascii_manufacturer ) return EFI_UNSUPPORTED;
    UINTN manufacturer_len = AsciiStrLen( ascii_manufacturer );


    /* Converting to unicode and printing */
    CHAR16* unicode_manufacturer = AllocateZeroPool( manufacturer_len * sizeof( CHAR16 ) + sizeof( CHAR16 ) );
    if ( !unicode_manufacturer ) return EFI_UNSUPPORTED;
    AsciiStrToUnicodeStr( ascii_manufacturer, unicode_manufacturer );
    Print( L"(+) Original manufacturer: %s\n", unicode_manufacturer );
    

    /* Changing the manufacturer ( Randomized ) */
    CopyMem( ascii_manufacturer, PSMB_GenRandASCIIString( manufacturer_len ), manufacturer_len );
    Print( L"(+) Spoofed\n" );


    /* cleaning */
    FreePool( unicode_manufacturer );
    return status;
}

