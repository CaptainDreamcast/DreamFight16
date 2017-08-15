
;---------------------------------------------------------------------------
; "Orreries Sun"
[Statedef 2000]
type    = U
movetype= I
physics = N
velset = 0,0
anim = 2000+1*(statetype=A)
ctrl = 0
sprpriority = 1
facep2 = 0;1

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
trigger1 = animelemtime(1)=0
value = 2000,0
channel = 0

[State 2000, IKEMEN Safeguard];IKEMEN ignores unhittable=1 in the superpause for some dumb fucking reason...
;actually this was fixed but fuck it, I'm leaving it in anyway in case some didnt update their IKEMEN or whatever.
type = nothitby
trigger1 = time = 0
value = SCA,AA,AP,AT
time = 74;40
ignorehitpause = 1

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 2000,0
channel = 0

[State 2000, VS]
type = varset
trigger1 = time=0
trigger1 = fvar(10)<1.0
fvar(10)=fvar(10)*0.925

[State 4000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Blackout Effect"
ID = 701
pos = 0,0
postype = p1
stateno = 701
ownpal = 1
facing = 1
pausemovetime = 74;40
supermovetime = 74;40

[State 2000, Spellcard Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Spellcard_FX"
id = 1999
stateno = 1999
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 74;40
supermovetime = 74;40

[state 1010, Za Warudo!]
type = SuperPause
trigger1 = time = 0
time = 74;40
anim = -1
sound = S4,0
p2defmul = 1
poweradd = -1000
unhittable = 1
movetime = 74;40
darken = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(7)=0
value = 2000,10

[State 2000, Spellcard Effect]
type = helper
trigger1 = animelemtime(7)=0
trigger1 = (var(39):=600)||1 ;SET ORRERIES TIMER
trigger1 = (var(38):=6)||1 ;SET ORRERIES # OF HITS
helpertype = normal
name = "Orreries"
id = 22000
stateno = 22000
pos = 10,-60
postype = p1
ownpal = 1
facing = 1
pausemovetime = 26
supermovetime = 26

[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(16)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(16)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(16)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; Orreries Sun Landing
[Statedef 2001]
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
; "Orreries Sun" Launch
[Statedef 2010]
type    = U
movetype= A
physics = N
velset = 0,0
anim = 2010+1*(statetype=A)
ctrl = 0
sprpriority = 1
facep2 = 0;1

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
trigger1 = time=0
trigger1 = (var(39):=0) || 1
value = 2000,0
channel = 0

[State 2000, IKEMEN Safeguard];IKEMEN ignores unhittable=1 in the superpause for some dumb fucking reason...
;actually this was fixed but fuck it, I'm leaving it in anyway in case some didnt update their IKEMEN or whatever.
type = nothitby
trigger1 = time = 0
value = SCA,AA,AP,AT
time = 40
ignorehitpause = 1

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 2000,0
channel = 0

[State 2000, VS]
type = varset
trigger1 = time=0
trigger1 = fvar(10)<1.0
fvar(10)=fvar(10)*0.925

[State 4000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Blackout Effect"
ID = 701
pos = 0,0
postype = p1
stateno = 701
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[State 2000, Spellcard Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Spellcard_FX"
id = 1999
stateno = 1999
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[state 1010, Za Warudo!]
type = SuperPause
trigger1 = time = 0
time = 40
anim = -1
sound = S4,0
p2defmul = 1
unhittable = 1
movetime = 40
darken = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(14)=0
value = 2000,11

[State 2010, STS]
type = statetypeset
trigger1 = animelemtime(14)=1
movetype = I

[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(27)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(27)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(27)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22000]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 22000, VS]
type = varset
trigger1 = time = 0
trigger2 = root,power < var(1)
var(1) = root,power

[State 1010, VS]
type = varset
trigger1 = time = 0
var(51) = random*1000+random

[State 22000, BTR]
type = bindtoroot
trigger1 = 1
pos = 0,-60
ignorehitpause = 1

[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22010
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22011
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22012
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22013
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22014
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orreries"
id = 22015
stateno = 22010
pos = 0,0
postype = p1
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
pausemovetime = 30
supermovetime = 30

[State 1000, 5]
type = Projectile
triggerall = root,var(39)<600
triggerall = (root,stateno != 2000) && (root,stateno!=2001)
triggerall = root,var(38)>0
trigger1 = (root,var(39))%10=9;6=5;10=9
;persistent = 10
attr = S,HA
projanim = 22010
projid = 22010
projremove = 1
projremovetime = 6
projscale = 1.0,1.0
projpriority = 9999
velocity = 0
offset = 0,0
animtype  = Hard
damage = ceil(34.5*root,fvar(11)),4
priority  = 7,Hit
hitflag = MAF
guardflag = MA
pausetime = 0,6;10
sparkxy = floor(p2dist x),0
sparkno = s8010+(random%2)
guard.sparkno = s8030
hitsound   = s1,2
guardsound = s2,0
ground.type = High
ground.slidetime = 6;14
ground.hittime  = 6;14
guard.slidetime = 1;9
ground.slidetime = 1;9
ground.velocity = -1.0,0.0
guard.velocity = -1.0
air.velocity = -1.0,-4.5
airguard.velocity = -1.0, -4.5
fall = 0
air.fall=1
fall.recover=0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
getpower = 0,0
givepower = 25,25
id = 22010
guard.dist = 120
p2facing = 1

;[State 200, Orreries Dampen]
;type = parentvarset
;trigger1 = root,projhit22010=1
;trigger1 = (var(38):=(var(38)-1))||1
;fvar(10) = (fvar(10))*0.95
;ignorehitpause = 1
[State 22000, BTR]
type = changestate
trigger1 = root,stateno = 2010
value = 22020

[State 22000, BTR]
type = changestate
trigger1 = root,var(39)<=0
value = 22001

;---------------------------------------------------------------------------
; Orreries Center
[Statedef 22001]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 22000, VS]
type = varset
trigger1 = time = 0
trigger2 = root,power < var(1)
var(1) = root,power

[State 22000, BTR]
type = destroyself
trigger1 = time>=8

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22010]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 22000+1*ishelper(22011)+2*ishelper(22012)+3*ishelper(22013)+4*ishelper(22014)+5*ishelper(22015)
sprpriority=5
ctrl = 0

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

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22010, Starting]
type = null
triggerall = time = 0
trigger1 = ishelper(22010)
trigger1 = (var(1):=0)||1
trigger2 = ishelper(22011)
trigger2 = (var(1):=60)||1
trigger3 = ishelper(22012)
trigger3 = (var(1):=120)||1
trigger4 = ishelper(22013)
trigger4 = (var(1):=180)||1
trigger5 = ishelper(22014)
trigger5 = (var(1):=240)||1
trigger6 = ishelper(22015)
trigger6 = (var(1):=300)||1

[State 22000, BTR]
type = bindtoroot
trigger1 = 1
pos = floor(cond(time>=6,60.0,10.0*time)*cos(var(1)*pi/180.0)),-60+floor(cond(time>=6,60.0,10.0*time)*sin(var(1)*pi/180.0))
ignorehitpause = 1

[State 22000, BTR]
type = changestate
trigger1 = root,stateno = 2010
value = 22025

[State 22000, BTR]
type = changestate
trigger1 = root,var(39)<=0
value = 22011

[State 22010, Timer]
type = varadd
trigger1 = 1
var(1) = 6

;---------------------------------------------------------------------------
; Orreries Orb
[Statedef 22011]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22000, BTR]
type = bindtoroot
trigger1 = 1
pos = floor(60.0*cos(var(1)*pi/180.0)),-60+floor(60.0*sin(var(1)*pi/180.0))
ignorehitpause = 1

[State 22011, Trans]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256-floor(256.0*time/8.0),floor(256.0*time/8.0)
ignorehitpause = 1

[State 22000, BTR]
type = destroyself
trigger1 = time>=8

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22020]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 1998+0*floor(var(0):=0)+0*floor(var(1):=0)+0*floor(var(2):=0)
sprpriority=5
ctrl = 0

