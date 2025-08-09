<#
.SYNOPSIS
Checks the current Secure Boot status and validates the Platform Key (PK) for test keys.

.DESCRIPTION
Runs Confirm-SecureBootUEFI to determine if Secure Boot is currently enabled and protecting the system.
Then retrieves and decodes the Platform Key (PK) UEFI variable, checking if it contains known test keys
marked as "DO NOT TRUST" or "DO NOT SHIP". Displays clear, user-friendly messages summarizing the results.
#>

Write-Host "=============================================="
Write-Host "        Secure Boot and PK State Checker"
Write-Host "=============================================="
Write-Host ""

# Run Confirm-SecureBootUEFI
Write-Host "[*] Executing command: Confirm-SecureBootUEFI"
$secureBootEnabled = Confirm-SecureBootUEFI

if ($secureBootEnabled -eq $true) {
	Write-Host "[+] Secure Boot is ENABLED and protecting your system." -ForegroundColor Green
} elseif ($secureBootEnabled -eq $false) {
	Write-Host "[-] Secure Boot is DISABLED. Your system is not protected." -ForegroundColor Red
} else {
	Write-Host "[-] Secure Boot state could not be determined." -ForegroundColor Yellow
}

# Check PK variable for test keys
Write-Host "[*] Executing command: Get-SecureBootUEFI -Name PK"
$pkBytes = (Get-SecureBootUEFI -Name PK).bytes
$pkString = [System.Text.Encoding]::ASCII.GetString($pkBytes)

if ($pkString -match "DO NOT TRUST|DO NOT SHIP") {
	Write-Host "[-] Platform Key (PK) contains TEST keys: DO NOT TRUST / DO NOT SHIP detected!" -ForegroundColor Red
} else {
	Write-Host "[+] Platform Key (PK) appears VALID. No test keys detected." -ForegroundColor Green
}

# Done
Write-Host "[+] Secure Boot and PK check completed. Review the results above."
