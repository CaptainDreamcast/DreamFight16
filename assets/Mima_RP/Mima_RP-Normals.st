

;---------------------------------------------------------------------------
; Standing Weak Attack
[Statedef 200]
type    = S
movetype= A
physics = S
ctrl = 0
;velset = 0,0
anim = 200
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(0*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(0*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 200,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 0, 0

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5020, 5040])
trigger1 = animelemtime(3)>=0 && animelemtime(4)<0
trigger1 = (movecontact=0&&movereversed=0)
attr = S, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(20.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Light                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = M                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 8,8                 ;Time attacker pauses, time opponent shakes
sparkno = S8005+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-66
hitsound = s1,0                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 13+1*(var(42))             ;Time that the opponent slides back
ground.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
air.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
guard.slidetime = 10
guard.hittime = 10
ground.velocity = -4.0             ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -4.0,-2.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 40,40
givepower = 20,20
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.95
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(4) = 0
trigger2 = movecontact||movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 240, 4]
type = ChangeState
trigger1 = AnimTime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Standing Medium Attack
[Statedef 210]
type    = S
movetype= A
physics = S
ctrl = 0
;velset = 0,0
anim = 210
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(1*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(1*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 210,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 0,1

[State 200, 1]
type = HitDef
triggerall= (p2stateno != [5020, 5040])
triggerall = !movecontact && !movereversed
trigger1 = animelemtime(5)>=0 && animelemtime(6)<0
attr = S, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(49.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Medium                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = M                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8010+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -15,-40
hitsound = s1,1                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 16+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 16+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 16+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 13
guard.hittime = 13
ground.velocity = -4.0             ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -4.0,-6.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 60,60
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit;=1
fvar(10) = fvar(10)*0.90
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(6)=0
trigger2 = movecontact||movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Back/Forward Medium Attack
; "Spook Smack" ~ Rodknee
[Statedef 215]
type    = S
movetype= A
physics = S
ctrl = 0
;velset = 0,0
anim = 215
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(4*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(4*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 0,1

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 220,0
channel = 0

[State 200, 1]
type = HitDef
triggerall= (p2stateno != [5020, 5040])
triggerall = !movecontact && !movereversed
trigger1 = (prevstateno = [200,410])
trigger1 = animelemtime(5)>=0 && animelemtime(7)<0
attr = S, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(70.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Medium                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = M                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-46
hitsound = s1,2                 ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 18+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 13
guard.hittime = 13
ground.velocity = -5.0             ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -5.0,-6.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 80,80
givepower = 40,40
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, 1]
type = HitDef
triggerall= (p2stateno != [5020, 5040])
triggerall = !movecontact && !movereversed
trigger1 = (prevstateno != [200,410])
trigger1 = animelemtime(5)>=0 && animelemtime(7)<0
attr = S, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(70.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Medium                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = H                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-46
hitsound = s1,2                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 18+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 13
guard.hittime = 13
ground.velocity = -5.0           ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -5.0,-6.0 ;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 80,80
givepower = 40,40
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
fall = 0
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.80
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(7) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 226, CS]
type = changestate
triggerall = !ishelper
trigger1 = animtime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Standing Strong Attack
[Statedef 220]
type    = S
movetype= A
physics = S
ctrl = 0
;velset = 0,0
anim = 220+0*floor(var(11):=-4)
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(3*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(3*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 220, Displace]
type = posadd
trigger1 = animelemtime(2)>=0 && animelemtime(4)<0
x = 4

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 220,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 0,1
channel = 1

[State 200, 1]
type = HitDef
triggerall = (p2stateno != [5020, 5040])
triggerall = !movecontact && !movereversed
trigger1 = animelemtime(3)>=0 && animelemtime(4)<0
attr = S, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(70.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Heavy                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = M                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -20,-65
hitsound = s1,2                 ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 18+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 18+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 14
guard.hittime = 14
ground.velocity = -4.0             ;Velocity at which opponent is pushed
air.type = Low                 ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -4.0,-6.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 90,90
givepower = 45,45
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
fall = 0
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
;trigger1 = animelemtime(2)>=0 && animelemtime(3)<0
fvar(10) = fvar(10)*0.85
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(4) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Standing Strong Attack
[Statedef 225]
type    = S
movetype= A
physics = S
ctrl = 0
;velset = 0,0
anim = 225
sprpriority = 1
juggle = ifelse((var(34)&floor(exp(7*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(34) = var(34) | floor(exp(7*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 0,1

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 225,0
channel = 0

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5020, 5040])
trigger1 = animelemtime(3)>=0&&animelemtime(4)<0
trigger1 = !movecontact && !movereversed
attr = C, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(60.0*fvar(11)*ifelse(var(42),1.25,1.0)),0                   ;Damage that move inflicts, guard damage
animtype = Heavy                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = MA                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -20,-50
hitsound = s1,2                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 25+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 25+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 25+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 20
guard.hittime = 20
ground.velocity = -1.0,-8.0             ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -1.0,-8.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 90,90
givepower = 45,45
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.4
fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 225

[State 420, Custom Trip]
type = TargetState
triggerall = numtarget(225)
trigger1 = target,stateno!=[120,155]
trigger1 = movehit=1 && !movereversed
trigger1 = !target(225),ishelper
;trigger1 = target(215),time=0 && target(215),movetype=H
value = 6215
ID = 225
;ignorehitpause = 1
persistent = 0

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.75
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(4) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 210, JC]
type = changestate
trigger1 = movecontact
trigger1 = command = "holdup" || command = "U"
trigger1 = animelemtime(4) < 5
value = 40
ctrl = 0

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Crouching Weak Attack
[Statedef 400]
type    = C
movetype= A
physics = C
ctrl = 0
velset = 0,0
anim = 400
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(5*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(5*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 200,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 0,0

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5020, 5040])
trigger1 = animelemtime(3)>=0 && animelemtime(4)<0
trigger1 = !movecontact && !movereversed
attr = C, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(25.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Light                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = L                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 8,8                 ;Time attacker pauses, time opponent shakes
sparkno = S8000+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-20
hitsound = s1,0                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = Low               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 13+1*(var(42))             ;Time that the opponent slides back
ground.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
air.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
guard.slidetime = 10
guard.hittime = 10
ground.velocity = -4.0             ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -4.0,-5.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 40,40
givepower = 20,20
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.92
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(4) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 11
ctrl = 1


;---------------------------------------------------------------------------
; Crouching Medium Attack
[Statedef 410]
type    = C
movetype= A
physics = C
ctrl = 0
;velset = 0,0
anim = 410
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(6*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(6*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 210,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 0,1

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5020, 5040])
trigger1 = !movecontact && !movereversed
trigger1 = animelemtime(4)>=0 && animelemtime(5)<0
attr = C, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(50.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Medium                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = L                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8010+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -15,-10
hitsound = s1,1                 ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = Low               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 16+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 16+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 16+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 13
guard.hittime = 13
ground.velocity = -6.0             ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -6.0,-4.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 60,60
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.875
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(5) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 11
ctrl = 1


;---------------------------------------------------------------------------
; Crouching Strong Attack
[Statedef 420]
type    = C
movetype= A
physics = C
ctrl = 0
;velset = 0,0
anim = 420
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(7*ln(2)) + 0.5)),2,0) ;use Juggle
facep2 = 1

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(7*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 225,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 0,1

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5020, 5040])
trigger1 = animelemtime(5)>=0&&animelemtime(6)<0
trigger1 = !movecontact && !movereversed
attr = C, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(75.0*fvar(11)*ifelse(var(42),1.25,1.0)),0                   ;Damage that move inflicts, guard damage
animtype = Heavy                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = L                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10, 10                 ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-10
hitsound = s1,2                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 25+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 25+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 25+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 17
guard.hittime = 17
ground.velocity = -1.5,-4.5             ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -1.5,-4.5;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 90,90
givepower = 45,45
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.75
fall = 1
fall.recover = 0
p2facing = 1
ID = 420

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.75
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(6) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 420, Custom Trip]
type = TargetState
triggerall = numtarget(420)
trigger1 = target,stateno!=[120,155]
trigger1 = movehit=1 && !movereversed
trigger1 = !target(420),ishelper
trigger1 = target(420),time=0 && target(420),movetype=H
value = 470
ID = 420
ignorehitpause = 1
persistent = 0

[State 240, 4]
type = ChangeState
triggerall = !ishelper
trigger1 = AnimTime = 0
value = 11
ctrl = 1



;---------------------------------------------------------------------------
; Aerial Weak Attack
[Statedef 600]
type    = A
movetype= A
;physics = A
ctrl = 0
anim = 600
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(8*ln(2)) + 0.5)),2,0) ;use Juggle

[State 600, Flight Physics]
type = statetypeset
trigger1 = var(24)>0
trigger1 = time = 0
physics = N
[State 600, No Flight Physics]
type = statetypeset
trigger1 = var(24)<=0
physics = A
[State 1, VS]
type = velmul
trigger1 = var(24)>0
trigger1 = abs(vel x)+abs(vel y) > 0
x = 0.95
y = 0.95

[State Ground]
type = posset
trigger1 = pos y >= -(vel y)
y = 0
[State Ground]
type = velset
trigger1 = vel y >= 0
trigger1 = pos y >= -(vel y)
y = 0

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(8*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 200,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(3)=0
value = 0,0

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5040, 5040])
trigger1 = animelemtime(4)>=0 && animelemtime(5)<0
trigger1 = !movecontact && !movereversed
attr = A, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(40.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Light                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = HA                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 8,8                 ;Time attacker pauses, time opponent shakes
sparkno = S8005+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -10,-18
hitsound = s1,0                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 13+1*(var(42))             ;Time that the opponent slides back
ground.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
air.hittime  = 13+1*(var(42))             ;Time opponent is in hit state
guard.slidetime = 10
guard.hittime = 10
ground.velocity = -4.5             ;Velocity at which opponent is pushed
air.type = Low                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -4.5,-4.0;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 40,40
givepower = 20,20
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.90
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(5) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = animelemtime(8)=0
value = cond(pos y>=0,0,301)
ctrl = 1

;---------------------------------------------------------------------------
; Aerial Medium Attack
[Statedef 610]
type    = A
movetype= A
;physics = A
ctrl = 0
anim = 610
sprpriority = 1
juggle = ifelse((var(33)&floor(exp(9*ln(2)) + 0.5)),2,0) ;use Juggle

[State 600, Flight Physics]
type = statetypeset
trigger1 = var(24)>0
trigger1 = time = 0
physics = N
[State 600, No Flight Physics]
type = statetypeset
trigger1 = var(24)<=0
physics = A
[State 1, VS]
type = velmul
trigger1 = var(24)>0
trigger1 = abs(vel x)+abs(vel y) > 0
x = 0.95
y = 0.95

[State Ground]
type = posset
trigger1 = pos y >= -(vel y)
y = 0
[State Ground]
type = velset
trigger1 = vel y >= 0
trigger1 = pos y >= -(vel y)
y = 0

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(33) = var(33) | floor(exp(9*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 210,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(4)=0
value = 0,1

[State 200, 1]
type = HitDef
trigger1= (p2stateno != [5040, 5040])
trigger1 = animelemtime(4)>=0 && animelemtime(5)<0
trigger1 = !movecontact && !movereversed
attr = A, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(55.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Medium                 ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = HA                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                 ;Time attacker pauses, time opponent shakes
sparkno = S8010+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -15,-50
hitsound = s1,1                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 13+4*(var(24)>0)+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 13+4*(var(24)>0)+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 13+4*(var(24)>0)+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 14
guard.hittime = 14
ground.velocity = -6.0*cond(p2dist x<0,-1,1)             ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -6.0*cond(p2dist x<0,-1,1),-4.5;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 60,60
givepower = 30,30
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1*cond(p2dist x<0,-1,1)
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit
fvar(10) = fvar(10)*0.85
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(6) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = animelemtime(9)=0
value = cond(pos y>=0,0,301)
ctrl = 1

;---------------------------------------------------------------------------
; Aerial Strong Attack
[Statedef 620]
type    = A
movetype= A
;physics = A
ctrl = 0
anim = 620
sprpriority = 1
juggle = ifelse((var(34)&floor(exp(0*ln(2)) + 0.5)),2,0) ;use Juggle

[State 600, Flight Physics]
type = statetypeset
trigger1 = var(24)>0
trigger1 = time = 0
physics = N
[State 600, No Flight Physics]
type = statetypeset
trigger1 = var(24)<=0
physics = A
[State 1, VS]
type = velmul
trigger1 = var(24)>0
trigger1 = abs(vel x)+abs(vel y) > 0
x = 0.95
y = 0.95

[State Ground]
type = posset
trigger1 = pos y >= -(vel y)
y = 0
[State Ground]
type = velset
trigger1 = vel y >= 0
trigger1 = pos y >= -(vel y)
y = 0

[State Give 'em a lil' 40, taken from The_None]
type = VarSet
triggerall = !ishelper
trigger1 = var(31) = [1,2]
trigger1 = numtarget
trigger1 = (target,statetype = A) || (target,statetype = L)
var(34) = var(34) | floor(exp(0*ln(2)) + 0.5) ;Flag the move if the move has hit in a juggle way.
ignorehitpause = 1
persistent = 0

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 220,0
channel = 0

[State 240, 1]
type = PlaySnd
trigger1 = animelemtime(5)=0
value = 620,10

[State 0, Explod]
type = Explod
trigger1 = animelemtime(5)=0
anim = 20620
ID = 20620
pos = 26,-64
postype = p1  ;p2,front,back,left,right
facing = 1
bindtime=-1
removetime = -2
scale = 1,1
sprpriority = 5
ownpal = 0
removeongethit = 0
ignorehitpause = 0

[State 200, 1]
type = HitDef
triggerall= (p2stateno != [5040, 5040])
triggerall = !movecontact && !movereversed
trigger1 = animelemtime(5)>=0 && animelemtime(6)<0
attr = A, NA                     ;Attribute: Standing, Normal Attack
damage = ceil(65.0*fvar(11)*ifelse(var(42),1.25,1.0)), 0                   ;Damage that move inflicts, guard damage
animtype = Heavy                ;Animation type: Light, Medium, Heavy, Back (def: Light)
guardflag = HA                   ;Flags on how move is to be guarded against
hitflag = MAF                    ;Flags of conditions that move can hit
priority = 4, Hit
pausetime = 10,10                ;Time attacker pauses, time opponent shakes
sparkno = S8020+(random%2)                      ;Spark anim no (Def: set above)
guard.sparkno = S8030
sparkxy = -20,-65
hitsound = s1,2                  ;Sound to play on hit
guardsound = s2,0                ;Sound to play on guard
ground.type = High               ;Type: High, Low, Trip (def: Normal)
ground.slidetime = 13+10*(var(24)>0)+3*(var(42))             ;Time that the opponent slides back
ground.hittime  = 13+10*(var(24)>0)+3*(var(42))             ;Time opponent is in hit state
air.hittime  = 13+10*(var(24)>0)+3*(var(42))             ;Time opponent is in hit state
guard.slidetime = 18
guard.hittime = 18
ground.velocity = -8.0             ;Velocity at which opponent is pushed
air.type = High                  ;Type: High, Low, Trip (def: same as ground.type)
air.velocity = -8.0,-7.5;-1.4,-3           ;X-velocity at which opponent is pushed,
;Y-velocity at which opponent is pushed
getpower = 90,90
givepower = 45,45
ground.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
air.cornerpush.veloff = 0
yaccel = 0.5
fall = 0
air.fall = 1
fall.recover = 1
fall.recovertime = 120
p2facing = 1
ID = 555

[State 200, Damage Dampen]
type = varset
trigger1 = movehit=1
fvar(10) = fvar(10)*0.80
ignorehitpause = 1
persistent = 0

[State -2, FLAG RECOVERY FRAMES]
type = statetypeset
trigger1 = animelemtime(6) = 0
trigger2 = movecontact || movereversed
movetype = I
persistent = 0
ignorehitpause = 1

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = animelemtime(9)=0
value = cond(pos y>=0,0,301)
ctrl = 1

;----------------
;Throw
[Statedef 800]
type    = S
movetype= A
physics = S
ctrl = 0
anim = 800
;velset=0,0
sprpriority = 1

[State 800, Tech Reset]
type = varset
trigger1 = 1
var(14) = 0

[State 800, Tech Set]
type = varset
trigger1 = numenemy > 0
trigger1 = enemynear(0),statetype != A
trigger1 = enemynear(0),stateno = [0,999]
trigger1 = enemynear(0),stateno != [200,799]
trigger1 = enemynear(0),stateno != [100,119]
var(14) = 7

[State Asshole]
type = StateTypeSet
trigger1 = animelemtime(3)=0
movetype = I

[State 800, VS]
type = varset
trigger1 = AILevel <= 0
trigger1 = animelemtime(1) = 0
var(10)=ifelse(command="holdback",1,0)

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 210,0
channel = 0

[State 800, PlaySnd]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 0,1

[State 800, IDDQD]
type = HitDef
trigger1 = Animelemtime(2)=0
trigger1 = var(30) = 0; Throw Limited;!Movecontact && !Movereversed ;Just in case 
attr = S,NT
hitflag = M-
guardflag = 
fall.animtype = Diagup
priority = 7,Miss
damage = 0,0
pausetime = 0 ,0
sparkno = -1
hitsound = 1,1
yaccel = 0.5
;ground.velocity = -1.5,-12
ground.cornerpush.veloff = 0
air.cornerpush.veloff = 0
down.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
airguard.cornerpush.veloff = 0
p1facing = 1;
p2facing = 1
p1stateno = 810;+1*(var(10)=1)
fall = 1
fall.recover = 0
id = 800
numhits = 0

[State ASS]
type = AttackDist
trigger1 = 1
value = 0

[State 800, Flag Recovery]
type = statetypeset
trigger1 = animelemtime(3)=0
movetype = I

[State IDCLIP]
type = ChangeState
trigger1 = Animtime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
;Ground Grab - Part 2
[Statedef 810]
type    = S
movetype= A
physics = S
ctrl = 0
anim = 810+0*floor(var(11):=0)+0*floor(var(4):=1)+0*floor(fvar(10):=(fvar(10)*0.9));Set proration flag
velset = 0, 0
sprpriority = 1

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(1)=0
value = 810,0
channel = 0

[State 810, PSND]
type = playsnd
trigger1 = time = 0
value = 1,0

[State 810, T]
type = turn
trigger1 = animelemtime(2)=0
trigger1 = var(10)=1

[State 0, Width]
type = Width
trigger1 = 1
edge = 38,38
ignorehitpause = 1
;persistent = 

[State 801, TS]
type = targetstate
trigger1 = time = 0
id = 800
value = 850

[State 801, TB]
type = targetbind
trigger1 = time = 0
;trigger2 = animelemtime(2) = 0
id = 800
pos = 38,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(2) = 0
id = 800
pos = 34,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(3) = 0
id = 800
pos = 18,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(4) = 0
id = 800
pos = 34,-87
;ignorehitpause = 
;persistent = 

[State 801, TB]
type = targetbind
trigger1= animelemtime(5) = 0
id = 800
pos = 34,-87-(target(800),const(size.mid.pos.y))
;ignorehitpause = 
;persistent = 

[State 801, TS]
type = targetstate
trigger1 = animelemtime(4)=0
id = 800
value = 851

[State 801, TF]
type = targetfacing
trigger1 = time = 0
trigger2 = animelemtime(2) = 0
value = -1
id = 800

[State 801, Tech Success]
type = ChangeState
triggerall = NumTarget
triggerall = var(14)>0
trigger1 = (target,command = "holdfwd" || target,command = "holdback") && target,command != "holdup" && target,command != "holddown"
trigger1 = (target,AILevel=0)&&(target,command = "x" || target,command = "y" || target,command = "z")
trigger2 = (target,command = "holdfwd" || target,command = "holdback") && target,command != "holdup" && target,command != "holddown"
trigger2 = (target,AILevel=0)&&(target,command = "a" || target,command = "b" || target,command = "c")
trigger3 = (target,AILevel>0)&&(random<=floor(100*AIlevel*AIlevel/64.0))
value = 890

[State 810, Throw Escape]
type = VarAdd
trigger1 = var(14) > 0
var(14) = -1



[State 801, TS]
type = targetstate
trigger1 = animelemtime(5)=0
id = 800
value = 853

[State 0, EnvShake]
type = EnvShake
trigger1 = animelemtime(5)=0
time = 15
freq = 120
ampl = -4


[State 810, PSND]
type = playsnd
trigger1 = animelemtime(5)=0
value = 800,10

[State 0, Explod]
type = Explod
trigger1 = animelemtime(5)=0
anim = 8020+(random%2)
ID = 8000
pos = 34,-83
postype = p1  ;p2,front,back,left,right
bindtime = 1
vel = 0,0
accel = 0,0
random = 0,0
removetime = -2
pausemovetime = 0
scale = 1,1
sprpriority = 5
ontop = 1
ownpal = 1
removeongethit = 0

[State 1000, Effect]
type = helper
trigger1 = animelemtime(5) = 0
helpertype = normal
name = "Ring FX"
ID = 20800
pos = 34,-87
postype = p1
stateno = 20800
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
[State 1000, Effect]
type = helper
trigger1 = animelemtime(5) = 0
helpertype = normal
name = "Ring FX"
ID = 20801
pos = 34,-87
postype = p1
stateno = 20800
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5


[State 810, PSND]
type = hitadd
trigger1 = animelemtime(5)=0
value = 1

[State 810, PSND]
type = targetlifeadd
trigger1 = animelemtime(5)=0
value = -70
id = 800
kill = 1

[State 810, PSND]
type = targetpoweradd
trigger1 = animelemtime(5)=0
value = 40
id = 800

[State 810, PSND]
type = poweradd
trigger1 = animelemtime(5)=0
value = 80

[State 1007, CS]
type = changestate
trigger1 = animtime = 0
value = 0
ctrl = 1

;----------------
;Air Throw
[Statedef 820]
type    = A
movetype= A
physics = N;S
ctrl = 0
anim = 801
;velset=0,0
sprpriority = 1

[State 600, Flight Physics]
type = statetypeset
trigger1 = var(24)>0
trigger1 = time = 0
physics = N
[State 600, No Flight Physics]
type = statetypeset
trigger1 = var(24)<=0
physics = A
[State 1, VS]
type = velmul
trigger1 = var(24)>0
trigger1 = abs(vel x)+abs(vel y) > 0
x = 0.95
y = 0.95

[State Ground]
type = posset
trigger1 = pos y >= -(vel y)
y = 0
[State Ground]
type = velset
trigger1 = vel y >= 0
trigger1 = pos y >= -(vel y)
y = 0

[State Asshole]
type = StateTypeSet
trigger1 = animelemtime(3)=0
movetype = I

[State 800, VS]
type = varset
trigger1 = AILevel <= 0
trigger1 = animelemtime(1) = 0
var(10)=ifelse(command="holdback",1,0)

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 210,0
channel = 0

[State 800, PlaySnd]
type = PlaySnd
trigger1 = animelemtime(2)=0
value = 0,1

[State 800, IDDQD]
type = HitDef
trigger1 = Animelemtime(2)=0
trigger1 = var(30) = 0; Throw Limited;!Movecontact && !Movereversed ;Just in case 
attr = A,NT
hitflag = A-
guardflag = 
fall.animtype = Diagup
priority = 7,Miss
damage = 0,0
pausetime = 0 ,0
sparkno = -1
hitsound = 1,1
yaccel = 0.5
;ground.velocity = -1.5,-12
ground.cornerpush.veloff = 0
air.cornerpush.veloff = 0
down.cornerpush.veloff = 0
guard.cornerpush.veloff = 0
airguard.cornerpush.veloff = 0
p1facing = 1;
p2facing = 1
p1stateno = 830;+1*(var(10)=1)
fall = 1
fall.recover = 0
id = 820
numhits = 0

[State ASS]
type = AttackDist
trigger1 = 1
value = 0

[State 800, Flag Recovery]
type = statetypeset
trigger1 = animelemtime(3)=0
movetype = I

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = animelemtime(10)=0
value = cond(pos y>=0,0,301)
ctrl = 1

;---------------------------------------------------------------------------
;Air Grab - Part 2
[Statedef 830]
type    = A
movetype= A
physics = S
ctrl = 0
anim = 820+0*floor(var(11):=0)+0*floor(var(4):=1)+0*floor(fvar(10):=(fvar(10)*0.9));Set proration flag
velset = 0, 0
sprpriority = 1

[State 200, PSND]
type = PlaySnd
trigger1 = animelemtime(1)=0
value = 810,0
channel = 0

[State 810, PSND]
type = playsnd
trigger1 = time = 0
value = 1,0

[State 810, T]
type = turn
trigger1 = animelemtime(2)=0
trigger1 = var(10)=1

[State 0, Width]
type = Width
trigger1 = 1
edge = 38,38
ignorehitpause = 1
;persistent = 

[State 801, TS]
type = targetstate
trigger1 = time = 0
id = 820
value = 850

[State 801, TB]
type = targetbind
trigger1 = time = 0
;trigger2 = animelemtime(2) = 0
id = 820
pos = 38,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(2) = 0
id = 820
pos = 34,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(3) = 0
id = 820
pos = 18,0

[State 801, TB]
type = targetbind
trigger1= animelemtime(4) = 0
id = 820
pos = 34,-57
;ignorehitpause = 
;persistent = 

[State 801, TB]
type = targetbind
trigger1= animelemtime(5) = 0
id = 820
pos = 34,-57-(target(820),const(size.mid.pos.y))
;ignorehitpause = 
;persistent = 

[State 801, TS]
type = targetstate
trigger1 = animelemtime(4)=0
id = 820
value = 861

[State 801, TF]
type = targetfacing
trigger1 = time = 0
trigger2 = animelemtime(2) = 0
value = -1
id = 820

[State 801, TS]
type = targetstate
trigger1 = animelemtime(5)=0
id = 820
value = 862

[State 0, EnvShake]
type = EnvShake
trigger1 = animelemtime(5)=0
time = 15
freq = 120
ampl = -4


[State 810, PSND]
type = playsnd
trigger1 = animelemtime(5)=0
value = 800,10

[State 0, Explod]
type = Explod
trigger1 = animelemtime(5)=0
anim = 8020+(random%2)
ID = 8000
pos = 34,-53
postype = p1  ;p2,front,back,left,right
bindtime = 1
vel = 0,0
accel = 0,0
random = 0,0
removetime = -2
pausemovetime = 0
scale = 1,1
sprpriority = 5
ontop = 1
ownpal = 1
removeongethit = 0

[State 1000, Effect]
type = helper
trigger1 = animelemtime(5) = 0
helpertype = normal
name = "Ring FX"
ID = 20802
pos = 34,-57
postype = p1
stateno = 20800
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5
[State 1000, Effect]
type = helper
trigger1 = animelemtime(5) = 0
helpertype = normal
name = "Ring FX"
ID = 20803
pos = 34,-57
postype = p1
stateno = 20800
ownpal = 1
facing = 1
size.xscale = 0.5
size.yscale = 0.5


[State 810, PSND]
type = hitadd
trigger1 = animelemtime(5)=0
value = 1

[State 810, PSND]
type = targetlifeadd
trigger1 = animelemtime(5)=0
value = -60
id = 820
kill = 1

[State 810, PSND]
type = targetpoweradd
trigger1 = animelemtime(5)=0
value = 40
id = 820

[State 810, PSND]
type = poweradd
trigger1 = animelemtime(5)=0
value = 80

[State 600, No Flight Physics]
type = statetypeset
trigger1 = animelemtime(11)=0
trigger1 = var(24)<=0
physics = A

[State 600, Return to flight State]
type = changestate
trigger1 = var(24)>0
trigger1 = animelemtime(11)=0
value = cond(pos y>=0,0,301)
ctrl = 1

;---------------------------------------------------------------------------
;Throw Tech Success
[StateDef 890]
type = S
physics = N
movetype = A;I
anim = 5945
ctrl = 0
velset = 0,0
sprPriority = 1

[State 800, REX]
type = removeexplod
trigger1 = animelemtime(1)=0
id =20800

[State 890, PSND]
type = PlaySnd
trigger1 = AnimElemtime(2) = 0
value = 890,0
channel = 0

[State 890, PSND]
type = PlaySnd
trigger1= time = 0
value = 890,10

[State 890, T]
type = null;turn
trigger1 = time = 0

[State 890, PA]
type = PosAdd
trigger1 = time = 0
x = 24

[State 890, PS]
type = PosSet
trigger1 = time = 0
y = 0

[State 890, TB]
type = TargetBind
trigger1 = time = 0
trigger1 = numtarget
pos = 0,0

[State 890, TS]
type = TargetState
trigger1 = time = 0
trigger1 = numtarget
value = 895

[State 890, TF]
type = TargetFacing
trigger1 = time = 0
trigger1 = numtarget
value = -1

[State 890, NHB]
type = NotHitBy
trigger1 = 1
value = SCA
time = 1

[State 890, PP]
type = PlayerPush
trigger1 = 1
value = 0
ignoreHitPause = 1

[State 0, Explod]
type = Explod
trigger1 = time = 0
persistent = 4
anim = 8020
ID = 8000
pos = 0,-70
postype = p1  ;p2,front,back,left,right
facing = 1
removetime = -2
pausemovetime = 0
scale = 1,1
sprpriority = 5
ontop = 1
ownpal = 1
removeongethit = 0
pausemovetime = 999999
supermovetime = 999999

[State 890, VS]
type = VelSet
trigger1 = time = 0
x = -8;-6.14
y = 0

[State 890, VM]
type = veladd;mul
trigger1 = time>=0
trigger1 = vel x < 0
x = 0.5;85

[State 890, PA]
type = posadd
triggerall = numtarget
trigger1 = target,frontedgebodydist <= abs(target,vel x + target,const(size.ground.front))
trigger1 = target,vel x > 0.0
trigger2 = target,backedgebodydist <= abs(target,vel x - target,const(size.ground.back))
trigger2 = target,vel x < 0.0
x = cond((target,pos x - pos x) * facing > 0, -abs(target,vel x), abs(target,vel x))


[State 890, CS]
type = ChangeState
trigger1 = time >= 27
value = 0
ctrl = 1

;------------------------
;Target Throw Escape
[StateDef 895]
type = S
physics = N
moveType = H
ctrl = 0
velSet = 0,0
sprPriority = 0

[State 895, PS]
type = PosSet
trigger1 = time = 0
y = 0

[State 895, CA]
type = ChangeAnim
trigger1 = time = 0
value = ifElse(SelfAnimExist(5940), 5940, 130)

[State 895, CA]
type = ChangeAnim
trigger1 = time = 0
trigger1 = !selfanimexist(5945)
trigger1 = selfanimexist(5940)
value = 5940

[State 895, CA]
type = ChangeAnim
trigger1 = time = 0
trigger1 = !selfanimexist(5945)
trigger1 = !selfanimexist(5940)
trigger1 = selfanimexist(5910)
value = 5910

[State 895, CA]
type = ChangeAnim
trigger1 = time >= 20
trigger1 = anim = 130
value = 140

[State 895, NHB]
type = NotHitBy
trigger1 = 1
value = SCA
time = 1

[State 895, PP]
type = PlayerPush
trigger1 = 1
value = 0

[State 890, VS]
type = VelSet
trigger1 = time = 0
x = -8;-6.14
y = 0

[State 890, VM]
type = veladd;mul
trigger1 = time>=0
trigger1 = vel x < 0
x = 0.5;85

[State 895, CS]
type = SelfState
trigger1 = Time >= 27
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Throw FX
[Statedef 20800]
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
trigger1 = ishelper(20800)||ishelper(20801)
trigger1 = (var(0):=45)||1
trigger2 = ishelper(20802)||ishelper(20803)
trigger2 = (var(0):=-45)||1

[State 22100, VS]
type = velset
trigger1 = time = 0
x = 0.4243*cos(var(0)*pi/180.0)*2
y = -0.4243*sin(var(0)*pi/180.0)*2

[State 22100, ASS]
type = assertspecial
trigger1 = 1
flag = noshadow
ignorehitpause = 1

[State 22100, VA]
type = varadd
trigger1 = 1;ishelper(22101)||ishelper(22103)
fvar(0) = cond(time<8,0.065+0.35*(ishelper(20800)||ishelper(20802)),0.01+0.02*(ishelper(20800)||ishelper(20802)))

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
trigger1 = time>=16;30
trigger2 = root,movetype = H