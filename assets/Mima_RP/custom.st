;---------------------------------------------------------------------------
; Custom Hitstates :
;---------------------------------------------------------------------------

;------------------------------------------
; Wallslam - Flying
[Statedef 260]
type    = A
movetype = H
physics = N
sprpriority = 0
velset = 0,0

[State 250, T]
Type = Turn
Trigger1 = p2dist x < 0

[state 250, CA]
type = changeanim
trigger1 = time = 0
value = 5030

[state 250, CA]
type = ChangeAnim
trigger1 = !hitshakeover
trigger1 = time
value = anim
elem = animelemno(0)

[State 250, CA]
Type = ChangeAnim
Trigger1 = AnimTime = 0
Trigger1 = Anim = 5030
Value = 5050

[state 4250, VelS]
type = velset
trigger1 = hitshakeover
x = gethitvar(xvel)*facing;*hitshakeover
y = gethitvar(yvel);*hitshakeover
persistent = 0

[State 250, VA]
type = veladd
trigger1 = hitshakeover
y = gethitvar(yaccel)

[State 4250, SB]
type = screenbound
trigger1 = 1
value = 0
movecamera = 1,0
ignorehitpause = 0

[State 250, CS]
type = changestate
trigger1 = frontedgebodydist <= 0
trigger2 = backedgebodydist <= 0
value = 261

[State 250, CS]
type = selfstate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 5100

;---------------------------------------------------------------------------
; Wallslam
[Statedef 261]
type    = A
movetype = H
physics = N
sprpriority = 1

[State 251, PA]
type = null;posadd
trigger1 = time = 0
Y = -Const(Size.Mid.Pos.Y)

[State 251, Shake]
type = envshake
trigger1 = time = 0
time = 15
freq = 100
phase = 100
ampl = 8

[State 251, CA]
Type = ChangeAnim
Trigger1 = Time < 3
Trigger1 = SelfAnimExist(5035)
Value = 5035

[State 251, PF]
Type = PosFreeze
Trigger1 = Time < 4

[State 1451, LA]
type = null;lifeadd
trigger1 = time = 0
value = -5

[state 4250, CA2]
type = changeanim
trigger1 = time = 3
trigger1 = selfanimexist(5061)
value = 5061

[state 4250, CA2]
type = changeanim2
trigger1 = time = 3
trigger1 = !selfanimexist(5061)
value = 5061



[State 251]
Type = Turn
trigger1 = p2dist x < 0
Trigger1 = Time = 4

[State 1112]
type = VelSet
trigger1 = Time = 4
x = 0.25
y = -8.0;-6.0

[State 251, VA]
type = VelAdd
trigger1 = time > 4
y = .35

[State 251, AD]
Type = null;AngleDraw
Trigger1 = Time > 3
trigger1 = Pos Y < 0
Value = Time*30

[State 251, OS]
Type = null;OffSet
Trigger1 = anim = 250
trigger1 = pos y < 0
Y = Const(Size.Mid.Pos.Y)

[State 243, Recover] ; Recover near ground (use ChangeState)
type = SelfState
triggerall = time >= 10
triggerall = Vel Y > 0
triggerall = Pos Y >= -20
triggerall = alive
triggerall = CanRecover
trigger1 = Command = "recovery"
value = 5200 ;HITFALL_RECOVER

[State 243, Recover] ; Recover in mid air (use SelfState)
type = SelfState
triggerall = time >= 10
triggerall = Vel Y > 0
triggerall = alive
triggerall = CanRecover
trigger1 = Command = "recovery"
value = 5210 ;HITFALL_AIRRECOVER

[State -2, PS]
type = posset
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
y = 0

[State 4250, CS]
type = changestate;selfstate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 262;5100

;---------------------------------------------------------------------------
; Wallslam
[Statedef 262]
type    = L
movetype = H
physics = N
velset = 0,0

[State -2, PS]
type = posset
trigger1 = 1
y = 0

[State 4250, CS]
type = selfstate
trigger1 = 1
value = 5110

;---------------------------------------------------------------------------
; Groundslam
[Statedef 450]
type     = A
movetype = H
physics  = N
velset = 0,0

