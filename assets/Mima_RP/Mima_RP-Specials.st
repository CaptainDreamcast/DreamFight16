;---------------------------------------------------------------------------
; Green Spread
[Statedef 1000]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 1000+1*(statetype=A)
poweradd = 15
ctrl = 0
sprpriority = 1
facep2 = 0;1
;juggle = ifelse((var(34)&floor(exp(1*ln(2)) + 0.5)),2,0) ;use Juggle

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(7)=0
value = 1000,0
channel = 0

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(7)=0
value = 1000,10

[State 1000, Effect]
type = helper
trigger1 = animelemtime(7) = 0
helpertype = normal
name = "Projectile"
ID = 21000
pos = 22,-57
postype = p1
stateno = 21000
ownpal = 1
facing = 1
size.xscale = 2.5
size.yscale = 2.5

[State 1000, STS]
type = statetypeset
trigger1 = animelemtime(7)=1
movetype = I
ignorehitpause = 1

[State 1000, CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime = 0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(20)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(20)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(20)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 1001

;---------------------------------------------------------------------------
; Green Spread Landing
[Statedef 1001]
type    = S
movetype= I
physics = S
velset = 0,0
anim = 47
ctrl = 0
sprpriority = 1
movehitpersist=1

[State 1205, PS]
type = posset
trigger1 = time = 0
y = 0

[State 1202, PSND]
type = playsnd
trigger1 = time = 0
value = 40,0

[State 1205, CS]
type = changestate
trigger1 = animtime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Green Spread Projectile
[Statedef 21000]
type    = A
movetype= A
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 1000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Projectile"
ID = 21010
pos = 0,0
postype = p1
stateno = 21010
ownpal = 0
facing = 1
size.xscale = 0.75
size.yscale = 0.75
[State 1000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Projectile"
ID = 21011
pos = 0,0
postype = p1
stateno = 21010
ownpal = 0
facing = 1
size.xscale = 0.75
size.yscale = 0.75
[State 1000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Projectile"
ID = 21012
pos = 0,0
postype = p1
stateno = 21010
ownpal = 0
facing = 1
size.xscale = 0.75
size.yscale = 0.75
[State 1000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Projectile"
ID = 21013
pos = 0,0
postype = p1
stateno = 21010
ownpal = 0
facing = 1
size.xscale = 0.75
size.yscale = 0.75

[State 21000, PSND]
type = playsnd
trigger1 = time = 8;16
value = 1000,11

[State 21000, VS]
type = varset
trigger1 = time = 0
var(10) = root,var(10)

[State 21000, PA]
type = posadd
trigger1 = time=8;16
x = cond(var(10)<=1,32,cond(var(10)>=3,240,132))

[State 21000, CS]
type = changestate
trigger1 = time>=8;16
value = 21001

;---------------------------------------------------------------------------
; Green Spread Projectile
[Statedef 21001]
type    = A
movetype= A
physics = N
velset = 0,0
;anim = 21100
sprpriority=5
ctrl = 0

[State 1010, NHB]
type = nothitby
trigger1 = 1
value = ,AA,AT
ignorehitpause = 1

[State 1010, HitOverride]
Type = HitOverride
Trigger1 = 1
Attr = SCA,AA,AP;,AT
slot = 0
time = -1
Stateno = 21005
IgnoreHitPause = 1

[State 21100, VS]
type = varset
trigger1 = prevstateno != 21005
trigger1 = time = 0
var(10)=root,var(10)

[State 21100, CA]
type = changeanim
trigger1 = time = 0
trigger1 = prevstateno != 21005
trigger1 = anim != 21000
value = 21000

[State 21000, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 0, AngleDraw]
type = AngleDraw
trigger1 = 1
;value = 1
scale = 1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.1,0.0)),1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.1,0.0))
ignorehitpause = 1
;persistent = 


[State 21000, T]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State Movehitoner]
type = VarSet
trigger1 = numenemy
var(31) = ifelse(movehit=1&&(enemynear,movereversed!=[1,2]),var(31)+1,0)
ignorehitpause = 1

