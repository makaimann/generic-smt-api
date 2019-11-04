#include "cvc4_solver.h"

namespace smt
{

/* CVC4 Op mappings */
const std::unordered_map<PrimOp, ::CVC4::api::Kind> primop2kind(
    { { And, ::CVC4::api::AND },
      { Or, ::CVC4::api::OR },
      { Xor, ::CVC4::api::XOR },
      { Not, ::CVC4::api::NOT },
      { Implies, ::CVC4::api::IMPLIES },
      { Ite, ::CVC4::api::ITE },
      { Iff, ::CVC4::api::EQUAL },
      { Equal, ::CVC4::api::EQUAL },
      { Distinct, ::CVC4::api::DISTINCT },
      /* Uninterpreted Functions */
      { Apply, ::CVC4::api::APPLY_UF },
      /* Arithmetic Theories */
      { Plus, ::CVC4::api::PLUS },
      { Minus, ::CVC4::api::MINUS },
      { Negate, ::CVC4::api::UMINUS },
      { Mult, ::CVC4::api::MULT },
      { Div, ::CVC4::api::DIVISION },
      { Lt, ::CVC4::api::LT },
      { Le, ::CVC4::api::LEQ },
      { Gt, ::CVC4::api::GT },
      { Ge, ::CVC4::api::GEQ },
      { Mod, ::CVC4::api::INTS_MODULUS },
      { Abs, ::CVC4::api::ABS },
      { Pow, ::CVC4::api::POW },
      { To_Real, ::CVC4::api::TO_REAL },
      { To_Int, ::CVC4::api::TO_INTEGER },
      { Is_Int, ::CVC4::api::IS_INTEGER },
      /* Fixed Size BitVector Theory */
      { Concat, ::CVC4::api::BITVECTOR_CONCAT },
      // Indexed Op
      { Extract, ::CVC4::api::BITVECTOR_EXTRACT },
      { BVNot, ::CVC4::api::BITVECTOR_NOT },
      { BVNeg, ::CVC4::api::BITVECTOR_NEG },
      { BVAnd, ::CVC4::api::BITVECTOR_AND },
      { BVOr, ::CVC4::api::BITVECTOR_OR },
      { BVXor, ::CVC4::api::BITVECTOR_XOR },
      { BVNand, ::CVC4::api::BITVECTOR_NAND },
      { BVNor, ::CVC4::api::BITVECTOR_NOR },
      { BVXnor, ::CVC4::api::BITVECTOR_XNOR },
      { BVComp, ::CVC4::api::BITVECTOR_COMP },
      { BVAdd, ::CVC4::api::BITVECTOR_PLUS },
      { BVSub, ::CVC4::api::BITVECTOR_SUB },
      { BVMul, ::CVC4::api::BITVECTOR_MULT },
      { BVUdiv, ::CVC4::api::BITVECTOR_UDIV },
      { BVSdiv, ::CVC4::api::BITVECTOR_SDIV },
      { BVUrem, ::CVC4::api::BITVECTOR_UREM },
      { BVSrem, ::CVC4::api::BITVECTOR_SREM },
      { BVSmod, ::CVC4::api::BITVECTOR_SMOD },
      { BVShl, ::CVC4::api::BITVECTOR_SHL },
      { BVAshr, ::CVC4::api::BITVECTOR_ASHR },
      { BVLshr, ::CVC4::api::BITVECTOR_LSHR },
      { BVUlt, ::CVC4::api::BITVECTOR_ULT },
      { BVUle, ::CVC4::api::BITVECTOR_ULE },
      { BVUgt, ::CVC4::api::BITVECTOR_UGT },
      { BVUge, ::CVC4::api::BITVECTOR_UGE },
      { BVSlt, ::CVC4::api::BITVECTOR_SLT },
      { BVSle, ::CVC4::api::BITVECTOR_SLE },
      { BVSgt, ::CVC4::api::BITVECTOR_SGT },
      { BVSge, ::CVC4::api::BITVECTOR_SGE },
      // Indexed Op
      { Zero_Extend, ::CVC4::api::BITVECTOR_ZERO_EXTEND },
      // Indexed Op
      { Sign_Extend, ::CVC4::api::BITVECTOR_SIGN_EXTEND },
      // Indexed Op
      { Repeat, ::CVC4::api::BITVECTOR_REPEAT },
      // Indexed Op
      { Rotate_Left, ::CVC4::api::BITVECTOR_ROTATE_LEFT },
      // Indexed Op
      { Rotate_Right, ::CVC4::api::BITVECTOR_ROTATE_RIGHT },
      // Conversion
      { BV_To_Nat, ::CVC4::api::BITVECTOR_TO_NAT },
      // Indexed Op
      { Int_To_BV, ::CVC4::api::INT_TO_BITVECTOR },
      { Select, ::CVC4::api::SELECT },
      { Store, ::CVC4::api::STORE },
      { Const_Array, ::CVC4::api::STORE_ALL } });

const std::unordered_map<::CVC4::api::Kind, PrimOp> kind2primop(
    { { ::CVC4::api::AND, And },
      { ::CVC4::api::OR, Or },
      { ::CVC4::api::XOR, Xor },
      { ::CVC4::api::NOT, Not },
      { ::CVC4::api::IMPLIES, Implies },
      { ::CVC4::api::ITE, Ite },
      { ::CVC4::api::EQUAL, Iff },
      { ::CVC4::api::EQUAL, Equal },
      { ::CVC4::api::DISTINCT, Distinct },
      { ::CVC4::api::BITVECTOR_CONCAT, Concat },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_EXTRACT, Extract },
      { ::CVC4::api::BITVECTOR_NOT, BVNot },
      { ::CVC4::api::BITVECTOR_NEG, BVNeg },
      { ::CVC4::api::BITVECTOR_AND, BVAnd },
      { ::CVC4::api::BITVECTOR_OR, BVOr },
      { ::CVC4::api::BITVECTOR_XOR, BVXor },
      { ::CVC4::api::BITVECTOR_NAND, BVNand },
      { ::CVC4::api::BITVECTOR_NOR, BVNor },
      { ::CVC4::api::BITVECTOR_XNOR, BVXnor },
      { ::CVC4::api::BITVECTOR_COMP, BVComp },
      { ::CVC4::api::BITVECTOR_PLUS, BVAdd },
      { ::CVC4::api::BITVECTOR_SUB, BVSub },
      { ::CVC4::api::BITVECTOR_MULT, BVMul },
      { ::CVC4::api::BITVECTOR_UDIV, BVUdiv },
      { ::CVC4::api::BITVECTOR_SDIV, BVSdiv },
      { ::CVC4::api::BITVECTOR_UREM, BVUrem },
      { ::CVC4::api::BITVECTOR_SREM, BVSrem },
      { ::CVC4::api::BITVECTOR_SMOD, BVSmod },
      { ::CVC4::api::BITVECTOR_SHL, BVShl },
      { ::CVC4::api::BITVECTOR_ASHR, BVAshr },
      { ::CVC4::api::BITVECTOR_LSHR, BVLshr },
      { ::CVC4::api::BITVECTOR_ULT, BVUlt },
      { ::CVC4::api::BITVECTOR_ULE, BVUle },
      { ::CVC4::api::BITVECTOR_UGT, BVUgt },
      { ::CVC4::api::BITVECTOR_UGE, BVUge },
      { ::CVC4::api::BITVECTOR_SLT, BVSlt },
      { ::CVC4::api::BITVECTOR_SLE, BVSle },
      { ::CVC4::api::BITVECTOR_SGT, BVSgt },
      { ::CVC4::api::BITVECTOR_SGE, BVSge },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ZERO_EXTEND, Zero_Extend },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_SIGN_EXTEND, Sign_Extend },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_REPEAT, Repeat },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ROTATE_LEFT, Rotate_Left },
      // Indexed Op
      { ::CVC4::api::BITVECTOR_ROTATE_RIGHT, Rotate_Right },
      { ::CVC4::api::SELECT, Select },
      { ::CVC4::api::STORE, Store },
      { ::CVC4::api::STORE_ALL, Const_Array } });

