syms n

# number of processors
p = [1:32];

# number of operations
O1 = n;
On = n;

# time
T1 = n;
Tn = log2(n);

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
legend('S','E', 'R', 'U', 'Q');