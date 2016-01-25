@rem ========= cleanup ==========
@rmdir inc64 /S /Q
@rmdir tmp64.dbg /S /Q
@rmdir out64.dbg.vc10 /S /Q

@rem ========= make config ==========
@perl Configure debug-VC-WIN64A --prefix=.
@call ms\do_win64a

pause

@rem ========= compile ========== 
@nmake -f ms\nt-x64-debug-vs10.mak

@rem ========= copy pdb ========== 
@copy /B tmp64.dbg\app.pdb out64.dbg.vc10\app.pdb /B
@copy /B tmp64.dbg\lib.pdb out64.dbg.vc10\lib.pdb /B
