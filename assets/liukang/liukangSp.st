;Cartwheel Stun
[Statedef 3073]
type    = S     
movetype= A     
physics = S    
juggle  = 1     
velset = 0,0
ctrl = 0
anim = 3073
poweradd = 40

[State 3073, 3]
type = PlaySnd
trigger1 = time = 0
value = 60,10

[State 3073, Velocity1]
type = Velset
trigger1 = AnimElem = 3
x = 4

[State 3073, Velocity2]
type = Velset
trigger1 = AnimElem = 4
x = -4

[State 3073, 4]
type = HitDef
trigger1 = AnimElem = 3
trigger2 = AnimElem = 4
trigger3 = AnimElem = 5
trigger4 = AnimElem = 6
attr = S, NA  
damage    = 50,10
animtype = Back
guardflag = M
hitflag = MAF
pausetime = 2,40
sparkxy = -9999,-9999
hitsound = s27,27
guardsound = s0,0
ground.slidetime = 10
ground.hittime  = 15
ground.Velocity = 3
air.Velocity = 2,-3
down.Velocity = 0,0
air.fall = 1
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall = 0

[State 3073, 7]
type = ChangeState
triggerall = Var(50) = 1
trigger1 = AnimTime = 0
trigger1 = movehit = 1
value = 430
ctrl = 1

[State 3073, 7]
type = ChangeState
trigger1 = AnimTime = 0
value = 11
ctrl = 1

;Fireblast
[StateDef 4172]
type    = S     
movetype= A     
physics = S     
juggle  = 1     
velset = 0,0
ctrl = 0
anim = 4172

[State 4172. Super D]
type = AfterImage
trigger1 = time = 1
FrameGap = 5
length = 20
PalBright   =   0,  0,  0
PalContrast = 128,128,128
PalAdd      =   0,  0,  0
PalMul      =  1,1,1
Trans = Add1
time = 0

[State 4172, Super]
type = SuperPause
trigger1 = AnimElem = 1
pos = 9920, -99120
anim = 100
sound = S20,16
poweradd = -2000

[State 4172, Super C]
type = PowerAdd
trigger1 = Time = 1
value = -2000

[State 4172, Fireball Throw]
type = playsnd
trigger1 = time = 5
trigger2 = time = 15
trigger3 = time = 25
value = 4,10

[State 4172, Fireball Shoot]
type = playsnd
trigger1 = time = 11
trigger2 = time = 21
trigger3 = time = 31
value = 60,14

[State 4172, Low Fire Explode]
type = Explod
trigger1 = time = 5
trigger2 = time = 15
trigger3 = time = 25
anim = 131401
id = 131401
pos = -5*const(size.xscale), -60*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 1
ownpal = 1

[State 4172, Low Fireball1]
type = Projectile
trigger1 = time = 11
projanim = 131301
id = 32101
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 30*const(size.xscale), 0*const(size.xscale)
offset = 185*const(size.xscale), -50*const(size.xscale)
velocity = 7.5, 2
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 30, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 25
ground.hittime  = 16
ground.velocity = -5, -4
air.velocity = -1.5,-4
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5


[State 4172, Low Fireball2]
type = Projectile
trigger1 = time = 21
projanim = 131301
id = 32101
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 30*const(size.xscale), 0*const(size.xscale)
offset = 185*const(size.xscale), -50*const(size.xscale)
velocity = 7.5, 1
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 30, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 25
ground.hittime  = 16
ground.velocity = -5, -4
air.velocity = -1.5,-4
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5


[State 4172, Low Fireball3]
type = Projectile
trigger1 = time = 31
projanim = 131301
id = 32101
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 30*const(size.xscale), 0*const(size.xscale)
offset = 185*const(size.xscale), -50*const(size.xscale)
velocity = 7.5, 0
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 30, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 25
ground.hittime  = 16
ground.velocity = -5, -6
air.velocity = -1.5,-6
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5

[State 4172, Fireball Throw]
type = playsnd
trigger1 = time = 45
trigger2 = time = 55
trigger3 = time = 65
value = 4,10

[State 4172, Fireball Shoot]
type = playsnd
trigger1 = time = 51
trigger2 = time = 61
trigger3 = time = 71
value = 60,14

[State 8850, Fire Explode]
type = Explod
trigger1 = time = 45
trigger2 = time = 55
trigger3 = time = 65
anim = 131401
id = 131401
pos = -10*const(size.xscale), -105*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 1
ownpal = 1

