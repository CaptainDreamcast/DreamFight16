;========================================================================
;What makes the grass grow?  BLOOD! BLOOD! BLOOD
;========================================================================
;Nice Blood splat A
[Statedef 6000]
type    = S
movetype= I
juggle  = 0
anim = 6002
velset = 0, 9.4
ctrl = 0

[State 6000, 1]
type = VarRandom
trigger1 = time = 0
v = 11
range = 1, 100

[State 6000, 2]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [1, 24]
X = -.1

[State 6000, 2.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [25, 49]
X = -.2

[State 6000, 3]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [50, 74]
X = -.5

[State 6000, 3.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [75, 100]
X = -1

[State 6000, 4]
type = Veladd
trigger1 = time = [1, 10]
X = -.05


[State 6000, 5]
type = Changestate
trigger1 = Pos Y > 0
value = 6001
;------------------------------------------------------------------------
;Nice Blood splat A lands
[Statedef 6001]
type    = S
movetype= I
juggle  = 0
anim = 6003
velset = 0,0
ctrl = 0

[State 6001, 1]
type = Destroyself
trigger1 = animtime = 0
;------------------------------------------------------------------------
;Nice Blood splat A2
[Statedef 6002]
type    = S
movetype= I
juggle  = 0
anim = 6000
velset = 0, 9.4
ctrl = 0

[State 6002, 1]
type = VarRandom
trigger1 = time = 0
v = 11
range = 1, 100


[State 6002, 2]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [1, 24]
X = .1

[State 6002, 2.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [25, 49]
X = .2

[State 6002, 3]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [50, 74]
X = .5

[State 6002, 3.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [75, 100]
X = 1

[State 6002, 4]
type = Veladd
trigger1 = time = [1, 10]
X = .05


[State 6002, 5]
type = Changestate
trigger1 = Pos Y > 0
value = 6003
;------------------------------------------------------------------------
;Nice Blood splat A2 lands
[Statedef 6003]
type    = S
movetype= I
juggle  = 0
anim = 6001
velset = 0,0
ctrl = 0


[State 6003, 1]
type = Destroyself
trigger1 = animtime = 0
;------------------------------------------------------------------------
;------------------------------------------------------------------------
;Fatality A Spurt
[Statedef 6010]
type    = S
movetype= I
juggle  = 0
anim = 6010
velset = 0,0
ctrl = 0

[State 6010, 0.5]
type = Targetbind
trigger1 = time = [0, 2000]
pos = 0,0


[State 6010, 1]
type = Playsnd
trigger1 = animelem = 2
value = 1, 5


[State 6010, 2]
type = DestroySelf
trigger1 = time = 160
;------------------------------------------------------------------------
;High Punch Blood
[Statedef 6015]
type    = S
movetype= A
juggle  = 0
anim = 6015
velset = 0, 0
ctrl = 0

[State 6015, 1]
type = bindtotarget
trigger1 = time = 0
pos = 0,0,head


[State 6015, 2]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "Blood"
pos = 50, 40
helpertype = normal
sprpriority = 5
keyctrl = 0
ownpal = 0
under = 1

[State 6015, 3]
type = DestroySelf
trigger1 = animtime = 0
;=========================================================================
;-------------------------------------------------------------------------
;High Kick Blood
[Statedef 6020]
type    = S
movetype= A
juggle  = 0
anim = 6020
velset = 0, 0
ctrl = 0

[State 6020, .5]
type = bindtotarget
trigger1 = time = 0
time = 1
pos = 0,0,head

[State 6020, 1]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodA"
pos = 64, 15
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 2]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodB"
pos = 94, 43
helpertype = normal
sprpriority = 2
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 3]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodC"
pos = 86, 51
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 4]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodD"
pos = 61, 84
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 5]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodE"
pos = 40, 42
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 7]
type = DestroySelf
trigger1 = animtime = 0
;========================================================================
;-------------------------------------------------------------------------
;HARPOON
[Statedef 6030]
type    = S
movetype= A
juggle  = 0
anim = 6030
velset = 0, 0
ctrl = 0

[State 6030, 1]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodA"
pos = -96, 33
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6030, 2]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodB"
pos = -72, 42
helpertype = normal
sprpriority = 2
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 3]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodC"
pos = -61, 40
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 4]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodD"
pos = -60, 25
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 5]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodE"
pos = -61, 23
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 6.4]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodD"
pos = -45, 16
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 5]
type = Helper
trigger1 = animtime = 0
stateno = 6031
name = "BloodE"
pos = -54, 18
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6020, 7]
type = DestroySelf
trigger1 = animtime = 0
;-------------------------------------------------------------------------
;THE HAPPY BLOOD
[Statedef 6031]
type    = S
movetype= I
juggle  = 0
anim = 6031
velset = 0, 4.8
ctrl = 0

[State 6031, 1]
type = VarRandom
trigger1 = time = 0
v = 11
range = 1, 100


[State 6031, 2]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [1, 24]
X = -6.6

[State 6031, 2.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [25, 49]
X = -7.7

[State 6031, 3]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [50, 74]
X = -4.5

[State 6031, 3.5]
type = Velset
trigger1 = time = 1
trigger1 = Var(11) = [75, 100]
X = -3.5

[State 6031, 5]
type = Changestate
trigger1 = Pos Y > 0
value = 6003
;-------------------------------------------------------------------------
;UPPERCUT
[Statedef 6040]
type    = S
movetype= A
juggle  = 0
anim = 6040
velset = 0, 0
ctrl = 0

[State 6040, 1]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodA"
pos = 37, -18
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6040, 2]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodB"
pos = 42, -25
helpertype = normal
sprpriority = 2
keyctrl = 0
ownpal = 0
under = 1

[State 6040, 3]
type = Helper
trigger1 = animtime = 0
stateno = 6002
name = "BloodC"
pos = 58, -51
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6040, 4]
type = Helper
trigger1 = animtime = 0
stateno = 6000
name = "BloodD"
pos = -37, -64
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6040, 5]
type = Helper
trigger1 = animtime = 0
stateno = 6000
name = "BloodE"
pos = -49, -44
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 6040, 7]
type = DestroySelf
trigger1 = animtime = 0
;------------------------------------------------------------------------------------------

