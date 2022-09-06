MCNPTools Ptrac Example 2
c
100 1 -1.87276e+1 -100      imp:n=1
200 2 -9.98207e-1  100 -200 imp:n=1
300 3 -1.20500e-3  200 -300 imp:n=1
666 0              300      imp:n=0

c
100 so 5
200 so 6
300 so 7

mode n
c
sdef
nonu
c
m1       92234     0.009849 $ Uranium, HEU, U.S. Average
         92235     0.932166 $ Density: 18.724760 g/cc
         92236     0.004484 $ Composition & Density from PNNL-15870, Rev. 1
         92238     0.053501
c
m2        1001     0.666657 $ Water, Liquid @ 23.15 deg-C
          8016     0.333343 $ Density: 0.998207 g/cc
mt2 lwtr.10                 $ Composition & Density from PNNL-15870, Rev. 1
c
m3        6000     0.000150 $ Air (Dry, Near Sea Level)
          7014     0.784431 $ Density: 0.001205 g/cc
          8016     0.210748 $ Composition & Density from PNNL-15870, Rev. 1
         18000     0.004671
c
f4:n 100
e4 0.01 0.1 1 2 3 4 5 6 7 8 9 10 11 12 13 14
c
rand gen=2 seed=12345
prdmp 2j 1 $ Write MCTAL file at conclusion of calculation
ptrac file=bin write=all $ Write PTRAC file
print
nps 1e5