[State Give 'em a lil' 40, taken from The_None]
type = parentVarSet
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(34) = (root,var(34)) | floor(exp(1*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 1010, HitDef]
type = Hitdef
triggerall = !((root,var(34))&floor(exp(1*ln(2)) + 0.5))
trigger1 = time
trigger1 = movecontact = 0 && movereversed = 0
attr = S, SP
animtype  = Hard
damage    = ceil(60.0*root,fvar(11)),12
priority  = 4,Hit
;guard.kill = 0
hitflag = MAF
guardflag = MA
pausetime = 0,6
sparkno = s8010+(random%2)
guard.sparkno = s8030
sparkxy = -10,0
hitsound   = s1,2
guardsound = s2,0
ground.type = Low
ground.slidetime = 19
ground.hittime  = 19
air.hittime  = 19
guard.slidetime = 15
guard.hittime = 15
ground.velocity = -10.0
guard.velocity = -10.0
air.velocity = -10.0,-6.0
airguard.velocity = -10.0,-6.0
fall = 0
air.fall = 1
fall.recover = 1
fall.recovertime = 120
getpower = 60,60
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0

[State 200, Damage Dampen]
type = parentvarset
trigger1 = movehit
fvar(10) = (root,fvar(10))*0.85
ignorehitpause = 1
persistent = 0

[State 21100, CS]
type = changestate
trigger1 = frontedgedist < -100
trigger2 = animelemtime(3)=0
value = 21005

[State 21001, VA]
type = varadd
trigger1 = 1
var(0) = 1

;-----------------------
; Spirits Projectile
[Statedef 21005]
type = S
movetype = I
physics = N
;anim = 21001
sprpriority = 5
velset = 0,0
movehitpersist = 1

[State -2, P2 Safe Fall]
type= targetstate
;triggerall = ishelper(1510)+ishelper(1511)+ishelper(1410)+ishelper(1420)+ishelper(1430)+ishelper(1411)+ishelper(1421)+ishelper(1431)+ishelper(1412)+ishelper(1422)+ishelper(1432)+ishelper(1413)+ishelper(1423)+ishelper(1433)+ishelper(1414)+ishelper(1424)+ishelper(1434)
triggerall= numtarget=1; Just to prevent Simul mode shenanigans
triggerall= target,pos y>=-20 && target,vel y>0
triggerall= target,alive && target,hitfall && target,gethitvar(fall.recover)
triggerall= target,stateno=5030 || target,stateno=5035 || target,stateno=5050 || target,stateno=5071
trigger1 = target,AILevel <= 0
trigger1 = target,command="recovery"
trigger2 = target,AILevel > 0
trigger2 = random%100 <= 10*ceil((target,AILevel)*(target,AILevel)/64.0)
value= 5205
ignorehitpause= 1

[State 0, AngleDraw]
type = AngleDraw
trigger1 = time
;value = 1
scale = 1.0+(0.025*var(0))+cond(var(0)%4=1,0.05,cond(var(0)%4=3,-0.1,0.0)),1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.05,0.0))
ignorehitpause = 1

[State 1015, T]
type = trans
trigger1 = animelemtime(3)>=0
trans = addalpha
alpha = 256-floor(256.0*animelemtime(3)/16.0),256
ignorehitpause = 1

[State 1015, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1015, NHB]
type = nothitby
trigger1 = 1
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 1015, Destroyself]
type = changestate
trigger1 = animelemtime(3) >= 16
value = 999

[State 21001, VA]
type = varadd
trigger1 = 1
var(0) = 1

;---------------------------------------------------------------------------
; Green Spread Projectile
[Statedef 21010]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 21001
sprpriority=5
ctrl = 0

[State 1010, VS]
type = varset
trigger1 = time = 0
var(51) = random*1000+random

