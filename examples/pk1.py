import torch

torch.set_default_dtype(torch.float64)

def PK1(F):
    E = 0.5 * (torch.matmul(F.T,F)-torch.eye(3))
    lambda_ = 4000
    mu_ = 6700
    PK2 = lambda_ * torch.trace(E) * torch.eye(3) + 2 * mu_ * E
    return torch.matmul(F, PK2)

traced = torch.jit.trace(PK1, torch.zeros(3, 3))

traced.save("PK1.pt")