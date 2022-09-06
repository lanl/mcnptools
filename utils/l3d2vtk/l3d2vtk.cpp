#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "mcnptools/Constants.hpp"
#include "mcnptools/Lnk3Dnt.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp"  // cyclops command line parser

std::string PrintGeomHumanReadable( const int& geom ) {
  std::string geom_str = "";

  switch ( geom ) {
    case mcnptools::Lnk3Dnt::SLAB1D:
      geom_str = "1-D Cartesian (x)";
      break;
    case mcnptools::Lnk3Dnt::CYL1D:
      geom_str = "1-D Cylindrical (r)";
      break;
    case mcnptools::Lnk3Dnt::SPH1D:
      geom_str = "1-D Spherical (r)";
      break;
    case mcnptools::Lnk3Dnt::XY:
      geom_str = "2-D Cartesian (x,y)";
      break;
    case mcnptools::Lnk3Dnt::RZ:
      geom_str = "2-D Cylindrical (r,z)";
      break;
    case mcnptools::Lnk3Dnt::RT:
      geom_str = "2-D Cylindrical (r,θ)";
      break;
    case mcnptools::Lnk3Dnt::XYZ:
      geom_str = "3-D Cartesian (x,y,z)";
      break;
    case mcnptools::Lnk3Dnt::RZT:
      geom_str = "3-D Cylindrical (r,z,θ)";
      break;
    default:
      geom_str = "Unknown";
      break;
  }

  return geom_str;
}

class Lnk3DntXMLVTKConverter {
 public:
  Lnk3DntXMLVTKConverter( mcnptools::Lnk3Dnt& l3d ) : geom( l3d.GetGeom( ) ) {
    ibounds = l3d.GetIMesh( );
    jbounds = l3d.GetJMesh( );
    kbounds = l3d.GetKMesh( );

    // Set geometry index boundaries based on geometry type.
    switch ( geom ) {
      case mcnptools::Lnk3Dnt::SLAB1D:
      case mcnptools::Lnk3Dnt::CYL1D:
      case mcnptools::Lnk3Dnt::SPH1D:
        jbounds = {-0.5, 0.5};
        kbounds = {-0.5, 0.5};
        break;
      case mcnptools::Lnk3Dnt::XY:
        kbounds = {-0.5, 0.5};
        break;
      case mcnptools::Lnk3Dnt::RZ:
        kbounds = {0.0, 0.002777777778};  // 0-1 degree.
        break;
      case mcnptools::Lnk3Dnt::RT:
        kbounds = jbounds;
        jbounds = {-0.5, 0.5};
        break;
      case mcnptools::Lnk3Dnt::XYZ:
        // Fully defined; no fixup needed.
        break;
      case mcnptools::Lnk3Dnt::RZT:
        // Fully defined; no fixup needed.
        break;
    }

    imax = ibounds.size( ) - 1;
    jmax = jbounds.size( ) - 1;
    kmax = kbounds.size( ) - 1;
    /* std::cout << imax << ", " << jmax << ", " << kmax << "\n"; */

    // Prepare VTK header independent of geometry.
    vtk_header << "<?xml version=\"1.0\"?>" << std::endl;
    vtk_header << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">"
               << std::endl;
    vtk_header << "  <StructuredGrid WholeExtent=\""
               << "0 " << imax << " "
               << "0 " << jmax << " "
               << "0 " << kmax << "\">" << std::endl;
    vtk_header << "    <Piece Extent=\""
               << "0 " << imax << " "
               << "0 " << jmax << " "
               << "0 " << kmax << "\">" << std::endl;

    // Construct points array to track all vertices in the mesh.  Currently
    // there is no curvature applied to non-Cartesian mesh.  That is, all curved
    // edges/faces are linear.
    vtk_points << "      <Points>" << std::endl;
    vtk_points
        << "        <DataArray type=\"Float32\" format=\"ascii\" NumberOfComponents=\"3\">"
        << std::endl;
    switch ( l3d.GetGeom( ) ) {
      case mcnptools::Lnk3Dnt::SLAB1D:
      case mcnptools::Lnk3Dnt::CYL1D:
      case mcnptools::Lnk3Dnt::SPH1D:
      case mcnptools::Lnk3Dnt::XY:
      case mcnptools::Lnk3Dnt::XYZ:
        ProcessCartesianPoints( );
        break;
      case mcnptools::Lnk3Dnt::RZ:
      case mcnptools::Lnk3Dnt::RT:
      case mcnptools::Lnk3Dnt::RZT:
        ProcessCylindricalPoints( );
        break;
      default:
        break;
    }
    vtk_points << "        </DataArray>" << std::endl;
    vtk_points << "      </Points>" << std::endl;

    // Process materials and densities into VTK data arrays.
    vtk_data << "      <CellData Scalars=\"cell_scalars\">" << std::endl;
    processDominantMaterialAndDensity( l3d );
    vtk_data << vtk_data_material_dominant.str( );
    vtk_data_material_dominant.str( std::string( ) );  // Clear contents.
    vtk_data << vtk_data_density_dominant.str( );
    vtk_data_density_dominant.str( std::string( ) );  // Clear contents.
    vtk_data << "      </CellData>" << std::endl;

    vtk_footer << "    </Piece>" << std::endl;
    vtk_footer << "  </StructuredGrid>" << std::endl;
    vtk_footer << "</VTKFile>" << std::endl;

    return;
  };

