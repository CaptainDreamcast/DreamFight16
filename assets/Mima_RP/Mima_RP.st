;---------------------------------------------------------------------------
; Variable usage:
; var(1)  - Used for combo condition (updated every tick)
; Var(10) - Variation Identifier

; Var(33) - Juggle points 1
;   0 - state 
; Var(34) - Juggle points 2
;   0 - state 

; FVar(10) - Damage Scaler


;-------------------------
; Upwards Jump
[Statedef 117]
type    = A
physics = N
ctrl = 0
velset = 0,0
anim = 117
sprpriority = 1

[State 100, PSND]
type = playsnd
trigger1 = animelemtime(3)=0
value = 100,0
persistent = 0

[State 105, 1]
type = VarSet
trigger1 = time=0
var(10) = ifelse((command="holdfwd"),45,ifelse((command="holdback"),135,90))

[State 105, 1]
type = VelSet
trigger1 = animelemtime(3)=0
x = 4.0*cos(var(10)*pi/180.0)
y = -7.5*sin(var(10)*pi/180.0)

[State 117, VA]
type = veladd
trigger1 = animelemtime(3)>1
y = 0.2+0.3*(animelemtime(6)>=0)

[State 700, Explod]
type = Explod
trigger1 = Time = 0;2
ID = 700
anim = 701
pos = 50,-50
removetime = 11
sprpriority = 1
facing = -1

[State 700, Effect]
type = helper
trigger1 = time = 0;2
helpertype = normal
name = "Dash_FX"
ID = 10110+1*(var(10)=135)+2*(var(10)=45)
pos = 0,-90;-15,-50
postype = p1
stateno = 10100
ownpal = 1
facing = -1

[State 105, 3]
type = ChangeState
trigger1 = Vel Y > 0
trigger1 = Pos Y >= -(vel y)
value = 106

;-------------------------
; Downwards Jump
[Statedef 118]
type    = A
physics = N
ctrl = 0
velset = 0,0
anim = 117
sprpriority = 1

[State 100, PSND]
type = playsnd
trigger1 = animelemtime(3)=0
value = 100,0
persistent = 0

[State 105, 1]
type = VarSet
trigger1 = time=0
var(10) = ifelse((command="holdfwd"),45,ifelse((command="holdback"),135,90))

[State 105, 1]
type = VelSet
trigger1 = animelemtime(3)=0
x = 4.0*cos(var(10)*pi/180.0)
y = 6.0*sin(var(10)*pi/180.0)

[State 117, VA]
type = veladd
trigger1 = animelemtime(3)>1
y = 0.2+0.3*(animelemtime(6)>=0)

[State 700, Explod]
type = Explod
trigger1 = Time = 0;2
ID = 700
anim = 701
pos = 50,-50
removetime = 11
sprpriority = 1
facing = -1

[State 700, Effect]
type = helper
trigger1 = time = 0;2
helpertype = normal
name = "Dash_FX"
ID = 10115+1*(var(10)=135)+2*(var(10)=45)
pos = 0,0;-15,-50
postype = p1
stateno = 10100
ownpal = 1
facing = -1

[State 105, 3]
type = ChangeState
trigger1 = Vel Y > 0
trigger1 = Pos Y >= -(vel y)
value = 106


;---------------------------------------------------------------------------
; Lose by Time Over
; CNS difficulty: basic
[Statedef 170]
type = S
ctrl = 0
anim = 170
velset = 0,0

[State 170, PSND]
type = playsnd
trigger1 = time = 0
value = 171,0

[State 170, 1]
type = NotHitBy
trigger1 = 1
value = SCA
time = 1

;---------------------------------------------------------------------------
; Win state decider
; CNS difficulty: basic
[Statedef 180]
type = S

[State 180, VQ]
type = victoryquote
trigger1 = time = 0
value = 1+(random%6)

[State 180, Reimu Win Quote]
type = victoryquote
triggerall = numenemy
triggerall = Time = 0
trigger1 = enemy,name = "Reimu_RP" || enemy,name = "Reimu" || enemy,name = "Reimu Hakurei"
trigger2 = enemy,name = "Hakurei Reimu" || enemy,name = "Reimu_Hakurei" || enemy,name = "Hakurei_Reimu" 
trigger3 = enemy,name = "Reimu_THIZ"
value = 7+(random%2)

