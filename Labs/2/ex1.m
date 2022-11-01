syms n

# number of processors
p = [1:32];

# number of operations
O1 = n^3;
On = n^3 + n^2 * log2(n);

# time
T1 = n^3;
Tn = 4*n^3/(n+3);

# speedup
s = T1/Tn;
S = function_handle(s);

# efficiency
ef = s/n;
E = function_handle(ef);

# redundancy
r = On/O1;
R = function_handle(r);

# utilization
u = r * ef;
U = function_handle(u);

# quality factor
q = s * ef / r;
Q = function_handle(q);

# plot parameters as functions of the number of processors
figure;
plot(p, S(p));
hold on;
plot(p, E(p));
hold on;
plot(p, R(p));
hold on;
plot(p, U(p));
hold on;
plot(p, Q(p));
legend('S', 'E', 'R', 'U', 'Q');