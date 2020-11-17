/*********************                                                        */
/*! \file unit-substitute.cpp
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Unit tests for term substitution.
**
**
**/

#include <utility>
#include <vector>

#include "available_solvers.h"
#include "gtest/gtest.h"
#include "substitution_walker.h"
#include "utils.h"

using namespace smt;
using namespace std;

namespace smt_tests {

class UnitSubstituteTests
    : public ::testing::Test,
      public ::testing::WithParamInterface<SolverConfiguration>
{
 protected:
  void SetUp() override
  {
    s = create_solver(GetParam());
    bvsort = s->make_sort(BV, 4);

    x = s->make_symbol("x", bvsort);
    y = s->make_symbol("y", bvsort);
    xpy = s->make_term(BVAdd, x, y);

    a = s->make_symbol("a", bvsort);
    b = s->make_symbol("b", bvsort);
  }
  SmtSolver s;
  Sort bvsort;
  Term x, y, xpy, a, b;
};

TEST_P(UnitSubstituteTests, SimpleSubstitution)
{
  UnorderedTermMap subs_map({ { x, a }, { y, b } });
  Term apb = s->substitute(xpy, subs_map);

  UnorderedTermSet free_syms;
  get_free_symbolic_consts(apb, free_syms);
  EXPECT_EQ(free_syms.size(), 2);

  for (auto v : { a, b })
  {
    EXPECT_TRUE(free_syms.find(v) != free_syms.end());
  }
}

TEST_P(UnitSubstituteTests, SimpleSubstitutionWalker)
{
  // substitution walker can substitute arbitrary terms
  // not just symbol keys
  Term apb_spec = s->make_term(BVAdd, a, b);
  UnorderedTermMap subs_map({ { xpy, apb_spec } });
  SubstitutionWalker sw(s, subs_map);

  Term apb = sw.visit(xpy);
  EXPECT_EQ(apb, apb_spec);

  // test it again to make sure not corrupted
  apb = sw.visit(xpy);
  EXPECT_EQ(apb, apb_spec);
}

INSTANTIATE_TEST_SUITE_P(
    ParametrizedUnitSubstitute,
    UnitSubstituteTests,
    testing::ValuesIn(filter_solver_configurations({ TERMITER })));

}  // namespace smt_tests