[State 1000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Tail"
ID = 21011
pos = 0,0
postype = p1
stateno = 21011
ownpal = 0
facing = 1
size.xscale = 0.25*(2.0/3.0)
size.yscale = 0.25*(2.0/3.0)

[State 21000, VS]
type = varset
trigger1 = time = 0
var(10) = root,var(10)

[State 21000, PA]
type = velset
trigger1 = time=0
x = cond(var(10)<=1,2.0,cond(var(10)>=3,15.0,8.5))*2.0

[State 21000, PA]
type = velset
trigger1 = 1
y = -10.0*cos(2*time*11.25*pi/180.0)*(1.0*ishelper(21010)+0.5*ishelper(21011)-0.5*ishelper(21012)-1.0*ishelper(21013))*2

[State 1015, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1015, NHB]
type = nothitby
trigger1 = 1
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 21000, CS]
type = Destroyself
trigger1 = time>=8;16

;-----------------------
; Spirits Tail
[Statedef 21011]
type = A
movetype = I
physics = N
anim = 21010
sprpriority = 5
velset = 0,0

[State 301, Parent Destruct Safeguard]
type = varset
trigger1 = time = 0
var(50) = parent,id ;Set to parent's PlayerID
[State 301, Parent Destruct Safeguard]
type = varset
trigger1 = time = 0
trigger1 = playeridexist(var(50))
var(51) = playerid(var(50)),var(51) ;Set to Parent's unique random identifier
[State 301, Parent Destruct Safeguard]
type = destroyself
;Scenario 1: Parent no longer exists, Helper with Parent's Player ID doesn't exist
trigger1 = !playeridexist(var(50))
;Scenario 2: Helper with Parent's Player ID exists, but unique identifiers don't match
;  (1/1000 chance of false positive)
trigger2 = playeridexist(var(50))
trigger2 = playerid(var(50)),var(51) != var(51)
trigger3 = playeridexist(var(50))
trigger3 = playerid(var(50)),stateno = 999

[State 1010, T]
type = trans
trigger1 = 1
trans = add
ignorehitpause = 1

[State 1010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1215, Tail]
type = AfterImage
trigger1 = time=0
trans = add
length = 15;30
timegap = 1
framegap = 1
time = 9999999
palbright = 0, 0, 0
palcontrast = 90, 90, 90
paladd = 0,0,0
palmul = 0.9,0.9,0.9
ignorehitpause = 1

[State 1215, PFX]
type = Palfx
trigger1 = 1
time = 1
add = -50,-20,-20
mul = 256,256,256
invertall = 0
color = 256
ignorehitpause = 1

[State 1215, BTP]
type = bindtoparent
trigger1 = 1
pos = 0,0
ignorehitpause = 1

[State 1200, AngleS]
type = angleset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),vel x != 0
trigger1 = playerid(var(50)),vel y != 0
value = -atan(playerid(var(50)),vel y/playerid(var(50)),vel x)*180/pi+270

[State 1200, AngleS]
type = angleset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),vel x = 0
value = -270-(playerid(var(50)),vel y <= 0)*180+270

[State 1200, AngleS]
type = angleset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),vel y = 0
value = 0+(playerid(var(50)),vel x < 0)*180+270

[State 1200, AngleS]
type = Varset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),Vel x != 0
trigger1 = playerid(var(50)),Vel y != 0
fvar(0) = playerid(var(50)),Vel x/(cos((atan(playerid(var(50)),Vel y/playerid(var(50)),Vel x)*180/pi)*pi/180))/12.8

[State 1200, AngleS]
type = Varset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),Vel x = 0
trigger1 = playerid(var(50)),Vel y > 0
fvar(0) = playerid(var(50)),Vel x/(cos(270*pi/180))/12.8

[State 1200, AngleS]
type = Varset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),Vel x = 0
trigger1 = playerid(var(50)),Vel y < 0
fvar(0) = playerid(var(50)),Vel x/(cos(90*pi/180))/12.8

[State 1200, AngleS]
type = Varset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),Vel x > 0
trigger1 = playerid(var(50)),Vel y = 0
fvar(0) = playerid(var(50)),Vel x/(cos(360*pi/180))/12.8

[State 1200, AngleS]
type = Varset
triggerall = playeridexist(var(50))
triggerall = playerid(var(50)),var(51) = var(51)
trigger1 = playerid(var(50)),Vel x < 0
trigger1 = playerid(var(50)),Vel y = 0
fvar(0) = playerid(var(50)),Vel x/(cos(180*pi/180))/12.8

