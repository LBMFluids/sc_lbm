function [Fx Fy]=Fsum(Nr,Mc,iabw,jabw,lena,psi,Up,Low)
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
    % Function for computing sums for multicomponent interactions
    % ---------------------------------------------------------------------
    % Last modified: August 17  2014
    % ---------------------------------------------------------------------
	% Input: Nr (number of rows - domain length), Mc (number of columns,  
    % domain width), iabw and jabw (i,j indexes of fluid nodes), lena
    % (total number of fluid nodes), psi (fluid density), Up and Low (upper
    % and lower density buffers)
    % ---------------------------------------------------------------------
    % Output: force compoenents of repulsive fluid-fluid interactions 
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
    N_c=9;
    Nc=N_c;
    F_x=zeros(Nr,Mc,N_c);
    F_y=zeros(Nr,Mc,N_c);

    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    w3=1/9; w4=1/36;
    Wpsi=[w3 w3 w3 w3 w4 w4 w4 w4 0];

    for ic=1:Nc-1
        for ia=1:lena
            i=iabw(ia); j=jabw(ia);
            i2 = i+C_y(ic); j2 = j+C_x(ic); 
            if(i2<1)
                F_x(i,j,ic)=Wpsi(ic)*Up(j2)*C_x(ic);
                F_y(i,j,ic)=Wpsi(ic)*Up(j2)*C_y(ic);
            elseif(i2>Nr)
                F_x(i,j,ic)=Wpsi(ic)*Low(j2)*C_x(ic);
                F_y(i,j,ic)=Wpsi(ic)*Low(j2)*C_y(ic);
            else
                F_x(i,j,ic)=Wpsi(ic)*psi(i2,j2)*C_x(ic);
                F_y(i,j,ic)=Wpsi(ic)*psi(i2,j2)*C_y(ic);
            end
        end
    end
    Fx=sum(F_x(:,:,1:8),3);
    Fy=sum(F_y(:,:,1:8),3);
end