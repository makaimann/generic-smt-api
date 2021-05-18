/*********************                                                        */
/*! \file smtlib_reader_test_inputs.h
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief List test inputs for SmtLibReader
**
**
**/

#pragma once

#include <unordered_map>
#include <vector>

#include "smt.h"

// maps tests to their expected results
const std::unordered_map<std::string, std::vector<smt::Result>> qf_uflia_tests({
    { "test-uf.smt2",
      { smt::Result(smt::SAT),
        smt::Result(smt::UNSAT),
        smt::Result(smt::SAT) } },
    { "test-symbols.smt2",
      { smt::Result(smt::SAT),
        smt::Result(smt::UNSAT),
        smt::Result(smt::SAT) } },
});