[State 1200, AD]
type = angledraw
trigger1 = 1
scale = 2,fvar(0)
ignorehitpause = 1

[State 1210, VS]
type = changestate;destroyself
trigger1 = frontedgedist < -100
trigger2 = backedgedist < -100
trigger3 = !playeridexist(var(50))
trigger4 = playeridexist(var(50))
trigger4 = playerid(var(50)),stateno != 21010
value = 21012;999

;-----------------------
; Spirits Tail
[Statedef 21012]
type = A
movetype = I
physics = N
anim = 1998
sprpriority = 5
velset = 0,0

[State 1010, T]
type = trans
trigger1 = 1
trans = add
ignorehitpause = 1

[State 1010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1210, VS]
type = destroyself
trigger1 = time>30

;---------------------------------------------------------------------------
; Sweep Away
[Statedef 1100]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 1100+1*(var(10)=2)+2*(var(10)>=3)
poweradd = 30
ctrl = 0
sprpriority = 1
facep2 = 0;1
juggle = ifelse((var(34)&floor(exp(2*ln(2)) + 0.5)),2,0) ;use Juggle

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
;triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(34) = var(34) | floor(exp(2*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(4)=0
value = 1000,0
channel = 0

[State 0, Explod]
type = Explod
trigger1 = animelemtime(3)=0
anim = 7000
ID = 7000
pos = 0,0
postype = p1  ;p2,front,back,left,right
facing = 1
vel = 0,0
removetime = -2
pausemovetime = 0
scale = 1,1
sprpriority = 5
ownpal = 1
removeongethit = 0
ignorehitpause = 1

[State 1100, Physics]
type = statetypeset
trigger1 = animelemtime(3)=0
physics = N
ignorehitpause = 1

[State 1100, VS]
type = velset
trigger1 = animelemtime(3)>=0 && animelemtime(4)<0
x = 8.0

[State 1100, Friction]
type = velmul
trigger1 = animelemtime(4)>=0
x = 0.85

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(3)=0
value = 1100,10

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(4)=0
value = 0,1

[State 1200, HITDEF]
type = HitDef
triggerall = statetype!=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(4)>=0 && animelemtime(6)<0
attr = S, SA
animtype  = back
damage    = ceil((80.0+20*(var(10)=2)+40*(var(10)>=3))*fvar(11)*ifelse(var(42),1.25,1.0)),16+4*(var(10)=2)+8*(var(10)>=3)
priority = 4,hit
hitflag = MAF
guardflag = M
pausetime = 10,10
sparkno = S8010+(random%2)
guard.sparkno = S8030
sparkxy = -20,-60
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = low
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 20
guard.hittime  = 20
ground.velocity = -10.0,-7.0
air.velocity = -10.0,-7.0
getpower = 80,80
givepower = 40,40
yaccel = 0.5
fall = 1
fall.recover = 1
fall.recovertime = 120
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 1100

[State 1200, HITDEF]
type = HitDef
triggerall = statetype=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(4)>=0 && animelemtime(6)<0
attr = S, SA
animtype  = back
damage    = ceil((80+30*(var(10)=2)+40*(var(10)>=3))*fvar(11)*ifelse(var(42),1.25,1.0)),16+6*(var(10)=2)+8*(var(10)>=3)
priority = 4,hit
hitflag = HAF
guardflag = HA
pausetime = 10,10
sparkno = S8010+(random%2)
guard.sparkno = S8030
sparkxy = -20,-60
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = low
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 21
guard.hittime  = 21
ground.velocity = -10.0,-7.0
air.velocity = -10.0,-7.0
getpower = 80,80
givepower = 40,40
yaccel = 0.5
fall = 1
fall.recover = 1
fall.recovertime = 120
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 1100

[State 220, Wallslam]
type = targetstate
trigger1 = movehit
persistent = 0
ignorehitpause = 1
id = 1100
value = 260

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.80
ignorehitpause = 1
persistent = 0

[State 1000, STS]
type = statetypeset
trigger1 = movecontact||movereversed
trigger2 = animelemtime(6)=0
movetype = I
ignorehitpause = 1

[State 1000, CS]
type = changestate
trigger1 = statetype != A
trigger1 = animelemtime(12) = 0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(12)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = animelemtime(12)>0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = animelemtime(12)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 1001

;---------------------------------------------------------------------------
; Meteor Strike
[Statedef 1200]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 1200+1*(var(10)=2)+2*(var(10)>=3)
poweradd = 15
ctrl = 0
sprpriority = 1
facep2 = 0;1
;juggle = ifelse((var(34)&floor(exp(1*ln(2)) + 0.5)),2,0) ;use Juggle

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 1200, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype!=A
statetype = S
physics = S

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(1)=0
value = 1200,10
channel = 1

[State 1210, Explod]
type = Explod
trigger1 = animelemtime(1)=0
anim = 21201
ID = 21201
pos = 24,cond(anim=1200,-16,cond(anim=1201,-56,-93))
postype = p1  ;p2,front,back,left,right
facing = 1
bindtime = -1
vel = 0,0
removetime = 14
pausemovetime = 0
scale = 1,1
sprpriority = 5
ontop = 0
ownpal = 1
removeongethit = 1
ignorehitpause = 1

[State 0, ReversalDef]
type = ReversalDef
trigger1 = animelemtime(1)>=0 && animelemtime(3)<0
reversal.attr = SCA,NA,SA,HA           ;SCA,NA,SA,HA,NP,SP,HP,NT,ST,HT
pausetime = 10,10
sparkno = -1
;sparkxy = 0,0 ;offset
hitsound = S1,1
p1stateno = 1201
;ignorehitpause = 
;persistent = 

[State 1000, CS]
type = changestate
trigger1 = movecontact
value = 1201

[State 1000, STS]
type = statetypeset
trigger1 = animelemtime(3)=0
movetype = I
ignorehitpause = 1

[State 1000, Air CS]
type = changestate
trigger1 = animtime = 0
value = cond(statetype=A,301,0)
ctrl = 1

;---------------------------------------------------------------------------
; Meteor Strike - Counter State
[Statedef 1201]
type    = U
movetype= A
physics = N
velset = 0,0
anim = 1210
ctrl = 0
sprpriority = 1
facep2 = 0

[State 1200, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype!=A
statetype = S
physics = S

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(4)=0
value = 1200,11

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(6)=0
value = 1200,12

[State 1201, NHB]
type = nothitby
trigger1 = animelemtime(1)>=0 && animelemtime(8)<0
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 1210, Explod]
type = Explod
trigger1 = animelemtime(4)=0
anim = 21200
ID = 21200
pos = 0,0
postype = p1  ;p2,front,back,left,right
facing = 1
bindtime = -1
vel = 0,0
removetime = -2
pausemovetime = 0
scale = 1,1
sprpriority = 5
ontop = 0
ownpal = 1
removeongethit = 1
ignorehitpause = 0

[State 1001, Unblockable]
type = assertspecial
trigger1 = animelemtime(8)<0
flag = unguardable
ignorehitpause = 1


[State 200, 1]
type = HitDef
triggerall = (movecontact=0&&movereversed=0)
trigger1 = animelemtime(6)>=0 && animelemtime(8)<0
attr = S,SA                     ;Attribute: Standing, Normal Attack
damage = ceil(140.0*fvar(11)),28                   ;Damage that move inflicts, guard damage
animtype = Back                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag =                    ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 8,8                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -15,ceil(p2dist y+enemynear,const(size.mid.pos.y))
hitsound = s1,2                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 23+1*(var(42))             ;Time that the opponent slides back
ground.hittime  = 23+1*(var(42))             ;Time opponent is in hit state
air.hittime  = 23+1*(var(42))             ;Time opponent is in hit state
guard.slidetime = 19
guard.hittime = 19
ground.velocity = -12.0,-7.5            ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -12.0,-7.5           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 80,80
givepower = 40,40
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
fall = 1
fall.recover = 0
guard.dist = 0
;fall.recovertime = 120
p2facing = 1
ID = 1200

[State 220, Wallslam]
type = targetstate
trigger1 = movehit
persistent = 0
ignorehitpause = 1
id = 1200
value = 260

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.70
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = movecontact||movereversed
trigger2 = animelemtime(8)=0
movetype = I
persistent = 0
ignorehitpause = 1

[State 1000, Air CS]
type = changestate
trigger1 = animtime = 0
value = cond(statetype=A,301,0)
ctrl = 1

;---------------------------------------------------------------------------
; Stellar Missile
[Statedef 1300]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = cond(statetype=A,cond(var(10)<=2,1305,1306),cond(var(10)<=2,1300,1301));1300+5*(statetype=A)
poweradd = 15
ctrl = 0
sprpriority = 1
facep2 = 0;1
;juggle = ifelse((var(34)&floor(exp(1*ln(2)) + 0.5)),2,0) ;use Juggle

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 1000, PSND]
type = playsnd
trigger1 = animelemtime(6)=0
value = 1300,10



[State 1000, Effect]
type = helper
trigger1 = animelemtime(6) = 0
helpertype = normal
name = "Projectile"
ID = 21300
pos = 36+2*(anim=1301||anim=1306)-4*(anim=1305),-80+13*(anim=1301||anim=1306)+23*(anim=1305)
postype = p1
stateno = 21300
ownpal = 1
facing = 1
size.xscale = 1.0;2.5
size.yscale = 1.0;2.5

[State 1000, STS]
type = statetypeset
trigger1 = animelemtime(6)=13
movetype = I
ignorehitpause = 1

[State 1000, CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime = 0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(15)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(15)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(15)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 1001

;---------------------------------------------------------------------------
; Stellar Missile Projectile
[Statedef 21300]
type    = A
movetype= A
physics = N
velset = 0,0
anim = 21300
sprpriority=5
ctrl = 0

[State 21000, PSND]
type = playsnd
trigger1 = time = 0
value = 1300,10

[State 21000, VS]
type = varset
trigger1 = time = 0
trigger1 = (var(11):=cond((root,statetype=A),-1,1))||1
var(10) = root,var(10)

[State 21300, CVS]
type = null
triggerall = time = 0
trigger1 = var(10)<=1
trigger1 = (var(1):=70)||1
trigger2 = var(10)=2
trigger2 = (var(1):=35)||1
trigger3 = var(10)>=3
trigger3 = (var(1):=0)||1

[State 21000, PA]
type = velset
trigger1 = 1
x = 12.5*(3.0/2.0)*cos(var(1)*pi/180.0)*0.9**time
y = -12.5*(3.0/2.0)*sin(var(1)*pi/180.0)*var(11)*0.9**time

[State 0, NotHitBy]
type = NotHitBy
trigger1 = 1
value = SCA,AA,AP,AT
time = 1
ignorehitpause = 1
;persistent = 

[State 0, AngleDraw]
type = AngleDraw
trigger1 = 1;time
value = var(1)*var(11)
;scale = 1.0+(0.025*var(0))+cond(var(0)%4=1,0.05,cond(var(0)%4=3,-0.1,0.0)),1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.05,0.0))
ignorehitpause = 1

