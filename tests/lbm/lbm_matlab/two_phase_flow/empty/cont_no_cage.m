function cont_no_cage
load('POSTS_Re90_RSmaller_PDMS_1232.mat')
% load('MultiscaleOperator_SecondRound_BigB_PDMS_204.mat')
velocities
NUMBER=1;

% ADDITIONAL FORCES - VOLUME AND MULTISCALE OPERATOR
% External (body) forces
% This is actually needed and is transferred
force=-dPdL*(1/6)*1*[0 -1 0 1 -1 -1 1 1 0]';
% Interface
% Force that acts on the interface (sign by default opposite to force)
Fmag=0*2.35e-5;% 470*dPdL - from round 5, total 2.35e-5; % quadratic scaling for different mass
% To match the posts area
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

% Splits the domain into partitions; partitions are structures 
% so other components can be easily added to them in the future 
% partition_1 includes PBC and is 2x longer than 2,3,4
% Ordering in partition_1 is set such to have lower halo at the top 
% and upper at the bottom (system used in streaming in this parallelization)
% Nr/1 PBC boundary is in the middle, next to each other, resembling actual 
% boundary condidition, no need for separate streaming routine
% First partition - always the same + Channel2D
PARTITIONS.partition_1.f_1=f_1([nCores*Nprt:Nr, 1:Nprt],:,:); 
PARTITIONS.partition_1.f_2=f_2([nCores*Nprt:Nr, 1:Nprt],:,:);
PARTITIONS.partition_1.Channel2D=Channel2D([nCores*Nprt:Nr, 1:Nprt],:);

% Other partitions + Channel2D
for kCore=2:nCores
    PARTITIONS.(part_names{kCore}).f_1=f_1((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).f_2=f_2((kCore-1)*Nprt:kCore*Nprt,:,:);
    PARTITIONS.(part_names{kCore}).Channel2D=Channel2D((kCore-1)*Nprt:kCore*Nprt,:);
end

% Density buffers initialization - additional info needed for fluid/fluid interactions computations
% these are obtained from neighboring partitions, here for initial computation; they are extra rows above 
% upper and below lower halos

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

% Max # iterations
Max_Iter=Cur_Iter+101;

% Main loop
while (~StopFlag)
    Cur_Iter=Cur_Iter+1;
    
    % Solution 
    if (labindex==1)
        [PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.f_2,BUp_1, BUp_2, BLW_1, BLW_2]=all_ops_v2_INT(PARTITIONS.partition_1,G,omega,force,PARTITIONS.partition_1.forceINT,Cur_Iter);
        PARTITIONS.partition_1.f_1=stream_obstacles(PARTITIONS.partition_1.f_1,PARTITIONS.partition_1.Channel2D);
        PARTITIONS.partition_1.f_2=stream_obstacles(PARTITIONS.partition_1.f_2,PARTITIONS.partition_1.Channel2D);
    end
    
    for kCore=2:nCores
        if(labindex==kCore)
            [PARTITIONS.(part_names{kCore}).f_1,PARTITIONS.(part_names{kCore}).f_2,BUp_1, BUp_2, BLW_1, BLW_2]=all_ops_v2_INT(PARTITIONS.(part_names{kCore}),G,omega,force,PARTITIONS.(part_names{kCore}).forceINT,Cur_Iter);
            PARTITIONS.(part_names{kCore}).f_1=stream_obstacles(PARTITIONS.(part_names{kCore}).f_1,PARTITIONS.(part_names{kCore}).Channel2D);
            PARTITIONS.(part_names{kCore}).f_2=stream_obstacles(PARTITIONS.(part_names{kCore}).f_2,PARTITIONS.(part_names{kCore}).Channel2D);
        end
    end
    
   % Updates: send/receive
   % Upper halos (domain end) exchange 1,2,3 and 5
   % Lower halos (first row) - 4,7 and 8 
   % Also send out extra density rows for fluid/fluid interactions, BUp and BLW	 	
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
    
    % Every 100th iteration: send everything to processor 1, 
    % compute macro variables, save and terminate
    if (mod(Cur_Iter,1)==0)
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

            save(['POSTS_Re90_RSmaller_PDMS_1232_',num2str(NUMBER),'.mat'])
            NUMBER=NUMBER+1;
        end
        
       if  (Cur_Iter > Max_Iter)
            StopFlag=true;
            break 
       end  
    end    
end

end