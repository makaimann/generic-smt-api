/*********************                                                        */
/*! \file unit-walker.cpp
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Unit tests for term walkers.
**
**
**/

#include <utility>
#include <vector>

#include "available_solvers.h"
#include "gtest/gtest.h"
#include "identity_walker.h"
#include "smt.h"

using namespace smt;
using namespace std;

namespace smt_tests {

class UnitWalkerTests : public ::testing::Test,
                        public ::testing::WithParamInterface<SolverConfiguration>
{
 protected:
  void SetUp() override
  {
    s = create_solver(GetParam());

    bvsort = s->make_sort(BV, 4);
    funsort = s->make_sort(FUNCTION, SortVec{ bvsort, bvsort });
    arrsort = s->make_sort(ARRAY, bvsort, bvsort);
  }
  SmtSolver s;
  Sort bvsort, funsort, arrsort;
};

TEST_P(UnitWalkerTests, Simple)
{
  Term x = s->make_symbol("x", bvsort);
  Term xp1 = s->make_term(BVAdd, x, s->make_term(1, bvsort));
  IdentityWalker iw(s, false);
  EXPECT_EQ(xp1, iw.visit(xp1));
  // visit a second time
  EXPECT_EQ(xp1, iw.visit(xp1));
}

TEST_P(UnitWalkerTests, SimpleSubstitution)
{
  // using IdentityWalker for substitution
  // no reason to use this over the substitute method
  // only for testing

  Term x = s->make_symbol("x", bvsort);
  Term xp1 = s->make_term(BVAdd, x, s->make_term(1, bvsort));

  Term y = s->make_symbol("y", bvsort);
  Term yp1 = s->make_term(BVAdd, y, s->make_term(1, bvsort));

  UnorderedTermMap subs({ { x, y } });
  IdentityWalker iw(s, false, &subs);
  EXPECT_EQ(yp1, iw.visit(xp1));
  // visit a second time
  EXPECT_EQ(yp1, iw.visit(xp1));
  EXPECT_EQ(subs.size(), 3);  // should contain, x, 1, and x+1
}

TEST_P(UnitWalkerTests, ArraySubstitution)
{
  // using IdentityWalker for substitution
  // no reason to use this over the substitute method
  // only for testing

  Term x = s->make_symbol("x", bvsort);
  Term arr = s->make_symbol("arr", arrsort);
  Term arrx = s->make_term(Select, arr, x);

  Term x_0 = s->make_symbol("x@0", bvsort);
  Term arr_0 = s->make_symbol("arr@0", arrsort);
  Term arrx_0 = s->make_term(Select, arr_0, x_0);

  UnorderedTermMap subs({ { x, x_0 }, { arr, arr_0 } });
  IdentityWalker iw(s, false, &subs);
  EXPECT_EQ(arrx_0, iw.visit(arrx));
  // visit a second time
  EXPECT_EQ(arrx_0, iw.visit(arrx));
}

TEST_P(UnitWalkerTests, FunSubstitution)
{
  // using IdentityWalker for substitution
  // no reason to use this over the substitute method
  // only for testing

  Term f = s->make_symbol("f", funsort);

  Term x = s->make_symbol("x", bvsort);
  Term fx = s->make_term(Apply, f, x);

  Term y = s->make_symbol("y", bvsort);
  Term fy = s->make_term(Apply, f, y);

  UnorderedTermMap subs({ { x, y } });
  IdentityWalker iw(s, false, &subs);
  EXPECT_EQ(fy, iw.visit(fx));
  // visit a second time
  EXPECT_EQ(fy, iw.visit(fx));
}

INSTANTIATE_TEST_SUITE_P(ParametrizedUnitWalker,
                         UnitWalkerTests,
                         testing::ValuesIn(filter_solver_configurations({ TERMITER })));

}  // namespace smt_tests
