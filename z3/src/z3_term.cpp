#include "z3_term.h"

#include <unordered_map>

#include "exceptions.h"
#include "ops.h"
#include "z3_sort.h"

using namespace std;

namespace smt {

// Z3TermIter implementation

// Z3TermIter::Z3TermIter(const Z3TermIter & it)
// {
//   throw NotImplementedException(
//       "Term iteration not implemented for Z3 backend.");
// }

Z3TermIter & Z3TermIter::operator=(const Z3TermIter & it)
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

void Z3TermIter::operator++()
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

const Term Z3TermIter::operator*()
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

TermIterBase * Z3TermIter::clone() const
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

bool Z3TermIter::operator==(const Z3TermIter & it)
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

bool Z3TermIter::operator!=(const Z3TermIter & it)
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

bool Z3TermIter::equal(const TermIterBase & other) const
{
  throw NotImplementedException(
      "Term iteration not implemented for Z3 backend.");
}

// end Z3TermIter implementation

// Z3Term implementation

size_t Z3Term::hash() const
{
  if (!is_function)
  {
    return term.hash();
  }
  else
  {
    return z_func.hash();
  }
}

std::size_t Z3Term::get_id() const
{
  if (!is_function)
  {
    return term.id();
  }
  else
  {
    return z_func.id();
  }
}

bool Z3Term::compare(const Term & absterm) const
{
  std::shared_ptr<Z3Term> zs = std::static_pointer_cast<Z3Term>(absterm);
  if (is_function && zs->is_function)
  {
    return z_func.hash() == (zs->z_func).hash();
  }
  else if (!is_function && !zs->is_function)
  {
    return term.hash() == (zs->term).hash();
  }
  return false;
}

Op Z3Term::get_op() const
{
  if (is_function || !term.is_app())
  {
    return Op();
  }
  else
  {
    func_decl decl = term.decl();
    z3::sort range = decl.range();
    string name = decl.name().str();
    Z3_decl_kind kind = decl.decl_kind();

    switch (kind)
    {
      // unary
      case Z3_OP_NOT: return Op(Not);
      case Z3_OP_UMINUS: return Op(Negate);
      case Z3_OP_FPA_ABS: return Op(Abs);
      case Z3_OP_FPA_TO_REAL: return Op(To_Real);
      case Z3_OP_STR_TO_INT: return Op(To_Int);
      case Z3_OP_IS_INT: return Op(Is_Int);
      case Z3_OP_BNOT: return Op(BVNot);
      case Z3_OP_BNEG:
        return Op(BVNeg);
        // binary
      case Z3_OP_XOR: return Op(Xor);
      case Z3_OP_IMPLIES: return Op(Implies);
      case Z3_OP_POWER: return Op(Pow);
      case Z3_OP_IDIV: return Op(IntDiv);
      case Z3_OP_DIV: return Op(Div);
      case Z3_OP_LT: return Op(Lt);
      case Z3_OP_FPA_ROUND_TO_INTEGRAL: return Op(To_Int);
      case Z3_OP_LE: return Op(Le);
      case Z3_OP_GT: return Op(Gt);
      case Z3_OP_GE: return Op(Ge);
      case Z3_OP_EQ: return Op(Equal);
      case Z3_OP_MOD: return Op(Mod);
      case Z3_OP_CONCAT: return Op(Concat);
      case Z3_OP_BAND: return Op(BVAnd);
      case Z3_OP_BOR: return Op(BVOr);
      case Z3_OP_BXOR: return Op(BVXor);
      case Z3_OP_BNAND: return Op(BVNand);
      case Z3_OP_BNOR: return Op(BVNor);
      case Z3_OP_BXNOR: return Op(BVXnor);
      case Z3_OP_BADD: return Op(BVAdd);
      case Z3_OP_BSUB: return Op(BVSub);
      case Z3_OP_BMUL: return Op(BVMul);
      case Z3_OP_BUDIV: return Op(BVUdiv);
      case Z3_OP_BUREM: return Op(BVUrem);
      case Z3_OP_BSDIV: return Op(BVSdiv);
      case Z3_OP_BSREM: return Op(BVSrem);
      case Z3_OP_BSMOD: return Op(BVSmod);
      case Z3_OP_BSHL: return Op(BVShl);
      case Z3_OP_BASHR: return Op(BVAshr);
      case Z3_OP_BLSHR: return Op(BVLshr);
      case Z3_OP_ULT: return Op(BVUlt);
      case Z3_OP_ULEQ: return Op(BVUle);
      case Z3_OP_UGT: return Op(BVUgt);
      case Z3_OP_UGEQ: return Op(BVUge);
      case Z3_OP_SLEQ: return Op(BVSle);
      case Z3_OP_SLT: return Op(BVSlt);
      case Z3_OP_SGEQ: return Op(BVSge);
      case Z3_OP_SGT: return Op(BVSgt);
      case Z3_OP_ROTATE_LEFT: return Op(Rotate_Left);
      case Z3_OP_ROTATE_RIGHT: return Op(Rotate_Right);
      case Z3_OP_SELECT:
        return Op(Select);
        // ternary
      case Z3_OP_ITE: return Op(Ite);
      case Z3_OP_STORE:
        return Op(Store);
        // variadic
      case Z3_OP_AND: return Op(And);
      case Z3_OP_OR: return Op(Or);
      case Z3_OP_ADD: return Op(Plus);
      case Z3_OP_SUB: return Op(Minus);
      case Z3_OP_MUL: return Op(Mult);
      case Z3_OP_DISTINCT:
        return Op(Distinct);
        // indexed
      case Z3_OP_EXTRACT: return Op(Extract);
      case Z3_OP_ZERO_EXT: return Op(Zero_Extend);
      case Z3_OP_SIGN_EXT: return Op(Sign_Extend);
      case Z3_OP_REPEAT: return Op(Repeat);
      case Z3_OP_INT2BV: {
        size_t out_width = range.bv_size();
        return Op(Int_To_BV, out_width);
      }
      case Z3_OP_BV2INT:
        return Op(BV_To_Nat);
        // Op(Apply) not handled...

      default: {
        std::string msg("Option - ");
        msg += name;
        msg += " - not implemented for Z3 backend.";
        throw NotImplementedException(msg.c_str());
      }
    }
  }
}

Sort Z3Term::get_sort() const
{
  if (!is_function)
  {
    return std::make_shared<Z3Sort>(term.get_sort(), *ctx);
  }

  context c;
  z3::sort_vector domain(c);
  for (int i = 0; i < z_func.arity(); i++)
  {
    domain.push_back(z_func.domain(i));
  }

  z3::func_decl func = c.function(z_func.name(), domain, z_func.range());

  return std::make_shared<Z3Sort>(func, c);
}

bool Z3Term::is_symbol() const
{
  return is_function || (term.is_const() || term.is_var());
}

bool Z3Term::is_param() const { return term.is_var(); }

bool Z3Term::is_symbolic_const() const
{
  if (is_function || is_param())
  {
    return false;
  }
  return is_symbol();
  // return (!is_function && term.is_const());
}

bool Z3Term::is_value() const
{
  // return (!is_function && term.is_const());
  if (is_function)
  {
    return false;
  }
  if (!term.is_const() && !term.is_app())
  {
    return term.is_bool() || term.is_arith() || term.is_bv();
  }
  return false;
}

string Z3Term::to_string()
{
  if (is_function)
  {
    return z_func.to_string();
  }
  else
  {
    return term.to_string();
  }
}

uint64_t Z3Term::to_int() const
{
  std::string val = term.to_string();
  int base = 10;

  // Process bit-vector format.
  if (term.is_bv())
  {
    if (val.substr(0, 2) == "#x")
    {
      base = 16;
    }
    else if (val.substr(0, 2) == "#b")
    {
      base = 2;
    }
    else
    {
      std::string msg = val;
      msg += " is not a value term, can't convert to int.";
      throw IncorrectUsageException(msg.c_str());
    }
    val = val.substr(2, val.length());
    val = val.substr(0, val.find(" "));
  }

  // If not bit-vector, try parsing an int from the term.
  try
  {
    return std::stoi(val, nullptr, base);
  }
  catch (std::exception const & e)
  {
    std::string msg("Term ");
    msg += val;
    msg += " does not contain an integer representable by a machine int.";
    throw IncorrectUsageException(msg.c_str());
  }
}

TermIter Z3Term::begin()
{
  throw NotImplementedException("begin not implemented for Z3 backend.");
}

TermIter Z3Term::end()
{
  throw NotImplementedException("end not implemented for Z3 backend.");
}

std::string Z3Term::print_value_as(SortKind sk)
{
  if (!is_value())
  {
    throw IncorrectUsageException(
        "Cannot use print_value_as on a non-value term.");
  }
  return term.to_string();
}

// string Z3Term::const_to_string() const {
//	return term.to_string();
//}

// end Z3Term implementation

}  // namespace smt
