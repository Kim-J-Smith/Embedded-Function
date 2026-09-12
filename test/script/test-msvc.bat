@echo off
setlocal enabledelayedexpansion
rem Test with MSVC compilers in Windows

set "BUILD_TYPE=Debug"
set "CXX_STANDARDS=%*"

rem Use vcwhere to find vcvars64.bat
set "VSWHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe""
echo find %VSWHERE%
for /f "usebackq delims=" %%i in (`%VSWHERE% -find VC\Auxiliary\Build\vcvars64.bat`) do set "VCVARS64_BAT=%%i"

rem Use vcvars64.bat to configure environment
if not defined VCVARS64_BAT (
    echo vcvars64.bat not found
    exit /b 1
)
echo find "%VCVARS64_BAT%"
call "%VCVARS64_BAT%"

for %%S in (%CXX_STANDARDS%) do (
    call :config_cmake_and_run_test %%S "Ninja" ""
    if errorlevel 1 exit /b 1
    call :config_cmake_and_run_test %%S "Ninja" "EBD_TEST_USE_FALLBACK"
    if errorlevel 1 exit /b 1
)

exit /b 0

rem %1: C++ standard version
rem %2: C/C++ compiler generator (-G)
rem %3: Test use macros
:config_cmake_and_run_test
echo.
echo ============================================================
echo C++ standard version: %~1 ^| C++ compiler: MSVC
echo ============================================================
echo Build type: %BUILD_TYPE%
echo Test use macros: %~3
echo ============================================================
cmake -B build -S . -G "%~2" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_CXX_STANDARD=%~1 ^
    -DTEST_USE_MACROS=%~3
cmake --build build --config "%BUILD_TYPE%" --target test --parallel
exit /b %errorlevel%