// the kinds CVC4 needs to build an OpTerm for an indexed op
const std::unordered_map<PrimOp, ::CVC4::api::Kind> primop2optermcon(
    { { Extract, ::CVC4::api::BITVECTOR_EXTRACT_OP },
      { Zero_Extend, ::CVC4::api::BITVECTOR_ZERO_EXTEND_OP },
      { Sign_Extend, ::CVC4::api::BITVECTOR_SIGN_EXTEND_OP },
      { Repeat, ::CVC4::api::BITVECTOR_REPEAT_OP },
      { Rotate_Left, ::CVC4::api::BITVECTOR_ROTATE_LEFT_OP },
      { Rotate_Right, ::CVC4::api::BITVECTOR_ROTATE_RIGHT_OP },
      { Int_To_BV, ::CVC4::api::INT_TO_BITVECTOR_OP } });

/* CVC4Solver implementation */

void CVC4Solver::set_opt(const std::string option, const std::string value)
{
  try
  {
    solver.setOption(option, value);
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::set_logic(const std::string logic) const
{
  try
  {
    solver.setLogic(logic);
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(bool b) const
{
  try
  {
    Term c(new CVC4Term(solver.mkBoolean(b)));
    return c;
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(int64_t i, const Sort & sort) const
{
  try
  {
    SortKind sk = sort->get_sort_kind();
    ::CVC4::api::Term c;

    if ((sk == INT) || (sk == REAL))
    {
      c = solver.mkReal(i);
    }
    else if (sk == BV)
    {
      // CVC4 uses unsigned integers for mkBitVector
      // to avoid casting issues, always use a string in base 10
      std::string sval = std::to_string(i);
      c = solver.mkBitVector(sort->get_width(), sval, 10);
    }
    else
    {
      std::string msg = "Can't create constant with integer for sort ";
      msg += sort->to_string();
      throw IncorrectUsageException(msg.c_str());
    }

    Term res(new CVC4Term(c));
    return res;
  }
  catch (std::exception & e)
  {
    // pretty safe to assume that an error is due to incorrect usage
    throw IncorrectUsageException(e.what());
  }
}

Term CVC4Solver::make_term(std::string val,
                           const Sort & sort,
                           uint64_t base) const
{
  try
  {
    SortKind sk = sort->get_sort_kind();
    ::CVC4::api::Term c;

    if ((sk == INT) || (sk == REAL))
    {
      // TODO: Only do these checks in debug
      if (base != 10)
      {
        throw IncorrectUsageException("Can't use non-decimal base for reals and ints");
      }
      c = solver.mkReal(val);
    }
    else if (sk == BV)
    {
      c = solver.mkBitVector(sort->get_width(), val, base);
    }
    else
    {
      std::string msg = "Can't create constant with integer for sort ";
      msg += sort->to_string();
      throw IncorrectUsageException(msg.c_str());
    }

    Term res(new CVC4Term(c));
    return res;
  }
  catch (std::exception & e)
  {
    // pretty safe to assume that an error is due to incorrect usage
    throw IncorrectUsageException(e.what());
  }
}

Term CVC4Solver::make_term(const Term & val, const Sort & sort) const
{
  throw NotImplementedException("Constant arrays not yet implemented.");
}

void CVC4Solver::assert_formula(const Term& t) const
{
  try
  {
    std::shared_ptr<CVC4Term> cterm = std::static_pointer_cast<CVC4Term>(t);
    solver.assertFormula(cterm->term);
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Result CVC4Solver::check_sat()
{
  try
  {
    ::CVC4::api::Result r = solver.checkSat();
    if (r.isUnsat())
    {
      return Result(UNSAT);
    }
    else if (r.isSat())
    {
      return Result(SAT);
    }
    else if (r.isSatUnknown())
    {
      return Result(UNKNOWN, r.getUnknownExplanation());
    }
    else
    {
      throw NotImplementedException("Unimplemented result type from CVC4");
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Result CVC4Solver::check_sat_assuming(const TermVec & assumptions)
{
  try
  {
    // expecting (possibly negated) boolean literals
    for (auto a : assumptions)
    {
      if (!a->is_symbolic_const() || a->get_sort()->get_sort_kind() != BOOL)
      {
        if (a->get_op() == Not && (*a->begin())->is_symbolic_const())
        {
          continue;
        }
        else
        {
          throw IncorrectUsageException(
              "Expecting boolean indicator literals but got: "
              + a->to_string());
        }
      }
    }

    std::vector<::CVC4::api::Term> cvc4assumps;
    cvc4assumps.reserve(assumptions.size());

    std::shared_ptr<CVC4Term> cterm;
    for (auto a : assumptions)
    {
      cvc4assumps.push_back(std::static_pointer_cast<CVC4Term>(a)->term);
    }
    ::CVC4::api::Result r = solver.checkSatAssuming(cvc4assumps);
    if (r.isUnsat())
    {
      return Result(UNSAT);
    }
    else if (r.isSat())
    {
      return Result(SAT);
    }
    else if (r.isSatUnknown())
    {
      return Result(UNKNOWN, r.getUnknownExplanation());
    }
    else
    {
      throw NotImplementedException("Unimplemented result type from CVC4");
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::push(uint64_t num)
{
  try
  {
    solver.push(num);
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::pop(uint64_t num)
{
  try
  {
    solver.pop(num);
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::get_value(Term & t) const
{
  try
  {
    std::shared_ptr<CVC4Term> cterm = std::static_pointer_cast<CVC4Term>(t);
    Term val(new CVC4Term(solver.getValue(cterm->term)));
    return val;
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort CVC4Solver::make_sort(const std::string name, uint64_t arity) const
{
  try
  {
    Sort s(new CVC4Sort(solver.declareSort(name, arity)));
    return s;
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort CVC4Solver::make_sort(SortKind sk) const
{
  try
  {
    if (sk == BOOL)
    {
      Sort s(new CVC4Sort(solver.getBooleanSort()));
      return s;
    }
    else if (sk == INT)
    {
      Sort s(new CVC4Sort(solver.getIntegerSort()));
      return s;
    }
    else if (sk == REAL)
    {
      Sort s(new CVC4Sort(solver.getRealSort()));
      return s;
    }
    else
    {
      std::string msg("Can't create sort with sort constructor ");
      msg += to_string(sk);
      msg += " and no arguments";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort CVC4Solver::make_sort(SortKind sk, uint64_t size) const
{
  try
  {
    if (sk == BV)
    {
      Sort s(new CVC4Sort(solver.mkBitVectorSort(size)));
      return s;
    }
    else
    {
      std::string msg("Can't create sort with sort constructor ");
      msg += to_string(sk);
      msg += " and an integer argument";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort CVC4Solver::make_sort(SortKind sk, const Sort & sort1) const
{
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take one sort parameter yet.");
}

Sort CVC4Solver::make_sort(SortKind sk,
                           const Sort & sort1,
                           const Sort & sort2) const
{
  try
  {
    if (sk == ARRAY)
    {
      std::shared_ptr<CVC4Sort> cidxsort =
          std::static_pointer_cast<CVC4Sort>(sort1);
      std::shared_ptr<CVC4Sort> celemsort =
          std::static_pointer_cast<CVC4Sort>(sort2);
      Sort s(new CVC4Sort(solver.mkArraySort(cidxsort->sort, celemsort->sort)));
      return s;
    }
    else
    {
      std::string msg("Can't create sort with sort constructor ");
      msg += to_string(sk);
      msg += " and two Sort arguments";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Sort CVC4Solver::make_sort(SortKind sk,
                           const Sort & sort1,
                           const Sort & sort2,
                           const Sort & sort3) const
{
  throw NotImplementedException(
      "Smt-switch does not have any sorts that take three sort parameters "
      "yet.");
}

Sort CVC4Solver::make_sort(SortKind sk, const SortVec & sorts) const
{
  try
  {
    if (sk == FUNCTION)
    {
      if (sorts.size() < 2)
      {
        throw IncorrectUsageException(
            "Function sort must have >=2 sort arguments.");
      }

      // arity is one less, because last sort is return sort
      uint32_t arity = sorts.size() - 1;

      std::vector<::CVC4::api::Sort> csorts;
      csorts.reserve(arity);
      ::CVC4::api::Sort csort;
      for (uint32_t i = 0; i < arity; i++)
      {
        csort = std::static_pointer_cast<CVC4Sort>(sorts[i])->sort;
        csorts.push_back(csort);
      }

      csort = std::static_pointer_cast<CVC4Sort>(sorts.back())->sort;
      ::CVC4::api::Sort cfunsort = solver.mkFunctionSort(csorts, csort);
      Sort funsort(new CVC4Sort(cfunsort));
      return funsort;
    }
    else if (sorts.size() == 1)
    {
      return make_sort(sk, sorts[0]);
    }
    else if (sorts.size() == 2)
    {
      return make_sort(sk, sorts[0], sorts[1]);
    }
    else if (sorts.size() == 3)
    {
      return make_sort(sk, sorts[0], sorts[1], sorts[2]);
    }
    else
    {
      std::string msg("Can't create sort from sort constructor ");
      msg += to_string(sk);
      msg += " with a vector of sorts";
      throw IncorrectUsageException(msg.c_str());
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_symbol(const std::string name, const Sort & sort)
{
  // check that name is available
  // to make CVC4 behave the same as other solvers
  if (symbols.find(name) != symbols.end())
  {
    throw IncorrectUsageException("symbol " + name + " has already been used.");
  }

  try
  {
    std::shared_ptr<CVC4Sort> csort = std::static_pointer_cast<CVC4Sort>(sort);
    ::CVC4::api::Term t = solver.mkConst(csort->sort, name);
    Term res(new ::smt::CVC4Term(t));
    symbols[name] = res;
    return res;
  }
  catch(std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(Op op, const Term & t) const
{
  try
  {
    std::shared_ptr<CVC4Term> cterm = std::static_pointer_cast<CVC4Term>(t);
    if (op.num_idx == 0)
    {
      Term result(
          new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op), cterm->term)));
      return result;
    }
    else
    {
      ::CVC4::api::OpTerm ot = make_op_term(op);
      Term result(new CVC4Term(
          solver.mkTerm(primop2kind.at(op.prim_op), ot, cterm->term)));
      return result;
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(Op op, const Term & t0, const Term & t1) const
{
  try
  {
    std::shared_ptr<CVC4Term> cterm0 = std::static_pointer_cast<CVC4Term>(t0);
    std::shared_ptr<CVC4Term> cterm1 = std::static_pointer_cast<CVC4Term>(t1);
    if (op.num_idx == 0)
    {
      Term result(new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op),
                                             cterm0->term,
                                             cterm1->term)));
      return result;
    }
  else
    {
      ::CVC4::api::OpTerm ot = make_op_term(op);
      Term result(new CVC4Term(solver.mkTerm(
          primop2kind.at(op.prim_op), ot, cterm0->term, cterm1->term)));
      return result;
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(Op op,
                           const Term & t0,
                           const Term & t1,
                           const Term & t2) const
{
  try
  {
    std::shared_ptr<CVC4Term> cterm0 = std::static_pointer_cast<CVC4Term>(t0);
    std::shared_ptr<CVC4Term> cterm1 = std::static_pointer_cast<CVC4Term>(t1);
    std::shared_ptr<CVC4Term> cterm2 = std::static_pointer_cast<CVC4Term>(t2);
    if (op.num_idx == 0)
    {
      Term result(new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op),
                                             cterm0->term,
                                             cterm1->term,
                                             cterm2->term)));
      return result;
    }
  else
    {
      ::CVC4::api::OpTerm ot = make_op_term(op);
      Term result(new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op),
                                             ot,
                                             cterm0->term,
                                             cterm1->term,
                                             cterm2->term)));
      return result;
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

Term CVC4Solver::make_term(Op op, const TermVec & terms) const
{
  try
  {
    std::vector<::CVC4::api::Term> cterms;
    cterms.reserve(terms.size());
    std::shared_ptr<CVC4Term> cterm;
    for (auto t : terms)
    {
      cterm = std::static_pointer_cast<CVC4Term>(t);
      cterms.push_back(cterm->term);
    }
    if (op.num_idx == 0)
    {
      Term result(
          new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op), cterms)));
      return result;
    }
    else
    {
      ::CVC4::api::OpTerm ot = make_op_term(op);
      Term result(
          new CVC4Term(solver.mkTerm(primop2kind.at(op.prim_op), ot, cterms)));
      return result;
    }
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::reset()
{
  try
  {
    solver.reset();
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::reset_assertions()
{
  try
  {
    solver.resetAssertions();
  }
  catch (std::exception & e)
  {
    throw InternalSolverException(e.what());
  }
}

void CVC4Solver::dump_smt2(FILE * file) const
{
  throw NotImplementedException("Not yet implemented dumping smt2");
}

/**
   Helper function for creating an OpTerm from an Op
   Preconditions: op must be indexed, i.e. op.num_idx > 0
*/
::CVC4::api::OpTerm CVC4Solver::make_op_term(Op op) const
{
  if (op.num_idx == 1)
  {
    return solver.mkOpTerm(primop2optermcon.at(op.prim_op), op.idx0);
  }
  else if (op.num_idx == 2)
  {
    return solver.mkOpTerm(primop2optermcon.at(op.prim_op), op.idx0, op.idx1);
  }
  else
  {
    throw NotImplementedException(
        "CVC4 does not have any indexed "
        "operators with more than two indices");
  }
}

/* end CVC4Solver implementation */

}