[State 22000, BTR]
type = bindtoroot
trigger1 = 1
pos = 0,-60
ignorehitpause = 1

[State 22000, BTR]
type = bindtoroot
trigger1 = root,stateno = 2010
trigger1 = root,animelemtime(14)>=0
pos = 20,-56
ignorehitpause = 1

[State 22000, CA]
type = changeanim
trigger1 = root,stateno = 2010
trigger1 = root,animelemtime(14)>=0
trigger1 = anim != 22020
value = 22020

[State 22020, NHB]
type = nothitby
trigger1 = 1
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 0, HitOverride]
type = HitOverride
trigger1 = 1
attr = SCA,AA,AP        ;SCA,NA,SA,HA,NP,SP,HP,NT,ST,HT
slot = 0
stateno = 22023
time = -1
forceair = 0
ignorehitpause = 1 
;persistent = 

[State 22020, CS]
type = changestate
trigger1 = root,stateno = 2010
trigger1 = root,animelemtime(14)>=0
value = 22021

[State 22020, CS]
type = changestate
trigger1 = root,stateno != 2010
trigger2 = root,movetype = H
value = 22001

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22021]
type    = A
movetype= A
physics = N
velset = 0,0
sprpriority=5
ctrl = 0
movehitpersist = 1

[State 22021, VS]
type = velset
trigger1 = var(0)>0
x = cond(var(2)=0,6.0,0.0)

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
Stateno = 22022
IgnoreHitPause = 1

[State 21000, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22021, HitDef]
type = Hitdef
trigger1 = var(1)<=5 && var(2)=0
trigger1 = movecontact = 0 && movereversed = 0
attr = A, HP
animtype  = Hard
damage    = ceil(33.5*root,fvar(11)),6
priority  = 4,Hit
;guard.kill = 0
hitflag = MAFP
guardflag = MA
pausetime = 0,7
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
ground.velocity = -6.0,-4.0
guard.velocity = -6.0
air.velocity = -6.0,-4.0
airguard.velocity = -6.0,-4.0
fall = 1
fall.recover = 0
p1sprpriority = 5
getpower = 0,0
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
id = 2010

[State 200, Damage Dampen]
type = parentvarset
trigger1 = movehit=1
fvar(10) = (root,fvar(10))*0.92

[State 22021, CS]
type = changestate
trigger1 = movecontact || movereversed
value = 22022

[State 22021, CS]
type = changestate
trigger1 = frontedgedist < -100
trigger2 = var(1)>=6
value = 22023

[State 22021, Custom Timer]
type = varadd
trigger1 = 1
var(0) = 1

[State 22021, Hitdef Cooldown]
type = varadd
trigger1 = var(2)>0
var(2) = -1

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22022]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0
movehitpersist = 1

[State 22021, VS]
type = velset
trigger1 = var(0)>0
x = 6.0

[State 22022, VS]
type = varadd
trigger1 = time = 0
var(1) = 1
[State 22022, VS]
type = varset
trigger1 = time = 0
var(2) = 6

[State 1010, NHB]
type = nothitby
trigger1 = 1
value = SCA,AA,AP,AT
ignorehitpause = 1

[State 1010, HitOverride]
Type = HitOverride
Trigger1 = 1
Attr = SCA,AA,AP;,AT
slot = 0
time = -1
Stateno = 22022
IgnoreHitPause = 1

[State 21000, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22021, MHR]
type = movehitreset
trigger1 = time = 0
trigger1 = var(1)<=5

[State 22022, CS]
type = changestate
trigger1 = time;>=6
value = 22021+2*(var(1)>=6)

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22023]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0
movehitpersist = 1

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22000, BTR]
type = destroyself
trigger1 = time>=8


;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22025]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0

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

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22000, BTR]
type = bindtoparent
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),var(51) = var(51)
pos = floor(cond(time<10,60.0-3.0*time,30.0)*cos(var(1)*pi/180.0)),floor(cond(time<10,60.0-3.0*time,30.0)*sin(var(1)*pi/180.0))
ignorehitpause = 1

[State 22020, CS]
type = changestate
trigger1 = root,stateno = 2010
trigger1 = root,animelemtime(14)>=0
value = 22026

[State 22020, CS]
type = changestate
trigger1 = root,stateno != 2010
trigger2 = root,movetype = H
value = 22011

[State 22010, Timer]
type = varadd
trigger1 = 1
var(1) = 6

;---------------------------------------------------------------------------
; Orreries Projectile
[Statedef 22026]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0

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

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22000, BTR]
type = bindtoparent
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),var(51) = var(51)
pos = floor(30.0*cos(var(1)*pi/180.0)),floor(30.0*sin(var(1)*pi/180.0))
ignorehitpause = 1

