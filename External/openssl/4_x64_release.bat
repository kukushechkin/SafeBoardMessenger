@rem ========= cleanup ==========
@rmdir inc64 /S /Q
@rmdir tmp64 /S /Q
@rmdir out64.vc10 /S /Q

@rem ========= make config ==========
@perl Configure VC-WIN64A --prefix=.
@call ms\do_win64a

pause

@rem ========= compile ========== 
@nmake -f ms\nt-x64-release-vs10.mak

@rem ========= copy pdb ========== 
@copy /B tmp64\app.pdb out64.vc10\app.pdb /B
@copy /B tmp64\lib.pdb out64.vc10\lib.pdb /B