[State 820, CA]
type = changeanim
trigger1 = time = 0
persistent = 0
value = 5030

[State 820, CA]
type = changeanim
trigger1 = hitshakeover
trigger1 = selfanimexist(5060)
persistent = 0
value = 5060

[State 820, CA]
type = changeanim2
trigger1 = hitshakeover
trigger1 = !selfanimexist(5060)
persistent = 0
value = 450

[State 250, T]
Type = Turn
Trigger1 = p2dist x < 0

[state 4250, VelS]
type = velset
trigger1 = hitshakeover
x = gethitvar(xvel)*facing;*hitshakeover
y = gethitvar(yvel);*hitshakeover
persistent = 0

[State 250, VA]
type = veladd
trigger1 = hitshakeover
y = gethitvar(yaccel)

[State 250, CS]
type = changestate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 451

;------------------------------------------
; Groundslam
[Statedef 451]
type    = A
movetype= H
physics = N
velset = 0,0

[State 251, PS]
type = posset
trigger1 = time = 0
y = 0;const(size.mid.pos.y)

[State 251, Shake]
type = envshake
trigger1 = time = 0
time = 15
freq = 100
phase = 100
ampl = 8

[State 251, CA]
Type = ChangeAnim
Trigger1 = Time < 3
Trigger1 = SelfAnimExist(5035)
Value = 5035

[State 251, PF]
Type = PosFreeze
Trigger1 = Time < 4

[State 1451, LA]
type = null;lifeadd
trigger1 = time = 0
value = -5

[state 4250, CA2]
type = changeanim2
trigger1 = time = 3
value = 250

[State 1112]
type = VelSet
trigger1 = Time = 0
x = -1.0
y = -6.0

[State 251, VA]
type = VelAdd
trigger1 = time > 4
y = .2

[State 251, AD]
Type = AngleDraw
Trigger1 = Time > 3
trigger1 = Pos Y < 0
Value = Time*30

[State 251, OS]
Type = OffSet
Trigger1 = anim = 250
trigger1 = pos y < 0
Y = Const(Size.Mid.Pos.Y)

[State 250, CS]
type = selfstate
triggerall = time >= 10
triggerall = Vel Y > 0
triggerall = alive
triggerall = CanRecover
trigger1 = Command = "recovery"
value = 5040

[State 851, SS]
type = selfstate
trigger1 = vel y > 0
trigger1 = pos y >= 0
value = 5100

;---------------------------------------------------------------------------
; CUSTOM TRIP (shaking) (Modified)
[Statedef 470]
type    = A
movetype= H
physics = N
velset = 0,0
     
[State 5070, 1]
type = ChangeAnim
trigger1 = 1
value = 5070
     
[State 5070, 2]
type = ChangeState
trigger1 = HitShakeOver
value = 471
     
;---------------------------------------------------------------------------
; CUSTOM TRIP2 (fall through air) (Modded)
[Statedef 471]
type    = A
movetype= H
physics = N
     
[State 5071, 1]
type = HitVelSet
trigger1 = Time = 0
x = 1
y = 1
     
[State 5071, 2] ;Acceleration
type = VelAdd
trigger1 = 1
y = 0.7;GetHitVar(yaccel)
     
[State 5071, 3] ;Hit ground
trigger1 = Vel Y > 0
trigger1 = Pos Y >= 15
type = changestate;SelfState
value = 6110;5110

;---------------------------------------------------------------------------
; Grabbed - 1
[Statedef 850]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 1

[State 0, ScreenBound]
type = ScreenBound
trigger1 = 1
value = 1
movecamera = 1,0
;ignorehitpause = 
;persistent = 

[State 4150, CA2]
type = changeanim2
trigger1 = time = 0
value = 850


;---------------------------------------------------------------------------
; Grabbed - 1
[Statedef 851]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 1

[State 4150, CA2]
type = changeanim2
trigger1 = time = 0
value = 851

;-------------------------------------------
; Grabbed
[Statedef 853]
type    = A
movetype = H
physics = N
sprpriority = 1

