PCSX-Reloaded-plus
-------------------------------

PCSX-Reloaded-plus is a forked version of PCSX-Reloaded , located at
http://pcsxr.codeplex.com/ , and itself a fork of the dead PCSX emulator.

If you are looking for a good PEX emulator, go to http://pcsxr.codeplex.com/ 

I simply forked this, because codeplex did not allow me to create a account
 nor login to post a patch. 

Modifications are 
    Fix path handling logic for -cfg option
    Use gchar for cfgfile_basename , cfgfile
    Use libarchive 2.85 instead of the newer 3.12

I still have local patches that need cleanup before uploading...
so look back in another few weeks.
Modification are mostly around
 gtk window handling of memory editor
 add tagged diffrential search to lookup code 
 client side support for a crude debugger  
 trace of bios calls (not that great as it sounds)

this is a WIP MOD done under free time, so I am just modifying code
that others have written. Credit goes to where its due.
Have a look at http://pcsxr.codeplex.com/team/view for who keeps it alive.

GreenFox

