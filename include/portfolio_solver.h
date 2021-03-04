/*********************                                                        */
/*! \file portfolio_solver.h
** \verbatim
** Top contributors (to current version):
**   Amalee Wilson
** This file is part of the smt-switch project.
** Copyright (c) 2021 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Implementation of a portfolio solving function that takes a vector
**        of solvers and a term, and returns check_sat from the first solver
**        that finishes.
**/
#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "smt.h"

namespace smt {

// Used for synchronization.
std::mutex m;
std::condition_variable cv;

// Once a solver is done, is_sat has been set,
// and the main thread can terminate the others.
bool a_solver_is_done = false;
bool is_sat = false;

/** Translate the term t to the solver s, and check_sat.
 *  @param s The solver to translate the term t to.
 *  @param t The term being translated to solver s.
 */
void run_solver(SmtSolver s, Term t)
{
  TermTranslator to_s(s);
  Term a = to_s.transfer_term(t, smt::BOOL);
  s->assert_formula(a);
  is_sat = s->check_sat().is_sat();
  std::lock_guard<std::mutex> lk(m);
  a_solver_is_done = true;

  // The notify_one function is used here because there is only
  // one thread waiting on cv.
  cv.notify_one();
}

/** Launch many solvers and return whether the term is satisfiable when one of
 *  them has finished.
 *  @param solvers The solvers to run.
 *  @param t The term to be checked.
 */
bool portfolio_solve(std::vector<SmtSolver> solvers, Term t)
{
  // We must maintain a vector of pthreads in order to stop the threads that are
  // still running once one of the solvers finish because pthreads is assumed to
  // be the underlying implementation.
  pthread_t thr;
  std::vector<pthread_t> pthreads(solvers.size(), thr);

  for (auto s : solvers)
  {
    // Start a thread, store its handle, and detach the thread because we are
    // not interested in waiting for all of them to finish.
    std::thread t1(run_solver, s, t);
    pthreads.push_back(t1.native_handle());
    t1.detach();
  }

  // Wait until a solver is done to cancel the threads that are still running.
  std::unique_lock<std::mutex> lk(m);
  cv.wait(lk, [] { return a_solver_is_done; });
  for (int i = 0; i < pthreads.size(); ++i)
  {
    pthread_cancel(pthreads[i]);
  }

  return is_sat;
}
}  // namespace smt