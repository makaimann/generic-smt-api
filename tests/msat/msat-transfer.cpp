#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

#include "msat_factory.h"
#include "smt.h"
// after a full installation
// #include "smt-switch/msat_factory.h"
// #include "smt-switch/smt.h"

using namespace smt;
using namespace std;

int main()
{
  SmtSolver s = MsatSolverFactory::create();
  s->set_opt("produce-models", "true");
  Sort bvsort8 = s->make_sort(BV, 8);
  Term x = s->make_symbol("x", bvsort8);
  Term y = s->make_symbol("y", bvsort8);
  Term z = s->make_symbol("z", bvsort8);
  Term T = s->make_term(true);

  Term a = s->make_symbol("a", s->make_sort(INT));
  Term b = s->make_symbol("b", s->make_sort(INT));

  Term constraint = s->make_term(Equal, z, s->make_term(BVAdd, x, y));
  constraint = s->make_term(And, constraint, s->make_term(Lt, a, b));
  s->assert_formula(constraint);

  SmtSolver s2 = MsatSolverFactory::create();
  s2->set_opt("produce-models", "true");
  s2->set_opt("incremental", "true");

  TermTranslator tt(s2);

  Term constraint2 = tt.transfer_term(constraint);
  Term T2 = tt.transfer_term(T);
  // ensure it can handle transfering again (even though it already built the
  // node)
  constraint2 = tt.transfer_term(constraint);
  s2->assert_formula(constraint2);

  cout << "term from solver 1: " << constraint << endl;
  cout << "term from solver 2: " << constraint2 << endl;

  assert(s->check_sat().is_sat());
  assert(s2->check_sat().is_sat());

  return 0;
}
