#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "smt_defs.h"

namespace smt_tests {

typedef ::smt::SmtSolver (*create_solver_fun)(void);

enum SolverEnum
{
  BTOR = 0,
  CVC4,
  MSAT,
  YICES2
};

typedef std::unordered_map<SolverEnum, create_solver_fun> CreateSolverFunsMap;

// Create a map from enums to available solver creation functions
CreateSolverFunsMap available_solvers();

// Create a map from enums to available "lite" solver creation functions
// More specifically the "raw" version of solvers with no logging
// Some solvers (e.g. CVC4) never use logging
// Others need to provide the create_lite_solver() function
CreateSolverFunsMap available_lite_solvers();

// collect all the available solvers
std::vector<SolverEnum> available_solver_enums();

// collect all solvers that support term iteration
std::vector<SolverEnum> available_termiter_solver_enums();

CreateSolverFunsMap available_interpolators();

std::vector<SolverEnum> available_interpolator_enums();

std::vector<SolverEnum> available_int_solver_enums();

std::vector<SolverEnum> available_array_model_solver_enums();

std::ostream & operator<<(std::ostream & o, SolverEnum e);

}  // namespace smt_tests

// define hash for older compilers
namespace std {
// specialize template
template <>
struct hash<smt_tests::SolverEnum>
{
  size_t operator()(const smt_tests::SolverEnum se) const
  {
    return static_cast<size_t>(se);
  }
};
}  // namespace std
