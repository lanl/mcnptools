#ifndef MCNPTOOLS_PTRAC_ENUMS_HPP
#define MCNPTOOLS_PTRAC_ENUMS_HPP

namespace mcnptools {

class PtracEnums {
public:
  enum PtracFormat {
    BIN_PTRAC=0,
    ASC_PTRAC=1,
    HDF5_PTRAC=2
  };

  enum PtracEventType {
    SRC = 1000,
    BNK = 2000,
    SUR = 3000,
    COL = 4000,
    TER = 5000,
    LST = 9000
  };

  // extracted from mcnp6/Source/src/eventp.F90
  enum PtracBankType {
    BNK_DXT_TRACK      =  1,
    BNK_ERG_TME_SPLIT  =  2,
    BNK_WWS_SPLIT      =  3,
    BNK_WWC_SPLIT      =  4,
    BNK_UNC_TRACK      =  5,
    BNK_IMP_SPLIT      =  6,
    BNK_N_XN_F         =  7,
    BNK_N_XG           =  8,
    BNK_FLUORESCENCE   =  9,
    BNK_ANNIHILATION   = 10,
    BNK_PHOTO_ELECTRON = 11,
    BNK_COMPT_ELECTRON = 12,
    BNK_PAIR_ELECTRON  = 13,
    BNK_AUGER_ELECTRON = 14,
    BNK_PAIR_POSITRON  = 15,
    BNK_BREMSSTRAHLUNG = 16,
    BNK_KNOCK_ON       = 17,
    BNK_K_X_RAY        = 18,
    BNK_N_XG_MG        = 19,
    BNK_N_XF_MG        = 20,
    BNK_N_XN_MG        = 21,
    BNK_G_XG_MG        = 22,
    BNK_ADJ_SPLIT      = 23,
    BNK_WWT_SPLIT      = 24,
    BNK_PHOTONUCLEAR   = 25,
    BNK_DECAY          = 26,
    BNK_NUCLEAR_INT    = 27,
    BNK_RECOIL         = 28,
    BNK_DXTRAN_ANNIHIL = 29,
    BNK_N_CHARGED_PART = 30,
    BNK_H_CHARGED_PART = 31,
    BNK_N_TO_TABULAR   = 32,
    BNK_MODEL_UPDAT1   = 33,
    BNK_MODEL_UPDATE   = 34,
    BNK_DELAYED_NEUTRON= 35,
    BNK_DELAYED_PHOTON = 36,
    BNK_DELAYED_BETA   = 37,
    BNK_DELAYED_ALPHA  = 38,
    BNK_DELAYED_POSITRN= 39
  };

  enum PtracTerminationType {
    TER_ESCAPE = 1,
    TER_ENERGY_CUTOFF = 2,
    TER_TIME_CUTOFF = 3,
    TER_WEIGHT_WINDOW = 4,
    TER_CELL_IMPORTANCE = 5,
    TER_WEIGHT_CUTOFF = 6,
    TER_ENERGY_IMPORTANCE = 7,
    TER_DXTRAN = 8,
    TER_FORCED_COLLISION = 9,
    TER_EXPONENTIAL_TRANSFORM = 10,
  
    // neutron terminations
    TER_N_DOWNSCATTERING = 11,
    TER_N_CAPTURE = 12,
    TER_N_N_XN  = 13,
    TER_N_FISSION = 14,
    TER_N_NUCLEAR_INTERACTION = 15,
    TER_N_PARTICLE_DECAY = 16,
    TER_N_TABULAR_BOUNDARY =  17,

    // photon terminations
    TER_P_COMPTON_SCATTER = 11,
    TER_P_CAPTURE = 12,
    TER_P_PAIR_PRODUCTION = 13,
    TER_P_PHOTONUCLEAR = 14,

    // electron terminations
    TER_E_SCATTER = 11,
    TER_E_BREMSSTRAHLUNG = 12,
    TER_E_INTERACTION_DECAY = 13,

    // general neutral particle terminations
    TER_GENNEUT_NUCLEAR_INTERACTION = 11,
    TER_GENNEUT_ELASTIC_SCATTER = 12,
    TER_GENNEUT_DECAY = 13,

    // general charged particle terminations
    TER_GENCHAR_MULTIPLE_SCATTER = 11,
    TER_GENCHAR_BREMSSTRAHLUNG = 12,
    TER_GENCHAR_NUCLEAR_INTERACTION = 13,
    TER_GENCHAR_ELASTIC_SCATTER = 14,
    TER_GENCHAR_DECAY = 15,
    TER_GENCHAR_CAPTURE = 16,
    TER_GENCHAR_TABULAR_SAMPLING = 17
  };

  enum PtracDataType {
    NPS = 1,
    FIRST_EVENT_TYPE = 2,
    NPSCELL = 3,
    NPSSURFACE = 4,
    TALLY = 5 ,
    VALUE = 6,
    NEXT_EVENT_TYPE = 7,
    NODE = 8,
    NSR = 9,
    ZAID = 10,
    RXN = 11,
    SURFACE = 12,
    ANGLE = 13,
    TERMINATION_TYPE = 14,
    BRANCH = 15,
    PARTICLE = 16,
    CELL = 17,
    MATERIAL = 18,
    COLLISION_NUMBER = 19,
    X = 20,
    Y = 21,
    Z = 22,
    U = 23,
    V = 24,
    W = 25,
    ENERGY = 26,
    WEIGHT = 27,
    TIME = 28,
    SOURCE_TYPE = 29,
    BANK_TYPE = 30
  };
};

} //namespace mcnptools

#endif
