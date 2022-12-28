// MOOSE includes
#include "AverageRankFourTensor.h"

#include "libmesh/quadrature.h"

registerMooseObject("fe2App", AverageRankFourTensor);

InputParameters
AverageRankFourTensor::validParams()
{
  InputParameters params = ElementUserObject::validParams();
  params.addClassDescription("Performs a spatial averaging of a rank four tensor");
  params.addParam<MaterialPropertyName>("tensor", "name of the rank four tensor");
  return params;
}

AverageRankFourTensor::AverageRankFourTensor(const InputParameters & parameters)
  : ElementUserObject(parameters), _qp(0), _tensor(getMaterialProperty<RankFourTensor>("tensor"))
{
}

void
AverageRankFourTensor::initialize()
{
  _integral_value.zero();
}

void
AverageRankFourTensor::execute()
{
  _integral_value += computeIntegral();
}

RankFourTensor
AverageRankFourTensor::getValue()
{
  std::vector<Real> vals(RankFourTensor::N4);
  for (auto i : make_range(RankFourTensor::N))
    for (auto j : make_range(RankFourTensor::N))
      for (auto k : make_range(RankFourTensor::N))
        for (auto l : make_range(RankFourTensor::N))
          vals[i * RankFourTensor::N3 + j * RankFourTensor::N2 + k * RankFourTensor::N + l] =
              _tensor[_qp](i, j, k, l);

  gatherSum(vals);

  RankFourTensor sum;
  sum.fillFromInputVector(vals, RankFourTensor::general);
  return sum;
}

void
AverageRankFourTensor::threadJoin(const UserObject & y)
{
  const AverageRankFourTensor & pps = static_cast<const AverageRankFourTensor &>(y);
  _integral_value += pps._integral_value;
}

RankFourTensor
AverageRankFourTensor::computeIntegral()
{
  RankFourTensor sum;

  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    sum += _JxW[_qp] * _coord[_qp] * _tensor[_qp];
  return sum;
}
