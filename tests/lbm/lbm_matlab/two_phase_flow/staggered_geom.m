% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
% Program for creating interior geometry with staggered circular pillars in
% an equlateral arrangement
% -------------------------------------------------------------------------
% Last modified: October 30 2014
% -------------------------------------------------------------------------
% Input: none
% -------------------------------------------------------------------------
% Output: acts like a macrosubstitution when called from the flow programs
% (bubble_in_a_cage.m, no_cage.m) - in the future will be turned into
% function; all the changes to interior geometry go here
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % %

% --- INITIALIZATION OF THE DOMAIN
% Domain with walls and no posts
Len_Channel_2D=1014*1.5; 
Channel_2D_half_width=2.5*214; Width=Channel_2D_half_width*2;
% Fluid nodes
Channel2D=ones(Len_Channel_2D,Width);
% Solid nodes
Channel2D(:,1)=0; Channel2D(:,end)=0; 
[Nr Mc]=size(Channel2D);

% --- PROPERTIES OF THE PILLARS
% Circle radius (pillar radius)
Rc=10;
% Number of pillar rows and pillars in each row
Np_rws=20; Np_clm=20;
% Distance from domain boundaries and coordinates of 
% the first pillar center
y_dist=317; x_dist=2*Rc;
% Distance between circle/pillar centers
r_dist=50; 
htr=ceil(0.5*r_dist*sqrt(3));
% Circle post-filler - for more resolved curvature
nf=2; 

% --- CREATE THE FIRST PILLAR
% First pillar seed
Channel2D(y_dist,x_dist)=0;
% Select the grid around the pillar and pick points that will be part of
% the pillar
ch_ind_x=1:x_dist+ceil(1.5*Rc);
ch_ind_y=1:y_dist+ceil(1.5*Rc);
[xc,yc]=meshgrid(ch_ind_x,ch_ind_y);
[Nch,Mch]=size(xc);
for chy=1:Nch
    for chx=1:Mch
        % If within a circle, include in the pillar - mark as solid (0.0)
        % in Channel2D
        posc=(xc(chy,chx)-x_dist)^2+(yc(chy,chx)-y_dist)^2;
        if posc<=Rc^2
            Channel2D(chy,chx)=0;
        end
    end
end

% --- IMPROVE CURVATURE RESOLUTION OF THE FIRST PILLAR
% When a pillar is first created there is a single solid node sticking out
% of it on each of it's sides; this part adds nf nodes to the pillar on
% each side of the solitary node for all 4 sides of the pillar
[ifl,jfl]=find(Channel2D==0);
kx=jfl(find(jfl~=1&jfl~=Mc));
ky=ifl(find(jfl~=1&jfl~=Mc));
% Left filler (near wall)
fL=min(kx); fL2=fL+1; fL3=(ky(find(kx==fL2))); 
Channel2D(min(fL3)+nf:max(fL3)-nf,fL)=0;
% Right filler
[ifl,jfl]=find(Channel2D==0);
kx=jfl(find(jfl~=1&jfl~=Mc));
ky=ifl(find(jfl~=1&jfl~=Mc));
fL=max(kx); fL2=fL-1; fL3=(ky(find(kx==fL2))); 
Channel2D(min(fL3)+nf:max(fL3)-nf,fL)=0;
% Upper filler
[ifl,jfl]=find(Channel2D==0);
kx=jfl(find(jfl~=1&jfl~=Mc));
ky=ifl(find(jfl~=1&jfl~=Mc));
fL=max(ky); fL2=fL-1; fL3=(kx(find(ky==fL2))); 
Channel2D(fL,min(fL3)+nf:max(fL3)-nf)=0;
% Lower filler
[ifl,jfl]=find(Channel2D==0);
kx=jfl(find(jfl~=1&jfl~=Mc));
ky=ifl(find(jfl~=1&jfl~=Mc));
fL=min(ky); fL2=fL+1; fL3=(kx(find(ky==fL2))); 
Channel2D(fL,min(fL3)+nf:max(fL3)-nf)=0;

% --- CREATE REMAINING PILLARS
% Create the pillars identical to the first pillar by directly copying 
% it's nodes through the entire array
[iz,jz]=find(Channel2D(1:Nch,2:Mch)==0);
for cp=0:Np_rws
    for cpr=0:Np_clm
        % For staggered arrangement
        if (mod((cp+1),2)==0)
            for cpi=1:length(iz)
                if (max(iz)+htr*(cp))<Nr&(max(jz)+ceil(r_dist*(cpr+1/2)))<Mc
                    Channel2D((iz(cpi)+htr*(cp)),(jz(cpi)+ceil(r_dist*(cpr+1/2))))=0;
                end
            end
        else
            for cpi=1:length(iz)
                Channel2D((iz(cpi)+htr*(cp)),(jz(cpi)+r_dist*cpr))=0;
            end
        end
    end
end

% --- ADD SIDE WALLS
Channel2D(298:335,2:Mc-1)=1;

% --- UNCOMMENT FOR VISUALIZATION
% spy(Channel2D==0)
