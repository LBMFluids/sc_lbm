%% Extracts data from .mat files needed for the regression test
% Each dataset is treated individually

clear; close all

% Stationary droplet in a semi-periodic domain of bulk liquid
mat_name = 'developed_droplet_';
cpp_name = 'stationary_droplet_';

% Files to consider
file_list = dir([mat_name, '*.mat']);
file_names = {file_list.name};
ndirs = length(file_names);

for ii = 1:ndirs
    % Load the dataset
    fprintf('Processing: %s\n', file_names{ii})
    temp = load(file_names{ii});
    % Compute macroscopic quantities
end