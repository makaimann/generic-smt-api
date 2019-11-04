#include <sstream>

#include "boolector_sort.h"

namespace smt {

/* BoolectorSolver implementation */

BoolectorSortBase::~BoolectorSortBase() { boolector_release_sort(btor, sort); }

std::size_t BoolectorSortBase::hash() const
{
  // TODO: come up with better hash function
  std::size_t hash = sk;

  if(sk == BV)
  {
    hash ^= get_width();
  }
  else if(sk == ARRAY)
  {
    hash ^= get_indexsort()->hash();
    hash ^= get_elemsort()->hash();
  }
  return hash;
}


// by default the following get_* methods don't work
// overloaded in derived classes
uint64_t BoolectorSortBase::get_width() const
{
  throw IncorrectUsageException("Only defined for a bit-vector sort.");
};

Sort BoolectorSortBase::get_indexsort() const
{
  throw IncorrectUsageException("Only defined for an array sort.");
};

Sort BoolectorSortBase::get_elemsort() const
{
  throw IncorrectUsageException("Only defined for an array sort.");
};

SortVec BoolectorSortBase::get_domain_sorts() const
{
  throw IncorrectUsageException("Only defined for a function sort.");
};

Sort BoolectorSortBase::get_codomain_sort() const
{
  throw IncorrectUsageException("Only defined for a function sort.");
};

bool BoolectorSortBase::compare(const Sort s) const
{
  std::shared_ptr<BoolectorSortBase> bs =
      std::static_pointer_cast<BoolectorSortBase>(s);
  if (sk != bs->get_sort_kind())
  {
    // Note: bool and bv will still be equal for boolector, because always
    // create BV sort even if it's a bool
    return false;
  }

  switch (sk)
  {
    case ARRAY:
    {
      return (get_indexsort() == bs->get_indexsort())
             && (get_elemsort() == bs->get_elemsort());
      break;
    }
    case BOOL:
    case BV:
    {
      return get_width() == bs->get_width();
      break;
    }
    case FUNCTION:
    {
      SortVec domain_sorts = get_domain_sorts();
      SortVec bs_domain_sorts = bs->get_domain_sorts();

      if (domain_sorts.size() != bs_domain_sorts.size())
      {
        return false;
      }
      else if (get_codomain_sort() != bs->get_codomain_sort())
      {
        return false;
      }

      bool res = true;

      for (unsigned i = 0; i < domain_sorts.size(); ++i)
      {
        res &= (domain_sorts[i] == bs_domain_sorts[i]);
      }

      return res;
      break;
    }
    default:
    {
      Unreachable();
      break;
    }
  }
  return false;
}
/* end BoolectorSolver implementation */

}  // namespace smt
