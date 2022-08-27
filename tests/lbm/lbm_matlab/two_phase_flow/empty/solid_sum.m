function [Fxs Fys]=solid_sum(lena,iabw1,jabw1,Channel2D,Nr,Mc)
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
    % Function for computing fluid-solid interactions
    % ---------------------------------------------------------------------
    % Last modified: August 17 2014
    % ---------------------------------------------------------------------
	% Input: lena (total number of fluid nodes), iabw1 and jabw1 (i,j
	% indexes of fluid locations), Channel2D (logical array representing 
    % the domain with 0 indicating solid node and 1 - fluid node), Nr
    % (number of rows - domain length), Mc - number of columns (domain
    % width)
    % ---------------------------------------------------------------------
    % Output: matrix of x and y fluid-solid interaction force components
    % with 0 being no interaction and 1 being a region with a non-zero
    % interaction force
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %
    s=zeros(Nr,Mc); 
    N_c=9;
    yi2=[Nr,1:Nr,1];
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    w3=1/9; w4=1/36;
    Wpsi=[w3 w3 w3 w3 w4 w4 w4 w4 0];

    F_xs=zeros(Nr,Mc,N_c); F_ys=F_xs;

	for ic=1:1:N_c-1
		for ia=1:1:lena 
            i=iabw1(ia);  j=jabw1(ia); 
            i3 = i+C_y(ic); j3 = j+C_x(ic);                            
            i4=yi2(i3+1);
            % If node is a wall - firce becomes non-zero
            if Channel2D(i4,j3)==0
                s(i4,j3)=1;
            else
                s(i4,j3)=0;
            end    
            F_xs(i,j,ic)=Wpsi(ic)*C_x(ic)*s(i4,j3);
            F_ys(i,j,ic)=Wpsi(ic)*C_y(ic)*s(i4,j3);
         end
	end
	Fxs=sum(F_xs(:,:,1:8),3);
    Fys=sum(F_ys(:,:,1:8),3);
end