[State 8850, High Fireball1]
type = Projectile
trigger1 = time = 51
projanim = 131301
projid = 8850
projedgebound = 999
projstagebound = 999
postype = p1
pos = 25*const(size.xscale), 0*const(size.xscale)
offset = 180*const(size.xscale), -96*const(size.xscale)
velocity = 7.5, 0
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 35
ground.hittime  = 16
ground.velocity = -5, -4
air.velocity = -1.5,-4
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5


[State 8850, High Fireball1]
type = Projectile
trigger1 = time = 61
projanim = 131301
projid = 8850
projedgebound = 999
projstagebound = 999
postype = p1
pos = 25*const(size.xscale), 0*const(size.xscale)
offset = 180*const(size.xscale), -96*const(size.xscale)
velocity = 7.5, -1
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 35
ground.hittime  = 16
ground.velocity = -5, -4
air.velocity = -1.5,-4
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5


[State 8850, High Fireball1]
type = Projectile
trigger1 = time = 71
projanim = 131301
projid = 8850
projedgebound = 999
projstagebound = 999
postype = p1
pos = 25*const(size.xscale), 0*const(size.xscale)
offset = 180*const(size.xscale), -96*const(size.xscale)
velocity = 7.5, -2
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,10
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 35
ground.hittime  = 16
ground.velocity = -5, -4
air.velocity = -1.5,-4
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5

[State 4172, Smokehit]
type = Explod
trigger1 = ProjContact(8850) = 1
anim = 902101
pos = 8*const(size.xscale), -65*const(size.xscale)
postype = p2
bindtime = -1
sprpriority = 3 
shadow = 0
ownpal = 1
ontop = 0

[State 4172, 7]
type = ChangeState
trigger1 = AnimTime = 0
triggerall = Var(50) = 1
value = 131304
ctrl = 1

[State 4172, 8]
type = ChangeState
trigger1 = AnimTime = 0
triggerall = Var(50) = 0
value = 0
ctrl = 1

;Angle Kick
[StateDef 4162]
type    = S     
movetype= A     
physics = S     
juggle  = 1     
velset = 0,0
ctrl = 0
anim = 4162

[State 4162, Super C]
type = PowerAdd
trigger1 = Time = 1
value = -2000

[State 4162, Super]
type = SuperPause
trigger1 = AnimElem = 1
pos = 9920, -99120
anim = 100
sound = S20,16
poweradd = -2000

[State 4162, Super D]
type = AfterImage
trigger1 = time = 1
FrameGap = 1
length = 13
timegap = 2
PalBright   =   0,  0,  0
PalContrast = 80,80,80
PalAdd      =   10,  0,  0
PalMul      =  1,1,1
Trans = Add1
time = 77

[State 4162, 1]
type = VarRandom
trigger1 = AnimElem = 2
v = 11
range = 1, 2

[State 4162, 2]
type = Playsnd
triggerall = Var(11) = 1
trigger1 = AnimElem = 2
value = 60,8

[State 4162, 3]
type = Playsnd
triggerall = Var(11) = 2
trigger1 = AnimElem = 2
value = 60,1

[State 4162, Comin Down]
type = Playsnd
trigger1 = AnimElem = 9
value = 60,2

[State 4162, 4]
type = Playsnd
trigger1 = AnimElem = 2
trigger2 = AnimElem = 9
value = 60,13

[State 4162, Velocity3]
type = Velset
trigger1 = AnimElem = 2
trigger2 = AnimElem = 3
trigger3 = AnimElem = 4
trigger4 = AnimElem = 5
trigger5 = AnimElem = 6
trigger6 = AnimElem = 7
trigger7 = AnimElem = 8
x = 6
y = -5

[State 4162, Velocity4]
type = Velset
trigger1 = AnimElem = 9
trigger2 = AnimElem = 10
trigger3 = AnimElem = 11
trigger4 = AnimElem = 12
trigger5 = AnimElem = 13
trigger6 = AnimElem = 14
trigger7 = AnimElem = 15
x = 6
y = 5

[State 4162, Hit 1]
type = HitDef
trigger1 = AnimElem = 2
trigger2 = AnimElem = 3
trigger3 = AnimElem = 4
trigger4 = AnimElem = 5
trigger5 = AnimElem = 6
trigger6 = AnimElem = 7
trigger7 = AnimElem = 8
attr = S, NA  
damage    = 35,10
animtype = Back
guardflag = HA
hitflag = MAF
priority = 3, Hit
pausetime = 0,0
sparkxy = -40, -70
hitsound = S27,27
guardsound = S0,0
ground.type = Low
ground.slidetime = 5
ground.hittime  = 5
ground.Velocity = -4, -5
air.Velocity = -4,-5
down.Velocity = 0,0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall = 1
fall.recover = 0