;[State 850, SB]
;type = screenbound
;trigger1 = 1
;value = 0
;ignorehitpause = 1

[state 4250, CA2]
type = changeanim2
trigger1 = time = 0
value = 853

[State 810, Bind]
type = velset
trigger1 = animelemtime(1)=0
x = -4.0
y = -9.5

[State 807, Accel]
type = veladd;gravity
trigger1 = animelemtime(1)>0
y = 0.5

[State 807, hitground]
type = changestate;selfstate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 854;5100


;---------------------------------------------------------------------------
; Downed get-hit (hit ground from fall)
[Statedef 854]
type    = L
movetype= H
physics = N

[State 5100, 2004.04.14 behavior]
type = null;VelSet
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) = 0
trigger1 = vel x < -1
x = -1

[State 5100, EnvShake]
type = FallEnvShake
trigger1 = Time = 0

[State 5100, Var] ;Save fall velocity
type = VarSet
trigger1 = Time = 0
sysvar(1) = floor(vel y)

[State 5100, 2] ;Hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
trigger1 = (anim != [5051,5059]) && (anim != [5061,5069])
trigger2 = !SelfAnimExist(5100 + (anim % 10))
value = 5100

[State 5100, 3] ;Hit ground anim (for hit up)
type = ChangeAnim
trigger1 = time = 0
trigger1 = (anim = [5051,5059]) || (anim = [5061,5069])
trigger1 = SelfAnimExist(5100 + (anim % 10))
value = 5100 + (anim % 10)

[State 5100, 4]
type = PosSet
trigger1 = Time = 0
y = 0

[State 5100, 5]
type = VelSet
trigger1 = Time = 0
y = 0

[State 5100, 6] ;Reduce speed
type = VelMul
trigger1 = Time = 0
x = 0.75

[State 5100, 7]
type = ChangeState
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) = 0
value = 855;5110 ;Downed get-hit (lying down)

[State 5100, 8]
type = Explod
trigger1 = Time = 1
anim = F(60 + (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56)))
pos = 0, 0
sprpriority = ifelse (sysvar(1) <= Const720p(56), -10, 10)

[State 5100, 9]
type = HitFallDamage
trigger1 = Time = 3

[State 5100, 10]
type = PlaySnd
trigger1 = Time = 1
value = F7, (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56))

[State 5100, 11]
type = PosFreeze
trigger1 = 1

[State 5100, 12]
type = ChangeState
trigger1 = AnimTime = 0
value = 855;5101 ;Downed get-hit (bounce off ground)

[State 5100, 13]
type = ForceFeedback
trigger1 = Time = 0
waveform = sinesquare
ampl = 128,-3,-.2,.005
time = 20



;---------------------------------------------------------------------------
; Downed get-hit (bounce off ground)
[Statedef 855]
type    = L
movetype= H
physics = N

[State 5101, 1] ;Coming hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
trigger1 = anim != [5101,5109]
trigger2 = !SelfAnimExist(5160 + (anim % 10))
value = 5160

[State 5101, 2] ;Coming hit ground anim (for hit up)
type = ChangeAnim
triggerall = time = 0
trigger1 = anim = [5101,5109]
trigger1 = SelfAnimExist(5160 + (anim % 10))
value = 5160 + (anim % 10)

[State 5101, 3]
type = HitFallVel
trigger1 = Time = 0

[State 5101, 4]
type = PosSet
trigger1 = Time = 0
y = Const(movement.down.bounce.offset.y)

[State 5101, 5]
type = PosAdd
trigger1 = Time = 0
x = Const(movement.down.bounce.offset.x)

[State 5101, 6] ;Acceleration
type = VelAdd
trigger1 = 1
y = Const(movement.down.bounce.yaccel)

[State 5101, 7] ;Hit ground
trigger1 = Vel Y > 0
trigger1 = Pos Y >= Const(movement.down.bounce.groundlevel)
type = ChangeState
value = 856;5110


;-------------------------------------------
; Hard Knockdown
[Statedef 856]
type    = L
movetype = H
physics = N
velset = 0,0
sprpriority = 1