  // Write parsed and formatted mesh tally information to file.
  void WriteVTK( std::string filename ) {
    std::ofstream of;
    of.open( filename );
    of << vtk_header.str( );
    of << vtk_points.str( );
    of << vtk_data.str( );
    of << vtk_footer.str( );
    of.close( );
    return;
  }

 private:
  std::string filename;
  std::stringstream vtk_header;
  std::stringstream vtk_points;
  std::stringstream vtk_data_material_dominant;
  std::stringstream vtk_data_density_dominant;
  std::stringstream vtk_data;
  std::stringstream vtk_footer;

  const int geom;
  std::vector< double > ibounds;
  std::vector< double > jbounds;
  std::vector< double > kbounds;
  unsigned int imax = 1;
  unsigned int jmax = 1;
  unsigned int kmax = 1;

  void ProcessCartesianPoints( ) {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    for ( size_t k = 0; k < kmax + 1; ++k ) {
      for ( size_t j = 0; j < jmax + 1; ++j ) {
        for ( size_t i = 0; i < imax + 1; ++i ) {
          if ( geom == mcnptools::Lnk3Dnt::RZ ) {
            x = ibounds.at( i );
            y = jbounds.at( k );
            z = kbounds.at( j );
          } else {
            x = ibounds.at( i );
            y = jbounds.at( j );
            z = kbounds.at( k );
          }

          vtk_points << "         " << std::setw( 13 ) << std::scientific
                     << std::setprecision( 5 ) << x << std::setw( 13 )
                     << std::scientific << std::setprecision( 5 ) << y
                     << std::setw( 13 ) << std::scientific
                     << std::setprecision( 5 ) << z << std::endl;
        }
      }
    }
    return;
  }

  void ProcessCylindricalPoints( ) {
    double theta = 0.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    for ( size_t k = 0; k < kmax + 1; ++k ) {
      for ( size_t j = 0; j < jmax + 1; ++j ) {
        for ( size_t i = 0; i < imax + 1; ++i ) {
          theta = kbounds.at( k ) * 2.0 * mcnptools::pi;
          x = ibounds.at( i ) * cos( theta );
          y = ibounds.at( i ) * sin( theta );
          z = jbounds.at( j );
          vtk_points << "         " << std::setw( 13 ) << std::scientific
                     << std::setprecision( 5 ) << x << std::setw( 13 )
                     << std::scientific << std::setprecision( 5 ) << y
                     << std::setw( 13 ) << std::scientific
                     << std::setprecision( 5 ) << z << std::endl;
        }
      }
    }
    return;
  }

  // Find dominant material (the material number with the highest density) and
  // corresponding density in zone i, j, k and push it back onto the appropriate
  // data array.
  void pushDominantMaterialAndDensity( mcnptools::Lnk3Dnt& l3d, const int& i,
                                       const int& j, const int& k ) {
    const std::vector< double > densities( l3d.GetDensities( i, j, k ) );
    const std::vector< int > materials( l3d.GetMaterials( i, j, k ) );
    std::vector< std::pair< double, int > > paired_materials;
    /* std::cout << i << ", " << j << ", " << k << std::endl; */
    for ( size_t n = 0; n < materials.size( ); ++n ) {
      paired_materials.push_back(
          std::make_pair( densities[ n ], materials[ n ] ) );
    }
    if( paired_materials.size() > 0 ) {
      std::sort( paired_materials.begin( ), paired_materials.end( ) );
      vtk_data_material_dominant << std::setw( 12 ) << std::scientific
                                 << std::setprecision( 5 )
                                 << paired_materials.back( ).second;
      vtk_data_density_dominant << std::setw( 12 ) << std::scientific
                                << std::setprecision( 5 )
                                << paired_materials.back( ).first;
    } else { // Material 0 or 1 found; so no materials returned from MCNPTools.
      vtk_data_material_dominant << std::setw( 12 ) << std::scientific
                                 << std::setprecision( 5 )
                                 << 0;
      vtk_data_density_dominant << std::setw( 12 ) << std::scientific
                                << std::setprecision( 5 )
                                << 0;
    }
    return;
  }

