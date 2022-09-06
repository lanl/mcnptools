#!/usr/bin/env python3

import mcnptools
import numpy as np

nmaxxs = 3

nimesh = 5
njmesh = 6
nkmesh = 7

def make_x(  nimesh = 5, njmesh = 1, nkmesh = 1, outfilename = 'x.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.SLAB1D, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,  5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0,  1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    mesh1.SetMaterials( [    2,    4,    3 ], 0, 0, 0)
    mesh1.SetDensities( [  9.0,  8.0,  7.0 ], 0, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 1, 0, 0)
    mesh1.SetDensities( [  8.0,  9.0, 10.0 ], 1, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 2, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 2, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 3, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 3, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 4, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 4, 0, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_r_cyl(  nimesh = 1, njmesh = 1, nkmesh = 1, outfilename = 'r_cyl.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.CYL1D, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,  5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0,  1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    mesh1.SetMaterials( [    2,    4,    3 ], 0, 0, 0)
    mesh1.SetDensities( [  9.0,  8.0,  7.0 ], 0, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 1, 0, 0)
    mesh1.SetDensities( [  8.0,  9.0, 10.0 ], 1, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 2, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 2, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 3, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 3, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 4, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 4, 0, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_r_sph(  nimesh = 1, njmesh = 1, nkmesh = 1, outfilename = 'r_sph.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.SPH1D, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,  5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0,  1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    mesh1.SetMaterials( [    2,    4,    3 ], 0, 0, 0)
    mesh1.SetDensities( [  9.0,  8.0,  7.0 ], 0, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 1, 0, 0)
    mesh1.SetDensities( [  8.0,  9.0, 10.0 ], 1, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 2, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 2, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 3, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 3, 0, 0)
    mesh1.SetMaterials( [    2,    4,    3 ], 4, 0, 0)
    mesh1.SetDensities( [  9.0, 11.0, 10.0 ], 4, 0, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_xy( nimesh = 3, njmesh = 4, nkmesh = 1, outfilename = 'xy.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.XY, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,  5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0,  1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    for i in range( nimesh ):
        for j in range( int( njmesh / 3 ) ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0,  8.0,  7.0 ], i, j, 0)
        for j in range( int( njmesh / 3 ), int( 2 * njmesh / 3 ) ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  8.0,  9.0, 10.0 ], i, j, 0)
        for j in range( int( 2 * njmesh / 3 ), njmesh ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0, 11.0, 10.0 ], i, j, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_rz( nimesh, njmesh, nkmesh = 1, outfilename = 'rz.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.RZ, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,  5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0,  1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    for i in range( nimesh ):
        for j in range( int( njmesh / 3 ) ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0,  8.0,  7.0 ], i, j, 0)
        for j in range( int( njmesh / 3 ), int( 2 * njmesh / 3 ) ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  8.0,  9.0, 10.0 ], i, j, 0)
        for j in range( int( 2 * njmesh / 3 ), njmesh ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0, 11.0, 10.0 ], i, j, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_rt( nimesh, njmesh, nkmesh = 1, outfilename = 'rt.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.RT, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0,  10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(   0, 0.5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(  -1,   1, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    for j in range( njmesh ):
        for i in range( 1 ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0,  8.0,  7.0 ], i, j, 0)
        for i in range( 1, 2 ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  8.0,  9.0, 10.0 ], i, j, 0)
        for i in range( 2, nimesh ):
            mesh1.SetMaterials( [    2,    4,    3 ], i, j, 0)
            mesh1.SetDensities( [  9.0, 11.0, 10.0 ], i, j, 0)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_rzt( nimesh, njmesh, nkmesh, outfilename = 'rzt.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.RZT, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0,  10, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(  -5,   5, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0, 0.5, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    for k in range( nkmesh ):
        for j in range( njmesh ):
            for i in range( 1 ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0,  8.0,  7.0 ], i, j, k)
            for i in range( 1, 2 ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0,  8.0, 10.0 ], i, j, k)
            for i in range( 2, nimesh ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0, 11.0, 10.0 ], i, j, k)

    for k in range( nkmesh ):
            mesh1.SetMaterials( [    2,    4,    3 ], nimesh-1, njmesh-1, k)
            mesh1.SetDensities( [  9.0,  8.0,  7.0 ], nimesh-1, njmesh-1, k)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

def make_xyz( nimesh, njmesh, nkmesh, outfilename = 'xyz.l3d' ):
    mesh1 = mcnptools.Lnk3Dnt(  mcnptools.Lnk3Dnt.XYZ, # geometry
                                nmaxxs, # max number of materials
                                nimesh, # cells in i direction
                                njmesh, # cells in j direction
                                nkmesh  # cells in k direction
                             )

    # Set mesh boundaries in each direction.
    imesh = list( np.linspace(   0, 3, num = nimesh+1, endpoint = True ) )
    jmesh = list( np.linspace(   0, 4, num = njmesh+1, endpoint = True ) )
    kmesh = list( np.linspace(   0, 5, num = nkmesh+1, endpoint = True ) )
    mesh1.SetIMesh( imesh )
    mesh1.SetJMesh( jmesh )
    mesh1.SetKMesh( kmesh )

    # Set materials for each zone.
    # Note "1" is a reserved material integer = void.
    for k in range( nkmesh ):
        for j in range( njmesh ):
            for i in range( 1 ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0,  8.0,  7.0 ], i, j, k)
            for i in range( 1, 2 ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0,  8.0, 10.0 ], i, j, k)
            for i in range( 2, nimesh ):
                mesh1.SetMaterials( [    2,    4,    3 ], i, j, k)
                mesh1.SetDensities( [  9.0, 11.0, 10.0 ], i, j, k)

    for j in range( njmesh ):
            mesh1.SetMaterials( [    2,    4,    3 ], nimesh-1, j, nkmesh-1)
            mesh1.SetDensities( [  9.0,  8.0,  7.0 ], nimesh-1, j, nkmesh-1)

    mesh1.Save( outfilename )

    return imesh, jmesh, kmesh

make_x( nimesh )
make_r_cyl( nimesh )
make_r_sph( nimesh )
make_xy( nimesh, njmesh )
make_rz( nimesh, njmesh )
make_rt( nimesh, njmesh )
make_rzt( nimesh, njmesh, nkmesh )
make_xyz( nimesh, njmesh, nkmesh )