[State 5110, EnvShake]
type = FallEnvShake
trigger1 = Time = 0

[State 5110, 1] ;For hit up/up-diag type (from state 5081)
type = ChangeAnim
persistent = 0
trigger1 = SelfAnimExist(5110 + (anim % 10))
trigger1 = anim = [5081,5089]
value = 5110 + (anim % 10)

[State 5110, 2] ;Hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
triggerall = anim != [5110,5119] ;Not already changed anim
trigger1 = anim != [5161,5169]
trigger2 = !SelfAnimExist(5170 + (anim % 10))
value = 5170

[State 5110, 3] ;Hit ground anim (for hit up)
type = ChangeAnim
triggerall = time = 0
triggerall = anim != [5110,5119] ;Not already changed anim
trigger1 = anim = [5161,5169]
trigger1 = SelfAnimExist(5170 + (anim % 10))
value = 5170 + (anim % 10)

[State 5110, 4]
type = HitFallDamage
trigger1 = Time = 0

[State 5110, 5]
type = PosSet
trigger1 = Time = 0
y = 0

[State 5110, Var] ;Get fall velocity
type = VarSet
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) != 0
sysvar(1) = floor(vel y)

[State 5110, 6]
type = PlaySnd
trigger1 = Time = 0
trigger1 = !SysVar(0)
value = F7, (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56))

[State 5110, 7]
type = Explod
trigger1 = Time = 0
trigger1 = !SysVar(0)
anim = F(60 + (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56)))
pos = 0, 0
sprpriority = ifelse (sysvar(1) <= Const720p(56), -10, 10)

[State 5110, 8]
type = VelSet
trigger1 = Time = 0
y = 0

[State 5110, 9] ;For hit up type
type = ChangeAnim
persistent = 0
triggerall = anim = [5171,5179]
triggerall = SelfAnimExist(5110 + (anim % 10))
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground anim
value = 5110 + (anim % 10)

[State 5110, 10] ;For normal
type = ChangeAnim
persistent = 0
triggerall = Anim != [5111,5119]
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground frame
value = 5110

[State 5110, 11] ;If just died
type = Selfstate;ChangeState
triggerall = !alive
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground frame
trigger3 = Anim = [5110,5119]
value = 5150

[State 5110, 12]
type = VarSet
trigger1 = SysVar(0)
trigger1 = Time = 0
sysvar(0) = 0

[State 5110, 13] ;Friction
type = VelMul
trigger1 = 1
x = 0.85

[State 5110, 14] ;Friction
type = VelSet
trigger1 = abs(Vel x) < Const(movement.down.friction.threshold)
persistent = 0
x = 0

[State 5110, 15]
type = ForceFeedback
trigger1 = alive
trigger1 = Time = 0
time = 8
ampl = 240
waveform = sine

[State 5110, 16]
type = ForceFeedback
trigger1 = !alive
trigger1 = Time = 0
ampl = 200, 7, -.467
time = 30
waveform = sine

[State 852, SS]
type = selfstate
trigger1 = alive
trigger1 = time >= 10
value = 5120

;---------------------------------------------------------------------------
; Air Grabbed - 1
[Statedef 860]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 1

[State 0, ScreenBound]
type = ScreenBound
trigger1 = 1
value = 1
movecamera = 1,0
;ignorehitpause = 
;persistent = 

[State 4150, CA2]
type = changeanim2
trigger1 = time = 0
value = 850


;---------------------------------------------------------------------------
; Grabbed - 1
[Statedef 861]
type    = A
movetype= H
physics = N
velset = 0,0
ctrl = 0
sprpriority = 1

[State 4150, CA2]
type = changeanim2
trigger1 = time = 0
value = 851

;-------------------------------------------
; Grabbed
[Statedef 862]
type    = A
movetype = H
physics = N
sprpriority = 1

;[State 850, SB]
;type = screenbound
;trigger1 = 1
;value = 0
;ignorehitpause = 1

[state 4250, CA2]
type = changeanim2
trigger1 = time = 0
value = 853;62

[State 810, Bind]
type = velset
trigger1 = animelemtime(1)=0
x = -5.0
y = 9.5