[State 180, Marisa Win Quote]
type = victoryquote
triggerall = numenemy
triggerall = Time = 0
trigger1 = enemy,name = "Marisa_RP" || enemy,name = "Marisa_K" || enemy,name = "Marisa Kirisame"
trigger2 = enemy,name = "Marisa_Kirisame" || enemy,name = "Kirisame_Marisa" || enemy,name = "Kirisame Marisa"
trigger3 = enemy,name = "Marisa_THIZ"
value = 9+(random%2)

[State 180, Yuuka Win Quote]
type = victoryquote
triggerall = numenemy
triggerall = Time = 0
trigger1 = enemy,name = "Yuuka_RP" || enemy,name = "Yuuka Kazami" || enemy,name = "Yuuka_Kazami"
trigger2 = enemy,name = "Yuka Kazami" || enemy,name = "Kazami Yuuka" || enemy,name = "Kazami_Yuuka"
trigger3 = enemy,name = "Yuka_Kazami" || enemy,name = "Kazami Yuka"  || enemy,name = "Kazami_Yuka"
trigger4 = enemy,name = "Ultimate Sadistic Creature" || enemy,name = "Yuuka_THIZ"
value = 11+(random%2)

[State 180, Alice Win Quote]
type = victoryquote
triggerall = numenemy
triggerall = Time = 0
trigger1 = enemy,name = "Alice_RP" || enemy,name = "Alice Margatroid"
trigger2 = enemy,name = "Alice_THIZ"
value = 13+(random%2)

[State 180, 1]
type = ChangeState
trigger1 = Time = 0
value = 181

;---------------------------------------------------------------------------
; Win pose
[Statedef 181]
type = S
ctrl = 0
anim = 18100+10*floor(2.0*random/1000.0)
velset = 0,0

[State 181, ASS]
type = assertspecial
trigger1 = time<120
flag = roundnotover

[State 181, PSND]
type = playsnd
trigger1 = anim = 18110
trigger1 = animelemtime(6)=0
value = 0,1

[State 181, 1]
type = NotHitBy
trigger1 = 1
value = SCA
time = 1

