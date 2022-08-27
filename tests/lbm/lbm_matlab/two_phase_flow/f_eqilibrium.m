function feq=f_eqilibrium(ux,uy,rho,ija,NxM,Nr,Mc,N_c)
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
    % Function for computing equilibrium density distribution
    % ---------------------------------------------------------------------
    % Last modified: August 17  2014
    % ---------------------------------------------------------------------
	% Input: ux, uy (equlibrium velocity components), rho (macroscopic
	% density), ija (all fluid nodes - linear index), NxM (just Nr*Mc), 
    % Nr (number of rows - domain length), Mc - number of columns (domain
    % width), N_c - number of dimensions in the lattice, D2Q{N_c}
    % ---------------------------------------------------------------------
    % Output: equilibrium density distribution
    % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %    
    
    feq=zeros(Nr,Mc,N_c);
    uxsq=zeros(Nr,Mc);uysq=zeros(Nr,Mc);usq=zeros(Nr,Mc);
    w0=16/36.;w1=4/36.;w2=1/36.;
    f1=3.; f2=4.5; f3=1.5;
     
    uxsq(ija)=ux(ija).^2;uysq(ija)=uy(ija).^2;
    usq(ija)=uxsq(ija)+uysq(ija);
    
    rt0=w0.*rho; rt1=w1.*rho; rt2=w2.*rho;
    
    % --- EQUILIBRIUM DENSITY DISTRIBUTION
    % Non-diagonal directions
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