%% Visual verification of distributions used in C++

% Gamma
disp('------- Gamma -------')
disp('C++:')

cpp_data = load('test_data/gamma_values.txt');
cpp_pd = fitdist(cpp_data, 'Gamma')
mean(cpp_pd)
std(cpp_pd)

disp('Orig:')
pd= makedist('Gamma', 'a', 0.7696, 'b', 3.4192)
mean(pd)
std(pd)

x = [0:0.01:30];
subplot(3,1,1), plot(x, pdf(pd, x), 'b', x(1:100:end), pdf(cpp_pd, x(1:100:end)), 'ks')

% Lognormal
disp('------- Log-normal -------')
disp('C++:')

cpp_data = load('test_data/lognormal_values.txt');
cpp_pd = fitdist(cpp_data, 'Lognormal')
mean(cpp_pd)
std(cpp_pd)

disp('Orig:')
pd= makedist('Lognormal', 'mu', 2.6696, 'sigma', 0.4760)
mean(pd)
std(pd)

x = [0:0.01:40];
subplot(3,1,2), plot(x, pdf(pd, x), 'b', x(1:100:end), pdf(cpp_pd, x(1:100:end)), 'ks')

% Weibull
disp('------- Weibull -------')
disp('C++:')

cpp_data = load('test_data/weibull_values.txt');
cpp_pd = fitdist(cpp_data, 'Weibull')
mean(cpp_pd)
std(cpp_pd)

disp('Orig:')
pd= makedist('Weibull', 'a', 10.1237, 'b', 1.6726)
mean(pd)
std(pd)

x = [0:0.01:30];
subplot(3,1,3), plot(x, pdf(pd, x), 'b', x(1:100:end), pdf(cpp_pd, x(1:100:end)), 'ks')