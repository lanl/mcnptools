#include <iostream>
#include <sstream>
#include <memory>
#include <cmath>
#include <limits>

#include "mcnptools/Constants.hpp"
#include "mcnptools/McnpTools.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

class MeshtalTallyXMLVTKConverter{
public:

  MeshtalTallyXMLVTKConverter( mcnptools::MeshtalTally& tally ) {

    const std::vector<double> ibounds = tally.GetXRBounds();
    const std::vector<double> jbounds = tally.GetYZBounds();
    const std::vector<double> kbounds = tally.GetZTBounds();
    const std::vector<double> ebounds = tally.GetEBounds();
    const std::vector<double> tbounds = tally.GetTBounds();

    double tmp = 0.0; // temporary scalar
    double largest_value = -1.0; // largest tally value for the tally
    double smallest_value = 1e99; // smallest tally value for the tally

    // Determine whether to write times/energies.
    bool write_times = true;
    size_t tstart = 0;
    size_t tlast = tbounds.size();
    if( tbounds.size() == 2 ) {
      if( tbounds.at(0) == -1e36 && tbounds.at(1) == 1e36 ) {
        write_times = false;
        tlast = 1;
      }
    }
    bool write_energies = true;
    size_t estart = 0;
    size_t elast = ebounds.size();
    if( ebounds.size() == 2 ) {
      if( ebounds.at(1) == 1e36 ) {
        write_energies = false;
        elast = 1;
      }
    }

    // Prepare VTK header independent of geometry.
    vtk_header << "<?xml version=\"1.0\"?>" << std::endl;
    vtk_header << "<VTKFile type=\"StructuredGrid\" version=\"0.1\">" << std::endl;
    vtk_header << "  <StructuredGrid WholeExtent=\""
               << "0 "
               << ibounds.size() - 1 << " "
               << "0 "
               << jbounds.size() - 1 << " "
               << "0 "
               << kbounds.size() - 1 << "\">" << std::endl;
    vtk_header << "    <Piece Extent=\""
               << "0 "
               << ibounds.size() - 1 << " "
               << "0 "
               << jbounds.size() - 1 << " "
               << "0 "
               << kbounds.size() - 1 << "\">" << std::endl;

    // Construct points array to track all vertices in the mesh.  This is more
    // verbose than specifying RectangularGrid cardinal positions; however, it
    // allows structured grids to be used for either Cartesian or cylindrical
    // (or, in the future, other structured) mesh tally geometries.
    vtk_points << "      <Points>" << std::endl;
    vtk_points << "        <DataArray type=\"Float32\" format=\"ascii\" NumberOfComponents=\"3\">" << std::endl;
    double theta = 0.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    switch(tally.GetType()) {
      case(mcnptools::MeshtalTally::XYZ):
        for(size_t k=0; k < kbounds.size(); ++k) {
          for(size_t j=0; j < jbounds.size(); ++j) {
            for(size_t i=0; i < ibounds.size(); ++i) {
              x = ibounds.at(i);
              y = jbounds.at(j);
              z = kbounds.at(k);
              vtk_points << "         "
                         << std::setw(13) << std::scientific << std::setprecision(5) << x
                         << std::setw(13) << std::scientific << std::setprecision(5) << y
                         << std::setw(13) << std::scientific << std::setprecision(5) << z << std::endl;
            }
          }
        }
        break;
      case(mcnptools::MeshtalTally::RZT):
        for(size_t k=0; k < kbounds.size(); ++k) {
          for(size_t j=0; j < jbounds.size(); ++j) {
            for(size_t i=0; i < ibounds.size(); ++i) {
              theta = kbounds.at(k) * 2.0 * mcnptools::pi;
              x = ibounds.at(i) * cos(theta);
              y = ibounds.at(i) * sin(theta);
              z = jbounds.at(j);
              vtk_points << "         "
                         << std::setw(13) << std::scientific << std::setprecision(5) << x
                         << std::setw(13) << std::scientific << std::setprecision(5) << y
                         << std::setw(13) << std::scientific << std::setprecision(5) << z << std::endl;
            }
          }
        }
        break;
      default:
        throw "Mesh tallies must be XYZ (Cartesian) or RZT (cylindrical).";
        break;
    }
    vtk_points << "        </DataArray>" << std::endl;
    vtk_points << "      </Points>" << std::endl;

    vtk_data << "      <CellData Scalars=\"cell_scalars\">" << std::endl;

    // Capture values for all time/energy binnings.
    for(size_t e=estart; e<elast; e++) {
      for(size_t t=tstart; t<tlast; t++) {
        std::stringstream FieldName;
        FieldName << "Tally_Value";
        if( write_energies ) {
          if( e < elast - 1) {
            FieldName << "_";
            FieldName << std::scientific << std::setprecision(5) << ebounds.at(e);
            FieldName << "_lt_" << "E[" << e << "]" << "_lt_";
            FieldName << std::scientific << std::setprecision(5) << ebounds.at(e+1);
          } else {
            FieldName << "_E[total]";
          }
        }
        if( write_times ) {
          if( t < tlast - 1 ) {
            FieldName << "_";
            FieldName << std::scientific << std::setprecision(5) << tbounds.at(t);
            FieldName << "_lt_" << "T[" << t << "]" << "_lt_";
            FieldName << std::scientific << std::setprecision(5) << tbounds.at(t+1);
          } else {
            FieldName << "_T[total]";
          }
        }
        vtk_data << "        <DataArray type=\"Float32\" Name=\"" << FieldName.str() << "\" format=\"ascii\">" << std::endl;
        vtk_data << "          ";
        for(size_t k=0; k<kbounds.size()-1; k++) {
          for(size_t j=0; j<jbounds.size()-1; j++) {
            for(size_t i=0; i<ibounds.size()-1; i++) {
              tmp = tally.GetValue(i, j, k, e, t);
              vtk_data << std::setw(12) << std::scientific << std::setprecision(5) << tmp;
              if( tmp > largest_value ) { largest_value = tmp; }
              if( tmp < smallest_value && tmp > 0.0 ) { smallest_value = tmp; }
            }
            if( j != jbounds.size()-2 ) { vtk_data << std::endl << "          ";}
          }
          if( k != kbounds.size()-2 ) { vtk_data << std::endl << std::endl << "          ";}
        }
        vtk_data << std::endl;
        vtk_data << "        </DataArray>" << std::endl;
        if( t != tlast-1 ) { vtk_data << std::endl; }
      }
      if( e != elast-1 ) { vtk_data << std::endl; }
    }

    // Capture errors for all time/energy binnings.
    for(size_t e=estart; e<elast; e++) {
      for(size_t t=tstart; t<tlast; t++) {
        std::stringstream FieldName;
        FieldName << "Tally_Error";
        if( write_energies ) {
          if( e < elast - 1) {
            FieldName << "_";
            FieldName << std::scientific << std::setprecision(5) << ebounds.at(e);
            FieldName << "_lt_" << "E[" << e << "]" << "_lt_";
            FieldName << std::scientific << std::setprecision(5) << ebounds.at(e+1);
          } else {
            FieldName << "_E[total]";
          }
        }
        if( write_times ) {
          if( t < tlast - 1 ) {
            FieldName << "_";
            FieldName << std::scientific << std::setprecision(5) << tbounds.at(t);
            FieldName << "_lt_" << "T[" << t << "]" << "_lt_";
            FieldName << std::scientific << std::setprecision(5) << tbounds.at(t+1);
          } else {
            FieldName << "_T[total]";
          }
        }
        vtk_data << "        <DataArray type=\"Float32\" Name=\"" << FieldName.str() << "\" format=\"ascii\">" << std::endl;
        vtk_data << "          ";
        for(size_t k=0; k<kbounds.size()-1; k++) {
          for(size_t j=0; j<jbounds.size()-1; j++) {
            for(size_t i=0; i<ibounds.size()-1; i++) {
              vtk_data << std::setw(12) << std::scientific << std::setprecision(5) << tally.GetError(i, j, k, e, t);
            }
            if( j != jbounds.size()-2 ) { vtk_data << std::endl << "          ";}
          }
          if( k != kbounds.size()-2 ) { vtk_data << std::endl << std::endl << "          ";}
        }
        vtk_data << std::endl;
        vtk_data << "        </DataArray>" << std::endl;
        if( t != tlast-1 ) { vtk_data << std::endl; }
      }
      if( e != elast-1 ) { vtk_data << std::endl; }
    }

    vtk_data << "      </CellData>" << std::endl;

    vtk_footer << "    </Piece>" << std::endl;
    vtk_footer << "  </StructuredGrid>" << std::endl;
    vtk_footer << "</VTKFile>" << std::endl;
    std::cout << "  Smallest non-zero tally value: " << std::setw(12) << std::scientific << std::setprecision(5) << smallest_value << std::endl;
    std::cout << "            Largest tally value: " << std::setw(12) << std::scientific << std::setprecision(5) << largest_value << std::endl;
    return;
  };