[State 22026, CS]
type = changestate
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),var(51) = var(51)
trigger1 = (playerid(var(50)),stateno != [22020,22022])
value = 22027

[State 22010, Timer]
type = varadd
trigger1 = 1
var(1) = 9;6

;---------------------------------------------------------------------------
; Orreries Orb
[Statedef 22027]
type    = A
movetype= I
physics = N
velset = 0,0
sprpriority=5
ctrl = 0

[State 22010, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22011, Trans]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256-floor(256.0*time/8.0),floor(256.0*time/8.0)
ignorehitpause = 1

[State 22000, BTR]
type = destroyself
trigger1 = time>=8

;---------------------------------------------------------------------------
; "Escape Velocity"
[Statedef 2100]
type    = U
movetype= A
physics = N
velset = 0,0
anim = 2100+1*(statetype=A)
ctrl = 0
sprpriority = 1
facep2 = 0;1

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
trigger1 = animelemtime(1)=0
value = 2000,0
channel = 0

[State 2000, Startup Inv]
type = nothitby
trigger1 = time = 0
value = SCA,AA,AP,AT
time = 52
ignorehitpause = 1

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 2000,0
channel = 0

[State 2000, VS]
type = varset
trigger1 = time=0
trigger1 = fvar(10)<1.0
fvar(10)=fvar(10)*0.925

