//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "fe2TestApp.h"
#include "fe2App.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
fe2TestApp::validParams()
{
  InputParameters params = fe2App::validParams();
  return params;
}

fe2TestApp::fe2TestApp(InputParameters parameters) : MooseApp(parameters)
{
  fe2TestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

fe2TestApp::~fe2TestApp() {}

void
fe2TestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  fe2App::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"fe2TestApp"});
    Registry::registerActionsTo(af, {"fe2TestApp"});
  }
}

void
fe2TestApp::registerApps()
{
  registerApp(fe2App);
  registerApp(fe2TestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
fe2TestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  fe2TestApp::registerAll(f, af, s);
}
extern "C" void
fe2TestApp__registerApps()
{
  fe2TestApp::registerApps();
}
