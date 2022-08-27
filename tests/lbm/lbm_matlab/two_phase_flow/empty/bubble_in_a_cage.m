function bubble_in_a_cage
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
% Parallel Shan and Chen MCMP LBM v.3
% -------------------------------------------------------------------------
% Last modified: October 28  2014
% -------------------------------------------------------------------------
% Two component, two phase laminar flow in LBM past a bubble/droplet that
% is trapped with a solid wall - bulk flow field developement stage
% Periodic boundary conditions on top and bottom, no-slip wall 
% boundaries on the sides; driven by a body force/pressure drop
% x direction is horizontal (matrix columns), y is vertical (rows)
% flow is in the y direction
% Fluid 1 is the droplet/bubble, fluid 2 is the continuous phase
% -------------------------------------------------------------------------
% This program concatenates domain with the droplet from bubble_ini_dev.m
% with a larger - final domain. The domain from bubble_ini_dev.m is shorter
% and appears before any interior geometry
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
% User input: .mat file with an equilibrated bubble/droplet developed with
% bubble_ini_dev.m programs; this program enlarges the simulation domain 
% by concatenating matrices
% -------------------------------------------------------------------------
% Output: saved .mat files every user determined number of steps
% -------------------------------------------------------------------------
% Functions used by the program:
% all_ops_v2.m - all local operations without multiscale modeling part
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
load('developed_droplet_14.mat')
% Compute the macroscopic densities and velocities
% likely acts as macro subsititution - turn into a function in the future
velocities

% Naming template for the output files
output_file_name = 'trapped_droplet_';
% Adjust the saving tag - this appeares when saving data during the
% simulation; names of the datasets are of a format 
% output_file_name_NUMBER.mat
% NUMBER is incremented every time data is saved
NUMBER=1;

% Number of cores
nCores=12;

% % % % % % % % % % % % % % % % 
% 
% GEOMETRY
%
% % % % % % % % % % % % % % % %

% --- DOMAIN/CHANNEL SETUP 
Len_Channel_2D=299; 
Channel_2D_half_width=2.5*214; Width=Channel_2D_half_width*2;
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
% Final length of the channel/domain
Len_Channel_2D=1014*1.5; 
Channel_2D_half_width=2.5*214; Width=Channel_2D_half_width*2;
% Fluid area
Channel2D=ones(Len_Channel_2D,Width);
% Walls
Channel2D(:,1)=0; Channel2D(:,end)=0;
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
% Concatenate fluids
LongDensity_1=0.06*ones(Nri,Mci);
LongDensity_2=2*ones(Nri,Mci);
density_1=cat(1,rho_1,LongDensity_1);
density_2=cat(1,rho_2,LongDensity_2);
% Fluid locations (global)
[iabw1 jabw1]=find(Channel2D==1);
lena=length(iabw1);  
ija= (jabw1-1)*Nr+iabw1; 
% Computing new density distributions
f_1=zeros(Nr,Mc,N_c);
f_2=zeros(Nr,Mc,N_c);
for ia=1:lena 
    iW=iabw1(ia); jW=jabw1(ia); 
    f_1(iW,jW,:)=density_1(iW,jW)/9;
    f_2(iW,jW,:)=density_2(iW,jW)/9;
end
% Compute new macroscopic densities
rho_1=sum(f_1,3);
rho_2=sum(f_2,3);

% --- SET UP THE CAGE
[iIn4 jIn4]=find((((rho_2-rho_1)<=0)&rho_1<=2&rho_1>6e-2));
for jin=1:length(iIn4)
        Channel2D(iIn4(jin),jIn4(jin))=0;
end
% Fluid locations (global)
[iabw1 jabw1]=find(Channel2D==1);
lena=length(iabw1);  
ija= (jabw1-1)*Nr+iabw1; 
% Computing new density distributions
f_1=zeros(Nr,Mc,N_c);
f_2=zeros(Nr,Mc,N_c);
for ia=1:lena 
    iW=iabw1(ia); jW=jabw1(ia); 
    f_1(iW,jW,:)=density_1(iW,jW)/9;
    f_2(iW,jW,:)=density_2(iW,jW)/9;
end

% % % % % % % % % % % % % % % % 
% 
% LBM PARAMETERS
%
% % % % % % % % % % % % % % % %

% --- FLUID AND FLOW PROPERTIES, LBM CONSTANTS
% Mostly defined in bubble_ini_dev.m
% Pressure drop - body force
dPdL=3.5e-6;
% Magnitude of force that acts on the interface 
% (sign by default opposite to the bulk force)
Fmag=0;

% --- INTERACTION POTENTIALS 
% Only solids - fluid-fluid defined in the droplet development stage
% "-" is attractive, "+", repulsive
Gs_2=-0*0.45;
Gs_1=-0*Gs_2;

% % % % % % % % % % % % % % % % 
% 
% OTHER SETTINGS
%
% % % % % % % % % % % % % % % %

% --- SIMULATION SETTINGS
% Simulation settings
% Maximum number of steps
Max_Iter=20e3;
% Current step
Cur_Iter=1;
% Save this many steps
save_every=5e3;
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

% --- SOLID INTERACTIONS
% The same for both species
[Fxs Fys]=solid_sum(lena,iabw1,jabw1, Channel2D,Nr,Mc);
% This is due to the fact that psi=rho, so density term in the forcing term 
% will cancel so it can be precomputed rather than used all the time
% Different psi will cause it to be included in the loop
% So this will be incorporated into the computation of equilibrium velocity
% as tau_1*Fxs_1, no division by density, same for species 2
Fxs_1=-Gs_1*Fxs; Fys_1=-Gs_1*Fys; 
Fxs_2=-Gs_2*Fxs; Fys_2=-Gs_2*Fys;
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

% --- INTERFATIAL AND BODY FORCES
% External (body) forces
force=-dPdL*(1/6)*1*[0 -1 0 1 -1 -1 1 1 0]';
% Force that acts on the interface (sign by default opposite to force)
forceINT=Fmag*(1/6)*1*[0 -1 0 1 -1 -1 1 1 0]';

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
        [PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.f_2,BUp_1, BUp_2, BLW_1, BLW_2]=all_ops_v2(PARTITIONS.partition_1,G,omega_1,force,Cur_Iter);
        PARTITIONS.partition_1.f_1=stream_obstacles(PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.Channel2D);
        PARTITIONS.partition_1.f_2=stream_obstacles(PARTITIONS.partition_1.f_2,PARTITIONS.partition_1.Channel2D);
    end
    % All other processors
    for kCore=2:nCores
        if(labindex==kCore)
            [PARTITIONS.(part_names{kCore}).f_1,PARTITIONS.(part_names{kCore}).f_2,BUp_1, BUp_2, BLW_1, BLW_2]=all_ops_v2(PARTITIONS.(part_names{kCore}),G,omega_1,force,Cur_Iter);
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
    if (mod(Cur_Iter,save_every)==0)
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
            
            % Remove temporary velocity holders - otherwise the next part
            % will not run correctly
            clearvars rho_10
            clearvars rho_20
            
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

