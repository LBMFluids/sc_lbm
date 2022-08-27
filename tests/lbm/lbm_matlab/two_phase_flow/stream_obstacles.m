function f=stream_obstacles(f,Channel2D)
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
    % Function for streaming step of the LBM algorithm
    % ---------------------------------------------------------------------
    % Last modified: August 17 2014
    % ---------------------------------------------------------------------
	% Input: f (density ditribution function) and Channel2D (logical 
    % array representing the domain with 0 indicating solid 
    % node and 1 - fluid node)
    % ---------------------------------------------------------------------
    % Output: density distribution after the streaming step
    % ---------------------------------------------------------------------
    % i: rows, j: columns
    % Each partition goes from 1 to the partition length (local Nr)
    % i.e indexing is local indexing
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
    
    % --- SETTINGS
    [Nr Mc Nc]=size(f);
    % Indexing
    Channel2D=logical(Channel2D);
    % Obstacles for Bounce Back (in front of the grain)
    % Perimeter of the grains for bounce back BC:
    Obstacles=bwperim(Channel2D,8); 
    % Obstacles 
    [iobs jobs]=find(Obstacles);lenobs=length(iobs); ijobs=(jobs-1)*Nr+iobs; 
    % Internal fluid locations : fluid locations & ~obstables
    [iawint jawint]=find(( Channel2D==1 & ~Obstacles)); 
    lenwint=length(iawint); 
    ijaint= (jawint-1)*Nr+iawint;
    NxM=Nr*Mc;

    % Declarations
    f_temp=f;

    % Other quantities
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    
    % Bounce back directions
    ic_op=[3 4 1 2 7 8 5 6];

    % --- STREAMING
    for ic=1:Nc-1
        temp1=f_temp(:,:,ic);
        ic2=ic_op(ic);
        for ia=1:1:lenwint
            i=iawint(ia);  j=jawint(ia);
            if(ic~=6&&ic~=2&&ic~=5)
                if(i==Nr)
                    i2 = i+C_y(ic);j2 = j+C_x(ic); 
                    f(i2,j2,ic)=temp1(i,j);
                elseif(i>1&&i<Nr)
                    i2 = i+C_y(ic); j2 = j+C_x(ic); 
                    f(i2,j2,ic)=temp1(i,j);
                end
            elseif(ic==6||ic==2||ic==5)
                if(i==1)
                    i2=i+C_y(ic); j2 = j+C_x(ic); 
                    f(i2,j2,ic)=temp1(i,j);
                elseif(i>1&&i<Nr)
                    i2 = i+C_y(ic); j2 = j+C_x(ic); 
                    f(i2,j2,ic)=temp1(i,j);
               end
            end
        end
       for ia=1:1:lenobs
            i=iobs(ia);  j=jobs(ia);
            if(ic~=6&&ic~=2&&ic~=5)
                if(i==Nr)
                    i2 = i+C_y(ic);j2 = j+C_x(ic); 
                    % Bounce back
                    if( Channel2D(i2,j2) ==0 )
                        f(i,j,ic2)=temp1(i,j);
                    else
                        f(i2,j2,ic)=temp1(i,j);
                    end
                elseif(i>1&&i<Nr)
                    i2 = i+C_y(ic); j2 = j+C_x(ic); 
                    % Bounce back
                    if( Channel2D(i2,j2) ==0 )
                        f(i,j,ic2)=temp1(i,j);
                    else
                        f(i2,j2,ic)=temp1(i,j);
                    end
                end
            elseif(ic==6||ic==2||ic==5)
                if(i==1)
                    i2=i+C_y(ic); j2 = j+C_x(ic); 
                    % Bounce back
                    if( Channel2D(i2,j2) ==0 )
                        f(i,j,ic2)=temp1(i,j);
                    else
                        f(i2,j2,ic)=temp1(i,j);
                    end
                elseif(i>1&&i<Nr)
                    i2 = i+C_y(ic); j2 = j+C_x(ic); 
                    % Bounce back
                    if( Channel2D(i2,j2) ==0 )
                        f(i,j,ic2)=temp1(i,j);
                    else
                        f(i2,j2,ic)=temp1(i,j);
                    end
               end
            end
        end
    end

end