  // Get materials and densities, pair, and sort them to obtain the dominant
  // material.
  void processDominantMaterialAndDensity( mcnptools::Lnk3Dnt& l3d ) {
    vtk_data_material_dominant
        << "        <DataArray type=\"Int32\" Name=\"material_dominant\" format=\"ascii\">"
        << std::endl;
    vtk_data_material_dominant << "          ";
    vtk_data_density_dominant
        << "        <DataArray type=\"Float32\" Name=\"density_dominant\" format=\"ascii\">"
        << std::endl;
    vtk_data_density_dominant << "          ";

    // A conditional is used here because r,θ geometries use i,j,k, as r,θ,z
    // rather than r,z,θ as in the 3-D case.  This makes r,θ unique in that it
    // is a 2-D geometry that does not exist as an organizational subset of the
    // comparable 3-D geometry.
    if ( geom == mcnptools::Lnk3Dnt::RT ) {
      for ( size_t j = 0; j < kmax; ++j ) {
        for ( size_t i = 0; i < imax; ++i ) {
          pushDominantMaterialAndDensity( l3d, i, j, 0 );
        }
        if ( j != jbounds.size( ) - 2 ) {
          vtk_data_material_dominant << std::endl << "          ";
          vtk_data_density_dominant << std::endl << "          ";
        }
      }
    } else {
      for ( size_t k = 0; k < kmax; ++k ) {
        for ( size_t j = 0; j < jmax; ++j ) {
          for ( size_t i = 0; i < imax; ++i ) {
            /* std::cout << i << ", " << j << ", " << k << std::endl; */
            pushDominantMaterialAndDensity( l3d, i, j, k );
          }
          if ( j != jbounds.size( ) - 2 ) {
            vtk_data_material_dominant << std::endl << "          ";
            vtk_data_density_dominant << std::endl << "          ";
          }
        }
        if ( k != kbounds.size( ) - 2 ) {
          vtk_data_material_dominant << std::endl << std::endl << "          ";
          vtk_data_density_dominant << std::endl << std::endl << "          ";
        }
      }
    }
    vtk_data_material_dominant << std::endl
                               << "        </DataArray>" << std::endl;
    vtk_data_density_dominant << std::endl
                              << "        </DataArray>" << std::endl;
    return;
  }
};

int main( int argc, char* argv[] ) {
  cyclops::CLParser<> parser( "l3d2vtk" );
  parser.MakeHelp( );
  parser.Description(
      "$0 converts a LNK3DNT file into an XML-formatted StructuredGrid (.vts) VTK "
      "file." );
  parser.Epilog( "AUTHOR: Joel A. Kulesza [jkulesza@lanl.gov]" );

  cyclops::CLMessageOp version(
      "--version,-v",
      std::string( "Version " ) + std::string( MCNPTOOLS_VERSION ) );
  version.Help( "Print version and exit" );
  parser.AddOption( version );

  cyclops::CLStringPosValueOp l3dname( "LNK3DNT" );
  l3dname.Required( true );
  l3dname.Help( "LNK3DNT file name to convert" );
  parser.AddOption( l3dname );

  cyclops::CLTrueSwitchOp verbose( "--verbose,-V" );
  verbose.Help( "Produce standard output giving status (Default: False)" );
  parser.AddOption( verbose );

  cyclops::CLStringPosValueOp l3dout( "OUTPUT", "lnk3dnt.vts" );
  l3dout.Help( "Converted LNK3DNT output name (Default: lnk3dnt.vts)" );
  parser.AddOption( l3dout );

  parser.ParseOptions( argc, argv );

  mcnptools::Lnk3Dnt l3d( l3dname.Value( ) );

  if ( verbose.Value( ) ) {
    std::cout << "Processing file: " << l3dname.Value( ) << std::endl;
    std::cout << "  Found " << PrintGeomHumanReadable( l3d.GetGeom( ) )
              << " geometry." << std::endl;
  }

  if ( verbose.Value( ) ) {
    std::cout << "  Reading materials..." << std::endl;
  }
  l3d.ReadMaterials( );

  if ( verbose.Value( ) ) {
    std::cout << "  Constructing VTS file..." << std::endl;
  }
  Lnk3DntXMLVTKConverter vtkwriter( l3d );

  if ( verbose.Value( ) ) {
    std::cout << "  Writing VTS file..." << std::endl;
  }
  vtkwriter.WriteVTK( l3dout.Value( ) );

  if ( verbose.Value( ) ) {
    std::cout << "  Done." << std::endl;
  }
  return 0;
}
