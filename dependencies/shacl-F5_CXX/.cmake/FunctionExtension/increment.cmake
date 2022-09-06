macro(increment n)
  MATH(EXPR ${n} "${${n}}+1")
endmacro()
