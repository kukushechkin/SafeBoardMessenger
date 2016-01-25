@set flavor=%~1
@set platform=%~2
@set VS=%~3
@set SourceDir=%~4
@set BinariesRoot=%~5

@if /i _%VS%==_VS12.0 set script="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
@if /i _%VS%==_VS10.0 set script="%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"
@if /i _%VS%==_VS9.0 set script="%VS90COMNTOOLS%..\..\VC\vcvarsall.bat"
@if /i _%VS%==_VS8.0 set script="%VS80COMNTOOLS%..\..\VC\vcvarsall.bat"

@if /i _%flavor%==_release @(
	set tmp=tmp32
	set out=out32

	if /i _%platform%==_x86 (
		set flav=VC-WIN32
		set binout=Win32
		set bat=do_ms
	)
	if /i _%platform%==_amd64 (
		set flav=VC-WIN64A
		set binout=x64
		set bat=do_win64a
	)
)

@if /i _%flavor%==_debug @(
	set tmp=tmp32.dbg
	set out=out32.dbg

	if /i _%platform%==_x86 (
		set flav=debug-VC-WIN32
		set binout=Win32
		set bat=do_ms
	)
	if /i _%platform%==_amd64 (
		set flav=debug-VC-WIN64A
		set binout=x64
		set bat=do_win64a
	)
)


@call %script% %platform%
@set prefix=%BinariesRoot%\%VS%\%binout%\%flavor%

@pushd %SourceDir%
@attrib -R /S
@perl configure %flav% --prefix="%prefix%" no-shared
@call ms\%bat%
@nmake -f ms\nt.mak
@nmake -f ms\nt.mak install
@xcopy /y %SourceDir%\%tmp%\app.pdb %prefix%\pdb\
@xcopy /y %SourceDir%\%tmp%\lib.pdb %prefix%\pdb\
@xcopy /y %SourceDir%\%out%\openssl.pdb %prefix%\pdb\
@nmake -f ms\nt.mak clean
@del /F ms\nt.mak
@del /F ms\ntdll.mak
@rmdir /S /Q %out%

@popd