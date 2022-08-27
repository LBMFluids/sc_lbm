function [f_1,f_2,BUp_1, BUp_2, BLW_1, BLW_2]=all_ops_v2(partition,G,omega,force,Cur_Iter)
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
    % Function for performing all local MCMP SC LBM operations before the
    % streaming portion of the algorithm
    % ---------------------------------------------------------------------
    % Last modified: December 8th 2014
    % ---------------------------------------------------------------------
	% Input: partion (structure), omega (inverse relaxation time), 
	% force (bulk force, gravity or pressure drop), 
    % Cur_Iter for velocity computation
    % ---------------------------------------------------------------------
    % Output: density distributions for each of the two components, upper 
    % and lower density buffers (extra rows) to be send to corresponding 
    % neighboring partitions
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
    
    % --- RETRIEVE/PROCESS DATA FROM PARTITIONS
    [Nr Mc N_c]=size(partition.f_1);
    Nc=N_c;
    f_1=partition.f_1;
    f_2=partition.f_2;
    Fxs_1=partition.Fxs_1;
    Fxs_2=partition.Fxs_2;
    Fys_1=partition.Fys_1;
    Fys_2=partition.Fys_2;
    omega_1=omega; omega_2=omega;

    % Indexing
    [iabw1 jabw1]=find(f_1(:,:,9)~=0);
    lena=length(iabw1);
    % Linear index
    ija=(jabw1-1)*Nr+iabw1; 
    
  	% Density
    rho_1=sum(f_1,3);
    rho_2=sum(f_2,3);

	% Buffers
    BUp_1=rho_1(Nr-1,:);
    BUp_2=rho_2(Nr-1,:);

    BLW_1=rho_1(2,:);
    BLW_2=rho_2(2,:);
    
    % --- LBM CONSTANTS
    % Direction vectors
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    % Density weights
    w0=16/36.;w1=4/36.;w2=1/36.;
    W=[w1 w1 w1 w1 w2 w2 w2 w2 w0];
    cs2=1/3;cs2x2=2*cs2;cs4x2=2*cs2.^2;
    f1=3.; f2=4.5; f3=1.5;
    NxM=Nr*Mc;
   
    % --- IMMISCIBLE FLUIDS INTERACTIOS
    psi_1=rho_1;
    psi_2=rho_2;
    % Fsum - interaction forces
    [Fxtemp_1 Fytemp_1]=Fsum(Nr,Mc,iabw1,jabw1,lena,psi_1,partition.Up_1,partition.Low_1);
    [Fxtemp_2 Fytemp_2]=Fsum(Nr,Mc,iabw1,jabw1,lena,psi_2,partition.Up_2,partition.Low_2);
	% Interaction forces, final form
    Fx_1=-G*psi_1.*Fxtemp_2; Fx_2=-G*psi_2.*Fxtemp_1;
    Fy_1=-G*psi_1.*Fytemp_2; Fy_2=-G*psi_2.*Fytemp_1;
    
    % --- MACROSCOPIC, COMPOSITE, AND EQUILIBRIUM VELOCITIES
	% Velocity
    ux_1=zeros(Nr,Mc);uy_1=zeros(Nr,Mc); ux_2=zeros(Nr,Mc);uy_2=zeros(Nr,Mc);
    if Cur_Iter>0
         ux_1=zeros(Nr,Mc);uy_1=zeros(Nr,Mc); ux_2=zeros(Nr,Mc);uy_2=zeros(Nr,Mc);
        for ic=1:N_c-1
            ux_1=ux_1+C_x(ic).*f_1(:,:,ic);
            uy_1=uy_1+C_y(ic).*f_1(:,:,ic);
            ux_2=ux_2+C_x(ic).*f_2(:,:,ic);
            uy_2=uy_2+C_y(ic).*f_2(:,:,ic);
        end
    end
    % Composite velocity
    ux=(ux_1*omega_1+ux_2*omega_2)./(rho_1*omega_1+rho_2*omega_2);
    uy=(uy_1*omega_1+uy_2*omega_2)./(rho_1*omega_1+rho_2*omega_2);
 	% Equilibrium velocities
    for nzr=1:lena
        if rho_1(ija(nzr))>0
            ux_1(ija(nzr))=ux(ija(nzr))+Fx_1(ija(nzr))/rho_1(ija(nzr))/omega_1+Fxs_1(ija(nzr))/omega_1; 
            uy_1(ija(nzr))=uy(ija(nzr))+Fy_1(ija(nzr))/rho_1(ija(nzr))/omega_1+Fys_1(ija(nzr))/omega_1;
        end
        if rho_2(ija(nzr))>0
            ux_2(ija(nzr))=ux(ija(nzr))+Fx_2(ija(nzr))/rho_2(ija(nzr))/omega_2+Fxs_2(ija(nzr))/omega_2; 
            uy_2(ija(nzr))=uy(ija(nzr))+Fy_2(ija(nzr))/rho_2(ija(nzr))/omega_2+Fys_2(ija(nzr))/omega_2;
        end
    end

    % --- EQUILIBRIUM DENSITY DISTRIBUTIONS
    feq_1=f_eqilibrium(ux_1,uy_1,rho_1,ija,NxM,Nr,Mc,N_c);
    feq_2=f_eqilibrium(ux_2,uy_2,rho_2,ija,NxM,Nr,Mc,N_c);
    
    % --- COLLISIONS
    f_1=(1.-omega_1).*f_1+omega_1.*feq_1;
    f_2=(1.-omega_2).*f_2+omega_2.*feq_2;
    
    % --- VOLUME/BODY FORCE
    for ic=1:N_c;
        for ia=1:lena
            i=iabw1(ia); j=jabw1(ia);
            f_1(i,j,ic)=f_1(i,j,ic)+force(ic);
			f_2(i,j,ic)=f_2(i,j,ic)+force(ic);
        end
    end
        
end
