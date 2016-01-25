@rem ========= cleanup ==========
@rmdir inc32 /S /Q
@rmdir tmp32.dbg /S /Q
@rmdir out32.dbg.vc10 /S /Q

@rem ========= make config ==========
perl Configure debug-VC-WIN32 --prefix=.
@call ms\do_nasm

pause

@rem ========= compile ==========
@nmake -f ms\nt-x32-debug-vs10.mak

@rem ========= copy pdb ==========
@copy /B tmp32.dbg\app.pdb out32.dbg.vc10\app.pdb /B
@copy /B tmp32.dbg\lib.pdb out32.dbg.vc10\lib.pdb /B