[State 807, Accel]
type = veladd;gravity
trigger1 = animelemtime(1)>0
y = 0.5

[State 807, hitground]
type = changestate;selfstate
trigger1 = vel y > 0
trigger1 = pos y >= -(vel y)
value = 451

;----------------------------
; FORCE RECOVERY NEAR GROUND
[Statedef 5205]
type= A
physics= N
movetype= H

[State 5205, End]; Sends character back to his own recovery states
type= selfstate
trigger1= 1
value= ifelse(pos y<-20, 5210, 5200)

;--------------
; FEET LANDING
[Statedef 5085]
type= A
physics= N
movetype= H
velset= 0, 0
facep2= 1

[State 5085, P]
type = null;pause 
trigger1 = time = 0
time = 120

[State 5025, HFS]; Resets the fall parameter, so that the opponent will not fall on his back
type= hitfallset
trigger1= 1
value= 0

[State 5025, End]; Sends opponent back to his own states
type= selfstate
trigger1= 1
value= ifelse(pos y, 5020, 5000)

;---------------------------------------------------------------------------
; Air get-hit (knocked away)
[Statedef 6215]
type    = A
movetype= H
physics = N
ctrl = 0

[State 5030, 1]
type = ChangeAnim
trigger1 = Anim != 5030 && anim!=6215;[5000, 5199]
trigger1 = SelfAnimExist(5030)
value = 5030

[State 5030, 1]
type = ChangeAnim2
trigger1 = Anim != 5030 && anim!=6215;[5000, 5199]
trigger1 = !SelfAnimExist(5030)
value = 6215

[State 5030, 2]
type = VelAdd
trigger1 = 1
y = GetHitVar(yaccel)

[State 5030, 3]
type = HitVelSet
trigger1 = Time = 0
x = 1
y = 1

[State 5030, 6]
type = ChangeState
trigger1 = AnimTime = 0
trigger2 = time>=5
value = 6216 ;Air get-hit (transition)

[State 5035, 3]
type = SelfState
;triggerall = !HitFall
trigger1 = vel y > 0 ;Peak of Launch
value = cond(alive=0,5050,5210)

;---------------------------------------------------------------------------
; Launcher State
[Statedef 6216]
type    = A
movetype= H
physics = N

[State 5035, 1]
type = ChangeAnim
trigger1 = Time = 0
trigger1 = SelfAnimExist(5035)
trigger1 = Anim != [5051,5059] ;Not if in hit up anim
trigger1 = Anim != 5090 ;Not if hit off ground anim
trigger1 = anim != 6215
value = 5035

[State 5035, 2]
type = VelAdd
trigger1 = 1
y = GetHitVar(yaccel)

[State 5035, 3]
type = SelfState
;triggerall = !HitFall
trigger1 = vel y >= 0 ;Peak of Launch
value = cond(alive=0,5050,5210);value = 5210 ;Air get-hit (recovering in air, not falling)

;---------------------------------------------------------------------------
; Air get-hit (shaking)
[Statedef 6020]
type    = A
movetype= H
physics = N
velset = 0,0

[State 5020, 1] ;Anim for HIT_LIGHT to HIT_HARD
type = ChangeAnim
trigger1 = Time = 0
trigger1 = GetHitVar(animtype) != [3,5]
value = ifelse((GetHitVar(airtype) = 1),5000,5010) + GetHitVar(animtype)

[State 5020, 2] ;Anim for HIT_BACK
type = ChangeAnim
trigger1 = Time = 0
trigger1 = GetHitVar(animtype) = [3,5]
value = 5030

[State 5020, 3] ;Anim for HIT_UP/HIT_DIAGUP (only if it exists)
type = ChangeAnim
trigger1 = Time = 0
trigger1 = (GetHitVar(animtype) = [4,5]) && (SelfAnimExist(5047 + GetHitVar(animtype)))
value = 5047 + GetHitVar(animtype) ;5051 - 4 + type

[State 5020, 4] ;Freeze anim
type = ChangeAnim
trigger1 = Time > 0
value = anim

