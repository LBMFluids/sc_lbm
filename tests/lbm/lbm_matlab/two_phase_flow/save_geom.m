% Save geometry for the C++ part

clear; close all;

g_file_name = '../../test_data/droplet_tests_final_geometry.txt';

temp = load('pillars_1.mat');
geom = temp.Channel2D;
save(g_file_name, 'geom', '-ascii');
