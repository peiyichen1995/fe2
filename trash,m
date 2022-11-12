%% FEM in 1D (P2 elements): linear elasticity with constant body force, EBC and NBC
% Strong form is
% Eu" + b = 0, with u(0) = q and Eu'(L) = p
clc
close all
clear
%% Physical inputs
L = 1;                              % Length of the bar
b = 10;                             % Body source (constant)
E = 2;                              % Constant Young's modulus across elements
p = 1;                              % NBC
q = 0;                              % EBC
%% Theoretical solution
syms y(x)
Dy = diff(y);
ode = E*diff(y, x, 2) == - b;
cond1 = y(0) == q;
cond2 = Dy(L) == p/E;
conds = [cond1 cond2];
ySol(x) = dsolve(ode, conds);
ySol = simplify(ySol);
%% FEM inputs
nEl = 3;                           % Number of elements in mesh
nNo = 2*nEl + 1;                      % Number of nodes in mesh
A = 1;                              % Constant area for all elements
nodalSourceInfo = [nNo, p];         % Point sources in the form:
                                    % [nodeNumber1 sourceValue1
                                    %  nodeNumber2 sourceValue2
                                    %  ...         ...
                                    %  nodeNumberN sourceValueN]
EBC = [1  q];                       % EBC in the form:
                                    % [nodeNumber1 dofValue1
                                    %  nodeNumber2 dofValue2
                                    %  ...         ...
                                    %  nodeNumberN dofValueN]
%% Mesh in 1D                                    
coord = linspace(0, L, nNo)';       % Vector of node coordinates
elCon = [(1:2:nNo-2)', (2:2:nNo-1)', (3:2:nNo)']; % Element connectivity
%% Assembly for the stiffness matrix and force vector
tic();
K = zeros(nNo);
F = zeros(nNo, 1);
for i = 1:nEl
    % Get the stiffness for the element
    Ke = getElementStiffnessMatrix(i, coord, elCon, A, E);
    % Find the entries for the nodes
    globalNodes = elCon(i, :);
    % Add the entries at the right positions
    K(globalNodes, globalNodes) = K(globalNodes, globalNodes) + Ke(:, :);
    % Get the force vector for the element
    fe = getElementForceVector(i, coord, elCon, b);
    % Add the entries at the right positions
    F(globalNodes) = F(globalNodes) + fe(:);
end
% Add NBC
F(nodalSourceInfo(:, 1)) = F(nodalSourceInfo(:, 1)) + nodalSourceInfo(:, 2);
% Convert system matrices into sparse matrices
%Ko = K;
%Fo = F;
K = sparse(K);                                    
F = sparse(F);                                    
%% Applying the EBC                                    
Kmod = K;
Fmod = F;
numEBC = size(EBC,1);
for i = 1:numEBC
  % Node where EBC is applied
  node = EBC(i, 1);
  % Value of the EBC
  value = EBC(i, 2);
  % Modify the right-hand side first
  Fmod = Fmod - Kmod(:, node)*value;
  Fmod(node) = value;
  % Modify the left-hand side then (stiffness)
  Kmod(node, :) = 0; %#ok<*SPRIX>
  Kmod(:, node) = 0;
  Kmod(node, node) = 1; 
end
%% Solving for u
u = Kmod\Fmod;
%Kmod = full(Kmod);
cpuTime = toc();
%% Plot
figure;
fplot(ySol, [0, L], 'LineWidth', 1);
hold on
plot(coord, u, 'LineWidth', 1);
% plot(coord, u, 'or');
legend('Analytical Solution', 'FEM Solution', 'Location', 'Best');
hold off
grid on
%% Additional functions for element stiffness and force 
function Ke = getElementStiffnessMatrix(elID, coord, elCon, A, E)
node1 = elCon(elID, 1);
%node2 = elCon(elID, 2);
node3 = elCon(elID, 3);
x1 = coord(node1);
%x2 = coord(node2);
x3 = coord(node3);
L = x3 - x1;
Ke = A*E/L/3*[7, 1, -8; 1, 7, -8; -8, -8, 16];
end
%
function fe = getElementForceVector(elID, coord, elCon, value)
node1 = elCon(elID, 1);
%node2 = elCon(elID, 2);
node3 = elCon(elID, 3);
x1 = coord(node1);
%x2 = coord(node2);
x3 = coord(node3);
L = x3 - x1;
fe = value*L*[1/6;2/3;1/6];
end