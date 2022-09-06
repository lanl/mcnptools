program main
use iso_c_binding
implicit none

interface
subroutine b() bind(C, name="b")
end subroutine
end interface

call b()
end program main