[State 4162, Hit 2]
type = HitDef
trigger1 = AnimElem = 9
trigger2 = AnimElem = 10
trigger3 = AnimElem = 11
trigger4 = AnimElem = 12
trigger5 = AnimElem = 13
trigger6 = AnimElem = 14
trigger7 = AnimElem = 15
attr = S, NA  
damage    = 35,10
animtype = Back
guardflag = HA
hitflag = MAF
priority = 3, Hit
pausetime = 0,0
sparkxy = -40, 10
hitsound = S27,27
guardsound = S0,0
ground.type = Low
ground.slidetime = 5
ground.hittime  = 5
ground.Velocity = -4, 5
air.Velocity = -4,5
down.Velocity = 0,0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall = 1
fall.recover = 0

[State 4162, Fall]
type = ChangeState
trigger1 = AnimTime = 0
value = 1101

[State 4162, 7]
type = ChangeState
trigger1 = AnimTime = 0
trigger1 = Pos Y >= 0
trigger1 = Vel Y > 0
value = 0
ctrl = 1

;Air Fireball
[StateDef 88500]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 88500
sprpriority = 9

[State 88500, Fireball Chuck]
type = playsnd
trigger1 = AnimElem = 1
value = 4,10

[State 88500, Fireball]
type = playsnd
trigger1 = AnimElem = 2
value = 60,7

[State 88500, Fire Explode]
type = Explod
trigger1 = AnimElem = 2
anim = 131401
id = 131401
pos = -10*const(size.xscale), -105*const(size.xscale)
postype = p1
sprpriority = -2
ontop = 0
ownpal = 1

[State 88500, High Fireball]
type = Projectile
trigger1 = time = 10
projanim = 131301
projid = 8850
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 25*const(size.xscale), 0*const(size.xscale)
offset = 180*const(size.xscale), -96*const(size.xscale)
velocity = 7.5, 0
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,5
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 35
ground.hittime  = 16
ground.velocity = -5
air.velocity = -1.5,-5
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5

[State 88500, Smokehit]
type = Explod
trigger1 = ProjContact(8850) = 1
anim = 902101
pos = 7*const(size.xscale), -103*const(size.xscale)
postype = p2
bindtime = -1
sprpriority = 3 
shadow = 0
ownpal = 1
ontop = 0

[State 88500, NotFreindly]
type = VarSet
trigger1 = AnimElem = 4 
v = 5
value = 1

[State 88500, Fall]
type = ChangeState
trigger1 = AnimTime = 0
value = 1101


; Drop
[Statedef 1101]
type    = A
movetype= I
physics = A
juggle  = 4
poweradd= 120
velset = 0,0
anim = 43 
ctrl = 0

[State 1101]
type = Veladd
trigger1 = time > 0
y = .3

;Bicycle Kick
[StateDef 2515]
type = A
movetype= A
physics = N
juggle = 5
ctrl = 0
anim = 1300


[State 2515, 2]
type = Velset
trigger1 = AnimElem = 1
trigger1 = movehit = 0
x = 9
y = .1

[State 2515, 2]
type = Velset
trigger1 = movehit = 1
x = 6
y = .04


[State 2515, 1]
type = VarRandom
trigger1 = AnimElem = 1
v = 11
range = 1, 2

[State 2515, 3.3]
type = Playsnd
trigger1 = Var(11) = 1
trigger1 = AnimElem = 1
value = 60,0

[State 2515, 3.4]
type = Playsnd
trigger1 = Var(11) = 2
trigger1 = AnimElem = 1
value = 20,6

[State 1300, 2]
type = HitDef
trigger1 = AnimElem = 1
trigger2 = AnimElem = 2
trigger3 = AnimElem = 3
trigger4 = AnimElem = 4
trigger5 = AnimElem = 5
trigger6 = AnimElem = 6
trigger7 = AnimElem = 7
trigger8 = AnimElem = 8
trigger9 = AnimElem = 9
trigger10 = AnimElem = 10
trigger11 = AnimElem = 11
trigger12 = AnimElem = 12
trigger13 = AnimElem = 13
trigger14 = AnimElem = 14
trigger15 = AnimElem = 15
trigger16 = AnimElem = 16
attr = S, NA
damage    = 9,2
animtype  = Back
getpower = 10,5
givepower = 10,5
guardflag = M
hitflag = HAF
priority = 7
pausetime = 3,0
sparkno = -1
guard.sparkno = -1
hitsound   = s27,27
guardsound = s0,0
ground.type = High
ground.slidetime = 50
ground.hittime  = 50
ground.velocity = -4,0
air.velocity = -4,-0
fall = 0
airfall = 1
envshake.time = 5
fall.envshake.time = 40
fall.recover = 0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)

