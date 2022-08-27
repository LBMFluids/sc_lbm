function no_cage
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
% Parallel Shan and Chen MCMP LBM v.3
% -------------------------------------------------------------------------
% Last modified: December 10 2014
% -------------------------------------------------------------------------
% Two component, two phase laminar flow in LBM in a channel/domain with
% obstacles
% Periodic boundary conditions on top and bottom, no-slip wall 
% boundaries on the sides; driven by a body force/pressure drop
% x direction is horizontal (matrix columns), y is vertical (rows)
% flow is in the y direction
% Fluid 1 is the droplet/bubble, fluid 2 is the continuous phase
% -------------------------------------------------------------------------
% Program includes three forms of fluid-solid interaction 1) Standard form
% with two-valued interactions 2) Random interaction values in a specified
% range 3) Two-valued interactions with various degree of random
% perturbation; Schemes 2) and 3) were used to introduce defects in the
% solid material
% -------------------------------------------------------------------------
% This program concatenates domain with the droplet from bubble_ini_dev.m
% with a larger - final domain. The domain from bubble_ini_dev.m is shorter
% and appears before any interior geometry
% The progam then installed the velocity/density fields for both fluids
% previously developed with bubble_in_a_cage.m
% It can be used to model the empty domain with no solid objects, and a
% domain with posts or pillars defined in staggered_geom.m; choose in the
% the geometry part of the program
% -------------------------------------------------------------------------
% Initial framework of this program was adapted from single phase LBM
% serial program from MATLAB file exchange:
%   https://www.mathworks.com/matlabcentral/fileexchange/6904-basic-lattice-boltzmann--lb--matlab-code
% -------------------------------------------------------------------------
% Runs on n processors (make sure the domain length is divisible by n+1)
% Currently coded until 12 cores - to use more, add extra partition names
% in part_names structure
% -------------------------------------------------------------------------
% Problem is solved in parallel on each partition, 
% partitions communicate only density distributions f1 and f2 at halo 
% nodes + one extra row. Every m iterations all f1/f2 values are send to 
% processor 1 and saved. For efficiency, macroscopic properties computation
% is skipped at this step and has to be made independently through 
% macro_vel_tot.m script after loading the saved data.
% Values not enclosed in if(labindex=...)loops are visible to all the cores
% Partitions (important): 1st row is the lower halo, last row is the 
% upper halo changing this requires re-coding
% -------------------------------------------------------------------------
% User input: .mat files from bubble_ini_dev.m and bubble_in_a_cage.m; 
% this program enlarges the simulation domain by concatenating matrices
% -------------------------------------------------------------------------
% Output: saved .mat files every user determined number of steps
% -------------------------------------------------------------------------
% Functions used by the program:
% One of the two following:
% *** all_ops_v2.m - all local operations without multiscale modeling part
% *** all_ops_v2_INT.m - all local operations with multiscale modeling part
%                          this function needs changes in the interface
%                          tracking parameters if different fluid densities
%                          are used
% stream_obstacles.m - streaming with arbitrarly positioned walls and
% obtacles
% solid_sum.m - summation term for solid objects (fluid/solid interactions); 
% 		computed once before the time while loop
% Fsum.m - fluid/fluid interactions (repulsive) used by all_ops_v2_INT.m
% f_equilibrium.m - equilibrium distribution computation
% velocities - program that computes macroscopic densities and velocities
% of the fluids
% staggered_geom - program that sets up the geometry/architecture of the
% domain if using solid posts
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

% % % % % % % % % % % % % % % % 
% 
% INITIAL INPUT
%
% % % % % % % % % % % % % % % %

% Initial bubble/droplet
% From bubble_ini_dev.mat
% For initial, equilibrium density distribution
load('developed_droplet_14.mat')
rho_10=sum(f_1,3);
rho_20=sum(f_2,3);

% Developed flow field
% From bubble_in_a_cage.mat
load('trapped_droplet_4.mat')
% Lower the bulk force if necessary
% dPdL=0.3*dPdL;