[State 4000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Blackout Effect"
ID = 701
pos = 0,0
postype = p1
stateno = 701
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[State 2000, Spellcard Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Spellcard_FX"
id = 1999
stateno = 1999
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[state 1010, Za Warudo!]
type = SuperPause
trigger1 = time = 0
time = 40
anim = -1
sound = S4,0
p2defmul = 1
poweradd = -1000
unhittable = 1
movetime = 40
darken = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 2100,10

[State 2100, PA]
type = posadd
trigger1 = animelemtime(5)=0
x = 10

[State 2100, STS]
type = statetypeset
trigger1 = animelemtime(5)=0
physics = N
ignorehitpause = 1

[State 2100, VS]
type = velset
trigger1 = animelemtime(5)>=0 && animelemtime(7)<0
x = 6.8

[State 2100, VS]
type = velmul
trigger1 = animelemtime(7)>=0
trigger1 = vel x > 0
x = 0.85

[State 4000, Effect]
type = helper
trigger1 = animelemtime(5)=0
helpertype = normal
name = "FX"
ID = 22100
pos = 60,-60
postype = p1
stateno = 22100
ownpal = 1
facing = 1
[State 4000, Effect]
type = helper
trigger1 = animelemtime(5)=0
helpertype = normal
name = "FX"
ID = 22101
pos = 60,-60
postype = p1
stateno = 22100
ownpal = 1
facing = 1

[State 1200, Ground HITDEF]
type = HitDef
triggerall = statetype!=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(5)>=0 && animelemtime(7)<0
attr = S,HA
animtype  = back
damage    = ceil(60.0*fvar(11)*ifelse(var(42),1.25,1.0)),16
kill = 0
guard.kill = 1
priority = 4,hit
hitflag = MAF
guardflag = M
pausetime = 1,10
guard.pausetime = 10,10
sparkno = S8020+(random%2)
guard.sparkno = S8030
sparkxy = -20,-62
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = low
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 20
guard.hittime  = 20
ground.velocity = -10.0;,-7.0
air.velocity = -10.0,-10.0
getpower = 0,0
givepower = 50,300
yaccel = 0.5
fall = 1
fall.recover = 0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 2100
[State 1200, Air HITDEF]
type = HitDef
triggerall = statetype=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(5)>=0 && animelemtime(7)<0
attr = S,HA
animtype  = back
damage    = ceil(70.0*fvar(11)*ifelse(var(42),1.25,1.0)),16
kill = 0
guard.kill = 1
priority = 4,hit
hitflag = MAF
guardflag = HA
pausetime = 1,10
guard.pausetime = 10,10
sparkno = S8020+(random%2)
guard.sparkno = S8030
sparkxy = -20,-62
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = High
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 20
guard.hittime  = 20
ground.velocity = -10.0;,-7.0
air.velocity = -10.0,-10.0
getpower = 0,0
givepower = 50,300
yaccel = 0.5
fall = 1
fall.recover = 0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 2100

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.90
ignorehitpause = 1
persistent = 0

[State 2100, CS]
type = changestate
trigger1 = movehit
trigger1 = numtarget(2100)
trigger1 = !(target(2100),ishelper)
value = 2101
ignorehitpause = 1
persistent = 0

[State 2100, STS]
type = statetypeset
trigger1 = animelemtime(7)=0
trigger2 = movecontact||movereversed
movetype = I
;ignorehitpause = 1
persistent = 0

[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(11)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(11)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(11)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; "Escape Velocity" 2nd hit (Shin Shoryuken?)
; Kinda interesting how just because of the connection between the two in PC-98 canon, you 
; can just take Marisa's spellcards, give em to Mima, and throw a unique twist on them to
; make Mima feel more unique, isn't it? Oh my god, is this comment gonna be on TCRF now?
[Statedef 2101]
type    = U
movetype= A
physics = N
velset = 0,0
anim = 2102
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 2101, TS]
type = targetstate
trigger1 = time = 0
id = 2100
value = 2150

[State 0, EnvShake]
type = EnvShake
trigger1 = time = 0
trigger2 = animelemtime(8)=0
trigger3 = animelemtime(9)=0
time = 20
freq = 240
ampl = -4
phase = 90

[State 2101, Microsoft Edge is just Internet Explorer gone edgy]
; Puns aside, it's still inferior to Firefox, imo.
type = width
trigger1 = 1
edge = 40,0
ignorehitpause = 1

[State 2101, TB]
type = targetbind
trigger1 = time = 0
id = 2100
pos = 35,0

[State 4000, Effect]
type = helper
trigger1 = animelemtime(8)=0||animelemtime(9)=0
helpertype = normal
name = "FX"
ID = 22102
pos = 23,-80
postype = p1
stateno = 22100
ownpal = 1
facing = 1
[State 4000, Effect]
type = helper
trigger1 = animelemtime(8)=0||animelemtime(9)=0
helpertype = normal
name = "FX"
ID = 22103
pos = 23,-80
postype = p1
stateno = 22100
ownpal = 1
facing = 1

[State 2101, HA]
type = hitadd
trigger1 = animelemtime(8)=0||animelemtime(9)=0
value = 1

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(8)=0
value = 2100,10
[State 2101, TS]
type = targetstate
trigger1 = animelemtime(8) = 0
id = 2100
value = 2151
[State 0, TargetLifeAdd]
type = TargetLifeAdd
trigger1 = animelemtime(8)=0
value = -ceil(65.0*fvar(11))
ID = 2100
kill = 0
[State 2101, TPA]
type = targetpoweradd
trigger1 = animelemtime(8)=0
id = 2100
value = 50
[State 200, Damage Dampen]
type = varset
trigger1 = animelemtime(8)=0
fvar(10) = fvar(10)*0.90
ignorehitpause = 1
persistent = 0
[State 0, Explod]
type = Explod
trigger1 = animelemtime(8)=0
anim = 8020+(random%2)
ID = 8000
pos = 30,-72
postype = p1  ;p2,front,back,left,right
facing = 1
vel = 0,0
removetime = -2
scale = 1,1
sprpriority = 5
ontop = 1
ownpal = 1
removeongethit = 0



[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(9)=0
value = 2100,11
[State 2101, TS]
type = targetstate
trigger1 = animelemtime(9) = 0
id = 2100
value = 2152
[State 0, TargetLifeAdd]
type = TargetLifeAdd
trigger1 = animelemtime(9)=0
value = -ceil(155.0*fvar(11))
ID = 2100
kill = 1
[State 2101, TPA]
type = targetpoweradd
trigger1 = animelemtime(9)=0
id = 2100
value = 200
[State 200, Damage Dampen]
type = varset
trigger1 = animelemtime(9)=0
fvar(10) = fvar(10)*0.70
ignorehitpause = 1
persistent = 0
[State 0, Explod]
type = Explod
trigger1 = animelemtime(9)=0
anim = 8020+(random%2)
ID = 8000
pos = 30,-84
postype = p1  ;p2,front,back,left,right
facing = 1
vel = 0,0
removetime = -2
scale = 1,1
sprpriority = 5
ontop = 1
ownpal = 1
removeongethit = 0

[State 2101, STS]
type = statetypeset
trigger1 = animelemtime(9)=0
statetype = A
movetype = I
physics = N

[State 2101, VS]
type = velset
trigger1 = animelemtime(9)=0
y = cond((pos y)+const(size.mid.pos.y) > -const240p(240),-10.0,0.0)

[State 2101, Grav]
type = veladd
trigger1 = animelemtime(9)>0
trigger1 = vel y < 0
y = 0.5

[State 2101, Grav]
type = velset
trigger1 = animelemtime(9)>0
trigger1 = vel y > 0
trigger2 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
y = 0.0

[State 2101, Set Flight Timer]
type = varset
trigger1 = animtime = 0
trigger1 = var(24)<=0
var(24)=600

[State 600, Return to flight State]
type = changestate
;trigger1 = var(24)>0
trigger1 = animtime = 0
value = 301
ctrl = 1


;---------------------------------------------------------------------------
; "Escape Velocity" Hitstate 1
[Statedef 2150]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 1

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 2150

;---------------------------------------------------------------------------
; "Escape Velocity" Hitstate 2
[Statedef 2151]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 0

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 2151

;---------------------------------------------------------------------------
; "Escape Velocity" Hitstate 3
[Statedef 2152]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 0

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 2152

[State 2152, VS]
type = velset
trigger1 = time = 0
x = -1.0
y = -18.0

[State 2152, Grav]
type = veladd
trigger1 = time
y = 0.5

[State 2152, CA]
type = changeanim2
trigger1 = anim != 2153
trigger1 = vel y >= 0
value = 2153

[State 2152, CS]
type = changestate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 6100

;---------------------------------------------------------------------------
; FX
[Statedef 22100]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 22100
sprpriority=5
ctrl = 0

[State 22100, VS]
type = null
triggerall = time = 0
trigger1 = ishelper(22100)||ishelper(22101)
trigger1 = (var(0):=0)||1
trigger2 = ishelper(22102)||ishelper(22103)
trigger2 = (var(0):=70)||1

[State 22100, VS]
type = velset
trigger1 = time = 0
x = 0.6*cos(var(0)*pi/180.0)
y = -0.6*sin(var(0)*pi/180.0)

[State 22100, BTR]
type = bindtoroot
trigger1 = ishelper(22100)
pos = 60,-60
ignorehitpause = 1

[State 22100, BTR]
type = bindtoroot
trigger1 = ishelper(22102)
pos = 23,-80
ignorehitpause = 1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22100, VA]
type = varadd
trigger1 = ishelper(22100)||ishelper(22102)
fvar(0) = cond(time<8,0.1,0.03)

[State 22100, VA]
type = varadd
trigger1 = ishelper(22101)||ishelper(22103)
fvar(0) = cond(time<8,0.065,0.01)

[State 22100, AD]
type = angledraw
trigger1 = 1
value = var(0)
scale = (fvar(0)/6.0),fvar(0)
ignorehitpause = 1

[State 22100, Bootlegged "Subalpha" trans type]
type = palfx
trigger1 = 1
time = 2
mul = 256-floor(256.0*time/30.0),256-floor(256.0*time/30.0),256-floor(256.0*time/30.0)
invertall = 0
color = 256
ignorehitpause = 1

[State 22100, T]
type = trans
trigger1 = 1
trans = sub
ignorehitpause = 1

[State 22100, Zeeky]
type = destroyself
trigger1 = time>=30
trigger2 = root,movetype = H

;---------------------------------------------------------------------------
; "Twilight Spark"
[Statedef 4000]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 4000+1*(statetype=A);+0*floor(var(11):=0)
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 4000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 4000,0
channel = 0

[State 2000, VS]
type = varset
trigger1 = time=0
trigger1 = fvar(10)<1.0
fvar(10)=fvar(10)*0.925

[State 4000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Blackout Effect"
ID = 701
pos = 0,0
postype = p1
stateno = 701
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[State 2000, Spellcard Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Spellcard_FX"
id = 3999
stateno = 1999
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[state 1010, Za Warudo!]
type = SuperPause
trigger1 = time = 0
time = 40
anim = -1
sound = S4,2
p2defmul = 1
poweradd = -3000
unhittable = 1
movetime = 40
darken = 0

[State 2100, PA]
type = posadd
trigger1 = animelemtime(6)=0
x = 10

[State 2100, STS]
type = statetypeset
trigger1 = animelemtime(7)=0
physics = N
ignorehitpause = 1

[State 2100, VS]
type = velset
trigger1 = animelemtime(7)>=0 && animelemtime(8)<0
x = 7.0

[State 2100, VS]
type = velmul
trigger1 = animelemtime(8)>=0
trigger1 = vel x > 0
x = 0.85

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(7)=0
trigger2 = animelemtime(7)=8
trigger3 = animelemtime(7)=15
value = 4000,10
channel = 6

[State 4000, Effect]
type = helper
trigger1 = animelemtime(7)=0
trigger2 = animelemtime(7)=8
trigger3 = animelemtime(7)=15
helpertype = normal
name = "Pulse"
ID = 24000
pos = 76,-54
postype = p1
stateno = 24000
ownpal = 1
facing = 1

[State 1200, Ground HITDEF]
type = HitDef
triggerall = statetype!=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(7)>=0 && animelemtime(8)<0
attr = S,HA
animtype  = back
damage    = 20,88
kill = 0
guard.kill = 1
priority = 4,hit
hitflag = MAF
guardflag = M
pausetime = 1,10
guard.pausetime = 10,10
sparkno = S8020+(random%2)
guard.sparkno = S8030
sparkxy = -20,-54
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = low
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 20
guard.hittime  = 20
ground.velocity = -10.0;,-7.0
air.velocity = -10.0,-6.0
getpower = 0,0
givepower = 0,600
yaccel = 0.5
fall = 1
fall.recover = 0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 4000
[State 1200, Ground HITDEF]
type = HitDef
triggerall = statetype=A
triggerall = movecontact = 0 && movereversed = 0
trigger1 = animelemtime(7)>=0 && animelemtime(8)<0
attr = S,HA
animtype  = back
damage    = 20,88
kill = 0
guard.kill = 1
priority = 4,hit
hitflag = MAF
guardflag = HA
pausetime = 1,10
guard.pausetime = 10,10
sparkno = S8020+(random%2)
guard.sparkno = S8030
sparkxy = -20,-54
hitsound = s1,2
guardsound = s2,0
ground.type = High
air.type = low
ground.slidetime = 22+3*((var(42)=1))
ground.hittime  = 22+3*((var(42)=1))
air.hittime = 22+3*((var(42)=1))
guard.slidetime = 20
guard.hittime  = 20
ground.velocity = -10.0,-6.0
air.velocity = -10.0,-6.0
getpower = 0,0
givepower = 0,600
yaccel = 0.5
fall = 1
fall.recover = 0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
p2facing = 1
id = 4000

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.55
ignorehitpause = 1
persistent = 0

[State 2100, CS]
type = changestate
trigger1 = movehit
trigger1 = numtarget(4000)
trigger1 = !(target(4000),ishelper)
value = 4001
ignorehitpause = 1
persistent = 0

[State 2100, STS]
type = statetypeset
trigger1 = animelemtime(8)=0
trigger2 = movecontact||movereversed
movetype = I
;ignorehitpause = 1
persistent = 0


[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
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
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(12)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(12)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; "Twilight Spark" 2nd hit
; I literally just copy & pasted shit from Escape Velocity for this Last Word. I guess that's what happens when
; you're up at 3 in the morning on a Saturday watching reruns of the latest season of Doctor Who while coding
; and you're out of ideas. But I guess the sparks have come full circle, haven't they? Marisa's is projectile,
; Yuuka's is a command grab, and Mima's is a melee attack. I literally had no other ideas for implementing this
; attack without making it seem like just another generic beam super. By the way, I literally just found out that
; Smogon banned Marshadow from Gen 7 OU as I was coding this move. If you're not into competitive Pokemon like I 
; am, then I wouldn't worry too much about what I just said, just know that this means Mega Gallade & Mega Gardevoir
; are going to be viable now. If you're a Doubles or VGC player then good for you, I could never get into playing
; Doubles for some reason, and the fact that the main stories of the Pokemon games are done entirely in a Singles
; format doesn't help either. Unless its Colosseum or XD, we need more games like those.
[Statedef 4001]
type    = U
movetype= A
physics = N
velset = 0,0
anim = 4002
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 4001, No Timer Scamming]
type = assertspecial
trigger1 = 1
flag = timerfreeze
ignorehitpause = 1

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 2101, TS]
type = targetstate
trigger1 = time = 0
id = 4000
value = 2150

[State 0, EnvShake]
type = EnvShake
trigger1 = time = 0
time = 90
freq = 240
ampl = -2
phase = 90

[State 2101, Microsoft Edge is just Internet Explorer gone edgy]
; Puns aside, it's still inferior to Firefox, imo.
type = width
trigger1 = 1
edge = 60,0
ignorehitpause = 1

[State 2101, TB]
type = targetbind
trigger1 = time = 0
id = 4000
pos = 55,0

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(1)=20
trigger2 = animelemtime(1)=40
trigger3 = animelemtime(1)=60
value = 4000,10
channel = 6

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(1)=60
value = 4000,11
channel = 5

[State 4000, Effect]
type = helper
trigger1 = animelemtime(1)=20
trigger2 = animelemtime(1)=40
trigger3 = animelemtime(1)=60
helpertype = normal
name = "Pulse"
ID = 24000
pos = 76,-54
postype = p1
stateno = 24000
ownpal = 1
facing = 1

[State 4001, ALLPFX]
type = allpalfx
trigger1 = animelemtime(2)>=0&&animelemtime(3)<0
time = 2
mul = 256-floor(256.0*animelemtime(2)/30.0),256-floor(256.0*animelemtime(2)/30.0),256-floor(256.0*animelemtime(2)/30.0)
ignorehitpause =1

[State 2101, TS]
type = targetstate
trigger1 = animelemtime(3)=0
id = 4000
value = 4050

[State 4001, ASS]
type = AssertSpecial
trigger1 = animelemtime(3)>=0
flag = noFG
flag2 = noBG
flag3 = globalnoshadow
ignorehitpause = 1
[State 4001, ASS]
type = AssertSpecial
trigger1 = animelemtime(3)>=0
flag = nobardisplay
ignorehitpause = 1


[State 2101, TS]
type = targetstate
trigger1 = animelemtime(4)=0
id = 4000
value = 4051
[State 4001, TB]
type = targetbind
trigger1 = animelemtime(4)=0
pos = floor(frontedgedist),floor(gameheight*2.0/3.0)-floor(screenpos y);floor(-pos y)
[State 4001, ENVC]
type = EnvColor
trigger1 = animelemtime(4)=0
value = 255,255,255
time = 3
under = 1
ignorehitpause = 1
[State 0, Explod]
type = Explod
trigger1 = animelemtime(4)=0
anim = 4003
ID = 4003
pos = -floor(backedgedist),floor(gameheight*2.0/3.0)-floor(screenpos y)
postype = p1;back  ;p2,front,back,left,right
facing = 1
bindtime = -1
vel = 0,0
removetime = -1
pausemovetime = 0
scale = 0.5,0.5
sprpriority = 5;1
ontop = 0
shadow = 0,0,0
ownpal = 0
trans = addalpha
alpha = 0,0
removeongethit = 0
ignorehitpause = 1

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(4)=0
value = 4000,12
channel = 5

[State 4001, ENVC]
type = EnvColor
trigger1 = animelemtime(5)>=0;&&animelemtime(6)<0
value = 255,255,255
time = 3
under = 1
ignorehitpause = 1

[State 4001, MEX]
type = modifyexplod
trigger1 = animelemtime(5)=0
id = 4003
trans = addalpha
alpha = 0,192
ignorehitpause = 1

[State 0, Explod]
type = Explod
trigger1 = animelemtime(5)=0
anim = 24000
ID = 24000;4003
pos = -floor(backedgedist)+30,floor(gameheight*2.0/3.0)-floor(screenpos y)-29
postype = p1;back  ;p2,front,back,left,right
facing = 1
bindtime = -1
vel = 0,0
removetime = -1
pausemovetime = 0
scale = 2.5,5.5
sprpriority = 4;5
ontop = 0
shadow = 0,0,0
ownpal = 1
removeongethit = 0
ignorehitpause = 1

[State 0, Explod]
type = Explod
trigger1 = animelemtime(5)=0
anim = 24001
ID = 24000;4003
pos = -floor(backedgedist)+floor(gamewidth/2.0),floor(gameheight*1.0/2.0)-floor(screenpos y)
postype = p1  ;p2,front,back,left,right
facing = 1
bindtime = -1
vel = 0,0
removetime = -1
pausemovetime = 0
scale = 1.10,1.10
sprpriority = -1
ontop = 0
shadow = 0,0,0
ownpal = 1
removeongethit = 0
ignorehitpause = 1

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(5)=10
value = 4000,13
channel = 5
[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(5)=10
value = 4000,13
channel = 6

[State 4000, PSND]
type = playsnd
trigger1 = animelemtime(5)=10
value = 4000,14
channel = 2

[State 0, EnvShake]
type = EnvShake
trigger1 = animelemtime(5)=0
time = 180
freq = 240
ampl = -2
phase = 90

[State 4001, HA]
type = hitadd
trigger1 = animelemtime(5)>=0 && animelemtime(6)<0
value = 1
persistent = 3

[State 4001, HA]
type = targetlifeadd
trigger1 = animelemtime(5)>=0 && animelemtime(6)<0
id = 4000
value = -7;cond(animelemtime(5)<177,7,7)
kill = cond(animelemtime(5)<177,0,1)
persistent = 3

[State 4001, HA]
type = targetpoweradd
trigger1 = animelemtime(5)>=0 && animelemtime(6)<0
id = 4000
value = 10
persistent = 3

[State 0, AllPalFX]
type = AllPalFX
trigger1 = animelemtime(6)>=0
time = 2
add = cond(animelemtime(6)<10,floor(256.0*animelemtime(6)/10.0),256),cond(animelemtime(6)<10,floor(256.0*animelemtime(6)/10.0),256),cond(animelemtime(6)<10,floor(256.0*animelemtime(6)/10.0),256)
ignorehitpause = 1

[State 4001, MEX]
type = modifyexplod
trigger1 = animelemtime(6)>=0
id = 4003
trans = addalpha
alpha = 0,192+floor(64.0*animelemtime(6)/10.0)
ignorehitpause = 1

[State 4001, REX]
type = removeexplod
trigger1 = animelemtime(6)=10
id = 4003
[State 4001, REX]
type = removeexplod
trigger1 = animelemtime(6)=10
id = 24000

[State 4001, CS]
type = changestate
trigger1 = animelemtime(6)=15
value = 4002

;---------------------------------------------------------------------------
; "Twilight Spark" Recovery Frames
[Statedef 4002]
type    = U
movetype= I
physics = N
velset = 0,0
anim = 4004+1*(statetype=A)
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 2101, TB]
type = targetbind
trigger1 = time = 0
id = 4000
pos = 55,0

[State 2101, TS]
type = targetstate
trigger1 = time = 0
id = 4000
value = 4052

[State 0, AllPalFX]
type = AllPalFX
trigger1 = time<10
time = 2
add = 256-floor(256.0*time/10.0),256-floor(256.0*time/10.0),256-floor(256.0*time/10.0)
ignorehitpause = 1

[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(10)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(10)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(10)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; FX
[Statedef 24000]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 22100
sprpriority=5
ctrl = 0

[State 22100, BTR]
type = bindtoroot
trigger1 = root,stateno = [4000,4001]
pos = 76,-54
ignorehitpause = 1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22100, VA]
type = varset
trigger1 = 1
fvar(0) = 0.5*(1.0+(0.15*time))

[State 22100, AD]
type = angledraw
trigger1 = 1
value = var(0)
scale = fvar(0),fvar(0)
ignorehitpause = 1

[State 22100, Bootlegged "Subalpha" trans type]
type = palfx
trigger1 = 1
time = 2
mul = 256-floor(256.0*time/16.0),256-floor(256.0*time/16.0),256-floor(256.0*time/16.0)
invertall = 0
color = 256
ignorehitpause = 1

[State 22100, T]
type = trans
trigger1 = 1
trans = sub
ignorehitpause = 1

[State 22100, Zeeky]
type = destroyself
trigger1 = time>=16

;---------------------------------------------------------------------------
; "Twilight Spark" Hitstate 1
[Statedef 4050]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 1

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 1998

;---------------------------------------------------------------------------
; "Twilight Spark" Hitstate 2
[Statedef 4051]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 1

[State 0, ScreenBound]
type = ScreenBound
trigger1 = 1
value = 1
movecamera = 0,0
ignorehitpause = 1

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 4050

[State 4051, AD]
type = angledraw
trigger1 = 1
value = 0
scale = 0.5,0.5
ignorehitpause = 1

[State 4051, Trans]
type = trans
trigger1 = time<210
trans = addalpha
alpha = 0,0

[State 4051, Trans]
type = trans
trigger1 = time=[210,219]
trans = addalpha
alpha = 0,floor(256.0*time/10.0)

[State 4051, Invis]
type = assertspecial
trigger1 = time>=220
flag = invisible
ignorehitpause = 1

;---------------------------------------------------------------------------
; "Twilight Spark" Hitstate 3
[Statedef 4052]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 0
facep2 = 0

[State 2150, CA2]
type = changeanim2
trigger1 = time = 0
value = 4051

[State 2152, VS]
type = velset
trigger1 = time = 5
x = -18.0
y = -6.0

[State 2152, Grav]
type = veladd
trigger1 = time>5
y = 0.25

[State 2152, CS]
type = changestate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 6100

;---------------------------------------------------------------------------
; "Flare Star"
[Statedef 4100]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 4100+10*(statetype=A)+0*floor(var(11):=4)+0*floor(var(12):=0)
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 2010, No Air Turn]
type = turn
trigger1 = time = 0
trigger1 = (statetype != A) || (var(39)>0)
trigger1 = p2dist x < 0

[State 4000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 4000,0
channel = 0

[State 2000, VS]
type = varset
trigger1 = time=0
trigger1 = fvar(10)<1.0
fvar(10)=fvar(10)*0.925

[State 4000, Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Blackout Effect"
ID = 701
pos = 0,0
postype = p1
stateno = 701
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[State 2000, Spellcard Effect]
type = helper
trigger1 = time = 0
helpertype = normal
name = "Spellcard_FX"
id = 3999
stateno = 1999
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 40
supermovetime = 40

[state 1010, Za Warudo!]
type = SuperPause
trigger1 = time = 0
time = 40
anim = -1
sound = S4,2
p2defmul = 1
poweradd = -3000
unhittable = 1
movetime = 40
darken = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 4100,10
channel = 6

[State 2000, Spellcard Effect]
type = helper
trigger1 = animelemtime(4)=0
helpertype = normal
name = "Orb"
id = 24100
stateno = 24100
pos = 0,-50
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8

[State 4100, VS]
type = varset
trigger1 = var(10)=1
trigger1 = command != "holdc"
trigger2 = var(10)=2
trigger2 = !(command = "holdx" && command = "holdy" && command = "holdz")
var(12) = 1
persistent = 0

[State 4100, Increase Hit Count]
type = null
trigger1 = time=48
trigger1 = (var(11):=5)||1
trigger2 = time=63
trigger2 = (var(11):=6)||1
trigger3 = time=78
trigger3 = (var(11):=7)||1
trigger4 = time=96
trigger4 = (var(11):=8)||1

[State 0, No Premature Blocking]
type = AttackDist
trigger1 = 1
value = 0
ignorehitpause = 1

[State 4100, CS]
type = changestate
triggerall = animelemtime(7)>=0
trigger1 = var(12)=1
trigger2 = time>=96
value = 4101

;---------------------------------------------------------------------------
; "Flare Star"
[Statedef 4101]
type    = U;S
movetype= A
physics = N;S
velset = 0,0
anim = 4101+10*(statetype=A);+0*floor(var(11):=0)
ctrl = 0
sprpriority = 1
facep2 = 0;1

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 4100,11
;channel = 6

[State 0, AttackDist]
type = AttackDist
trigger1 = 1
value = 320
ignorehitpause = 1

[State 4000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = S

[State 4101, STS]
type = statetypeset
trigger1 = animelemtime(16)=0
movetype = I
ignorehitpause = 1

[State 1000, Air CS]
type = changestate
trigger1 = statetype != A
trigger1 = animtime=0
value = 0
ctrl = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = statetype = A
trigger1 = animelemtime(6)=0
value = 301
ctrl = 1

[State 1000, STS]
type = veladd
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(6)>=0
y = 0.5

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)<=0 && statetype = A
trigger1 = animelemtime(6)>=0
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 2001

;---------------------------------------------------------------------------
; FX
[Statedef 24100]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 1010, VS]
type = varset
trigger1 = time = 0
var(51) = random*1000+random

[State 24100, Initialize]
type = null
triggerall = time = 0
trigger1 = (var(10):=floor(root,pos x))||1
trigger1 = (var(11):=floor(root,pos y)-50)||1

[State 24100, Initialize]
type = null
triggerall = numenemy
trigger1 = (var(10):=floor(enemynear(0),pos x))||1
trigger1 = (var(11):=floor(enemynear(0),pos y)+floor(enemynear(0),const(size.mid.pos.y)))||1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 24100, PS]
type = posset
trigger1 = 1
x = var(10)
y = var(11)

[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24110
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24111
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24112
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24113
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24114
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65
[State 2000, Spellcard Effect]
type = helper
trigger1 = time=0
helpertype = normal
name = "Orb"
id = 24115
stateno = 24110
pos = 0,0
postype = p1
ownpal = 1
facing = 1
pausemovetime = 8
supermovetime = 8
;size.xscale = 0.65
;size.yscale = 0.65

[State 1010, VS]
type = varset
trigger1 = var(5) != root,var(11)
var(5) = root,var(11)

[State 24100, CS]
type = changestate
trigger1 = root,stateno != [4100,4101]
trigger2 = root,movetype = H
trigger3 = root,alive = 0
value = 999

[State 24100, CS]
type = changestate
trigger1 = root,stateno = 4101
value = 24101

;---------------------------------------------------------------------------
; Projectile
[Statedef 24101]
type    = A
movetype= A
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1



[State 24101, CS]
type = changestate
trigger1 = time>=8
value = 24102

;---------------------------------------------------------------------------
; Projectile
[Statedef 24102]
type    = A
movetype= A
physics = N
velset = 0,0
anim = 24101
sprpriority=5
ctrl = 0

[State 200, PSND]
type = PlaySnd
trigger1 = time=0
value = 4100,12

[State 0, EnvShake]
type = EnvShake
trigger1 = 1
persistent = 4
time = 4
freq = 180
ampl = -4

[State 24102, VS]
type = varset
trigger1 = time = 0
var(6) = var(5)

[State 24102, VS]
type = null ;This isn't going to evolve into Silvally any time soon. Also Gamefreak pls nuff Silvally, its so
; underwhelming when its sitting all the way down in PU why is something so awesome so shit?
triggerall = time = 0
trigger1 = (var(6)<=4)
trigger1 = (var(8):=100)||1 ;400
trigger2 = (var(6)=5)
trigger2 = (var(8):=94)||1 ;470
trigger3 = (var(6)=6)
trigger3 = (var(8):=90)||1 ;540
trigger4 = (var(6)=7)
trigger4 = (var(8):=87)||1 ;610
trigger5 = (var(6)>=8)
trigger5 = (var(8):=85)||1 ;680

[State 24102, VS]
type = varset
trigger1 = 1
var(7) = floor(360.0*random/1000.0)

[State 24102, FVS]
type = varset
trigger1 = 1
fvar(0) = 1.0+(time/64.0)+ifelse(time%4=0,-0.1,ifelse(time%4=2,0.1,0.0))

[State 24102, AD]
type = angledraw
trigger1 = 1
value = var(7)
scale = fvar(0),fvar(0)
ignorehitpause = 1

[State 0, Explod]
type = Explod
trigger1 = 1
persistent = 4
anim = 24102
ID = 24102
pos = 0,0
postype = p1  ;p2,front,back,left,right
facing = cond(random%2=0,1,-1)
vfacing = cond(random%2=0,1,-1)
bindtime = -1
vel = 0,0
removetime = -2
pausemovetime = 0
scale = 2.0*fvar(0),2.0*fvar(0)
sprpriority = 5
ownpal = 0
removeongethit = 0
ignorehitpause = 1

[State 24102, T]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 1000, 5]
type = Projectile
;triggerall = (root,stateno = 4101)
trigger1 = time%8=0
trigger1 = var(5)>0
;persistent = 10
attr = S,HP
projanim = 22010
projid = 24010
projremove = 1
projremovetime = 10
projscale = 1.0,1.0
projpriority = 9999
velocity = 0
offset = 0,0
animtype  = Hard
damage = 50+100*(var(5)<=1),10+20*(var(5)<=1)
priority  = 7,Hit
hitflag = MAFP
guardflag = MA
pausetime = 0,8
sparkxy = floor(p2dist x),0
sparkno = s8020+(random%2)
guard.sparkno = s8030
hitsound   = s1,7
guardsound = s2,0
ground.type = High
ground.slidetime = 24
ground.hittime  = 24
guard.slidetime = 23
ground.slidetime = 23
ground.velocity = -6.5,-6.0
guard.velocity = -6.5
air.velocity = -6.5,-4.5
airguard.velocity = -6.5, -4.5
fall = 1
air.fall=1
fall.recover=0
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
getpower = 0,0
givepower = var(8),var(8)
id = 4100
guard.dist = 120
p2facing = 1

[State 1000, 5]
type = Varadd
;triggerall = (root,stateno = 4101)
trigger1 = time%8=0
trigger1 = var(5)>0
var(5) = -1

[State 24102, CS]
type = changestate
trigger1 = var(5)<=0
value = 24103;999

;---------------------------------------------------------------------------
; Projectile
[Statedef 24103]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 1998
sprpriority=5
ctrl = 0

[State 24102, CS]
type = changestate
trigger1 = time>20
value = 999

;---------------------------------------------------------------------------
; FX
[Statedef 24110]
type    = A
movetype= I
physics = N
velset = 0,0
anim = 24100;22000+1*ishelper(24111)+2*ishelper(24112)+3*ishelper(24113)+4*ishelper(24114)+5*ishelper(24115)
sprpriority=5
ctrl = 0

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

[State 24110, Initial Angle]
type = null
triggerall = time = 0
trigger1 = ishelper(24110)
trigger1 = (var(10):=0)||1
trigger2 = ishelper(24111)
trigger2 = (var(10):=60)||1
trigger3 = ishelper(24112)
trigger3 = (var(10):=120)||1
trigger4 = ishelper(24113)
trigger4 = (var(10):=180)||1
trigger5 = ishelper(24114)
trigger5 = (var(10):=240)||1
trigger6 = ishelper(24115)
trigger6 = (var(10):=300)||1

[State 1215, Tail]
type = AfterImage
trigger1 = time=0
trans = add
length = 15;30
timegap = 1
framegap = 1
time = 9999999
palbright = 0, 0, 0
palcontrast = 90,0,0; 90, 90
paladd = 0,0,0
palmul = 0.9,0,0;0.9,0.9
ignorehitpause = 1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22100, PS]
type = bindtoparent
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),var(51) = var(51)
pos = floor(90*cos(var(10)*pi/180.0)),floor(20*sin(var(10)*pi/180.0))

