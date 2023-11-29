import torch
from torch.func import jacrev

dtype = torch.float64


def PK1(F):
    E = 0.5 * (torch.matmul(F.T, F) - torch.eye(3, dtype=dtype))
    lambda_ = 4000
    mu_ = 6700
    PK2 = lambda_ * torch.trace(E) * torch.eye(3, dtype=dtype) + 2 * mu_ * E
    return torch.matmul(F, PK2)


def dPK1(F):
    return jacrev(PK1)(F)


PK1_traced = torch.jit.trace(PK1, torch.zeros(3, 3, dtype=dtype))
PK1_traced.save("PK1.pt")

dPK1_traced = torch.jit.trace(dPK1, torch.zeros(3, 3, dtype=dtype))
dPK1_traced.save("dPK1.pt")