[State 1015, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1015, T]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State 21300, Zeeky]
type = destroyself
trigger1 = movetype = H
trigger1 = root,stateno != [120,155]

[State 21000, CS]
type = changeanim
trigger1 = time>=8;12
value = 21301

[State 21000, CS]
type = changestate
trigger1 = time>=8;12
value = 21301

;---------------------------------------------------------------------------
; Stellar Missile Projectile
[Statedef 21301]
type    = A
movetype= A
physics = N
velset = 0,0
;anim = 1998
sprpriority=5
ctrl = 0

[State 0, NotHitBy]
type = NotHitBy
trigger1 = 1
value = ,AA,AT
time = 1
ignorehitpause = 1

[State 21301, HitOverride]
Type = HitOverride
Trigger1 = 1
Attr = SCA,AA,AP;,AT
slot = 0
time = -1
Stateno = 21305
IgnoreHitPause = 1

[State 21000, PSND]
type = playsnd
trigger1 = time = 0
value = 1300,11

[State 0, AngleDraw]
type = AngleDraw
trigger1 = time
;value = 1
scale = 1.0+(0.025*var(0))+cond(var(0)%4=1,0.05,cond(var(0)%4=3,-0.1,0.0)),1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.05,0.0))
ignorehitpause = 1

[State 1015, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1015, T]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State Movehitoner]
type = VarSet
trigger1 = numenemy
var(31) = ifelse(movehit=1&&(enemynear,movereversed!=[1,2]),var(31)+1,0)
ignorehitpause = 1

