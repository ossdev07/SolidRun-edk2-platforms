/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <PiSmm.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>

#define NEXT_MEMORY_DESCRIPTOR(MemoryDescriptor, Size) \
  ((EFI_MEMORY_DESCRIPTOR *)((UINT8 *)(MemoryDescriptor) + (Size)))

extern EFI_MEMORY_DESCRIPTOR *mUefiMemoryMap;
extern UINTN                 mUefiMemoryMapSize;
extern UINTN                 mUefiDescriptorSize;

extern EFI_GCD_MEMORY_SPACE_DESCRIPTOR *mGcdMemoryMap;
extern EFI_GCD_IO_SPACE_DESCRIPTOR     *mGcdIoMap;
extern UINTN                           mGcdMemoryMapNumberOfDescriptors;
extern UINTN                           mGcdIoMapNumberOfDescriptors;

EFI_STATUS
TestPointCheckPageTable (
  IN EFI_PHYSICAL_ADDRESS   BaseAddress,
  IN UINTN                  Length,
  IN BOOLEAN                IsCode,
  IN BOOLEAN                IsOutsideSmram
  );

BOOLEAN
IsUefiPageNotPresent (
  IN EFI_MEMORY_DESCRIPTOR  *MemoryMap
  )
{
  switch (MemoryMap->Type) {
  case EfiLoaderCode:
  case EfiLoaderData:
  case EfiBootServicesCode:
  case EfiBootServicesData:
  case EfiConventionalMemory:
  case EfiUnusableMemory:
  case EfiACPIReclaimMemory:
    return TRUE;
  default:
    return FALSE;
  }
}

EFI_STATUS
TestPointCheckSmmCommunicationBuffer (
  IN EFI_MEMORY_DESCRIPTOR *UefiMemoryMap,
  IN UINTN                 UefiMemoryMapSize,
  IN UINTN                 UefiDescriptorSize
  )
{
  EFI_STATUS            ReturnStatus;
  EFI_STATUS            Status;
  EFI_MEMORY_DESCRIPTOR *MemoryMap;
  UINTN                 MemoryMapEntryCount;
  UINTN                 Index;

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmCommunicationBuffer - Enter\n"));

  ReturnStatus = EFI_SUCCESS;
  MemoryMapEntryCount = UefiMemoryMapSize/UefiDescriptorSize;
  MemoryMap = UefiMemoryMap;
  for (Index = 0; Index < MemoryMapEntryCount; Index++) {
    if (IsUefiPageNotPresent(MemoryMap)) {
      DEBUG ((DEBUG_INFO, "UEFI MemoryMap Checking 0x%lx - 0x%x\n", MemoryMap->PhysicalStart, EFI_PAGES_TO_SIZE(MemoryMap->NumberOfPages)));
      Status = TestPointCheckPageTable (
                 MemoryMap->PhysicalStart,
                 EFI_PAGES_TO_SIZE((UINTN)MemoryMap->NumberOfPages),
                 FALSE,
                 TRUE
                 );
      if (EFI_ERROR(Status)) {
        ReturnStatus = Status;
      }
    }
    MemoryMap = NEXT_MEMORY_DESCRIPTOR(MemoryMap, UefiDescriptorSize);
  }
  
  if (EFI_ERROR (ReturnStatus)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_CODE \
        TEST_POINT_SMM_READY_TO_BOOT \
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_STRING
      );
  }

  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmCommunicationBuffer - Exit\n"));
  return EFI_SUCCESS;
}