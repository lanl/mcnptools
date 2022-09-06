testprob01 -- simple neutron problem
1     1  -2.25    -1  imp:n=1   $ graphite ball
2     2  -8.95  1 -2  imp:n=1   $ copper shell
3     0         2 -3  imp:n=1   $ void space
4     0         3     imp:n=0   $ zero-importance outside world
 
1     sph 0 0 0 5
2     s 0 0 0 7
3     so  9
 
c    biased isotropic point source at (0,0,0)
sdef  erg d1   vec 0 1 0   dir d2   tme d5
sc5  equiprobable bin treatment for time distribution.
si5  -50 -10 3i 10 5m
sp5  -41 10
sb5  0 .1 .2 .3 1r .2 .1
sc1  flat energy spectrum from 1 to 14.1 mev.
si1   1.0 14.1
sp1   0 1
sc2  direction is biased toward the point detector.
sb2   -31 1.5
c
m2    29000 1 
m1    6012 1 92235 0.02
c    xs3  6012.50c 11.8969 rmccs 0 3 92853 23390 0 0 2.53e-08
phys:n  14.2 .01
c
fc1  current across the graphite-copper interface.
f1:n  1.1
e1    .1 .5 .8 2 4 12 14.1
c1    -.866 -.5 0 .5 .866 1
em1  1 5i 7
cm1  8 4i 13
tm5  14 8i 23
t5   -50 -10 3i 10 5m 4m 5m 1.e20
fq1   c e
fc5  flux at a point in the void just outside the copper shell.
f5:n  0 8 0 0
f15:n 0 8 0 0
f25:n 0 8 0 0
e5    .1 .5 .8 2 4 12 14.1
c
cut:n 1.e20 .05 .05 .01 .01
nps 10
prdmp 2000
print 10 30 35 
c    For backward compatability, set the first calculation of the
c  point detector average contribution at nps 1400
c dbcn j 1000 512 513 2j 100 9999 5 57j 200$ non-multitask version.
dbcn 6j 100 9999 5 $ dbcn card for multitask version.
ptrac file=hdf5 nps=1,3 cell=1 surface=1 meph=15 write=all 