[State 3000, hit]
type = Changestate
trigger1 = moveguarded
value = 3030
ctrl = 1

[State 3000, 7]
type = ChangeState
triggerall = Var(50) = 1
triggerall = power > 2000
trigger1 = AnimTime = 0
trigger1 = Pos Y >= 0
trigger1 = Vel Y > 0
trigger1 = movehit = 1
value = 4162
ctrl = 1

[State 3000, End]
type = Changestate
trigger1 = AnimTime = 0
trigger1 = Pos Y >= 0
trigger1 = Vel Y > 0
value = 0
ctrl = 1
;--------------------------------------
; Bike kick hit
[Statedef 3020]
type    = A
movetype= A
physics = N
juggle  = 5
anim = 1300
Velset = 3, 0
ctrl = 0

[State 3020, posadd]
type = Posadd
trigger1 = time = 0
x = 25

[State 3020, change]
type = Changestate
trigger1 = time = 50
value = 2010

;----------------------------------
; Bike kick bounce off 
[Statedef 3030]
type    = A
movetype= A
physics = A
juggle  = 5
anim = 1300
Velset = -.3, -6
ctrl = 0

[State 3030, change]
type = Veladd
trigger1 = time = 50

;--------------------------------------
; P2 state
[Statedef 3010]
type    = S
movetype= A
physics = N
juggle  = 1
anim = 5012
Velset = -3, 0
ctrl = 0

[State 3010]
type = selfstate
trigger1 = p2stateno = 2010
value = 5050


;High Fireball
[StateDef 8850]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 131302
sprpriority = 9

[State 8850, Fireball Chuck]
type = playsnd
trigger1 = AnimElem = 2
value = 4,10

[State 8850, Fireball]
type = playsnd
trigger1 = AnimElem = 3
value = 60,7

[State 8850, Fire Explode]
type = Explod
trigger1 = AnimElem = 3
anim = 131401
id = 131401
pos = -10*const(size.xscale), -105*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 1
ownpal = 1

[State 8850, High Fireball]
type = Projectile
trigger1 = time = 16
projanim = 131301
projid = 8850
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 25*const(size.xscale), 0*const(size.xscale)
offset = 180*const(size.xscale), -96*const(size.xscale)
velocity = 7.5, 0
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,5
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 35
ground.hittime  = 16
ground.velocity = -5
air.velocity = -1.5,-5
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5

[State 8850, Smokehit]
type = Explod
trigger1 = ProjContact(8850) = 1
anim = 902101
pos = 7*const(size.xscale), -103*const(size.xscale)
postype = p2
bindtime = -1
sprpriority = 3 
shadow = 0
ownpal = 1
ontop = 0

[State 8850, NotFreindly]
type = VarSet
trigger1 = AnimElem = 4 
v = 5
value = 1

[State 8850, 10]
type = ChangeState
trigger1 = AnimTime = 0
ctrl = 1
value = 0

;Low Fireball
[StateDef 32101]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 14442
sprpriority = 9

[State 32101, Fireball Chuck]
type = playsnd
trigger1 = AnimElem = 1
value = 4,10

[State 32101, Fireball]
type = playsnd
trigger1 = AnimElem = 2
value = 60,14

[State 32101, Fire Explode]
type = Explod
trigger1 = AnimElem = 2
anim = 131401
id = 131401
pos = -5*const(size.xscale), -60*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 1
ownpal = 1

[State 32101, Low Fireball]
type = Projectile
trigger1 = time = 11
projanim = 131301
id = 32101
projedgebound = 900 
projstagebound = 900
postype = p1
pos = 30*const(size.xscale), 0*const(size.xscale)
offset = 185*const(size.xscale), -50*const(size.xscale)
velocity = 7.5, 0
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 70, 15
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,5
sparkno = -1
guard.sparkno = -1
hitsound   = s60,15
guardsound = s0,0
ground.type = High
ground.slidetime = 25
ground.hittime  = 16
ground.velocity = -5
air.velocity = -1.5,-5
air.juggle = 3
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
palfx.time = 60
palfx.mul = 251,173,23
palfx.add = 200,0,0
palfx.sinadd = 251,173,23,5

