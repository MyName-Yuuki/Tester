# Microsoft Developer Studio Project File - Name="ElementClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ELEMENTCLIENT - WIN32 DEBUGLAA
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ElementClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ElementClient.mak" CFG="ELEMENTCLIENT - WIN32 DEBUGLAA"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ElementClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DbgRelease" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DebugCRTAllocator" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DbgReleaseLAA" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DebugLAA" (based on "Win32 (x86) Application")
!MESSAGE "ElementClient - Win32 DbgReleaseLAALOG" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Element/ElementClient", AQDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ElementClient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /YX /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ArcAsiaSDK.lib psapi.lib zlibwapi.lib GfxCommon.lib CHBasedCD.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_u.lib AngelicaMedia_u.lib AngelicaFile_u.lib Angelica3D_u.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\OnScreenModelDisplay.exe" /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport"
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=getcodeseg                                                                                                                                                                                                                                                                                                                                                                                                   ..\CBin\ElementClient.exe                                                                                                                                                                                                                                                                                                                                                                                                  ..\CBin\interfaces\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ElementClient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "LOG_PROTOCOL" /FD /GZ /Zm150 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ArcAsiaSDK_d.lib version.lib psapi.lib zlibwapi.lib GfxCommon_d.lib CHBasedCD_d.lib ElementSkill_d.lib AUInterface_d.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib glu32.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib FWEditorLib_d.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib LuaWrapper_du.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib strmbase_u.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\CBin\OnScreenModelDisplay_d.exe" /pdbtype:sept /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DbgRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ElementClient___Win32_DbgRelease"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DbgRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgRelease"
# PROP Intermediate_Dir "DbgRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\..\Angelica2\Output\A3DSDK\Include" /I "..\..\Angelica2\Output\AFSDK\Include" /I "..\..\Angelica2\Output\ACSDK\Include" /I "..\..\Angelica2\Output\AMSDK\Include" /I "..\..\Angelica2\Output\3rdSDK\Include" /I "F:\DirectX 8.1 SDK\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon.lib CHBasedCD.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_u.lib AngelicaMedia_u.lib AngelicaFile_u.lib Angelica3D_u.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\Bin\ElementClient.exe" /libpath:"..\Lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /libpath:"..\..\Angelica2\Output\A3DSDK\Lib" /libpath:"..\..\Angelica2\Output\AFSDK\Lib" /libpath:"..\..\Angelica2\Output\ACSDK\Lib" /libpath:"..\..\Angelica2\Output\AMSDK\Lib" /libpath:"..\..\Angelica2\Output\3rdSDK\Lib" /libpath:"F:\DirectX 8.1 SDK\lib"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 ArcAsiaSDK.lib psapi.lib zlibwapi.lib GfxCommon_su.lib CHBasedCD_su.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\OnScreenModelDisplay_dr.exe" /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport" /OPT:REF
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=getcodeseg                                                                                                                                                                                                                                                                                           ..\CBin\ElementClient_dr.exe                                                                                                                                                                                                                                                                                          ..\CBin\interfaces\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DebugCRTAllocator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ElementClient___Win32_DebugCRTAllocator"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DebugCRTAllocator"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugCRT"
# PROP Intermediate_Dir "DebugCRT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /Fr /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "_A_FORBID_NEWDELETE" /D "_A_FORBID_MALLOC" /D "LOG_PROTOCOL" /Fr /FD /GZ /Zm150 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon_d.lib CHBasedCD_d.lib ElementSkill_d.lib AUInterface_d.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib glu32.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib FWEditorLib_d.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib LuaWrapper_du.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\CBin\ElementClient_d.exe" /pdbtype:sept /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib psapi.lib zlibwapi.lib GfxCommon_smd.lib CHBasedCD_d.lib AUInterface_smd.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_sud.lib AngelicaMedia_sud.lib AngelicaFile_sud.lib Angelica3D_sud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib glu32.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib LuaWrapper_du_CRT.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib strmbase_u.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\CBin\OnScreenModelDisplay_d_CRT.exe" /pdbtype:sept /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DbgReleaseLAA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ElementClient___Win32_DbgReleaseLAA"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DbgReleaseLAA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgReleaseLAA"
# PROP Intermediate_Dir "DbgReleaseLAA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /FD /Zm150 /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon_su.lib CHBasedCD_su.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\ElementClient_law.exe" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /OPT:REF /LARGEADDRESSAWARE
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 psapi.lib zlibwapi.lib GfxCommon_su.lib CHBasedCD_su.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\OnScreenModelDisplay_laa.exe" /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport" /OPT:REF /LARGEADDRESSAWARE
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DebugLAA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ElementClient___Win32_DebugLAA"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DebugLAA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugLAA"
# PROP Intermediate_Dir "DebugLAA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /FD /GZ /Zm150 /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "LOG_PROTOCOL" /FD /GZ /Zm150 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon_d.lib CHBasedCD_d.lib ElementSkill_d.lib AUInterface_d.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib glu32.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib FWEditorLib_d.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib LuaWrapper_du.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\CBin\ElementClient_d.exe" /pdbtype:sept /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib psapi.lib zlibwapi.lib GfxCommon_d.lib CHBasedCD_d.lib AUInterface_d.lib mpg123lib_d.lib Immwrapper_d.lib AngelicaCommon_ud.lib AngelicaMedia_ud.lib AngelicaFile_ud.lib Angelica3D_ud.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib glu32.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert_d.lib FTDriver_D.lib shlwapi.lib wininet.lib glu32.lib lua5.1.mtd.lib LuaWrapper_du.lib vorbisfile_static_d.lib vorbis_static_d.lib ogg_static_d.lib asynbase_u.lib strmbase_u.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"..\CBin\OnScreenModelDisplay_d_laa.exe" /pdbtype:sept /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport" /LARGEADDRESSAWARE
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ElementClient - Win32 DbgReleaseLAALOG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ElementClient___Win32_DbgReleaseLAALOG"
# PROP BASE Intermediate_Dir "ElementClient___Win32_DbgReleaseLAALOG"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DbgReleaseLAALOG"
# PROP Intermediate_Dir "DbgReleaseLAALOG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\Include\stlport" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /FD /Zm150 /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\Include\stlport" /I "..\..\..\SDK\3rdSDK\include" /I "..\..\..\SDK\A3DSDK\Include" /I "..\..\..\SDK\acsdk\Include" /I "..\..\..\SDK\afsdk\include" /I "..\..\..\SDK\Amsdk\include" /I "..\..\..\SDK\Apsdk\Include" /I "..\..\..\GfxCommon\Output\include" /I "..\AUInterface2" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Foundation\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Cooking\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\Physics\include" /I "..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\PhysXLoader\include" /I "..\include\stlport" /I ".\Home" /I "..\CCommon\Autoca" /I "..\CCommon\Face" /I ".\\" /I ".\Network\rpcdata" /I ".\Network\inl" /I ".\Network\IOLib" /I ".\Network" /I ".\Task" /I "..\include\CD" /I "..\Include" /I "..\CCommon" /I "..\CCommon\Gfx" /I "..\include\LuaWrapper" /I ".\CrossServer" /I "..\include\autopfimp" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_ELEMENTCLIENT" /D "LOG_PROTOCOL" /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlibwapi.lib GfxCommon_su.lib CHBasedCD_su.lib ElementSkill.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib FWEditorLib.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\ElementClient_laa.exe" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\stlport" /libpath:"..\Lib\AUI" /OPT:REF /LARGEADDRESSAWARE
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 psapi.lib zlibwapi.lib GfxCommon_su.lib CHBasedCD_su.lib UnicoWS.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib AUInterface_su.lib mpg123lib.lib Immwrapper.lib AngelicaCommon_dr.lib AngelicaMedia_dr.lib AngelicaFile_dr.lib Angelica3D_dr.lib ws2_32.lib Imm32.lib zliblib.lib dxguid.lib d3d8.lib d3dx8.lib ddraw.lib dsound.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib speedtreert.lib FTDriver.lib shlwapi.lib wininet.lib lua5.1.mt.lib LuaWrapper_u.lib vorbisfile_static.lib vorbis_static.lib ogg_static.lib asynbase_u.lib strmbase_u.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"..\CBin\OnScreenModelDisplay_laa_log.exe" /libpath:"..\lib\autopfimp" /libpath:"..\lib\CD" /libpath:"..\Lib" /libpath:"..\Lib\IOLib" /libpath:"..\Lib\AUI" /libpath:"..\..\..\SDK\DbgRelease\Angelica3D\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaCommon\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaFile\DBGRelease" /libpath:"..\..\..\SDK\DbgRelease\AngelicaMedia\DBGRelease" /libpath:"..\..\..\SDK\3rdSDK\lib" /libpath:"..\..\..\SDK\A3DSDK\Lib" /libpath:"..\..\..\SDK\acsdk\lib" /libpath:"..\..\..\SDK\afsdk\lib" /libpath:"..\..\..\SDK\Amsdk\lib" /libpath:"..\..\..\SDK\Apsdk\Lib" /libpath:"..\..\..\SDK\3rdSDK\lib\IMMLib" /libpath:"..\..\..\SDK\3rdSDK\lib\dshowbase" /libpath:"..\AUInterface2\Lib" /libpath:"..\..\..\GfxCommon\Output\Lib" /libpath:"..\..\..\APhysXIntegration\PhysX_SDK\v2.8.1\SDKs\lib\Win32" /libpath:"..\lib\stlport" /OPT:REF /LARGEADDRESSAWARE
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ElementClient - Win32 Release"
# Name "ElementClient - Win32 Debug"
# Name "ElementClient - Win32 DbgRelease"
# Name "ElementClient - Win32 DebugCRTAllocator"
# Name "ElementClient - Win32 DbgReleaseLAA"
# Name "ElementClient - Win32 DebugLAA"
# Name "ElementClient - Win32 DbgReleaseLAALOG"
# Begin Group "Main Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Input Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_GhostInputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GhostInputFilter.h
# End Source File
# Begin Source File

