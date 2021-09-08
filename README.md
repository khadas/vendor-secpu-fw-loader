# SECPU FW Loader

Loads SECPU Firmware.

# Build output

`tee_secpu_fw_load`
    CA loader executable built from ca/ and lib/ source.

`tee_secpu_fw_loader.so`
    Optional CA loader shared library.

`15c8ded6-3b23-48dd-bef2-b271bcac9607.ta`
    TA

# Run
```
cp 15c8ded6-3b23-48dd-bef2-b271bcac9607.ta /lib/teetz/
./tee_secpu_fw_load bl40.bin.signed
```