[State 24110, SPR]
type = sprpriority
trigger1 = 1
value = cond(floor(20*sin(var(10)*pi/180.0))<0,-1,5)

[State 24110, AD]
type = angledraw
trigger1 = 1
scale = cond(time<10,(time/10.0),1.0),cond(time<10,(time/10.0),1.0)
ignorehitpause = 1
[State 24110, Trans]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State 22100, PS]
type = changestate
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),stateno = 24101
value = 24111

[State 22100, Spin]
type = varadd
trigger1 = 1
var(10) = 2

;---------------------------------------------------------------------------
; FX
[Statedef 24111]
type    = A
movetype= I
physics = N
velset = 0,0
;anim = 22000+1*ishelper(24111)+2*ishelper(24112)+3*ishelper(24113)+4*ishelper(24114)+5*ishelper(24115)
;sprpriority=5
ctrl = 0

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

[State 22100, PS]
type = bindtoparent
trigger1 = playeridexist(var(50))
trigger1 = playerid(var(50)),var(51) = var(51)
pos = floor((90-(90*time*time/64.0))*cos(var(10)*pi/180.0)),floor((20-(20*time*time/64.0))*sin(var(10)*pi/180.0))

[State 24110, SPR]
type = sprpriority
trigger1 = 1
value = cond(floor(20*sin(var(10)*pi/180.0))<0,-1,5)

[State 24110, AD]
type = angledraw
trigger1 = time
scale = 1.0,1.0
ignorehitpause = 1
[State 24110, Trans]
type = trans
trigger1 = 1
trans = addalpha
alpha = 256,256
ignorehitpause = 1

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 24111, Destroyself]
type = destroyself
trigger1 = time>=8

[State 22100, Spin]
type = varadd
trigger1 = 1
var(10) = 2+2*(time**2)