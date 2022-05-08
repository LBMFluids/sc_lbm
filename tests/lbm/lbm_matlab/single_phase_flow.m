%% Generate data to compare with the C++ output
% geom_file - file with domain geometry (1 for fluid, 0 for solid node)
% rho_ini - initial (nominal) density 
% dPdL - pressure drop
% Max_Iter - how long to iterate (0 means just one step)
% out_file - file name template for saving the variables - it should follow
% the C++ template, see test code
function single_phase_flow(geom_file, rho_ini, force, Max_Iter, out_file)
    
    % Load the geometry
    Channel2D = load(geom_file);
    [Nr Mc]=size(Channel2D);
    
    % Viscosity
    omega = 1.0;

    % Compute the equlibrium distribution function for given distribution
    % function f and geometry
 
    % Compute indices for streaming
    Obstacles=bwperim(Channel2D,8); 
    border=logical(ones(Nr,Mc));
    Obstacles=Obstacles.*(border);

    [iobs jobs]=find(Obstacles);lenobs=length(iobs); ijobs= (jobs-1)*Nr+iobs; 
    [iawint jawint]=find(( Channel2D==1 & ~Obstacles)); 
    lenwint=length(iawint);
    ijaint= (jawint-1)*Nr+iawint;
    
    [iabw1 jabw1]=find(Channel2D~=0);
    lena=length(iabw1);
    ija=(jabw1-1)*Nr+iabw1; 

    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];

    % Initialize
    N_c = 9;
    f=zeros(Nr,Mc,N_c); 
    for ia=1:lena 
        i=iabw1(ia); j=jabw1(ia); 
        f(i,j,:)=rho_ini/N_c; 
    end
    % Velocity
    ux=zeros(Nr,Mc);uy=zeros(Nr,Mc);
    for ic=1:N_c-1
        ux=ux+C_x(ic).*f(:,:,ic);uy=uy+C_y(ic).*f(:,:,ic);
    end
    % Density
    rho=sum(f,3);
        
	% Dot products etc declarations 
    uxsq=zeros(size(rho)); uysq=zeros(size(rho)); usq=zeros(size(rho));
    feq=zeros(size(f));
    ux=zeros(size(rho));uy=zeros(size(rho));
    % Density weights
    w0=16/36.;w1=4/36.;w2=1/36.;
    W=[w1 w1 w1 w1 w2 w2 w2 w2 w0];
    cs2=1/3;cs2x2=2*cs2;cs4x2=2*cs2.^2;
    f1=3.; f2=4.5; f3=1.5;
    NxM=Nr*Mc;
    
    % While - main time evolution loop
    disp_every = 200; 
    save_every=10e3;
    Cur_Iter=0;
    StopFlag=false;
    
    % Save to file
    compute_feq();
    save_all(f, feq, ux, uy, rho, ['../test_data/matlab_', out_file, '_ini'], N_c);

    while (Cur_Iter <= Max_Iter)
        Cur_Iter=Cur_Iter+1;
        
        % Velocity
        ux=zeros(Nr,Mc);uy=zeros(Nr,Mc);
        for ic=1:N_c-1
            ux=ux+C_x(ic).*f(:,:,ic);uy=uy+C_y(ic).*f(:,:,ic);
        end
        % Density
        rho=sum(f,3);

        compute_feq();

        % Collision
        f=(1.-omega).*f+omega.*feq;
        % Save to file
        save_all(f, feq, ux, uy, rho, ['../test_data/matlab_', out_file, '_collide'], N_c);

        % Add force
        for ic=1:N_c
            for ia=1:lena
                i=iabw1(ia); j=jabw1(ia);
                f(i,j,ic)=f(i,j,ic)+force(ic);
            end
        end
        % Save to file
        save_all(f, feq, ux, uy, rho, ['../test_data/matlab_', out_file, '_add_force'], N_c);

        % Stream
        f=stream_wObs(f,Nr,Mc,iawint,jawint,lenwint, lenobs,iobs,jobs,Channel2D);
           
        if ~(mod(Cur_Iter, disp_every))
             disp(['Iteration ', num2str(Cur_Iter)])
        end
        if ~(mod(Cur_Iter, save_every))
             save(['step_', num2str(Cur_Iter)])
        end
    end

    % Density
    rho=sum(f,3);
    % Velocity
    ux=zeros(Nr,Mc);uy=zeros(Nr,Mc);
    for ic=1:N_c-1
        ux=ux+C_x(ic).*f(:,:,ic);uy=uy+C_y(ic).*f(:,:,ic);
    end
    ux(ija)=ux(ija)./rho(ija); uy(ija)=uy(ija)./rho(ija);

    % Save to file
    save_all(f, feq, ux, uy, rho, ['../test_data/matlab_', out_file, '_stream'], N_c);

    function compute_feq()
        % Dot products computation
        ux(ija)=ux(ija)./rho(ija); uy(ija)=uy(ija)./rho(ija);
        uxsq(ija)=ux(ija).^2;uysq(ija)=uy(ija).^2;
        usq(ija)=uxsq(ija)+uysq(ija);

        rt0=w0.*rho; rt1=w1.*rho; rt2=w2.*rho;

        % Equilibrium distribution
        feq(ija)=rt1(ija).*(1+f1*ux(ija)+f2*uxsq(ija)-f3*usq(ija));
        feq(ija+NxM*(2-1))=rt1(ija).*(1+f1*uy(ija)+f2*uysq(ija)-f3*usq(ija));
        feq(ija+NxM*(3-1))=rt1(ija).*(1-f1*ux(ija)+f2*uxsq(ija)-f3*usq(ija));
        feq(ija+NxM*(4-1))=rt1(ija).*(1-f1*uy(ija)+f2*uysq(ija)-f3*usq(ija));

        % Diagonals (X diagonals) (ic-1)
        feq(ija+NxM*(5-1))= rt2(ija) .*(1 +f1*(+ux(ija)+uy(ija)) +f2*(+ux(ija)+uy(ija)).^2 -f3.*usq(ija));
        feq(ija+NxM*(6-1))= rt2(ija) .*(1 +f1*(-ux(ija)+uy(ija)) +f2*(-ux(ija)+uy(ija)).^2 -f3.*usq(ija));
        feq(ija+NxM*(7-1))= rt2(ija) .*(1 +f1*(-ux(ija)-uy(ija)) +f2*(-ux(ija)-uy(ija)).^2 -f3.*usq(ija));
        feq(ija+NxM*(8-1))= rt2(ija) .*(1 +f1*(+ux(ija)-uy(ija)) +f2*(+ux(ija)-uy(ija)).^2 -f3.*usq(ija));

        feq(ija+NxM*(9-1))=rt0(ija).*(1-f3*usq(ija));

    end
