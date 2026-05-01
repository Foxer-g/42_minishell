import
    libft

{.passL: "-lc".}
proc open(path: cstring, flags: cint): t_ffile {.varargs, importc:"open", header:"<fcntl.h>".}