SOURCE=.\EC_InputCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_InputCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EC_InputFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_InputFilter.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AMiniDump.cpp
# End Source File
# Begin Source File

SOURCE=.\AMiniDump.h
# End Source File
# Begin Source File

SOURCE=.\EC_AssureMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_AssureMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_CameraCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CameraCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EC_CDR.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CDR.h
# End Source File
# Begin Source File

SOURCE=.\EC_CDS.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CDS.h
# End Source File
# Begin Source File

SOURCE=.\EC_CommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CommandLine.h
# End Source File
# Begin Source File

SOURCE=.\EC_Configs.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Configs.h
# End Source File
# Begin Source File

SOURCE=.\EC_Counter.h
# End Source File
# Begin Source File

SOURCE=.\EC_Ease.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Ease.h
# End Source File
# Begin Source File

SOURCE=.\EC_ElementDataHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ElementDataHelper.h
# End Source File
# Begin Source File

SOURCE=.\EC_FullGlowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_FullGlowRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_GFXCaster.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GFXCaster.h
# End Source File
# Begin Source File

SOURCE=.\EC_ImageRes.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ImageRes.h
# End Source File
# Begin Source File

SOURCE=.\EC_PortraitRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PortraitRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Profile.h
# End Source File
# Begin Source File

SOURCE=.\EC_Resource.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Resource.h
# End Source File
# Begin Source File

