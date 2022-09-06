template<typename Event>
auto EventToMcnptoolsEnum(const Event &);

inline auto EventToMcnptoolsEnum(const Source &) {
  return PtracEnums::SRC;
}

inline auto EventToMcnptoolsEnum(const Bank &) {
  return PtracEnums::BNK;
}

inline auto EventToMcnptoolsEnum(const SurfaceCrossing &) {
  return PtracEnums::SUR;
}

inline auto EventToMcnptoolsEnum(const Termination &) {
  return PtracEnums::TER;
}

inline auto EventToMcnptoolsEnum(const Collision &) {
  return PtracEnums::COL;
}
