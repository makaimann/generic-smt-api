#include "generic_datatype.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <unordered_map>

#include "assert.h"

using namespace std;

namespace smt {

GenericDatatypeDecl::GenericDatatypeDecl(const std::string name)
    : dt_name(name), param_count(0)
{
}

std::string GenericDatatypeDecl::get_name() const { return dt_name; }

int GenericDatatypeDecl::get_param_count() const { return param_count; }

std::vector<Sort> GenericDatatypeDecl::get_param_sorts() { return param_sorts; }

void GenericDatatypeDecl::register_param_sort(std::string param_name)
{
  for (unsigned int i = 0; i < param_count; ++i)
  {
    // Checks if the selector has already been added
    if (param_sorts[i]->to_string() == param_name)
    {
      // throw "Can't add selector. It already exists in this
      // datatype!";
      return;
    }
  }
  Sort new_param = make_generic_param_sort(param_name);
  param_sorts.push_back(new_param);
  param_count += 1;
}

GenericDatatypeConstructorDecl::GenericDatatypeConstructorDecl(
    const std::string & name)
    : cons_name(name), contains_param(false)
{
}

void GenericDatatypeConstructorDecl::add_new_selector(
    const SelectorComponents & newSelector)
{
  for (unsigned int i = 0; i < selector_vector.size(); ++i)
  {
    // Checks if the selector has already been added
    if (selector_vector[i].name == (newSelector).name)
    {
      throw "Can't add selector. It already exists in this datatype!";
    }
  }
  selector_vector.push_back(newSelector);
}

std::vector<SelectorComponents>
GenericDatatypeConstructorDecl::get_selector_vector()
{
  return selector_vector;
}

std::string GenericDatatypeConstructorDecl::get_name() const
{
  return cons_name;
}

int GenericDatatypeConstructorDecl::get_selector_count() const
{
  return selector_vector.size();
}
bool GenericDatatypeConstructorDecl::compare(
    const DatatypeConstructorDecl & d) const
{
  // Compares based off constructor's name
  return cons_name
         == static_pointer_cast<GenericDatatypeConstructorDecl>(d)->get_name();
}

std::string GenericDatatypeConstructorDecl::get_dt_name() const
{
  return static_pointer_cast<GenericDatatypeDecl>(dt_decl)->get_name();
}

void GenericDatatypeConstructorDecl::update_stored_dt(
    const DatatypeDecl & datatype_decl)
{
  dt_decl = datatype_decl;
}

GenericDatatype::GenericDatatype(const DatatypeDecl & dt_declaration)
    : dt_decl(dt_declaration)
{
}

void GenericDatatype::add_constructor(
    const DatatypeConstructorDecl & dt_cons_decl)
{
  // Checks if dt_cons_decl is already associated with the datatype
  if (std::find(cons_decl_vector.begin(), cons_decl_vector.end(), dt_cons_decl)
      != cons_decl_vector.end())
  {
    throw "Can't add constructor. It already has been added!";
  }
  shared_ptr<GenericDatatypeConstructorDecl> gdt_cons =
      static_pointer_cast<GenericDatatypeConstructorDecl>(dt_cons_decl);

  if (gdt_cons->contains_param == true)
  {
    for (int i = 0; i < gdt_cons->get_selector_count(); ++i)
    {
      if (gdt_cons->get_selector_vector()[i].sort->get_sort_kind()
          == SortKind::PARAM)
      {
        static_pointer_cast<GenericDatatypeDecl>(dt_decl)->register_param_sort(
            static_pointer_cast<ParamSort>(
                gdt_cons->get_selector_vector()[i].sort)
                ->to_string());
      }
    }
  }
  // Links the constructor to the datatype_decl of the datatype
  gdt_cons->update_stored_dt(dt_decl);
  // Links the datatype to the new constructor
  cons_decl_vector.push_back(dt_cons_decl);
}

void GenericDatatype::add_selector(const DatatypeConstructorDecl & dt_cons_decl,
                                   const SelectorComponents & newSelector)
{
  // Boolean used to keep track of if a successful match was found.
  bool success = false;
  for (unsigned int i = 0; i < cons_decl_vector.size(); ++i)
  {
    // If the constructor is associated with the datatype
    if (cons_decl_vector[i] == dt_cons_decl)
    {
      // Adds the selector to the correct constructor
      static_pointer_cast<GenericDatatypeConstructorDecl>(cons_decl_vector[i])
          ->add_new_selector(newSelector);
      success = true;
      break;
    }
  }
  if (!success)
  {
    throw InternalSolverException(
        "Can't add selector. The constructor is not a member of the datatype!");
  }
}
std::vector<DatatypeConstructorDecl> GenericDatatype::get_cons_vector()
{
  return cons_decl_vector;
}

std::string GenericDatatype::get_name() const
{
  return static_pointer_cast<GenericDatatypeDecl>(dt_decl)->get_name();
}

int GenericDatatype::get_num_constructors() const
{
  return cons_decl_vector.size();
}

int GenericDatatype::get_num_selectors(std::string cons) const
{
  // Used to keep track of the number of selectors in the constructor
  int num_selectors = 0;
  bool found = false;
  for (unsigned int i = 0; i < cons_decl_vector.size(); ++i)
  // Searches for a matching constructor
  {
    if (static_pointer_cast<GenericDatatypeConstructorDecl>(cons_decl_vector[i])
            ->get_name()
        == cons)
    {
      found = true;
      // Calls the constructor's get_selector_count() function
      num_selectors = static_pointer_cast<GenericDatatypeConstructorDecl>(
                          cons_decl_vector[i])
                          ->get_selector_count();
      break;
    }
  }
  if (!found)
  {
    throw InternalSolverException("Constructor not found");
  }
  return num_selectors;
}

/*
This function goes through every selector in the datatype and if
finalized is set to false, it replaces the previously stored sort
with new_sort
 */
void GenericDatatype::change_sort_of_selector(const Sort new_sort)
{
  // For every constructor
  for (unsigned int i = 0; i < cons_decl_vector.size(); ++i)
  {
    std::shared_ptr<GenericDatatypeConstructorDecl> cons_cast =
        static_pointer_cast<GenericDatatypeConstructorDecl>(
            cons_decl_vector[i]);
    // For every selector
    for (unsigned int f = 0; f < get_num_selectors(cons_cast->get_name()); ++f)
    {
      if (cons_cast->selector_vector[f].finalized == false)
      {
        // Updates the selector's members
        cons_cast->selector_vector[f].sort = new_sort;
        cons_cast->selector_vector[f].finalized = true;
      }
    }
  }
}
}  // namespace smt
