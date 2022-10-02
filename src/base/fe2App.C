#include "fe2App.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
fe2App::validParams()
{
  InputParameters params = MooseApp::validParams();

  return params;
}

fe2App::fe2App(InputParameters parameters) : MooseApp(parameters)
{
  fe2App::registerAll(_factory, _action_factory, _syntax);
}

fe2App::~fe2App() {}

void
fe2App::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"fe2App"});
  Registry::registerActionsTo(af, {"fe2App"});

  /* register custom execute flags, action syntax, etc. here */
}

void
fe2App::registerApps()
{
  registerApp(fe2App);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
fe2App__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  fe2App::registerAll(f, af, s);
}
extern "C" void
fe2App__registerApps()
{
  fe2App::registerApps();
}
