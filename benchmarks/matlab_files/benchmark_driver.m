% Generating test data

% Geometry files
x_walls_geom_input = '../output/domain_with_x_walls.txt';
y_walls_geom_input = '../output/domain_with_y_walls.txt';
step_geom_input = '../output/domain_with_step.txt';
cylinder_geom_input = '../output/domain_with_cylinder.txt';

array_no_walls_input = '../output/domain_with_array_no_walls.txt';
array_x_walls_input = '../output/domain_with_array_x_walls.txt';
array_y_walls_input = '../output/domain_with_array_y_walls.txt';

%
% Laminar flow in a 2D channel
%

Max_Iter = 30000;

% % Domains with walls
% rho_ini = 2.0;
% dPdL = 1e-4;
% force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'y_walls_results';
% single_phase_flow(y_walls_geom_input, rho_ini, force, Max_Iter, out_file);
% 
% % Flow in y-direction
% rho_ini = 1.0;
% dPdL = 1e-5;
% force = dPdL*(1/6)*[0, 1, 0, -1, 1, 1, -1, -1, 0]';
% out_file = 'x_walls_results';
% single_phase_flow(x_walls_geom_input, rho_ini, force, Max_Iter, out_file);

%
% Flow past objects
%

% Flow in x-direction
% rho_ini = 1.0;
% dPdL = 1e-4;
% force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'step_results';
% single_phase_flow(step_geom_input, rho_ini, force, Max_Iter, out_file);

% Flow in x-direction past a cylinder - low Re
rho_ini = 2.0;
dPdL = 1e-6;
force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
out_file = 'cylinder_slow_results';
single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);

% Flow in x-direction past a cylinder - medium Re
rho_ini = 2.0;
dPdL = 1e-5;
force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
out_file = 'cylinder_medium_results';
single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);

% Flow in x-direction past a cylinder - high Re
rho_ini = 2.0;
dPdL = 5e-5;
force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
out_file = 'von_karman_vortex_street';
single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);

% % Domains with arrays
% Max_Iter = 200;
% rho_ini = 1.5;
% out_file = 'uni_force_array_no_walls';
% single_phase_flow(array_no_walls_input, rho_ini, force, Max_Iter, out_file);
% 
% rho_ini = 1.5;
% force=-dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'uni_force_array_x_walls';
% single_phase_flow(array_x_walls_input, rho_ini, force, Max_Iter, out_file);
% 
% rho_ini = 5.25;
% force=-dPdL*(1/6)*[0, 1, 0, -1, 1, 1, -1, -1, 0]';
% out_file = 'uni_force_array_y_walls';
% single_phase_flow(array_y_walls_input, rho_ini, force, Max_Iter, out_file);

