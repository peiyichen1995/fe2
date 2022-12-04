//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "QuadraturePointMultiApp.h"
#include "MooseMesh.h"
#include "FEProblem.h"
#include "AddVariableAction.h"

// libMesh includes
#include "libmesh/parallel_algebra.h"
#include "libmesh/string_to_enum.h"

registerMooseObject("MooseApp", QuadraturePointMultiApp);

InputParameters
QuadraturePointMultiApp::validParams()
{
  InputParameters params = FullSolveMultiApp::validParams();
  params += BlockRestrictable::validParams();
  params.addClassDescription("Automatically generates Sub-App positions from quadrature points of "
                             "elements in the parent app "
                             " mesh.");
  MooseEnum order(
      "CONSTANT FIRST SECOND THIRD FOURTH FIFTH SIXTH SEVENTH EIGHTH NINTH TENTH ELEVENTH TWELFTH "
      "THIRTEENTH FOURTEENTH FIFTEENTH SIXTEENTH SEVENTEENTH EIGHTTEENTH NINETEENTH TWENTIETH "
      "TWENTYFIRST TWENTYSECOND TWENTYTHIRD TWENTYFOURTH TWENTYFIFTH TWENTYSIXTH TWENTYSEVENTH "
      "TWENTYEIGHTH TWENTYNINTH THIRTIETH THIRTYFIRST THIRTYSECOND THIRTYTHIRD THIRTYFOURTH "
      "THIRTYFIFTH THIRTYSIXTH THIRTYSEVENTH THIRTYEIGHTH THIRTYNINTH FORTIETH FORTYFIRST "
      "FORTYSECOND FORTYTHIRD",
      "FIRST",
      true);
  params.addParam<MooseEnum>("order",
                             order,
                             "Order of the FE shape function to use for this variable (additional "
                             "orders not listed here are allowed, depending on the family).");

  MooseEnum family{AddVariableAction::getNonlinearVariableFamilies()};

  params.addParam<MooseEnum>(
      "family", family, "Specifies the family of FE shape functions to use for this variable.");

  MooseEnum qudrature_types("CLOUGH CONICAL GAUSS GRID MONOMIAL SIMPSON TRAP GAUSS_LOBATTO",
                            "GAUSS");
  MooseEnum qudrature_order(
      "AUTO CONSTANT FIRST SECOND THIRD FOURTH FIFTH SIXTH SEVENTH EIGHTH NINTH TENTH "
      "ELEVENTH TWELFTH THIRTEENTH FOURTEENTH FIFTEENTH SIXTEENTH SEVENTEENTH "
      "EIGHTTEENTH NINTEENTH TWENTIETH",
      "AUTO");

  params.addParam<MooseEnum>("qudrature_type", qudrature_types, "Type of the quadrature rule");
  params.addParam<MooseEnum>("qudrature_order", qudrature_order, "Order of the quadrature");

  params.suppressParameter<std::vector<Point>>("positions");
  params.suppressParameter<std::vector<FileName>>("positions_file");
  return params;
}

QuadraturePointMultiApp::QuadraturePointMultiApp(const InputParameters & parameters)
  : FullSolveMultiApp(parameters), BlockRestrictable(this)
{
}

void
QuadraturePointMultiApp::fillPositions()
{
  //   MooseMesh & parent_app_mesh = _fe_problem.mesh();

  //   for (const auto & elem_ptr : parent_app_mesh.getMesh().active_local_element_ptr_range())
  //     if (hasBlocks(elem_ptr->subdomain_id()))
  //       _positions.push_back(elem_ptr->vertex_average());

  MooseMesh & master_mesh = _fe_problem.mesh();
  auto & mesh = master_mesh.getMesh();

  //  define quarature rule
  FEType fe_type(Utility::string_to_enum<Order>(getParam<MooseEnum>("order")),
                 Utility::string_to_enum<FEFamily>(getParam<MooseEnum>("family")));
  std::unique_ptr<FEBase> fe(FEBase::build(master_mesh.dimension(), fe_type));

  QuadratureType qudrature_type =
      Moose::stringToEnum<QuadratureType>(getParam<MooseEnum>("qudrature_type"));

  Order qudrature_order = Moose::stringToEnum<Order>(getParam<MooseEnum>("qudrature_order"));

  const std::vector<Point> & q_points = fe->get_xyz();
  auto qrule = QBase::build(qudrature_type, master_mesh.dimension(), qudrature_order);
  fe->attach_quadrature_rule(qrule.get());

  for (auto & elem : mesh.active_local_element_ptr_range())
  {
    fe->reinit(elem);
    for (size_t i = 0; i < q_points.size(); ++i)
    {
      _positions.push_back(q_points[i]);
    }
  }

  // Use the comm from the problem this MultiApp is part of
  libMesh::ParallelObject::comm().allgather(_positions);

  for (size_t i = 0; i < _positions.size(); i++)
  {
    std::cout << "Point " << i << ": " << _positions[i] << std::endl;
  }

  if (_positions.empty())
    mooseError("No positions found for QuadraturePointMultiApp ", _name);

  // An attempt to try to make this parallel stable
  std::sort(_positions.begin(), _positions.end());
}