SOURCE=.\EC_RewuShader.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_RewuShader.h
# End Source File
# Begin Source File

SOURCE=.\EC_RTDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_RTDebug.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneCheck.h
# End Source File
# Begin Source File

SOURCE=.\EC_ScreenEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ScreenEffect.h
# End Source File
# Begin Source File

SOURCE=.\EC_ShadowRender.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ShadowRender.h
# End Source File
# Begin Source File

SOURCE=.\EC_ShakeCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ShakeCamera.h
# End Source File
# Begin Source File

SOURCE=.\EC_SoundCache.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SoundCache.h
# End Source File
# Begin Source File

SOURCE=.\EC_Split.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Split.h
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_StringTab.h
# End Source File
# Begin Source File

SOURCE=.\EC_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Time.h
# End Source File
# Begin Source File

SOURCE=.\EC_TimeSafeChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_TimeSafeChecker.h
# End Source File
# Begin Source File

SOURCE=.\EC_Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Utility.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_Instance.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Instance.h
# End Source File
# Begin Source File

SOURCE=.\EC_RandomMapPreProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_RandomMapPreProcessor.h
# End Source File
# Begin Source File

SOURCE=.\EC_Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Scene.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneBlock.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLights.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLights.h
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SceneLoader.h
# End Source File
# Begin Source File

SOURCE=.\EC_World.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_World.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_WorldFile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_FixedMsg.h
# End Source File
# Begin Source File

SOURCE=.\EC_Game.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Game.h
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GameRun.h
# End Source File
# Begin Source File

SOURCE=.\EC_Global.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Global.h
# End Source File
# Begin Source File

SOURCE=.\EC_MsgDataDef.h
# End Source File
# Begin Source File

SOURCE=.\EC_MsgDef.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_MsgDef.h
# End Source File
# Begin Source File

SOURCE=.\EC_Viewport.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Viewport.h
# End Source File
# Begin Source File

SOURCE=.\ElementClient.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementClient.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Object Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Players"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_CastSkillWhenMove.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CastSkillWhenMove.h
# End Source File
# Begin Source File

SOURCE=.\EC_LoginPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoginPlayer.h
# End Source File
# Begin Source File

SOURCE=.\EC_Player.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Player.h
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerActionController.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerActionController.h
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerBodyController.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_PlayerBodyController.h
# End Source File
# Begin Source File

SOURCE=.\EC_RoleTypes.h
# End Source File
# End Group
# Begin Group "Scene Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CCommon\EC_Bezier.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_Bezier.h
# End Source File
# Begin Source File

SOURCE=.\EC_CritterGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_CritterGroup.h
# End Source File
# Begin Source File

