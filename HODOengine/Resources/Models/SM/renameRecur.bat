@echo off
setlocal enabledelayedexpansion

set "directory=%~dp0"

for %%f in ("%directory%*") do (
    for %%A in ("%%~nxf") do (
        set "filename=%%~nxA"
        if "!filename:~0,3!" neq "SM_" (
            set "new_filename=SM_!filename!"
            ren "%%f" "!new_filename!"
        )
    )
)

echo "SM_" has been prepended to the beginning of all file names.
pause