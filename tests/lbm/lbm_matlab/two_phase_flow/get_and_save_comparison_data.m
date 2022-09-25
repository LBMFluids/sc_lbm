%% Extracts data from .mat files needed for the regression test
% Each dataset is treated individually
% Nomenclature: 1 - droplet, 2 - bulk

clear; close all

% Stationary droplet in a semi-periodic domain of bulk liquid
mat_name = 'Gs_0_dPdL_0_';
comp_name = '../../test_data/matlab_stationary_droplet_';
save_data(mat_name, comp_name);

function save_data(mat_name, comp_name)
    % Files to process
    file_list = dir([mat_name, '*.mat']);
    file_names = {file_list.name};
    ndirs = length(file_names);
    
    for ii = 1:ndirs
        % Load the dataset
        fprintf('Processing: %s\n', file_names{ii})
        temp = load(file_names{ii});
        load(file_names{ii});
        % Compute macroscopic quantities
        [rho_1, rho_2, ux_1, ux_2, uy_1, uy_2] = ...
                  macroscopic_properties(temp.f_1, temp.f_2, temp.Cur_Iter,...
                  temp.N_c, temp.Nr, temp.Mc, temp.lena, temp.ija);
        % Save macroscopic quantities
        % First the droplet (fluid 1)
        temp_name = strcat(comp_name, 'droplet_step_', num2str(temp.Cur_Iter), '_');
        save(strcat(temp_name, 'density.txt'), "rho_1", '-ascii');
        save(strcat(temp_name, 'ux.txt'), "ux_1", '-ascii');
        save(strcat(temp_name, 'uy.txt'), "uy_1", '-ascii');
        save(strcat(temp_name, 'ucx.txt'), "ucx_1", '-ascii');
        save(strcat(temp_name, 'ucy.txt'), "ucy_1", '-ascii');
        save(strcat(temp_name, 'Fx.txt'), "Fx_1", '-ascii');
        save(strcat(temp_name, 'Fy.txt'), "Fy_1", '-ascii');
        % Then the bulk (fluid 2)
        temp_name = strcat(comp_name, 'bulk_step_', num2str(temp.Cur_Iter), '_');
        save(strcat(temp_name, 'density.txt'), "rho_2", '-ascii');
        save(strcat(temp_name, 'ux.txt'), "ux_2", '-ascii');
        save(strcat(temp_name, 'uy.txt'), "uy_2", '-ascii');
        save(strcat(temp_name, 'ucx.txt'), "ucx_2", '-ascii');
        save(strcat(temp_name, 'ucy.txt'), "ucy_2", '-ascii');
        save(strcat(temp_name, 'Fx.txt'), "Fx_2", '-ascii');
        save(strcat(temp_name, 'Fy.txt'), "Fy_2", '-ascii');
        % Save density distributions
        % Direction 9 is actual direction 0 (center, stationary) in the C++
        % code
        % First the droplet (fluid 1)
        temp_name = strcat(comp_name, 'droplet_f_step_', num2str(temp.Cur_Iter), '_');
        temp_name_eq = strcat(comp_name, 'droplet_f_eq_step_', num2str(temp.Cur_Iter), '_');
        for ic = 1:temp.N_c-1
            temp_f = temp.f_1(:,:,ic);
            save(strcat(temp_name, num2str(ic), '.txt'), "temp_f", '-ascii');
            temp_feq = temp.feq_1(:,:,ic);
            save(strcat(temp_name_eq, num2str(ic), '.txt'), "temp_feq", '-ascii');
        end
        temp_f = temp.f_1(:,:,9);
        save(strcat(temp_name, '0.txt'), "temp_f", '-ascii');
        temp_feq = temp.feq_1(:,:,9);
        save(strcat(temp_name_eq, '0.txt'), "temp_feq", '-ascii');
        % Then the bulk (fluid 2)
        temp_name = strcat(comp_name, 'bulk_f_step_', num2str(temp.Cur_Iter), '_');
        temp_name_eq = strcat(comp_name, 'bulk_f_eq_step_', num2str(temp.Cur_Iter), '_');
        for ic = 1:temp.N_c-1
            temp_f = temp.f_2(:,:,ic);
            save(strcat(temp_name, num2str(ic), '.txt'), "temp_f", '-ascii');
            temp_feq = temp.feq_2(:,:,ic);
            save(strcat(temp_name_eq, num2str(ic), '.txt'), "temp_feq", '-ascii');
        end
        temp_f = temp.f_2(:,:,9);
        save(strcat(temp_name, '0.txt'), "temp_f", '-ascii');
        temp_feq = temp.feq_2(:,:,9);
        save(strcat(temp_name_eq, '0.txt'), "temp_feq", '-ascii');
    end
end