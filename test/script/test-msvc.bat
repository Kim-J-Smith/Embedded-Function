@echo off
setlocal enabledelayedexpansion
rem Test with MSVC compilers in Windows

set "BUILD_TYPE=Debug"
set "CXX_STANDARDS=%*"

rem Allow the caller to use a private build directory (used by parallel runs).
if not defined BUILD_DIR set "BUILD_DIR=build"

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

for %%M in ("", "EBD_TEST_USE_FALLBACK") do (
    for %%S in (%CXX_STANDARDS%) do (
        call :config_cmake_and_run_test %%S "Ninja" %%M
        if errorlevel 1 goto fail
    )
    rmdir /s /q "%BUILD_DIR%"
)

exit /b 0

rem Workaround for cmd.exe bug.
rem See https://github.com/PowerShell/PowerShell/issues/17936.
:fail
exit /b 1

rem %1: C++ standard version
rem %2: C/C++ compiler generator (-G)
rem %3: Test use macros
:config_cmake_and_run_test
echo.
echo ============================================================
echo C++ standard: %~1 ^| C++ compiler: MSVC
echo ============================================================
echo Build type: %BUILD_TYPE%
echo Test use macros: %~3
echo ============================================================
cmake -B "%BUILD_DIR%" -S . -G "%~2" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_CXX_STANDARD=%~1 ^
    -DTEST_USE_MACROS=%~3

rem Share the compile budget with other jobs when several run in parallel.
set "PARALLEL_OPTION=--parallel"
if defined BUILD_JOBS set "PARALLEL_OPTION=--parallel %BUILD_JOBS%"

cmake --build "%BUILD_DIR%" --config "%BUILD_TYPE%" --target test %PARALLEL_OPTION%
exit /b %errorlevel%