  // Write parsed and formatted mesh tally information to file.
  void WriteVTK(std::string filename) {
    std::ofstream of;
    of.open(filename);
    of << vtk_header.str();
    of << vtk_points.str();
    of << vtk_data.str();
    of << vtk_footer.str();
    of.close();
    return;
  }

private:
  std::string filename;
  std::stringstream vtk_header;
  std::stringstream vtk_points;
  std::stringstream vtk_data;
  std::stringstream vtk_footer;
};

int main(int argc, char* argv[]) {

  cyclops::CLParser<> parser("meshtal2vtk");
  parser.MakeHelp();
  parser.Description(
    "$0 converts mesh tallies from an MCNP MESHTAL file into XML-formatted "
    "StructuredGrid (.vts) VTK files.  This utility only works for XYZ (Cartesian) "
    "and RZT (cylindrical) geometries.");
  parser.Epilog("AUTHOR: Joel A. Kulesza [jkulesza@lanl.gov]");

  cyclops::CLMessageOp version("--version,-v", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
  version.Help("Print version and exit");
  parser.AddOption(version);

  cyclops::CLStringPosValueOp meshtalfile("MESHTAL");
  meshtalfile.Required(true);
  meshtalfile.Help("MESHTAL file containing one or more mesh tallies");
  parser.AddOption(meshtalfile);

  cyclops::CLIntPosMultiOp tallies("TALLY",0);
  tallies.Help("Tally number for which to produce the VTK files");
  parser.AddOption(tallies);

  parser.ParseOptions(argc,argv);

  mcnptools::Meshtal meshtal(meshtalfile.Value());
  std::vector<int> tlist;
  if( tallies.Count() == 0 ) {
    tlist = meshtal.GetTallyList();
  } else {
    for(size_t i=0; i<tallies.Count(); i++) {
      int tnum = tallies.Value(i);
      tlist.push_back(tnum);
    }
  }

  for(auto it=tlist.begin(); it!=tlist.end(); it++) {
    std::cout << "Processing mesh tally: " << std::to_string(*it);
    mcnptools::MeshtalTally tally = meshtal.GetTally(*it);

    std::stringstream oss;
    oss << meshtalfile.Value() << "_" << std::to_string(*it) << ".vts";
    std::cout << " -> " << oss.str() << std::endl;
    MeshtalTallyXMLVTKConverter vtkwriter(tally);
    vtkwriter.WriteVTK(oss.str());
  }

  return 0;
}