[State 5020, 5]
type = ChangeState
trigger1 = HitShakeOver
value = 6030

[State 5020, FFB Light]
type = ForceFeedback
trigger1 = anim = 5000
trigger2 = anim = 5010
persistent = 0
time = 6
waveform = square

[State 5020, FFB Medium]
type = ForceFeedback
trigger1 = anim = 5001
trigger2 = anim = 5011
persistent = 0
time = 8
waveform = sinesquare
ampl = 110,-1,-.3

[State 5020, FFB Hard]
type = ForceFeedback
trigger1 = anim = 5012
trigger2 = anim = 5002
trigger3 = anim = 5030
trigger4 = anim = [5051,5059]
persistent = 0
time = 15
waveform = sinesquare
ampl = 140

;---------------------------------------------------------------------------
; Air get-hit (knocked away)
[Statedef 6030]
type    = A
movetype= H
physics = N
ctrl = 0

[State 5030, 1]
type = ChangeAnim
trigger1 = Anim != [5000, 5199]
trigger1 = SelfAnimExist(5030)
value = 5030

[State 5030, 2]
type = VelAdd
trigger1 = 1
y = GetHitVar(yaccel)

[State 5030, 3]
type = HitVelSet
trigger1 = Time = 0
x = 1
y = 1

[State 5030, 4]
type = SelfState
triggerall = !HitFall
trigger1 = HitOver
trigger2 = Vel Y > 0    ;Hit ground
trigger2 = Pos Y >= Const(movement.air.gethit.groundlevel)
value = 5040 ;HITA_RECOV

[State 5030, 5]
type = ChangeState
triggerall = HitFall
trigger1 = HitOver
trigger2 = Vel Y > 0    ;Hit ground
trigger2 = Pos Y >= Const(movement.air.gethit.groundlevel)
value = 6050 ;HITA_FALL

[State 5030, 6]
type = ChangeState
trigger1 = AnimTime = 0
value = 6035 ;Air get-hit (transition)

;---------------------------------------------------------------------------
; Air get-hit (transition)
[Statedef 6035]
type    = A
movetype= H
physics = N

[State 5035, 1]
type = ChangeAnim
trigger1 = Time = 0
trigger1 = SelfAnimExist(5035)
trigger1 = Anim != [5051,5059] ;Not if in hit up anim
trigger1 = Anim != 5090 ;Not if hit off ground anim
value = 5035

[State 5035, 2]
type = VelAdd
trigger1 = 1
y = GetHitVar(yaccel)

[State 5035, 3]
type = SelfState
triggerall = !HitFall
trigger1 = HitOver  ;Hit is over
trigger2 = AnimTime = 0 ;Animation over
trigger3 = Vel Y > 0    ;Hit ground
trigger3 = Pos Y >= Const(movement.air.gethit.groundlevel)
trigger4 = Time = 0     ;No transition anim
trigger4 = Anim != 5035 ;.
value = 5040 ;Air get-hit (recovering in air, not falling)

[State 5035, 4]
type = ChangeState
triggerall = HitFall
trigger1 = HitOver  ;Hit is over
trigger2 = AnimTime = 0 ;Anim is over
trigger3 = Vel Y > 0    ;Hit ground
trigger3 = Pos Y >= Const(movement.air.gethit.groundlevel)
trigger4 = Time = 0     ;No transition anim
trigger4 = Anim != 5035 ;.
value = 6050 ;Air get-hit (falling)



;---------------------------------------------------------------------------
; Air get-hit (falling)
[Statedef 6050]
type    = A
movetype= H
physics = N

[State 5050, 1] ;Change anim when done with transition
type = ChangeAnim
trigger1 = AnimTime = 0
trigger1 = Anim = 5035
trigger2 = Time = 0     ;If no transition anim
trigger2 = Anim != 5035
trigger2 = (Anim != [5051, 5059]) && (Anim != [5061, 5069])
trigger2 = Anim != 5090 ;Not if hit off ground anim
value = 5050

