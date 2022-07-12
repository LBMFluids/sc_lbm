% Generating test data

% Geometry files
x_walls_geom_input = '../output/domain_with_x_walls.txt';
y_walls_geom_input = '../output/domain_with_y_walls.txt';
step_geom_input = '../output/domain_with_step.txt';
cylinder_geom_input = '../output/domain_with_cylinder.txt';
array_x_walls_input = '../output/array_of_ellipses_x_walls.txt';
sarray_x_walls_input = '../output/staggered_array_of_circles_x_walls.txt';
array_y_walls_input = '../output/array_of_squares_y_walls.txt';
sarray_y_walls_input = '../output/staggered_array_of_rectangles_y_walls.txt';

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
% rho_ini = 2.0;
% dPdL = 1e-6;
% force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'cylinder_slow_results';
% single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);
% 
% % Flow in x-direction past a cylinder - medium Re
% rho_ini = 2.0;
% dPdL = 1e-5;
% force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'cylinder_medium_results';
% single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);
% 
% % Flow in x-direction past a cylinder - high Re
% rho_ini = 2.0;
% dPdL = 5e-5;
% force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
% out_file = 'von_karman_vortex_street';
% single_phase_flow(cylinder_geom_input, rho_ini, force, Max_Iter, out_file);

%
% Flow past arrays of objects
%

% Flow in x direction through an array of ellipses
rho_ini = 1.5;
dPdL = 1e-5;
force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
out_file = 'array_of_ellipses_x_walls_results';
single_phase_flow(array_x_walls_input, rho_ini, force, Max_Iter, out_file);

% Flow in x direction through a staggered array of circles
rho_ini = 2.5;
dPdL = 1e-5;
force = dPdL*(1/6)*[1, 0, -1, 0, 1, -1, -1, 1, 0]';
out_file = 'staggered_array_of_circles_x_walls_results';
single_phase_flow(sarray_x_walls_input, rho_ini, force, Max_Iter, out_file);

% Flow in y direction through an array of squares
rho_ini = 1.35;
dPdL = 2.5e-5;
force = dPdL*(1/6)*[0, 1, 0, -1, 1, 1, -1, -1, 0]';
out_file = 'array_of_squares_y_walls_results';
single_phase_flow(array_y_walls_input, rho_ini, force, Max_Iter, out_file);

% Flow in y direction through a staggered array of rectangles
rho_ini = 1.35;
dPdL = 2.5e-5;
force = dPdL*(1/6)*[0, 1, 0, -1, 1, 1, -1, -1, 0]';
out_file = 'staggered_array_of_rectangles_y_walls_results';
single_phase_flow(sarray_y_walls_input, rho_ini, force, Max_Iter, out_file);

