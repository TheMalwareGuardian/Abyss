#!/bin/bash

# sudo mkdir -p /mnt/shared
# sudo vmhgfs-fuse .host:/SharedFolder /mnt/shared -o allow_other

# -------------------------------------------------------------------------
# Configuration: Path to the DB key and certificate
KEY="/home/themalwareguardian/src/edk2/PKfail_DB_Private.pem"
CRT="/home/themalwareguardian/src/edk2/PKfail_DB_Public.pem"

# -------------------------------------------------------------------------
# Directory containing EFI binaries to sign
EFI_DIR="/mnt/shared"
cd "$EFI_DIR" || { echo "[-] Failed to access $EFI_DIR"; exit 1; }

# -------------------------------------------------------------------------
# Sign all .efi binaries (skip those already signed)
for f in *.efi; do
	if [[ "$f" == *_Signed.efi ]]; then
		continue  # Skip already signed binaries
	fi
	echo "[*] Signing $f -> ${f%.efi}_Signed.efi"
	sbsign --key "$KEY" --cert "$CRT" --output "${f%.efi}_Signed.efi" "$f"
done