end

function save_all(f, feq, ux, uy, rho, filename, Nc)
    %% Saves the all variables for C++ comparisons
    
    % Shift (different conventions)
    f = circshift(f,1,3);
    feq = circshift(feq,1,3);

    % Save
    for i = 1:Nc
       temp = f(:,:,i);
       save([filename, '_f_', num2str(i-1), '.txt'], 'temp', '-ascii'); 
       temp = feq(:,:,i);
       save([filename, '_feq_', num2str(i-1), '.txt'], 'temp', '-ascii'); 
    end

    save([filename, '_density.txt'], 'rho', '-ascii'); 
    save([filename, '_ux.txt'], 'ux', '-ascii');
    save([filename, '_uy.txt'], 'uy', '-ascii');
end

function f=stream_wObs(f,Nr,Mc,iawint,jawint,lenwint, lenobs,iobs,jobs,Channel2D)
    N_c=9; feq=zeros(Nr,Mc,N_c);
    %Bounce back
    ic_op=[3 4 1 2 7 8 5 6];
    %Periodic BC
    yi2=[Nr,1:Nr,1];
    xi2=[Mc,1:Mc,1];
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    feq = f; % temp storage of f in feq
        for ic=1:1:N_c-1, % select velocity layer
             ic2=ic_op(ic); % selects the layer of the velocity opposite to ic for BB
             temp1=feq(:,:,ic); %

            % from wet location that are NOT on the border to other wet locations
            for ia=1:1:lenwint % number of internal (i.e. not border) wet locations

                i=iawint(ia);  j=jawint(ia);  % so that we care for the wet space only !
                i2 = i+C_y(ic); j2 = j+C_x(ic); % Expected final locations to move

                i2=yi2(i2+1); j2=xi2(j2+1);
                f(i2,j2,ic)=temp1(i,j);

            end ; 

            % from wet locations that ARE on the border of obstacles
            for ia=1:1:lenobs % wet border locations
                i=iobs(ia);  j=jobs(ia);  % so that we care for the wet space only !
                i2 = i+C_y(ic); j2 = j+C_x(ic); % Expected final locations to move
                i2=yi2(i2+1); j2=xi2(j2+1);% i2 corrected for PBC

                if( Channel2D(i2,j2) ==0 ) % i.e the new position (i2,j2) is dry
                    f(i,j,ic2) =temp1(i,j); % invert direction: bounce-back in the opposite direction ic2
                else % otherwise, normal propagation from (i,j) to (i2,j2) on layer ic
                    f(i2,j2,ic)=temp1(i,j); % see circshift(..) fnct for circularly shifts
                end ; % b.b. and propagations

            end ; % i and j single loop
        end 
end


