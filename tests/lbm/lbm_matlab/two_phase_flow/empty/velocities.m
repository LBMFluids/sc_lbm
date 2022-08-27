% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
% Program for computing macroscopic densities and velocities
% ---------------------------------------------------------------------
% Last modified: August 17 2014
% ---------------------------------------------------------------------
% Input: loaded .mat file from LBM simulations before running
% ---------------------------------------------------------------------
% Output: macroscopic densities and velocities for two immiscible LBM
% species
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

% Lattice directions
C_x=[1 0 -1 0 1 -1 -1 1 0];
C_y=[0 1 0 -1 1 1 -1 -1 0];
% Density computation
rho_1=sum(f_1,3);
rho_2=sum(f_2,3);
% Velocity computation
if Cur_Iter>0
    ux_1=zeros(Nr,Mc);uy_1=zeros(Nr,Mc); ux_2=zeros(Nr,Mc);uy_2=zeros(Nr,Mc);
    for ic=1:N_c-1
        ux_1=ux_1+C_x(ic).*f_1(:,:,ic);
        uy_1=uy_1+C_y(ic).*f_1(:,:,ic);
        ux_2=ux_2+C_x(ic).*f_2(:,:,ic);
        uy_2=uy_2+C_y(ic).*f_2(:,:,ic);
    end
end
% Avoid NaNs on solid nodes (can be done with isnan and indexing)
for nzr=1:lena
    if rho_1(ija(nzr))>0
        ux_1(ija(nzr))=ux_1(ija(nzr))/rho_1(ija(nzr));
        uy_1(ija(nzr))=uy_1(ija(nzr))/rho_1(ija(nzr));
    end
    if rho_2(ija(nzr))>0
        ux_2(ija(nzr))=ux_2(ija(nzr))/rho_2(ija(nzr));
        uy_2(ija(nzr))=uy_2(ija(nzr))/rho_2(ija(nzr));
    end
end