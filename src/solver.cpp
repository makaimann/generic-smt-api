/*********************                                                        */
/*! \file solver.cpp
** \verbatim
** Top contributors (to current version):
**   Makai Mann, Clark Barrett
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Abstract interface for SMT solvers.
**
**
**/

#include "solver.h"

#include "assert.h"
#include "exceptions.h"

namespace smt {

// TODO: Implement a generic visitor

Term AbsSmtSolver::substitute(const Term term,
                              const UnorderedTermMap & substitution_map) const
{
  // cache starts with the substitutions
  UnorderedTermMap cache(substitution_map);
  TermVec to_visit{ term };
  TermVec cached_children;
  Term t;
  while (to_visit.size())
  {
    t = to_visit.back();
    to_visit.pop_back();
    if (cache.find(t) == cache.end())
    {
      // doesn't get updated yet, just marking as visited
      cache[t] = t;
      to_visit.push_back(t);
      for (auto c : t)
      {
        to_visit.push_back(c);
      }
    }
    else
    {
      cached_children.clear();
      for (auto c : t)
      {
        cached_children.push_back(cache.at(c));
      }

      // const arrays have children but don't need to be rebuilt
      // (they're constructed in a particular way anyway)
      if (cached_children.size() && !t->is_value())
      {
        cache[t] = make_term(t->get_op(), cached_children);
      }
    }
  }

  return cache.at(term);
}

Result AbsSmtSolver::get_sequence_interpolants(const TermVec & formulae,
                                               TermVec & out_I) const
{
  // we'll give a default implementation for sequence interpolants that
  // does a loop outside the solver
  // most solvers don't support interpolation, so this will fail with
  // a NotImplementedException from get_interpolant
  // for better performance, should specialize sequence_interpolants
  // in the backend solver implementation (if supported)
  // this way, the proof doesn't need to be regenerated for each new
  // interpolant the solver will likely use the same proof and just manipulate
  // it to get each sequence interpolant

  size_t formulae_size = formulae.size();
  if (formulae_size < 2)
  {
    throw IncorrectUsageException(
        "Require at least 2 input formulae for sequence interpolation.");
  }

  Term A = formulae.at(0);
  TermVec Bvec;
  Bvec.reserve(formulae_size - 1);
  // add to Bvec in reverse order so we can pop_back later
  for (int i = formulae_size - 1; i >= 1; --i)
  {
    Bvec.push_back(formulae[i]);
  }

  // now do a loop and create an interpolant for each partition
  // NOTE: this is likely much less performant than asking the solver
  //       for a sequence interpolant directly (if supported)
  while (Bvec.size())
  {
    // Note: have to pass the solver (defaults to solver_)
    Term B = make_term(true);
    for (auto tt : Bvec)
    {
      B = make_term(And, B, tt);
    }
    Term I;
    Result r = get_interpolant(A, B, I);
    if (!r.is_unsat())
    {
      return r;
    }
    else
    {
      out_I.push_back(I);
    }
    // move formula to A and remove from Bvec
    // recall they were added to Bvec in reverse order
    A = make_term(And, A, Bvec.back());
    Bvec.pop_back();
  }

  assert(out_I.size() == formulae.size() - 1);
  // created all the interpolants
  return Result(UNSAT);
}

}  // namespace smt