;---------------------------------------------------------------------------
; Introduction
; CNS difficulty: basic
; CNS ƒŒƒxƒ‹: Šî–{
[Statedef 191]
type = S
ctrl = 0
anim = 19000+10*floor(2.0*random/1000.0)
velset = 0,0

[State 191, 2] ;Assert this until you want "round 1, fight" to begin
type = AssertSpecial
trigger1 = 1
flag = Intro
flag2 = nobardisplay

[State 191, 3] ;Change to stand state when done
type = ChangeState
trigger1 = animtime=0
value = 0

;---------------------------------------------------------------------------
; Taunt
[Statedef 195]
type = S
ctrl = 0
anim = 195
velset = 0,0
movetype = I
physics = S
sprpriority = 1

;[State 195, VS]
;type = varset
;trigger1 = time = 0
;var(10) = (random%2)

[State 195, PSND]
type = playsnd
trigger1 = animelemtime(4)=0
value = 0,1

;[State 195, PSND]
;type = playsnd
;trigger1 = var(10)=1
;trigger1 = animelemtime(1)=0
;value = 195,1


[State 195, 2]
type = ChangeState
trigger1 = animtime = 0
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Jump Up
[Statedef 300]
type    = S
physics = S
ctrl = 0

[State 0, ChangeAnim]
type = ChangeAnim
trigger1 = time = 0
value = 40

[State 50, 1]
type = VarSet
trigger1 = Time = 0
sysvar(1) = 0

[State 50, 2]
type = ChangeAnim
trigger1 = anim=40
trigger1 = animtime = 0
value = 41

[State 300, STS]
type = statetypeset
trigger1 = anim=41
trigger1 = animelemtime(1)=0
statetype = A
physics = N

[State 300, VS]
type = varset
trigger1 = anim=41
var(24)=600

[State 300, VS]
type = playsnd
trigger1 = anim=41
trigger1 = animelemtime(1)=0
value = 300,0

[State 300, VS]
type = posadd;velset
trigger1 = anim=41
trigger1 = animelemtime(1)>=0
y = -10+animelemtime(1)

;[State 300, VS]
;type = veladd
;trigger1 = anim=41
;trigger1 = animelemtime(1)>0
;y = 1.0

[State 50, 3]
type = Changestate
trigger1 = anim=41
trigger1 = animelemtime(1)>=10;vel y > 0
trigger1 = (var(24):=600)||1
value = 301

;---------------------------------------------------------------------------
; Flight
[Statedef 301]
type = A
physics = N
sprpriority = 0

[State 1, VS]
type = velset
trigger1 = time = 0
trigger1 = prevstateno = 50
y = 0

[State 1, ASS]
type = assertspecial
trigger1 = 1
flag = nowalk
;flag2 = noairguard
ignorehitpause = 1

[State 1, T]
type = changeanim
trigger1 = p2dist x < -5
value = 5

[State 1, T]
type = turn
trigger1 = p2dist x < -5

[State 0, 1]
type = ChangeAnim
trigger1 = Anim != 0 && Anim != 5
trigger1 = command != "holdfwd" && command != "holdback"
trigger2 = Anim = 5 && AnimTime = 0 ;Turn anim over
trigger3 = Anim != 0 && Anim != 5
trigger3 = command = "holdfwd" || command = "holdback"
trigger3 = Anim != 20 && Anim != 21
value = 0

[State 1, CA]
type = changeanim
triggerall = anim!= 5
trigger1 = command = "holdfwd"
trigger1 = command != "holdback"
trigger1 = anim != 20
trigger2 = command != "holdfwd"
trigger2 = command = "holdback"
trigger2 = anim != 21
value = ifelse((command = "holdfwd"),20,21)

[State 1, VS]
type = velset
trigger1 = (command = "holdfwd") || (command = "holdback")
x = 0.95*ifelse((command = "holdfwd"),const(velocity.walk.fwd.x),const(velocity.walk.back.x))

[State 1, VS]
type = velset
;trigger1 = screenpos y > 60
trigger1 = (pos y)+const(size.mid.pos.y) > -const240p(240)
trigger1 = (command = "holdup")
trigger2 = (command = "holddown")
y = 0.95*ifelse((command = "holddown"),const(velocity.walk.fwd.x),const(velocity.walk.back.x))

[State 1, VS]
type = velset
;trigger1 = screenpos y <= 60
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
trigger1 = command != "holddown"
y = 0;const(movement.stand.friction)*2

[State 1, VS]
type = veladd
;trigger1 = screenpos y <= 60
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
;trigger1 = command != "holddown"
trigger1 = command != "holdup"
trigger2 = (pos y)+const(size.mid.pos.y) < -const240p(242)
y = 0.5

[State 1, VS]
type = velmul
trigger1 = (command != "holdfwd") && (command != "holdback")
trigger1 = abs(vel x) > 0
x = 0.95;const(movement.stand.friction)

[State 1, VS]
type = velmul
trigger1 = (command != "holdup") && (command != "holddown")
trigger1 = abs(vel y) > 0
y = 0.95;const(movement.stand.friction)

[State 0, 2]
type = VelSet
trigger1 = abs(vel y) < Const(movement.stand.friction.threshold)
y = 0

[State 0, 3]
type = VelSet
trigger1 = abs(vel x) < Const(movement.stand.friction.threshold)
x = 0

[State 0, 4] ;Are you dead?
type = ChangeState
trigger1 = !alive
value = 5050
ctrl = 0

[State 1, CS] ;Fall
type = changestate
trigger1 = time
trigger1 = var(24)<=0 || command="c" || (command="x"&&command="y"&&command="z")
value = 50
ctrl = 1

[State 1, CS] ;Back to Ground
type = changestate
trigger1 = vel y > 0
trigger1 = pos y >= 0
trigger2 = roundstate > 2
value = 0
ctrl = 1

;---------------------------------------------------------------------------
; Fly Dash forward
[Statedef 310]
type    = A
physics = N
anim = 100
ctrl = 0
velset = 0,0
sprpriority = 1

[State 100, 2] ;Prevent run from canceling into walk
type = AssertSpecial
trigger1 = 1
flag = NoWalk
flag2 = NoAutoTurn
ignorehitpause = 1

;[State 0, Width]
;type = Width
;trigger1 = animelemtime(2)>=0
;edge = 13,0
;ignorehitpause = 1

[State 100, PSND]
type = playsnd
trigger1 = animelemtime(3)=0
value = 100,0
persistent = 0

[State 100, Physics]
type = statetypeset
trigger1 = animelemtime(3)=0
physics = N
ignorehitpause = 1

[State 100, Velset]
type = velset
trigger1 = animelemtime(3)=0
x = const(velocity.run.fwd.x)

[State 100, Velset]
type = posadd
trigger1 = animelemtime(3)=[0,1]
x = floor(1.5*const(velocity.run.fwd.x))

[State 100, Friction]
type = velmul
trigger1 = animelemtime(3)>0
trigger1 = vel x > const(velocity.run.fwd.x)
x = 0.92

[State 100, Friction]
type = velset
trigger1 = animelemtime(3)>0
trigger1 = vel x < const(velocity.run.fwd.x)
x = const(velocity.run.fwd.x)

;--
; Flight
;----

[State 1, VS]
type = velset
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) > -const240p(240)
trigger1 = (command = "holdup")
trigger2 = (command = "holddown")
y = 0.95*ifelse((command = "holddown"),const(velocity.walk.fwd.x),const(velocity.walk.back.x))