[State Give 'em a lil' 40, taken from The_None]
type = parentVarSet
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(34) = (root,var(34)) | floor(exp(3*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 1010, HitDef]
type = Hitdef
triggerall = !((root,var(34))&floor(exp(3*ln(2)) + 0.5))
trigger1 = animelemtime(1)>=0 && animelemtime(2)<0
trigger1 = movecontact = 0 && movereversed = 0
attr = S, SP
animtype  = Hard
damage    = ceil(75.0*root,fvar(11)),15
priority  = 4,Hit
;guard.kill = 0
hitflag = MAF
guardflag = MA
pausetime = 0,6
sparkno = s8020+(random%2)
guard.sparkno = s8030
sparkxy = -10,0
hitsound   = s1,2
guardsound = s2,0
ground.type = High
ground.slidetime = 19
ground.hittime  = 19
air.hittime  = 19
guard.slidetime = 15
guard.hittime = 15
ground.velocity = -10.0,-6.0
guard.velocity = -10.0
air.velocity = -10.0,-6.0
airguard.velocity = -10.0,-6.0
fall = 1
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p1sprpriority = 5
getpower = 60,60
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
id = 1300

[State 200, Damage Dampen]
type = parentvarset
trigger1 = movehit
fvar(10) = (root,fvar(10))*0.80
ignorehitpause = 1
persistent = 0


[State 21000, CS]
type = changestate
trigger1 = time>=16
value = 21305

[State 21001, Custom Timer]
type = varadd
trigger1 = 1
var(0) = 1

;-----------------------
; Spirits Projectile
[Statedef 21305]
type = S
movetype = I
physics = N
;anim = 21001
sprpriority = 5
velset = 0,0
movehitpersist = 1

[State -2, P2 Safe Fall]
type= targetstate
;triggerall = ishelper(1510)+ishelper(1511)+ishelper(1410)+ishelper(1420)+ishelper(1430)+ishelper(1411)+ishelper(1421)+ishelper(1431)+ishelper(1412)+ishelper(1422)+ishelper(1432)+ishelper(1413)+ishelper(1423)+ishelper(1433)+ishelper(1414)+ishelper(1424)+ishelper(1434)
triggerall= numtarget=1; Just to prevent Simul mode shenanigans
triggerall= target,pos y>=-20 && target,vel y>0
triggerall= target,alive && target,hitfall && target,gethitvar(fall.recover)
triggerall= target,stateno=5030 || target,stateno=5035 || target,stateno=5050 || target,stateno=5071
trigger1 = target,AILevel <= 0
trigger1 = target,command="recovery"
trigger2 = target,AILevel > 0
trigger2 = random%100 <= 10*ceil((target,AILevel)*(target,AILevel)/64.0)
value= 5205
ignorehitpause= 1

[State 0, AngleDraw]
type = AngleDraw
trigger1 = time
;value = 1
scale = 1.0+(0.025*var(0))+cond(var(0)%4=1,0.05,cond(var(0)%4=3,-0.1,0.0)),1.0+(0.025*var(0))+cond(var(0)%4=1,0.1,cond(var(0)%4=3,-0.05,0.0))
ignorehitpause = 1

[State 1015, T]
type = trans
trigger1 = animelemtime(2)>=0
trans = addalpha
alpha = 256-floor(256.0*animelemtime(2)/16.0),256
ignorehitpause = 1

[State 1015, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1015, NHB]
type = nothitby
trigger1 = 1
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 1015, Destroyself]
type = changestate
trigger1 = animelemtime(2) >= 16
value = 999

[State 21001, VA]
type = varadd
trigger1 = 1
var(0) = 1