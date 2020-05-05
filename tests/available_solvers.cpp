#include "available_solvers.h"
#include "exceptions.h"

#if BUILD_BTOR
#include "boolector_factory.h"
#endif

#if BUILD_CVC4
#include "cvc4_factory.h"
#endif

#if BUILD_MSAT
#include "msat_factory.h"
#endif

#if BUILD_YICES2
#include "yices2_factory.h"
#endif

using namespace smt;

namespace smt_tests {

const std::vector<SolverEnum> solver_enums({
#if BUILD_BTOR
  BTOR,
#endif

#if BUILD_CVC4
      CVC4,
#endif

#if BUILD_MSAT
      MSAT,
#endif

#if BUILD_YICES2
      YICES2,
#endif
});

const CreateSolverFunsMap solvers({
#if BUILD_BTOR
  { BTOR, BoolectorSolverFactory::create },
#endif

#if BUILD_CVC4
      { CVC4, CVC4SolverFactory::create },
#endif

#if BUILD_MSAT
      { MSAT, MsatSolverFactory::create },
#endif

#if BUILD_YICES2
      { YICES2, Yices2SolverFactory::create },
#endif
});

const std::vector<SolverEnum> itp_enums({
#if BUILD_MSAT
                                         MSAT
#endif
  });

const CreateSolverFunsMap itps({
#if BUILD_MSAT
                                { MSAT, MsatSolverFactory::create }
#endif
  });

CreateSolverFunsMap available_solvers() { return solvers; }

std::vector<SolverEnum> available_solver_enums() { return solver_enums; }

std::vector<SolverEnum> available_termiter_solver_enums() {
  std::vector<SolverEnum> termiter_solvers;
  for (auto se : solver_enums)
  {
    if (se != YICES2)
    {
      termiter_solvers.push_back(se);
    }
  }
  return termiter_solvers;
}

CreateSolverFunsMap available_interpolators() { return itps; };

std::vector<SolverEnum> available_interpolator_enums() { return itp_enums; };

std::vector<SolverEnum> available_int_solver_enums()
{
  std::vector<SolverEnum> int_solvers;
  for (auto se : solver_enums)
  {
    if (se != BTOR)
    {
      int_solvers.push_back(se);
    }
  }
  return int_solvers;
}

std::vector<SolverEnum> available_constarr_solver_enums()
{
  std::vector<SolverEnum> arr_solvers;
  for (auto se : solver_enums)
  {
    if (se != YICES2)
    {
      arr_solvers.push_back(se);
    }
  }
  return arr_solvers;
}

std::vector<SolverEnum> available_full_transfer_solver_enums()
{
  std::vector<SolverEnum> solvers;
  for (auto se : available_termiter_solver_enums())
  {
    if (se != BTOR)
    {
      solvers.push_back(se);
    }
  }
  return solvers;
}

std::vector<SolverEnum> available_unsat_core_solver_enums()
{
  std::vector<SolverEnum> solvers;
  for (auto se : solver_enums)
  {
    solvers.push_back(se);
  }
  return solvers;
}

std::ostream & operator<<(std::ostream & o, SolverEnum e)
{
  switch (e)
  {
    case BTOR: o << "BTOR"; break;
    case CVC4: o << "CVC4"; break;
    case MSAT: o << "MSAT"; break;
    case YICES2: o << "YICES2"; break;
    default:
      // should print the integer representation
      throw NotImplementedException("Unknown SolverEnum: " + std::to_string(e));
      break;
  }

  return o;
}

}  // namespace smt_tests
