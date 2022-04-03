%% Generate equilibrium distribution to compare with the C++ output
function laminar_test(channel_file, f_file, omega, out_file)
    
    % Load the geometry
    Channel2D = load(channel_file);
    
    % Load f
    N_c = 9;
    f = load_f(f_file, N_c);
    
    % Force 
    dPdL = 1e-5;
    force=-dPdL*(1/6)*[0 -1 0 1 -1 -1 1 1 0]';
    
    % Compute the equlibrium distribution function for given distribution
    % function f and geometry
    [Nr Mc N_c]=size(f);
    % Compute indices for streaming
    Obstacles=bwperim(Channel2D,8); % perimeter of the grains for bounce back Bound.Cond.
    border=logical(ones(Nr,Mc));
    Obstacles=Obstacles.*(border);
    
    [iobs jobs]=find(Obstacles);lenobs=length(iobs); ijobs= (jobs-1)*Nr+iobs; 
    [iawint jawint]=find(( Channel2D==1 & ~Obstacles)); 
    lenwint=length(iawint);
    ijaint= (jawint-1)*Nr+iawint;
    
    [iabw1 jabw1]=find(f(:,:,9)~=0);
    lena=length(iabw1);
    ija=(jabw1-1)*Nr+iabw1; % Linear index
    % Density
    rho=sum(f,3);
	% Dot products etc declarations 
    uxsq=zeros(size(rho)); uysq=zeros(size(rho)); usq=zeros(size(rho));
    feq=zeros(size(f));
    ux=zeros(size(rho));uy=zeros(size(rho));
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    % Density weights
    w0=16/36.;w1=4/36.;w2=1/36.;
    W=[w1 w1 w1 w1 w2 w2 w2 w2 w0];
    cs2=1/3;cs2x2=2*cs2;cs4x2=2*cs2.^2;
    f1=3.; f2=4.5; f3=1.5;
    NxM=Nr*Mc;
    
    % Analytical solution
    wb = 1;
    Channel_2D_half_width = (Mc-2)/2.0;
    uy_fin_max=dPdL*(Channel_2D_half_width.^2)/(2*1/6);
    uyf_av=uy_fin_max*(2/3); % Average velocity.
    x_profile=([-Channel_2D_half_width:+Channel_2D_half_width-1]+0.5);
    uy_analy_profile=uy_fin_max.*(1-(x_profile/Channel_2D_half_width).^2);
    uy_analy_profile=[zeros(1,wb),uy_analy_profile,zeros(1,wb)];
    x_pro_fig=[[x_profile(1)-[wb:-1:1]],[x_profile,[1:wb]+x_profile(end)]];
    
    % While - main time evolution loop
    Max_Iter=1500;
    disp_every = 500; 
    save_every=15000;
    Cur_Iter=0;
    StopFlag=false;
    tic
    while (Cur_Iter <= Max_Iter)
        Cur_Iter=Cur_Iter+1;
        
        % Velocity
        ux=zeros(Nr,Mc);uy=zeros(Nr,Mc);
        for ic=1:N_c-1
            ux=ux+C_x(ic).*f(:,:,ic);uy=uy+C_y(ic).*f(:,:,ic);
        end
        % Density
        rho=sum(f,3);

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

        % Collision
        f=(1.-omega).*f+omega.*feq;

        % Add force
        for ic=1:N_c;
            for ia=1:lena
                i=iabw1(ia); j=jabw1(ia);
                f(i,j,ic)=f(i,j,ic)+force(ic);
            end
        end

        % Stream
         f=stream_wObs(f,Nr,Mc,iawint,jawint,lenwint, lenobs,iobs,jobs,Channel2D);
         
%         if ~(mod(Cur_Iter, disp_every))
%              disp(['Iteration ', num2str(Cur_Iter)])
%         end
%         if ~(mod(Cur_Iter, save_every))
%              save(['step_', num2str(Cur_Iter)])
%         end
    end
    toc
    % Save the equilibrium distribution function to be used in C++
    save_f(f, out_file, N_c);
end

function f = load_f(file, Nc)
    %% Loads the distribution function generated with C++ part
    for i = 1:Nc
       f(:,:,i) = load([file, '_', num2str(i-1), '.txt']); 
    end
    % Shift (different conventions)
    f = circshift(f,1,3);
end

function save_f(f, filename, Nc)
    %% Saves the f grid for C++ comparisons
    
    % Shift (different conventions)
    f = circshift(f,1,3);
    
    % Save
    for i = 1:Nc
       temp = f(:,:,i);
       save([filename, '_', num2str(i-1), '.txt'], 'temp', '-ascii'); 
    end
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