[State 32101, Smokehit]
type = Explod
trigger1 = ProjContact(32101) = 1
anim = 902101
pos = 8*const(size.xscale), -65*const(size.xscale)
postype = p2
bindtime = -1
sprpriority = 3 
shadow = 0
ownpal = 1
ontop = 0

[State 32101, NotFreindly]
type = VarSet
trigger1 = AnimElem = 4 
v = 5
value = 1

[State 32101, 10]
type = ChangeState
trigger1 = AnimTime = 0
ctrl = 1
value = 0


;Flying Kick
[StateDef 131304]
type    = S     
movetype= A     
physics = S     
juggle  = 1     
velset = 0,0
ctrl = 0
anim = 131304
poweradd = 40

[State 131304, 1]
type = VarRandom
trigger1 = AnimElem = 1
v = 11
range = 1, 3

[State 131304, 3.3]
type = Playsnd
trigger1 = Var(11) = 1
trigger1 = AnimElem = 1
value = 60,8

[State 131304, 3.4]
type = Playsnd
trigger1 = Var(11) = 2
trigger1 = AnimElem = 1
value = 60,2

[State 131304, 3.4]
type = Playsnd
trigger1 = Var(11) = 3
trigger1 = AnimElem = 1
value = 60,1

[State 131304, 3.3]
type = Playsnd
trigger1 = AnimElem = 2
value = 60,13

[State 131304, Velocity3]
type = Velset
trigger1 = time = 5
x = 15
y = -2

[State 131304, Velocity4]
type = Velset
trigger1 = time = 10
x = 20

[State 131304, Velocity5]
type = Velset
trigger1 = time = 25
y = 2

[State 131304, Velocity6]
type = Velset
trigger1 = time = 15
trigger2 = time = 25
trigger3 = time = 30
x = 25

[State 131304, Velocity7]
type = Velset
trigger1 = movehit = 1
x = 0
y = 1.5

[State 131304, 3]
type = HitDef
trigger1 = AnimElem = 3
attr = S, NA  
damage    = 45,10
animtype = Back
guardflag = HA
hitflag = MAF
priority = 3, Hit
pausetime = 10,5
sparkno = -1
guard.sparkno = -1
hitsound = S27,27
guardsound = S0,0
ground.type = Low
ground.slidetime = 10
ground.hittime  = 5
ground.Velocity = -8
air.Velocity = -6,-4
down.Velocity = 0,0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall = 1
fall.recover = 0

[State 131304, Fall]
type = ChangeState
trigger1 = AnimTime = 0
value = 1101

[State 131304, 7]
type = ChangeState
trigger1 = Pos Y >= 0
trigger1 = Vel Y > 0
value = 0
ctrl = 1



[Statedef 2081]
type    = S
movetype= I
physics = S
juggle  = 3
ctrl = 0
anim = 5300
sprpriority = 1

[State 2081, 1]
type = Changeanim2
trigger1 = statetype = S
trigger1 = !animexist(5300)
value = 0

[State 2081, 3]
type = Selfstate
trigger1 = time = 100
trigger1 = life != 0
value = 0
ctrl = 1


;----------------------------------------------------------------
[Statedef 1003]
type    = S
movetype= I
physics = N
velset = 0,0
anim = 9998
ctrl = 0

[State 1003, GO BYE BYE]
type = DestroySelf
trigger1 = animtime = 0

;---------------------------------------
; Jump Away
[Statedef 2006]
type = S
movetype = I
physics = A
velset = -4.5,-8,0
anim = 43
ctrl = 0

[State 2006, 1]
type = Veladd
trigger1 = Pos Y < 0
Y = 10

;---------------------------------------------------------------------------
[Statedef 666]
type    = S                      ;State-type: S-stand, C-crouch, A-air, L-liedown
movetype= A                      ;Move-type: A-attack, I-idle, H-gethit
physics = S                      ;Physics: S-stand, C-crouch, A-air
juggle  = 1                      ;Number of air juggle points move takes
velset = 0,0                     ;Set velocity (x,y) (Def: no change)
ctrl = 0                         ;Set ctrl (Def: no change)
poweradd = 20   

[State 666, ChangeAnim]
type = ChangeAnim
trigger1 = time = 1
value = 20 

[State 666, 1]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 666, 2]
type = PlaySnd
trigger1 = Time = 60
value = 4000,11

[State 666, 3]
type = ChangeState
trigger1 = p2bodydist X < 10
value = 430