% Naming template for the output files
output_file_name = 'empty_multiscale_';
% Adjust the saving tag - this appeares when saving data during the
% simulation; names of the datasets are of a format 
% output_file_name_NUMBER.mat
% NUMBER is incremented every time data is saved
NUMBER=1;

% Number of cores
nCores=12;

% --- FUNCTION FOR LOCAL OPERATIONS
% Choose one
% For domain with posts or pillars and empty domain with no multiscale
% modeling
% ops_function=@all_ops_v2;
% For empty domain with multiscale modeling
ops_function=@all_ops_v2_INT;

% % % % % % % % % % % % % % % % 
% 
% GEOMETRY
%
% % % % % % % % % % % % % % % %

% --- DOMAIN/CHANNEL SETUP 
Len_Channel_2D=299; 
Channel_half_width=2.5*214; Width=Channel_half_width*2;
% Fluid area
Channel=ones(Len_Channel_2D,Width); 
% Walls
Channel(:,1)=0; Channel(:,end)=0; 
[Nr Mc]=size(Channel);

% --- CHOOSE INTERIOR GEOMETRY
% -----------------------------------
% --- FOR DOMAIN WITH POSTS OR PILLARS
% staggered_geom
% -----------------------------------
% --- FOR EMPTY DOMAIN (WALLS BUT NO POSTS/PILLARS)
Len_Channel_2D=1014*1.5; 
Channel_2D_half_width=2.5*214; Width=Channel_2D_half_width*2;
% Fluid area
Channel2D=ones(Len_Channel_2D,Width);
[Nr Mc]=size(Channel2D);

% --- CONCATENATE DOMAINS
% Common for both domains - one from the bubble_ini_dev.m and the extension
% Extra domain
LongDomain=Channel2D(300:end,:);
LongDomain(:,1)=0; LongDomain(:,end)=0;
[Nri Mci]=size(Channel2D);
% Concatenate channel geometry
Channel2D=cat(1,Channel,LongDomain);
[Nr Mc]=size(Channel2D);
Channel2D(:,1)=0; Channel2D(:,end)=0; % Walls
% Fluid locations (global)
[iabw1 jabw1]=find(Channel2D==1);
lena=length(iabw1);  
ija=(jabw1-1)*Nr+iabw1; 
% Computing new density distributions
rho_1=sum(f_1,3);
rho_2=sum(f_2,3);

[iIn4 jIn4]=find((((rho_20-rho_10)<=0)&rho_10<=2&rho_10>6e-2));
for jin=1:length(iIn4)
        rho_1(iIn4(jin),jIn4(jin))=rho_10(iIn4(jin),jIn4(jin));
        rho_2(iIn4(jin),jIn4(jin))=rho_20(iIn4(jin),jIn4(jin));
        f_1(iIn4(jin),jIn4(jin),:)=rho_10(iIn4(jin),jIn4(jin))/9;
        f_2(iIn4(jin),jIn4(jin),:)=rho_20(iIn4(jin),jIn4(jin))/9;
end

% % % % % % % % % % % % % % % % 
% 
% LBM PARAMETERS
%
% % % % % % % % % % % % % % % %

% --- INTERACTION POTENTIALS
% Only solids - fluid-fluid defined in the droplet development stage
% "-" is attractive, "+", repulsive
Gs_2=-0.1421;
Gs_1=-Gs_2; 

% % % % % % % % % % % % % % % % 
% 
% SIMULATION SETTINGS
%
% % % % % % % % % % % % % % % %

% --- SIMULATION SETTINGS
% Simulation settings
% Maximum number of steps
Max_Iter=30e3;
% Current step
Cur_Iter=1;
% Save this many steps
save_every=100;
% Flag for stopping the simulation
StopFlag=false;

% --- INITIALIZATION OF PARTITIONS FOR PARALLEL COMPUTING
% Number of partitions
Nprt=Nr/(nCores+1);
% Generating partitions - modify only here if more cores are needed
part_names={'partition_1' 'partition_2' 'partition_3'...
          'partition_4' 'partition_5' 'partition_6'...
          'partition_7' 'partition_8' 'partition_9'...
          'partition_10' 'partition_11' 'partition_12'};

