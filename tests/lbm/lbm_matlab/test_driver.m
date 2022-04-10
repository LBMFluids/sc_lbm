% Generating test data

% Empty domain
omega = 3.0;
just_fluid_test('../test_data/empty_domain.txt', '../test_data/empty_f_dist', omega, '../test_data/empty_res.txt');

% Domain with walls
omega = 3.0;
just_fluid_test('../test_data/domain_with_walls.txt', '../test_data/initial_walls_f_dist', omega, '../test_data/walls_res.txt');