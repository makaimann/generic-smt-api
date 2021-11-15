/*********************                                                        */
/*! \file bitwuzla_solver.h
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Bitwuzla implementation of AbsSmtSolver
**
**
**/

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "bitwuzla_sort.h"
#include "bitwuzla_term.h"
#include "exceptions.h"
#include "result.h"
#include "smt.h"
#include "sort.h"

namespace smt {
/**
   Bzla Solver
 */
class BzlaSolver : public AbsSmtSolver
{
 public:
  BzlaSolver() : AbsSmtSolver(BZLA), bzla(bitwuzla_new()), context_level(0)
  {
    // set termination function -- throw an exception
    auto throw_exception = [](const char * msg) -> void {
      throw InternalSolverException(msg);
    };
    bitwuzla_set_abort_callback(throw_exception);
  };
  BzlaSolver(const BzlaSolver &) = delete;
  BzlaSolver & operator=(const BzlaSolver &) = delete;
  ~BzlaSolver()
  {
    // need to destruct all stored terms in symbol_table
    symbol_table.clear();
    bitwuzla_delete(bzla);
  };
  void set_opt(const std::string option, const std::string value) override;
  void set_logic(const std::string logic) override;
  void assert_formula(const Term & t) override;
  Result check_sat() override;
  Result check_sat_assuming(const TermVec & assumptions) override;
  Result check_sat_assuming_list(const TermList & assumptions) override;
  Result check_sat_assuming_set(const UnorderedTermSet & assumptions) override;
  void push(uint64_t num = 1) override;
  void pop(uint64_t num = 1) override;
  uint64_t get_context_level() const override;
  Term get_value(const Term & t) const override;
  UnorderedTermMap get_array_values(const Term & arr,
                                    Term & out_const_base) const override;
  void get_unsat_assumptions(UnorderedTermSet & out) override;
  Sort make_sort(const std::string name, uint64_t arity) const override;
  Sort make_sort(SortKind sk) const override;
  Sort make_sort(SortKind sk, uint64_t size) const override;
  Sort make_sort(SortKind sk, const Sort & sort1) const override;
  Sort make_sort(SortKind sk,
                 const Sort & sort1,
                 const Sort & sort2) const override;
  Sort make_sort(SortKind sk,
                 const Sort & sort1,
                 const Sort & sort2,
                 const Sort & sort3) const override;
  Sort make_sort(SortKind sk, const SortVec & sorts) const override;
  Sort make_sort(const Sort & sort_con, const SortVec & sorts) const override;
  Sort make_sort(const DatatypeDecl & d) const override;

  DatatypeDecl make_datatype_decl(const std::string & s) override;
  DatatypeConstructorDecl make_datatype_constructor_decl(
      const std::string s) override;
  void add_constructor(DatatypeDecl & dt,
                       const DatatypeConstructorDecl & con) const override;
  void add_selector(DatatypeConstructorDecl & dt,
                    const std::string & name,
                    const Sort & s) const override;
  void add_selector_self(DatatypeConstructorDecl & dt,
                         const std::string & name) const override;
  Term get_constructor(const Sort & s, std::string name) const override;
  Term get_tester(const Sort & s, std::string name) const override;
  Term get_selector(const Sort & s,
                    std::string con,
                    std::string name) const override;

  Term make_term(bool b) const override;
  Term make_term(int64_t i, const Sort & sort) const override;
  Term make_term(const std::string val,
                 const Sort & sort,
                 uint64_t base = 10) const override;
  Term make_term(const Term & val, const Sort & sort) const override;
  Term make_symbol(const std::string name, const Sort & sort) override;
  Term get_symbol(const std::string & name) override;
  Term make_param(const std::string name, const Sort & sort) override;
  /* build a new term */
  Term make_term(Op op, const Term & t) const override;
  Term make_term(Op op, const Term & t0, const Term & t1) const override;
  Term make_term(Op op,
                 const Term & t0,
                 const Term & t1,
                 const Term & t2) const override;
  Term make_term(Op op, const TermVec & terms) const override;
  void reset() override;
  void reset_assertions() override;
  Term substitute(const Term term,
                  const UnorderedTermMap & substitution_map) const override;
  TermVec substitute_terms(
      const TermVec & term,
      const UnorderedTermMap & substitution_map) const override;
  void dump_smt2(std::string filename) const override;

  // getters for solver-specific objects
  // for interacting with third-party Bitwuzla-specific software

  Bitwuzla * get_bitwuzla() const { return bzla; };

 protected:
  Bitwuzla * bzla;

  std::unordered_map<std::string, Term> symbol_table;

  uint64_t context_level;

  // helper functions
  template <class I>
  inline Result check_sat_assuming_internal(I it, const I & end)
  {
    std::shared_ptr<BzlaTerm> bt;
    while (it != end)
    {
      bt = std::static_pointer_cast<BzlaTerm>(*it);
      bitwuzla_assume(bzla, bt->term);
      ++it;
    }

    BitwuzlaResult res = bitwuzla_check_sat(bzla);
    if (res == BITWUZLA_SAT)
    {
      return Result(SAT);
    }
    else if (res == BITWUZLA_UNSAT)
    {
      return Result(UNSAT);
    }
    else
    {
      return Result(UNKNOWN);
    }
  }
};

}  // namespace smt
