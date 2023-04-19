// subsize specimen
G = 25;
W = 6;
R = 6;
L = 100;
A = 32;
B = 30;
C = 10;

lc = 1;
Point(1) = {0, 0, 0, lc};
Point(2) = {B, 0, 0, lc};
Point(3) = {L/2-A/2, C/2-W/2, 0, lc};
Point(4) = {32+Sqrt(31/5), 1-2*Sqrt(31/5), 0, lc};
Point(5) = {L/2, C/2-W/2, 0, lc};
Point(6) = {L/2, C/2, 0, lc};
Point(7) = {0, C/2, 0, lc};


Line(1) = {1, 2};
Circle(2) = {2, 4, 3};
Line(3) = {3, 5};
Line(4) = {5, 6};
Line(5) = {6, 7};
Line(6) = {7, 1};

Line Loop(1) = {1, 2, 3, 4, 5, 6};

Plane Surface(1) = {1};

Physical Surface("all") = {1};
Physical Line("top") = {5};
Physical Line("left") = {6};
Physical Line("right") = {4};