[State 1, VS]
type = velset
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
trigger1 = command != "holddown"
y = 0;const(movement.stand.friction)*2

[State 1, VS]
type = veladd
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
;trigger1 = command != "holddown"
trigger1 = command != "holdup"
trigger2 = (pos y)+const(size.mid.pos.y) < -const240p(242)
y = 0.5

[State 1, VS]
type = velmul
triggerall = animelemtime(3)>0
trigger1 = (command != "holdup") && (command != "holddown")
trigger1 = abs(vel y) > 0
y = 0.95;const(movement.stand.friction)

[State 0, 2]
type = VelSet
triggerall = animelemtime(3)>0
trigger1 = abs(vel y) < Const(movement.stand.friction.threshold)
y = 0

;----
; End Flight
;----

[State 311, PS]
type = posset
trigger1 = pos y > 0
y = 0

[State 311, VelS]
type = velset
trigger1 = pos y >= 0
trigger1 = vel y > 0
y = 0

[State 310, Grounded State]
type = statetypeset
trigger1 = pos y >= 0
trigger1 = statetype = A
statetype = S
physics = N

[State 310, Aerial State]
type = statetypeset
trigger1 = pos y < 0
trigger1 = statetype != A
statetype = A
physics = N

[State 100, Explod]
type = Explod
trigger1 = animelemtime(3)=0
anim = 10100
ID = 1010
pos = -4,-37
postype = p1  ;p2,front,back,left,right
facing = 1
removetime = -2
scale = 1,1
sprpriority = 5
ownpal = 1
removeongethit = 1
pausemovetime = 99999999999
supermovetime = 99999999999

[State 100, Effect]
type = helper
trigger1 = animelemtime(3)=0
helpertype = normal
name = "Dash_FX"
ID = 10100
pos = 38,-42
postype = p1
stateno = 10100
ownpal = 1
facing = 1
pausemovetime = 99999999999

[State 100, Explod]
type = Explod
trigger1 = animelemtime(3)>=0
trigger1 = time%3=0
trigger1 = random%2=0
anim = 10102
ID = 10102
pos = 20-floor(81.0*random/1000.0),5-floor(66.0*random/1000.0)
postype = p1  ;p2,front,back,left,right
facing = 1
removetime = -2
scale = 1,1
sprpriority = 5
ownpal = 1
removeongethit = 1
pausemovetime = 99999999999
supermovetime = 99999999999

[State 100, 4]
type = ChangeState
triggerall = time >= 16
trigger1 = command != "holdfwd"
trigger2 = time >= 60
value = 311

;---------------------------------------------------------------------------
; Run forward
[Statedef 311]
type    = U;A
physics = N
anim = 101
sprpriority = 1

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = N

[State 311, VM]
type = velmul
trigger1 = 1
x = 0.85
y = 0.85

[State 311, PS]
type = posset
trigger1 = pos y > 0
y = 0

[State 311, VelS]
type = velset
trigger1 = pos y >= 0
trigger1 = vel y > 0
y = 0

[State 100, 2] ;Prevent run from canceling into walk
type = AssertSpecial
trigger1 = 1
flag = NoWalk
flag2 = NoAutoTurn
ignorehitpause = 1

[State 100, 4]
type = ChangeState
trigger1 = animtime = 0
value = cond(statetype=A,301,0)
ctrl = 1

;---------------------------------------------------------------------------
; Fly Dash backwards
[Statedef 315]
type    = A
physics = N
ctrl = 0
anim = 105
velset = 0,0
sprpriority = 1

[State 100, 2] ;Prevent run from canceling into walk
type = AssertSpecial
trigger1 = 1
flag = NoWalk
flag2 = NoAutoTurn
ignorehitpause = 1