[State 5050, 2] ;Coming down anim
type = ChangeAnim
trigger1 = anim = [5050,5059]
trigger1 = Vel Y >= ifelse(anim = 5050, Const720p(4), Const720p(-8))
trigger1 = SelfAnimExist(anim+10)
value = anim+10
persistent = 0

[State 5050, 3] ;Gravity
type = VelAdd
trigger1 = 1
y = GetHitVar(yaccel)

[State 5050, 4] ;Recover near ground
type = SelfState
triggerall = Vel Y > 0
triggerall = Pos Y >= Const(movement.air.gethit.groundrecover.ground.threshold)
triggerall = alive
triggerall = CanRecover
trigger1 = Command = "recovery"
value = 5200 ;Air get-hit (fall recovery on ground)

[State 5050, 5]; Recover in mid air
type = SelfState
triggerall = Vel Y > Const(movement.air.gethit.airrecover.threshold)
triggerall = alive
triggerall = CanRecover
trigger1 = Command = "recovery"
value = 5210 ;Air get-hit (fall recovery in air)

[State 5050, 6]
type = ChangeState
trigger1 = Vel Y > 0
trigger1 = Pos Y >= ifelse((anim = [5051,5059]) || (anim = [5061,5069]), 0, Const(movement.air.gethit.groundlevel))
value = 6100 ;Downed get-hit (hit ground from fall)




;---------------------------------------------------------------------------
; Downed get-hit (hit ground from fall)
[Statedef 6100]
type    = L
movetype= H
physics = N

[State 5100, 2004.04.14 behavior]
type = null;VelSet
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) = 0
trigger1 = vel x < -1
x = -1

[State 5100, EnvShake]
type = FallEnvShake
trigger1 = Time = 0

[State 5100, Var] ;Save fall velocity
type = VarSet
trigger1 = Time = 0
sysvar(1) = floor(vel y)

[State 5100, 2] ;Hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
trigger1 = (anim != [5051,5059]) && (anim != [5061,5069])
trigger2 = !SelfAnimExist(5100 + (anim % 10))
value = 5100

[State 5100, 3] ;Hit ground anim (for hit up)
type = ChangeAnim
trigger1 = time = 0
trigger1 = (anim = [5051,5059]) || (anim = [5061,5069])
trigger1 = SelfAnimExist(5100 + (anim % 10))
value = 5100 + (anim % 10)

[State 5100, 4]
type = PosSet
trigger1 = Time = 0
y = 0

[State 5100, 5]
type = VelSet
trigger1 = Time = 0
y = 0

[State 5100, 6] ;Reduce speed
type = VelMul
trigger1 = Time = 0
x = 0.75

[State 5100, 7]
type = ChangeState
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) = 0
value = 6110 ;Downed get-hit (lying down)

[State 5100, 8]
type = Explod
trigger1 = Time = 1
anim = F(60 + (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56)))
pos = 0, 0
sprpriority = ifelse (sysvar(1) <= Const720p(56), -10, 10)

[State 5100, 9]
type = HitFallDamage
trigger1 = Time = 3

[State 5100, 10]
type = PlaySnd
trigger1 = Time = 1
value = F7, (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56))

[State 5100, 11]
type = PosFreeze
trigger1 = 1

[State 5100, 12]
type = ChangeState
trigger1 = AnimTime = 0
value = 6101 ;Downed get-hit (bounce off ground)

[State 5100, 13]
type = ForceFeedback
trigger1 = Time = 0
waveform = sinesquare
ampl = 128,-3,-.2,.005
time = 20

;---------------------------------------------------------------------------
; Downed get-hit (bounce off ground)
[Statedef 6101]
type    = L
movetype= H
physics = N

[State 5101, 1] ;Coming hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
trigger1 = anim != [5101,5109]
trigger2 = !SelfAnimExist(5160 + (anim % 10))
value = 5160

[State 5101, 2] ;Coming hit ground anim (for hit up)
type = ChangeAnim
triggerall = time = 0
trigger1 = anim = [5101,5109]
trigger1 = SelfAnimExist(5160 + (anim % 10))
value = 5160 + (anim % 10)

[State 5101, 3]
type = HitFallVel
trigger1 = Time = 0

