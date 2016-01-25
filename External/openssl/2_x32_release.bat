@rem ========= cleanup ==========
@rmdir inc32 /S /Q
@rmdir tmp32 /S /Q
@rmdir out32.vc10 /S /Q

@rem ========= make config ==========
@perl Configure VC-WIN32 --prefix=.
@call ms\do_nasm

pause

@rem ========= compile ========== 
@nmake -f ms\nt-x32-release-vs10.mak

@rem ========= copy pdb ========== 
@copy /B tmp32\app.pdb out32.vc10\app.pdb /B
@copy /B tmp32\lib.pdb out32.vc10\lib.pdb /B