[State 105, SCA]
type = nothitby
trigger1 = time = 0
value = SCA,AA,AP,AT
time = 8
ignorehitpause = 1

[State 105, STS]
type = statetypeset
trigger1 = animelemtime(3)=0
statetype = A
physics = N
ignorehitpause = 1

[State 100, PSND]
type = playsnd
trigger1 = animelemtime(3)=0
value = 100,0
persistent = 0

[State 105, 1]
type = VelSet
trigger1 = animelemTime(3) = 0
x = -const(velocity.run.fwd.x)

;--
; Flight
;----

[State 1, VS]
type = velset
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) > -const240p(240)
trigger1 = (command = "holdup")
trigger2 = (command = "holddown")
y = 0.95*ifelse((command = "holddown"),const(velocity.walk.fwd.x),const(velocity.walk.back.x))

[State 1, VS]
type = velset
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
trigger1 = command != "holddown"
y = 0;const(movement.stand.friction)*2

[State 1, VS]
type = veladd
triggerall = animelemtime(3)>0
trigger1 = (pos y)+const(size.mid.pos.y) <= -const240p(240)
;trigger1 = (command = "holdup") || (command = "holddown")
;trigger1 = command != "holddown"
trigger1 = command != "holdup"
trigger2 = (pos y)+const(size.mid.pos.y) < -const240p(242)
y = 0.5

[State 1, VS]
type = velmul
triggerall = animelemtime(3)>0
trigger1 = (command != "holdup") && (command != "holddown")
trigger1 = abs(vel y) > 0
y = 0.95;const(movement.stand.friction)

[State 0, 2]
type = VelSet
triggerall = animelemtime(3)>0
trigger1 = abs(vel y) < Const(movement.stand.friction.threshold)
y = 0

;----
; End Flight
;----

[State 311, PS]
type = posset
trigger1 = pos y > 0
y = 0

[State 311, VelS]
type = velset
trigger1 = pos y >= 0
trigger1 = vel y > 0
y = 0

[State 310, Grounded State]
type = statetypeset
trigger1 = pos y >= 0
trigger1 = statetype = A
statetype = S
physics = N

[State 310, Aerial State]
type = statetypeset
trigger1 = pos y < 0
trigger1 = statetype != A
statetype = A
physics = N

[State 100, Explod]
type = Explod
trigger1 = animelemtime(3)=0
anim = 10100
ID = 1010
pos = 10,-45
postype = p1  ;p2,front,back,left,right
facing = -1
removetime = -2
scale = 1,1
sprpriority = 5
ownpal = 1
;removeongethit = 1
pausemovetime = 99999999999
supermovetime = 99999999999

[State 100, Effect]
type = helper
trigger1 = animelemtime(3)=0
helpertype = normal
name = "Dash_FX"
ID = 10100
pos = -5,-42
postype = p1
stateno = 10100
ownpal = 1
facing = -1
pausemovetime = 99999999999

[State 100, Explod]
type = Explod
trigger1 = animelemtime(3)>=0
trigger1 = time%3=0
trigger1 = random%2=0
anim = 10102
ID = 10102
pos = 20-floor(81.0*random/1000.0),5-floor(66.0*random/1000.0)
postype = p1  ;p2,front,back,left,right
facing = 1
removetime = -2
scale = 1,1
sprpriority = 5
ownpal = 1
removeongethit = 1
pausemovetime = 99999999999
supermovetime = 99999999999

[State 100, 4]
type = ChangeState
triggerall = time >= 16
trigger1 = command != "holdback"
trigger2 = time >= 60
value = 316

;---------------------------------------------------------------------------
; Run forward
[Statedef 316]
type    = U;A
physics = N
anim = 316
sprpriority = 1

[State 1000, STS]
type = statetypeset
trigger1 = time = 0
trigger1 = statetype != A
statetype = S
physics = N;S

[State 311, VM]
type = velmul
trigger1 = 1
x = 0.85
y = 0.85

[State 311, PS]
type = posset
trigger1 = pos y > 0
y = 0

[State 311, VelS]
type = velset
trigger1 = pos y >= 0
trigger1 = vel y > 0
y = 0

[State 100, 2] ;Prevent run from canceling into walk
type = AssertSpecial
trigger1 = 1
flag = NoWalk
flag2 = NoAutoTurn
ignorehitpause = 1

[State 100, 4]
type = ChangeState
trigger1 = animtime = 0
value = cond(statetype=A,301,0)
ctrl = 1