% PARTIOTIONS - structure of structures which are partitions 
% #structures=nCores=#partitions (1st is x2 longer than others) 
% Splits the domain into partitions; partitions are structures 
% so other components can be easily added to them in the future 
% Partition_1 is 2x longer than the others
% Ordering in partition_1 is set such to have lower halo at the top 
% and upper at the bottom (system used in streaming in this parallelization)
% Nr-1 (last row-first row) periodic boundary is in the middle, 
% next to each other, resembling actualboundary condidition, 
% no need for separate streaming routine
% First density partition - always the same + Channel2D
PARTITIONS.partition_1.f_1=f_1([nCores*Nprt:Nr, 1:Nprt],:,:); 
PARTITIONS.partition_1.f_2=f_2([nCores*Nprt:Nr, 1:Nprt],:,:);
PARTITIONS.partition_1.Channel2D=Channel2D([nCores*Nprt:Nr, 1:Nprt],:);

% Other partitions + Channel2D
for kCore=2:nCores
    PARTITIONS.(part_names{kCore}).f_1=f_1((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).f_2=f_2((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).Channel2D=Channel2D((kCore-1)*Nprt:kCore*Nprt,:);
end

% % % % % % % % % % % % % % % % 
% 
% SOLID INTERACTIONS
%
% % % % % % % % % % % % % % % %
% Choose an interaction scheme

% --- REGULAR FLUID-SOLID INTERACTIONS
% The same for both species
[Fxs Fys]=solid_sum(lena,iabw1,jabw1, Channel2D,Nr,Mc);
% This is due to the fact that psi=rho, so density term in the forcing term 
% will cancel so it can be precomputed rather than used all the time
% Different psi will cause it to be included in the loop
% So this will be incorporated into the computation of equilibrium velocity
% as tau_1*Fxs_1, no division by density, same for species 2
Fxs_1=-Gs_1*Fxs; Fys_1=-Gs_1*Fys; 
Fxs_2=-Gs_2*Fxs; Fys_2=-Gs_2*Fys;

% --- RADOMIZED DISTRIBUTIONS OF THE FLUID-SOLID INTERACTIONS
% -------------------------------------------------------------------------
% -------------------------------------------------------------------------
% --- Gs DISTRIBUTION FROM A RANDOM DISTRIBUTION
% Size of matrices with solid interactions
% [NGs,MGs]=size(Fxs);
% % Max absolute Gs value
% Gs_max=0.4365; 
% % Random sign or 0 for Gs (matrix with -1, 0 or 1 entries)
% Gs_Sign=randi([-1,1],NGs,MGs);
% % Matrix of abosolute allowable values; rand from >0 to <1 entries 
% Gs_Value=rand(NGs,MGs)*Gs_max; 
% % (drawback - will never reach full wetting/non-wetting; 
% % it will reach up to 0.99 of it, which is fine in real systems
% % Final Gs distribution
% Gs_dist=Gs_Value.*Gs_Sign; 
% % Assign one to liquid (2) and opposite one to gas (1)
% % Liquid
% Gs_2=Gs_dist;
% % Gas (opposite)
% Gs_1=-Gs_2; 
% % Distribute into final force matrices
% Fxs_1=-Gs_1.*Fxs; Fys_1=-Gs_1.*Fys; 
% Fxs_2=-Gs_2.*Fxs; Fys_2=-Gs_2.*Fys;
% -------------------------------------------------------------------------
% --- RANDOMLY PERTURBED Gs DISTRIBUTION
% Size of matrices with solid interactions
% [NGs,MGs]=size(Fxs);
% % Desired Gs value for liquid
% Gs_av=-0.1421; 
% Gs_dist=ones(NGs,MGs)*Gs_av;
% Gmax=-0.4365;
% for igs=1:NGs
%     for jgs=1:MGs
%         % 0 for no change, 1 for change; the more *randi([0,1]) 
%         % the less random changes
%         yn=randi([0,1])*randi([0,1])*randi([0,1])*randi([0,1]); 
%         if yn
%             % Random sign or 0 for Gs; 1 indicates all inhomogeneities 
%             % in favor of gas
%             Gs_Sign=randi([-1,1]);
%             % Rand from >0 to <1 entries 
%             Gs_Value=rand(1,1)*Gmax;
%             % Final Gs distribution entry 
%             % (for liquid, for gas change Gs_av to 0.4365 for these systems)
%             Gs_dist(igs,jgs)=Gs_Value.*Gs_Sign; 
%         end
%     end
% end
% %  Assign one to liquid (2) and opposite one to gas (1)
% % Liquid
% Gs_2=Gs_dist;
% % Gas (opposite)
% Gs_1=-Gs_2; 
% % Distribute into final force matrices
% Fxs_1=-Gs_1.*Fxs; Fys_1=-Gs_1.*Fys; 
% Fxs_2=-Gs_2.*Fxs; Fys_2=-Gs_2.*Fys;
% % For visualization:
% % surf(Gs_2);

% --- COMMON FOR ANY FLUID-SOLID INTERACTION SCHEME
% Distribute into partitions
% Separate partition 1
PARTITIONS.partition_1.Fxs_1=Fxs_1([nCores*Nprt:Nr, 1:Nprt],:,:);
PARTITIONS.partition_1.Fxs_2=Fxs_2([nCores*Nprt:Nr, 1:Nprt],:,:);
PARTITIONS.partition_1.Fys_1=Fys_1([nCores*Nprt:Nr, 1:Nprt],:,:);
PARTITIONS.partition_1.Fys_2=Fys_2([nCores*Nprt:Nr, 1:Nprt],:,:);
% Other partitions
for kCore=2:nCores
    PARTITIONS.(part_names{kCore}).Fxs_1=Fxs_1((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).Fxs_2=Fxs_2((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).Fys_1=Fys_1((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).Fys_2=Fys_2((kCore-1)*Nprt:kCore*Nprt,:,:);
end

% % % % % % % % % % % % % % % % 
% 
% OTHER SETTINGS
%
% % % % % % % % % % % % % % % %

% --- INTERFATIAL AND BODY FORCES
% --- EXTERNAL BODY FORCES
force=-dPdL*(1/6)*1*[0 -1 0 1 -1 -1 1 1 0]';
% -------------------------------------------------------------------------
% --- INTERFACIAL FORCE - MULTISCALE MODELING
% Mgitude of interfacial force i.e. the coupling operator
Fmag=2.35e-5;
% To match the pillars area
ForceDist=zeros(Nr,Mc);  
ForceDist(351:1207,2:Mc-1)=ForceDist(351:1207,2:Mc-1)+Fmag*(1/6)*1;
Cforce=[0 -1 0 1 -1 -1 1 1 0];
% Distribute
forceINT=zeros(Nr,Mc,N_c);
for inc=1:N_c
    for ips=1:Nr
        for jps=1:Mc
            forceINT(ips,jps,inc)=ForceDist(ips,jps)*Cforce(inc);
        end
    end
end
% Split interface force distribution into partitions
% Separate partition 1
PARTITIONS.partition_1.forceINT=forceINT([nCores*Nprt:Nr, 1:Nprt],:,:);
% Other partitions
for kCore=2:nCores
    PARTITIONS.(part_names{kCore}).forceINT=forceINT((kCore-1)*Nprt:kCore*Nprt,:,:);
end

% ---- DENSITY BUFFERS FOR PARALLEL COMPUTING
% Density buffers initialization - additional info needed for fluid/fluid 
% interactions computations these are obtained from neighboring partitions,
% here for initial computation; they are extra rows above upper and 
% below lower halos
rho_1=sum(f_1,3);
rho_2=sum(f_2,3);
PARTITIONS.partition_1.Up_1=rho_1(nCores*Nprt-1,:);
PARTITIONS.partition_1.Up_2=rho_2(nCores*Nprt-1,:);
PARTITIONS.partition_1.Low_1=rho_1(Nprt+1,:);
PARTITIONS.partition_1.Low_2=rho_2(Nprt+1,:);
for kCore=2:nCores
    PARTITIONS.(part_names{kCore}).Up_1=rho_1(((kCore-1)*Nprt-1),:);
    PARTITIONS.(part_names{kCore}).Up_2=rho_2(((kCore-1)*Nprt-1),:);
    PARTITIONS.(part_names{kCore}).Low_1=rho_1((kCore*Nprt+1),:);
    PARTITIONS.(part_names{kCore}).Low_2=rho_2((kCore*Nprt+1),:);
end

% % % % % % % % % % % % % % % % 
% 
% MAIN SIMULATION PART
%
% % % % % % % % % % % % % % % %
% --- No user input required in this part --------
%
tic 
while (~StopFlag)
    Cur_Iter=Cur_Iter+1;
    
    % % % % % % % % % % % % % % % % 
    % 
    % COMPLETE LBM COMPUTATION
    %
    % % % % % % % % % % % % % % % %
    
    % Processor 1
    if (labindex==1)
        [PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.f_2,BUp_1, BUp_2, BLW_1, BLW_2]=ops_function(PARTITIONS.partition_1,G,omega_1,force,Cur_Iter);
        PARTITIONS.partition_1.f_1=stream_obstacles(PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.Channel2D);
        PARTITIONS.partition_1.f_2=stream_obstacles(PARTITIONS.partition_1.f_2,PARTITIONS.partition_1.Channel2D);
    end
     % All other processors
    for kCore=2:nCores
        if(labindex==kCore)
            [PARTITIONS.(part_names{kCore}).f_1,PARTITIONS.(part_names{kCore}).f_2,BUp_1, BUp_2, BLW_1, BLW_2]=ops_function(PARTITIONS.(part_names{kCore}),G,omega_1,force,Cur_Iter);
            PARTITIONS.(part_names{kCore}).f_1=stream_obstacles(PARTITIONS.(part_names{kCore}).f_1,PARTITIONS.(part_names{kCore}).Channel2D);
            PARTITIONS.(part_names{kCore}).f_2=stream_obstacles(PARTITIONS.(part_names{kCore}).f_2,PARTITIONS.(part_names{kCore}).Channel2D);
        end
    end

   % % % % % % % % % % % % % % % % % % % 
   % 
   % INTERPROCESSOR COMMUNICATION PART
   %
   % % % % % % % % % % % % % % % % % % %
   % Updates: send/receive
   % Upper halos (domain end) exchange 1,2,3,5, and 6
   % Lower halos (first row) - 4,7, and 8 
   % Also send out extra density rows for fluid/fluid interactions, 
   % BUp and BLW	 	
   % Level 1xNprt
   if(labindex==1)
        labSend(PARTITIONS.partition_1.f_1(end,:,1),2);
        labSend(PARTITIONS.partition_1.f_1(end,:,3),2);
        labSend(PARTITIONS.partition_1.f_1(end,:,5:6),2);
        labSend(PARTITIONS.partition_1.f_1(end,:,2),2);
        
        labSend(PARTITIONS.partition_1.f_2(end,:,1),2);
        labSend(PARTITIONS.partition_1.f_2(end,:,3),2);
        labSend(PARTITIONS.partition_1.f_2(end,:,5:6),2);
        labSend(PARTITIONS.partition_1.f_2(end,:,2),2);
                
        labSend(BUp_1,2);
        labSend(BUp_2,2);
        
    end
    
    if(labindex==2)
        PARTITIONS.partition_2.f_1(1,:,1)=labReceive(1);
        PARTITIONS.partition_2.f_1(1,:,3)=labReceive(1);
        PARTITIONS.partition_2.f_1(1,:,5:6)=labReceive(1);
        PARTITIONS.partition_2.f_1(1,:,2)=labReceive(1);
        
        PARTITIONS.partition_2.f_2(1,:,1)=labReceive(1);
        PARTITIONS.partition_2.f_2(1,:,3)=labReceive(1);
        PARTITIONS.partition_2.f_2(1,:,5:6)=labReceive(1);
        PARTITIONS.partition_2.f_2(1,:,2)=labReceive(1);
    
        PARTITIONS.partition_2.Up_1=labReceive(1);
        PARTITIONS.partition_2.Up_2=labReceive(1);
    end
    
    if(labindex==2)
        labSend(PARTITIONS.partition_2.f_1(1,:,4),1);
        labSend(PARTITIONS.partition_2.f_1(1,:,7:8),1);
        labSend(PARTITIONS.partition_2.f_2(1,:,4),1);
        labSend(PARTITIONS.partition_2.f_2(1,:,7:8),1);
        labSend(BLW_1,1);
        labSend(BLW_2,1);
    end
    
    if(labindex==1)
        PARTITIONS.partition_1.f_1(end,:,4)=labReceive(2);
        PARTITIONS.partition_1.f_1(end,:,7:8)=labReceive(2);
        PARTITIONS.partition_1.f_2(end,:,4)=labReceive(2);
        PARTITIONS.partition_1.f_2(end,:,7:8)=labReceive(2);
        
        PARTITIONS.partition_1.Low_1=labReceive(2);
        PARTITIONS.partition_1.Low_2=labReceive(2);
    end
    
    % All other partitions
    for kCore=2:nCores-1
    
    if(labindex==kCore)
        labSend(PARTITIONS.(part_names{kCore}).f_1(end,:,1),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_1(end,:,3),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_1(end,:,5:6),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_1(end,:,2),kCore+1);
        
        labSend(PARTITIONS.(part_names{kCore}).f_2(end,:,1),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_2(end,:,3),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_2(end,:,5:6),kCore+1);
        labSend(PARTITIONS.(part_names{kCore}).f_2(end,:,2),kCore+1);
        
        labSend(BUp_1,kCore+1);
        labSend(BUp_2,kCore+1);
    end
    
    if(labindex==kCore+1)
        PARTITIONS.(part_names{kCore+1}).f_1(1,:,1)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_1(1,:,3)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_1(1,:,5:6)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_1(1,:,2)=labReceive(kCore);
        
        PARTITIONS.(part_names{kCore+1}).f_2(1,:,1)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_2(1,:,3)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_2(1,:,5:6)=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).f_2(1,:,2)=labReceive(kCore);
        
        PARTITIONS.(part_names{kCore+1}).Up_1=labReceive(kCore);
        PARTITIONS.(part_names{kCore+1}).Up_2=labReceive(kCore);
    end
    
    if(labindex==kCore+1)
        labSend(PARTITIONS.(part_names{kCore+1}).f_1(1,:,4),kCore);
        labSend(PARTITIONS.(part_names{kCore+1}).f_1(1,:,7:8),kCore);
        labSend(PARTITIONS.(part_names{kCore+1}).f_2(1,:,4),kCore);
        labSend(PARTITIONS.(part_names{kCore+1}).f_2(1,:,7:8),kCore);
        
        labSend(BLW_1,kCore);
        labSend(BLW_2,kCore);
    end
    
    if(labindex==kCore)
        PARTITIONS.(part_names{kCore}).f_1(end,:,4)=labReceive(kCore+1);
        PARTITIONS.(part_names{kCore}).f_1(end,:,7:8)=labReceive(kCore+1);
        PARTITIONS.(part_names{kCore}).f_2(end,:,4)=labReceive(kCore+1);
        PARTITIONS.(part_names{kCore}).f_2(end,:,7:8)=labReceive(kCore+1);
        
        PARTITIONS.(part_names{kCore}).Low_1=labReceive(kCore+1);
        PARTITIONS.(part_names{kCore}).Low_2=labReceive(kCore+1);
    end
    end
    
   
    % Last level, nCoresxNprt (exchanges with first)
    if(labindex==nCores)
        labSend(PARTITIONS.(part_names{nCores}).f_1(end,:,1),1);
        labSend(PARTITIONS.(part_names{nCores}).f_1(end,:,3),1);
        labSend(PARTITIONS.(part_names{nCores}).f_1(end,:,5:6),1);
        labSend(PARTITIONS.(part_names{nCores}).f_1(end,:,2),1);
        
        labSend(PARTITIONS.(part_names{nCores}).f_2(end,:,1),1);
        labSend(PARTITIONS.(part_names{nCores}).f_2(end,:,3),1);
        labSend(PARTITIONS.(part_names{nCores}).f_2(end,:,5:6),1);
        labSend(PARTITIONS.(part_names{nCores}).f_2(end,:,2),1);
                
        labSend(BUp_1,1);
        labSend(BUp_2,1);
    end
    
    if(labindex==1)
        PARTITIONS.partition_1.f_1(1,:,1)=labReceive(nCores);
        PARTITIONS.partition_1.f_1(1,:,3)=labReceive(nCores);
        PARTITIONS.partition_1.f_1(1,:,5:6)=labReceive(nCores);
        PARTITIONS.partition_1.f_1(1,:,2)=labReceive(nCores);
        
        PARTITIONS.partition_1.f_2(1,:,1)=labReceive(nCores);
        PARTITIONS.partition_1.f_2(1,:,3)=labReceive(nCores);
        PARTITIONS.partition_1.f_2(1,:,5:6)=labReceive(nCores);
        PARTITIONS.partition_1.f_2(1,:,2)=labReceive(nCores);
        
        PARTITIONS.partition_1.Up_1=labReceive(nCores);
        PARTITIONS.partition_1.Up_2=labReceive(nCores);
    end
    
    if(labindex==1)
        labSend(PARTITIONS.partition_1.f_1(1,:,4),nCores);
        labSend(PARTITIONS.partition_1.f_1(1,:,7:8),nCores);
        
        labSend(PARTITIONS.partition_1.f_2(1,:,4),nCores);
        labSend(PARTITIONS.partition_1.f_2(1,:,7:8),nCores);
        
        labSend(BLW_1,nCores);
        labSend(BLW_2,nCores);
    end
    
    if(labindex==nCores)
        PARTITIONS.(part_names{nCores}).f_1(end,:,4)=labReceive(1);
        PARTITIONS.(part_names{nCores}).f_1(end,:,7:8)=labReceive(1);
        PARTITIONS.(part_names{nCores}).f_2(end,:,4)=labReceive(1);
        PARTITIONS.(part_names{nCores}).f_2(end,:,7:8)=labReceive(1);
        PARTITIONS.(part_names{nCores}).Low_1=labReceive(1);
        PARTITIONS.(part_names{nCores}).Low_2=labReceive(1);
    end
    
    % % % % % % % % % % % % % % % % % % % % %
    % 
    % SAVE DATA, CHECK TERMINATION CRITERIA 
    %
    % % % % % % % % % % % % % % % % % % % % %
    % --- SAVING
    % Every save_every iteration: send everything to processor 1, 
    % and save, also terminates if Max_Iter is reached
    if (mod(Cur_Iter,100)==0)
        f_1=zeros(Nr,Mc,Nc);
        f_2=zeros(Nr,Mc,Nc);
        
        for kCore=2:nCores
            if(labindex==kCore)           
                labSend(PARTITIONS.(part_names{kCore}).f_1,1);
                labSend(PARTITIONS.(part_names{kCore}).f_2,1);
            end
            if(labindex==1)
                f_1((kCore-1)*Nprt:(kCore)*Nprt,:,:)=labReceive(kCore);
                f_2((kCore-1)*Nprt:kCore*Nprt,:,:)=labReceive(kCore);
            end
        end

        if(labindex==1)
           
            f_1(1:(Nprt-1),:,:)=PARTITIONS.partition_1.f_1((Nprt+2):1:(end-1),:,:);
            f_1((nCores*Nprt+1):Nr,:,:)=PARTITIONS.partition_1.f_1(2:(Nprt+1),:,:);
            
            f_2(1:(Nprt-1),:,:)=PARTITIONS.partition_1.f_2((Nprt+2):1:(end-1),:,:);
            f_2((nCores*Nprt+1):Nr,:,:)=PARTITIONS.partition_1.f_2(2:(Nprt+1),:,:);

            save([output_file_name,num2str(NUMBER),'.mat'])
            NUMBER=NUMBER+1;
        end
        % --- CHECK FOR TERMINATION
        if  (Cur_Iter > Max_Iter)
            StopFlag=true;
            break 
        end  
    end    
end

toc
end

