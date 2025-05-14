#pragma once
#ifndef PerfectSMBios_h
#define PerfectSMBios_h


/* Basic UEFI Libraries ( I suggest compiling with VisualUefi ) */
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ShellLib.h>


//
// This is needed for SMBios structures however you 
// can manually define everything, just use the EDK2
// 
#include <Protocol/Smbios.h>	


/* This enum will represent all the different tables */
typedef enum _PSMB_TableEnum {
	PSMB_BIOSInformation			= 0,
	PSMB_SystemInformation			= 1,
	PSMB_BaseBoardInformation		= 2,
	PSMB_SystemEnclosureOrChassis	= 3,
	PSMB_ProcessorInformation		= 4,
	PSMB_CacheInformation			= 7,
	PSMB_PortConnectorInformation	= 8,
	PSMB_SystemSlots				= 9,
	PSMB_OEMStrings					= 11,
	PSMB_MemoryDevice				= 17,
	PSMB_TPMDevice					= 43,
}PSMB_TableEnum;


/* Target GUIDs */
static EFI_GUID PerfectSMBios_SMBiosTableGuid	= { 0xEB9D2D31, 0x2D88, 0x11D3, { 0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } };
static EFI_GUID PerfectSMBios_SMBios3TableGuid	= { 0xF2FD1544, 0x9794, 0x4A2C, { 0x99, 0x2E, 0xE5, 0xBB, 0xCF, 0x20, 0xE3, 0x94 } };


/* Config Table Finder */
static EFI_STATUS __fastcall PSMB_FindTable( unsigned long long SystemTable, unsigned long long* lookUpGUID, unsigned long long* out_info );


/* Getting the SMBios Table */
SMBIOS_TABLE_ENTRY_POINT* __fastcall PSMB_GetSMBiosTableEntry( EFI_SYSTEM_TABLE* SystemTable );


/* Getting the SMBios3 Table */
SMBIOS_TABLE_3_0_ENTRY_POINT* __fastcall PSMB_GetSMBios3TableEntry( EFI_SYSTEM_TABLE* SystemTable );


/* Get Target Table */
void* __fastcall PSMB_GetTargetTable( PSMB_TableEnum table_type, void* table_addr, unsigned long long table_length );


/* Getting an SMBios string */
CHAR8* __fastcall PSMB_GetSMBiosString( SMBIOS_STRUCTURE* Hdr, UINT8 str_idx );


/* Generating a random number within bounds */
UINTN PSMB_GenRandNumber( UINTN min, UINTN max );


/* Generating a random ASCII character */
CHAR8 PSMB_GenRandASCII( );


/* Generating a random ASCII string */
CHAR8* PSMB_GenRandASCIIString( UINTN len );

#endif