SOURCE=.\EC_GrassArea.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_GrassArea.h
# End Source File
# Begin Source File

SOURCE=.\EC_HomeOrnament.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_HomeOrnament.h
# End Source File
# Begin Source File

SOURCE=.\EC_Ornament.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Ornament.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_SceneObject.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_SceneThreadObj.h
# End Source File
# Begin Source File

SOURCE=.\EC_ScnBoxArea.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ScnBoxArea.h
# End Source File
# End Group
# Begin Group "Inventory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_IvtrArmor.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrArmor.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrArrow.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrConsume.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrConsume.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDecoration.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDecoration.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDestroyingEssence.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrDestroyingEssence.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquip.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquip.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquipMatter.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrEquipMatter.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrFashion.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrFashion.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrFlySword.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrFlySword.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrGoblin.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrGoblin.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMaterial.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMoneyConvertible.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrMoneyConvertible.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrPetItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrPetItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrScroll.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrStone.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrStone.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTaskItem.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTaskItem.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrTypes.h
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_IvtrWeapon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EC_Decal.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Decal.h
# End Source File
# Begin Source File

SOURCE=.\EC_Face.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Face.h
# End Source File
# Begin Source File

SOURCE=.\EC_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Object.h
# End Source File
# Begin Source File

SOURCE=.\EC_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\EC_SunMoon.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_SunMoon.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# Begin Group "Interface Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreate.h
# End Source File
# Begin Source File

SOURCE=.\DlgTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTheme.h
# End Source File
# Begin Source File

SOURCE=.\EC_BaseUIMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_BaseUIMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_LoginUIMan.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_LoginUIMan.h
# End Source File
# Begin Source File

SOURCE=.\EC_ProfConfigs.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ProfConfigs.h
# End Source File
# Begin Source File

SOURCE=.\EC_ProfDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ProfDisplay.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIAnimation.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIConfigs.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIConfigs.h
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_UIManager.h
# End Source File
# Begin Source File

SOURCE=..\CElementData\vector_string.cpp
# End Source File
# Begin Source File

SOURCE=..\CElementData\vector_string.h
# End Source File
# Begin Source File

SOURCE=.\WikiDataReader.cpp
# End Source File
# Begin Source File

SOURCE=.\WikiDataReader.h
# End Source File
# End Group
# Begin Group "Manager Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EC_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Manager.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManDecal.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManMessage.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManOrnament.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManOrnament.h
# End Source File
# Begin Source File

SOURCE=.\EC_ManPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_ManPlayer.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "GFX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CCommon\Gfx\A3DSkillGfxComposer2.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Gfx\A3DSkillGfxComposer2.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\Gfx\A3DSkillGfxEvent2.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Gfx\A3DSkillGfxEvent2.h
# End Source File
# End Group
# Begin Group "AutoCa"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoButterfly.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoButterfly.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoCrow.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoCrow.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoEagle.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoEagle.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoFirefly.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoFirefly.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoFish.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutoFish.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousAgent.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousAgent.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousBehaviour.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousBehaviour.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\A3DAutonomousGroup.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\AUTOCA\Utilities.h
# End Source File
# End Group
# Begin Group "Face"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CCommon\Face\Expression.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\Expression.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceAnimation.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceAnimation.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceBone.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceBone.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceBoneController.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceBoneController.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\Face\FaceCommon.h
# End Source File
# End Group
# Begin Group "Cloud"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CCommon\EL_Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Cloud.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_CloudManager.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_CloudManager.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_CloudSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_CloudSprite.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\CCommon\EC_BrushMan.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_BrushMan.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_BrushManUtil.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_TriangleMan.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EC_TriangleMan.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Archive.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Archive.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BackMusic.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BackMusic.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BrushBuilding.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BrushBuilding.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Building.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Building.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BuildingWithBrush.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_BuildingWithBrush.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Forest.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Forest.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Grassland.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Grassland.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_GrassType.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_GrassType.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Precinct.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Precinct.h
# End Source File
# Begin Source File

SOURCE=.\EL_RandomMapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EL_RandomMapInfo.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Region.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Region.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Tree.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\EL_Tree.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\elementdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\elementdataman.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\elementpckdir.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELOutlineQuadTree.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELOutlineQuadTree.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELOutlineTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELOutlineTexture.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELTerrainOutline2.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\ELTerrainOutline2.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\ExpTypes.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\generate_item_temp.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\globaldataman.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\globaldataman.h
# End Source File
# Begin Source File

SOURCE=..\CCommon\itemdataman.cpp
# End Source File
# Begin Source File

SOURCE=..\CCommon\itemdataman.h
# End Source File
# End Group
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LuaBind.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaBind.h
# End Source File
# End Group
# End Target
# End Project