[State 5101, 4]
type = PosSet
trigger1 = Time = 0
y = Const(movement.down.bounce.offset.y)

[State 5101, 5]
type = PosAdd
trigger1 = Time = 0
x = Const(movement.down.bounce.offset.x)

[State 5101, 6] ;Acceleration
type = VelAdd
trigger1 = 1
y = Const(movement.down.bounce.yaccel)

[State 5101, 7] ;Hit ground
trigger1 = Vel Y > 0
trigger1 = Pos Y >= Const(movement.down.bounce.groundlevel)
type = ChangeState
value = 6110

;---------------------------------------------------------------------------
; Downed get-hit (lying down)
[Statedef 6110]
type    = L
movetype= H
physics = N

[State 5110, EnvShake]
type = FallEnvShake
trigger1 = Time = 0

[State 5110, 1] ;For hit up/up-diag type (from state 5081)
type = ChangeAnim
persistent = 0
trigger1 = SelfAnimExist(5110 + (anim % 10))
trigger1 = anim = [5081,5089]
value = 5110 + (anim % 10)

[State 5110, 2] ;Hit ground anim (normal)
type = ChangeAnim
triggerall = time = 0
triggerall = anim != [5110,5119] ;Not already changed anim
trigger1 = anim != [5161,5169]
trigger2 = !SelfAnimExist(5170 + (anim % 10))
value = 5170

[State 5110, 3] ;Hit ground anim (for hit up)
type = ChangeAnim
triggerall = time = 0
triggerall = anim != [5110,5119] ;Not already changed anim
trigger1 = anim = [5161,5169]
trigger1 = SelfAnimExist(5170 + (anim % 10))
value = 5170 + (anim % 10)

[State 5110, 4]
type = HitFallDamage
trigger1 = Time = 0

[State 5110, 5]
type = PosSet
trigger1 = Time = 0
y = 0

[State 5110, Var] ;Get fall velocity
type = VarSet
trigger1 = Time = 0
trigger1 = GetHitVar(fall.yvel) != 0
sysvar(1) = floor(vel y)

[State 5110, 6]
type = PlaySnd
trigger1 = Time = 0
trigger1 = !SysVar(0)
value = F7, (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56))

[State 5110, 7]
type = Explod
trigger1 = Time = 0
trigger1 = !SysVar(0)
anim = F(60 + (sysvar(1) > Const720p(20)) + (sysvar(1) > Const720p(56)))
pos = 0, 0
sprpriority = ifelse (sysvar(1) <= Const720p(56), -10, 10)

[State 5110, 8]
type = VelSet
trigger1 = Time = 0
y = 0

[State 5110, 9] ;For hit up type
type = ChangeAnim
persistent = 0
triggerall = anim = [5171,5179]
triggerall = SelfAnimExist(5110 + (anim % 10))
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground anim
value = 5110 + (anim % 10)

[State 5110, 10] ;For normal
type = ChangeAnim
persistent = 0
triggerall = Anim != [5111,5119]
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground frame
value = 5110

[State 5110, 11] ;If just died
type = SelfState
triggerall = !alive
trigger1 = AnimTime = 0
trigger2 = SysVar(0) ;SysVar(0) = 1 avoids hit ground frame
trigger3 = Anim = [5110,5119]
value = 5150

[State 5110, 12]
type = VarSet
trigger1 = SysVar(0)
trigger1 = Time = 0
sysvar(0) = 0

[State 5110, 13] ;Friction
type = VelMul
trigger1 = 1
x = 0.85

[State 5110, 14] ;Friction
type = VelSet
trigger1 = abs(Vel x) < Const(movement.down.friction.threshold)
persistent = 0
x = 0

[State 5110, 15]
type = ForceFeedback
trigger1 = alive
trigger1 = Time = 0
time = 8
ampl = 240
waveform = sine

[State 5110, 16]
type = ForceFeedback
trigger1 = !alive
trigger1 = Time = 0
ampl = 200, 7, -.467
time = 30
waveform = sine

[State 6110, SS]
type = selfstate
trigger1 = alive
trigger1 = time >= 10
value = 5120
