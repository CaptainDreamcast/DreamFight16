; The CMD file.
;
; Two parts: 1. Command definition and  2. State entry
; (state entry is after the commands def section)
;
; 1. Command definition
; ---------------------
; Note: The commands are CASE-SENSITIVE, and so are the command names.
; The eight directions are:
;   B, DB, D, DF, F, UF, U, UB     (all CAPS)
;   corresponding to back, down-back, down, downforward, etc.
; The six buttons are:
;   a, b, c, x, y, z               (all lower case)
;   In default key config, abc are are the bottom, and xyz are on the
;   top row. For 2 button characters, we recommend you use a and b.
;   For 6 button characters, use abc for kicks and xyz for punches.
;
; Each [Command] section defines a command that you can use for
; state entry, as well as in the CNS file.
; The command section should look like:
;
;   [Command]
;   name = some_name
;   command = the_command
;   time = time (optional -- defaults to 15 if omitted)
;
; - some_name
;   A name to give that command. You'll use this name to refer to
;   that command in the state entry, as well as the CNS. It is case-
;   sensitive (QCB_a is NOT the same as Qcb_a or QCB_A).
;
; - command
;   list of buttons or directions, separated by commas.
;   Directions and buttons can be preceded by special characters:
;   slash (/) - means the key must be held down
;          egs. command = /D       ;hold the down direction
;               command = /DB, a   ;hold down-back while you press a
;   tilde (~) - to detect key releases
;          egs. command = ~a       ;release the a button
;               command = ~D, F, a ;release down, press fwd, then a
;          If you want to detect "charge moves", you can specify
;          the time the key must be held down for (in game-ticks)
;          egs. command = ~30a     ;hold a for at least 30 ticks, then release
;   dollar ($) - Direction-only: detect as 4-way
;          egs. command = $D       ;will detect if D, DB or DF is held
;               command = $B       ;will detect if B, DB or UB is held
;   plus (+) - Buttons only: simultaneous press
;          egs. command = a+b      ;press a and b at the same time
;               command = x+y+z    ;press x, y and z at the same time
;   You can combine them:
;     eg. command = ~30$D, a+b     ;hold D, DB or DF for 30 ticks, release,
;                                  ;then press a and b together
;   It's recommended that for most "motion" commads, eg. quarter-circle-fwd,
;   you start off with a "release direction". This matches the way most
;   popular fighting games implement their command detection.
;
; - time (optional)
;   Time allowed to do the command, given in game-ticks. Defaults to 15
;   if omitted
;
; If you have two or more commands with the same name, all of them will
; work. You can use it to allow multiple motions for the same move.
;
; Some common commands examples are given below.
;
; [Command] ;Quarter circle forward + x
; name = "QCF_x"
; command = ~D, DF, F, x
;
; [Command] ;Half circle back + a
; name = "HCB_a"
; command = ~F, DF, D, DB, B, a
;
; [Command] ;Two quarter circles forward + y
; name = "2QCF_y"
; command = ~D, DF, F, D, DF, F, y
;
; [Command] ;Tap b rapidly
; name = "5b"
; command = b, b, b, b, b
; time = 30
;
; [Command] ;Charge back, then forward + z
; name = "charge_B_F_z"
; command = ~60$B, F, z
; time = 10
; 
; [Command] ;Charge down, then up + c
; name = "charge_D_U_c"
; command = ~60$D, U, c
; time = 10
; 

;-| Button Remapping |-----------------------------------------------------
; This section lets you remap the player's buttons (to easily change the
; button configuration). The format is:
;   old_button = new_button
; If new_button is left blank, the button cannot be pressed.
[Remap]
x = x
y = y
z = z
a = a
b = b
c = c
s = s

;-| Default Values |-------------------------------------------------------
[Defaults]
; Default value for the "time" parameter of a Command. Minimum 1.
command.time = 30

; Default value for the "buffer.time" parameter of a Command. Minimum 1,
; maximum 30.
command.buffer.time = 1

;-| Super Motions |--------------------------------------------------------
[Command]
name = "Super_Sonic"
command = ~D, DF, F, x+y
time = 25

[Command]
name = "Super_Sonic"
command = ~D, DF, F, y+z
time = 25

[Command]
name = "Super_Sonic"
command = ~D, DF, F, z+x
time = 25

[Command]
name = "Dark_Super_Sonic"
command = ~D, DB, B, a+b
time = 25

[Command]
name = "Dark_Super_Sonic"
command = ~D, DB, B, b+c
time = 25

[Command]
name = "Dark_Super_Sonic"
command = ~D, DB, B, c+a
time = 25

;-| Special Motions |------------------------------------------------------
[Command]
name = "Spin_Dash_Up_Light"     ;Required (do not remove)
command = ~F, D, DF, x
time = 25

[Command]
name = "Spin_Dash_Up"     ;Required (do not remove)
command = ~F, D, DF, y
time = 25

[Command]
name = "Spin_Dash_Up_Heavy"     ;Required (do not remove)
command = ~F, D, DF, z
time = 25

[Command]
name = "Spin_Dash_Light"     ;Required (do not remove)
command = ~D, DB, B, x
time = 25

[Command]
name = "Spin_Dash_Medium"     ;Required (do not remove)
command = ~D, DB, B, y
time = 25

[Command]
name = "Spin_Dash_Heavy"     ;Required (do not remove)
command = ~D, DB, B, z
time = 25

[Command]
name = "Homing_Attack_Light"     ;Required (do not remove)
command = ~D, DF, F, a
time = 25

[Command]
name = "Homing_Attack_Medium"     ;Required (do not remove)
command = ~D, DF, F, b
time = 25

[Command]
name = "Homing_Attack_Heavy"     ;Required (do not remove)
command = ~D, DF, F, c
time = 25

[Command]
name = "Ring_Toss_Light"     ;Required (do not remove)
command = ~D, DF, F, x
time = 25

[Command]
name = "Ring_Toss_Medium"     ;Required (do not remove)
command = ~D, DF, F, y
time = 25

[Command]
name = "Ring_Toss_Heavy"     ;Required (do not remove)
command = ~D, DF, F, z
time = 25


[Command] ;Tap a rapidly
 name = "Foot_Storm_A"
 command = ~a, a, a
 time = 25
 
 [Command] ;Tap b rapidly
 name = "Foot_Storm_B"
 command = ~b, b, b
 time = 25
 
 [Command] ;Tap c rapidly
 name = "Foot_Storm_C"
 command = ~c, c, c
 time = 25

 [Command]
name = "Spring_Jump"     ;Required (do not remove)
command = ~D, U
time = 10

 [Command]
name = "Shadow"     ;Required (do not remove)
command = x+a
time = 10

;-| Double Tap |-----------------------------------------------------------
[Command]
name = "FF"     ;Required (do not remove)
command = F, F
time = 10

[Command]
name = "BB"     ;Required (do not remove)
command = B, B
time = 10

;-| 2/3 Button Combination |-----------------------------------------------
[Command]
name = "recovery" ;Required (do not remove)
command = x+y
time = 1

[Command]
name = "recovery"
command = y+z
time = 1

[Command]
name = "recovery"
command = x+z
time = 1

[Command]
name = "recovery"
command = a+b
time = 1

[Command]
name = "recovery"
command = b+c
time = 1

[Command]
name = "recovery"
command = a+c
time = 1

;-| Dir + Button |---------------------------------------------------------
[Command]
name = "back_x"
command = /$B,x
time = 1

[Command]
name = "back_y"
command = /$B,y
time = 1

[Command]
name = "back_z"
command = /$B,z
time = 1

[Command]
name = "down_x"
command = /$D,x
time = 1

[Command]
name = "down_y"
command = /$D,y
time = 1

[Command]
name = "down_z"
command = /$D,z
time = 1

[Command]
name = "fwd_x"
command = /$F,x
time = 1

[Command]
name = "fwd_y"
command = /$F,y
time = 1

[Command]
name = "fwd_z"
command = /$F,z
time = 1

[Command]
name = "up_x"
command = /$U,x
time = 1

[Command]
name = "up_y"
command = /$U,y
time = 1

[Command]
name = "up_z"
command = /$U,z
time = 1

[Command]
name = "back_a"
command = /$B,a
time = 1

[Command]
name = "back_b"
command = /$B,b
time = 1

[Command]
name = "back_c"
command = /$B,c
time = 1

[Command]
name = "down_a"
command = /$D,a
time = 1

[Command]
name = "down_b"
command = /$D,b
time = 1

[Command]
name = "down_c"
command = /$D,c
time = 1

[Command]
name = "fwd_a"
command = /$F,a
time = 1

[Command]
name = "fwd_b"
command = /$F,b
time = 1

[Command]
name = "fwd_c"
command = /$F,c
time = 1

[Command]
name = "up_a"
command = /$U,a
time = 1

[Command]
name = "up_b"
command = /$U,b
time = 1

[Command]
name = "up_c"
command = /$U,c
time = 1

;-| Single Button |---------------------------------------------------------
[Command]
name = "a"
command = a
time = 1

[Command]
name = "b"
command = b
time = 1

[Command]
name = "c"
command = c
time = 1

[Command]
name = "x"
command = x
time = 1

[Command]
name = "y"
command = y
time = 1

[Command]
name = "z"
command = z
time = 1

[Command]
name = "s"
command = s
time = 1

;-| Single Dir |------------------------------------------------------------
[Command]
name = "fwd" ;Required (do not remove)
command = $F
time = 1

[Command]
name = "downfwd"
command = DF
time = 1

[Command]
name = "down" ;Required (do not remove)
command = $D
time = 1

[Command]
name = "downback"
command = DB
time = 1

[Command]
name = "back" ;Required (do not remove)
command = $B
time = 1

[Command]
name = "upback"
command = UB
time = 1

[Command]
name = "up" ;Required (do not remove)
command = $U
time = 1

[Command]
name = "upfwd"
command = UF
time = 1

;-| Hold Button |--------------------------------------------------------------
[Command]
name = "hold_x"
command = /x
time = 1

[Command]
name = "hold_y"
command = /y
time = 1

[Command]
name = "hold_z"
command = /z
time = 1

[Command]
name = "hold_a"
command = /a
time = 1

[Command]
name = "hold_b"
command = /b
time = 1

[Command]
name = "hold_c"
command = /c
time = 1

[Command]
name = "hold_s"
command = /s
time = 1

;-| Hold Dir |--------------------------------------------------------------
[Command]
name = "holdfwd" ;Required (do not remove)
command = /$F
time = 1

[Command]
name = "holddownfwd"
command = /DF
time = 1

[Command]
name = "holddown" ;Required (do not remove)
command = /$D
time = 1

[Command]
name = "holddownback"
command = /DB
time = 1

[Command]
name = "holdback" ;Required (do not remove)
command = /$B
time = 1

[Command]
name = "holdupback"
command = /UB
time = 1

[Command]
name = "holdup" ;Required (do not remove)
command = /$U
time = 1

[Command]
name = "holdupfwd"
command = /UF
time = 1

;---------------------------------------------------------------------------
; 2. State entry
; --------------
; This is where you define what commands bring you to what states.
;
; Each state entry block looks like:
;   [State -1, Label]           ;Change Label to any name you want to use to
;                               ;identify the state with.
;   type = ChangeState          ;Don't change this
;   value = new_state_number
;   trigger1 = command = command_name
;   . . .  (any additional triggers)
;
; - new_state_number is the number of the state to change to
; - command_name is the name of the command (from the section above)
; - Useful triggers to know:
;   - statetype
;       S, C or A : current state-type of player (stand, crouch, air)
;   - ctrl
;       0 or 1 : 1 if player has control. Unless "interrupting" another
;                move, you'll want ctrl = 1
;   - stateno
;       number of state player is in - useful for "move interrupts"
;   - movecontact
;       0 or 1 : 1 if player's last attack touched the opponent
;                useful for "move interrupts"
;
; Note: The order of state entry is important.
;   State entry with a certain command must come before another state
;   entry with a command that is the subset of the first.  
;   For example, command "fwd_a" must be listed before "a", and
;   "fwd_ab" should come before both of the others.
;
; For reference on triggers, see CNS documentation.
;
; Just for your information (skip if you're not interested):
; This part is an extension of the CNS. "State -1" is a special state
; that is executed once every game-tick, regardless of what other state
; you are in.


; Don't remove the following line. It's required by the CMD standard.
[Statedef -1]

;===========================================================================
;---------------------------------------------------------------------------
;===========================================================================
;---------------------------------------------------------------------------
;Assists
;---------------------------------------------------------------------------
;Call Shadow
;カンフー突き手（弱）
[State -1, Shadow]
type = ChangeState
value = 8100
triggerall = command = "Shadow"
triggerall = NumHelper(6000) = 0
trigger1 = statetype = S
trigger1 = ctrl
;===========================================================================
;---------------------------------------------------------------------------
; Run Fwd
[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Run Back
[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = MoveContact
trigger2 = anim = 1500

;---------------------------------------------------------------------------
;Spring Jump
[State -1, Spring Jump]
type = ChangeState
value = 1050
triggerall = numexplod(1059) = 0
trigger1 = command = "Spring_Jump"
trigger1 = Pos Y = 0
trigger1 = ctrl
trigger2 =stateno = 450
trigger2 = movehit
trigger2 = command = "holdup"
trigger3 = MoveContact
trigger3 = anim = 1500
;===========================================================================
;-------------------------------------------------------------
; Dark Super Sonic
[State -1, Dark Super Sonic]
type = ChangeState
value = 7000
triggerall = command = "Dark_Super_Sonic"
triggerall = power = 3000
triggerall = StateType != A
triggerall = MoveType != H
trigger1 = Ctrl
;===========================================================================
;-------------------------------------------------------------
; Super Sonic
[State -1, Super Sonic]
type = ChangeState
value = 2000
triggerall = command = "Super_Sonic"
triggerall = power >= 1000
triggerall = StateType != A
triggerall = MoveType != H
trigger1 = ctrl = 1

;---------------------------------------------------------------------------
; Spin Dash Up Light
[State -1, Spin Dash Up Light]
type = ChangeState
value = 1550
triggerall = command = "Spin_Dash_Up_Light"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Spin Dash Up Light
[State -1, Spin Dash Up Light]
type = ChangeState
value = 1550
triggerall = command = "Spin_Dash_Up_Light"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Spin Dash Up
[State -1, Spin Dash Up ]
type = ChangeState
value = 1552
triggerall = command = "Spin_Dash_Up"
trigger1 = statetype = S
trigger1 = ctrl
;---------------------------------------------------------------------------
; Spin Dash Up Heavy
[State -1, Spin Dash Up Heavy]
type = ChangeState
value = 1554
triggerall = command = "Spin_Dash_Up_Heavy"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Spin Dash Light
[State -1, Spin Dash Light]
type = ChangeState
value = 1500
triggerall = command = "Spin_Dash_Light"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Spin Dash Medium
[State -1, Spin Dash Medium]
type = ChangeState
value = 1510
triggerall = command = "Spin_Dash_Medium"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Spin Dash Heavy
[State -1, Spin Dash Heavy]
type = ChangeState
value = 1520
triggerall = command = "Spin_Dash_Heavy"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Homing Attack Light
[State -1, Homing Attack Light]
type = ChangeState
value = 1300
triggerall = command = "Homing_Attack_Light"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Homing Attack Medium
[State -1, Homing Attack Medium]
type = ChangeState
value = 1310
triggerall = command = "Homing_Attack_Medium"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Homing Attack Heavy
[State -1, Homing Attack Heavy]
type = ChangeState
value = 1320
triggerall = command = "Homing_Attack_Heavy"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Ring Toss Light
[State -1, Ring Toss Light]
type = ChangeState
value = 1100
triggerall = command = "Ring_Toss_Light"
triggerall = NumProjID(1100) = 0
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Ring Toss Medium
[State -1, Ring Toss Medium]
type = ChangeState
value = 1111
triggerall = command = "Ring_Toss_Medium"
triggerall = NumProjID(1100) = 0
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Ring Toss Light
[State -1, Ring Toss Heavy]
type = ChangeState
value = 1121
triggerall = command = "Ring_Toss_Heavy"
triggerall = NumProjID(1100) = 0
trigger1 = statetype = S
trigger1 = ctrl


;---------------------------------------------------------------------------
; Foot Storm Light
[State -1, Foot Storm Light]
type = ChangeState
value = 900
triggerall = command = "Foot_Storm_A"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 210
trigger2 = moveContact
trigger3 = anim = 230
trigger3 = moveContact

;---------------------------------------------------------------------------
; Foot Storm Medium
[State -1, Foot Storm Medium]
type = ChangeState
value = 901
triggerall = command = "Foot_Storm_B"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 210
trigger2 = moveContact
trigger3 = anim = 230
trigger3 = moveContact

;---------------------------------------------------------------------------
; Foot Storm Heavy
[State -1, Foot Storm Heavy]
type = ChangeState
value = 902
triggerall = command = "Foot_Storm_C"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 210
trigger2 = moveContact
trigger3 = anim = 230
trigger3 = moveContact
;---------------------------------------------------------------------------
; Throw
;[State -1, Throw]
;type = ChangeState
;value = 800
;triggerall = command = "y" || command = "z"
;triggerall = statetype = S
;triggerall = ctrl
;triggerall = stateno != 100
;trigger1 = command = "holdfwd"
;trigger1 = p2bodydist X < 10
;trigger1 = (p2statetype = S) || (p2statetype = C)
;trigger1 = p2movetype != H
;trigger2 = command = "holdback"
;trigger2 = p2bodydist X < 10
;trigger2 = (p2statetype = S) || (p2statetype = C)
;trigger2 = p2movetype != H

;---------------------------------------------------------------------------
; Taunt
[State -1, Taunt]
type = ChangeState
value = 195
triggerall = command = "s"
trigger1 = statetype != A
trigger1 = ctrl

;---------------------------------------------------------------------------
; Stand Light Punch
[State -1, Stand Light Punch]
type = ChangeState
value = 200
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl

;---------------------------------------------------------------------------
; Stand Medium Punch
[State -1, Stand Medium Punch]
type = ChangeState
value = 210
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 200
trigger2 = movehit

;---------------------------------------------------------------------------
; Stand Strong Punch
[State -1, Stand Strong Punch]
type = ChangeState
value = 220
triggerall = command = "z"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 210
trigger2 = moveContact
trigger3 = anim = 200
trigger3 = moveContact


;---------------------------------------------------------------------------
; Stand Light Kick
[State -1, Stand Light Kick]
type = ChangeState
value = 230
triggerall = command = "a"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 210
trigger2 = moveContact

;---------------------------------------------------------------------------
; Standing Medium Kick
[State -1, Standing Medium Kick]
type = ChangeState
value = 240
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 230
trigger2 = MoveContact

;---------------------------------------------------------------------------
; Standing Strong Kick
[State -1, Standing Strong Kick]
type = ChangeState
value = 250
triggerall = command = "c"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = anim = 240
trigger2 = moveContact
trigger3 = anim = 230
trigger3 = moveContact

;---------------------------------------------------------------------------
; Crouching Light Punch
[State -1, Crouching Light Punch]
type = ChangeState
value = 400
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl

;---------------------------------------------------------------------------
; Crouching Medium Punch
[State -1, Crouching Medium Punch]
type = ChangeState
value = 410
triggerall = command = "y"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = anim = 400
trigger2 = movehit

;---------------------------------------------------------------------------
; Crouching Strong Punch
[State -1, Crouching Strong Punch]
type = ChangeState
value = 420
triggerall = command = "z"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = anim = 410
trigger2 = movehit

;---------------------------------------------------------------------------
; Crouching Light Kick
[State -1, Crouching Light Kick]
type = ChangeState
value = 430
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = anim = 410
trigger2 = moveContact

;---------------------------------------------------------------------------
; Crouching Medium Kick
[State -1, Crouching Medium Kick]
type = ChangeState
value = 440
triggerall = command = "b"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = anim = 430
trigger2  = moveContact

;---------------------------------------------------------------------------
; Crouching Strong Kick
[State -1, Crouching Strong Kick]
type = ChangeState
value = 450
triggerall = command = "c"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = anim = 440
trigger2  = moveContact

;---------------------------------------------------------------------------
; Jump Light Punch
[State -1, Jump Light Punch]
type = ChangeState
value = 600
triggerall = command = "x"
trigger1 = statetype = A
trigger1 = ctrl

;---------------------------------------------------------------------------
; Jump Medium Punch
[State -1, Jump Medium Punch]
type = ChangeState
value = 610
triggerall = command = "y"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = anim = 600
trigger2 = moveContact
trigger3 = anim = 630
trigger3 = moveContact

;---------------------------------------------------------------------------
; Jump Strong Punch
[State -1, Jump Strong Punch]
type = ChangeState
value = 620
triggerall = command = "z"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = anim = 610
trigger2 = moveContact
;---------------------------------------------------------------------------
; Jump Light Kick
[State -1, Jump Light Kick]
type = ChangeState
value = 630
triggerall = command = "a"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = anim = 610
trigger2 = moveContact

;---------------------------------------------------------------------------
; Jump Medium Kick
[State -1, Jump Medium Kick]
type = ChangeState
value = 640
triggerall = command = "b"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = anim = 630
trigger2 = movehit

;---------------------------------------------------------------------------
; Jump Strong Kick
[State -1, Jump Strong Kick]
type = ChangeState
value = 650
triggerall = command = "c"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = anim = 640
trigger2  = moveContact
trigger3 = anim = 620
trigger3 = movehit
trigger3 = time > 20

;---------------------------------------------------------------------------
;sonic flip
;[state -1]
;type = changestate
;value = 1000
;triggerall = command = "up" && stateno != 100 && stateno != 105
;trigger1 = statetype = A
;trigger1 = ctrl = 1

;---------------------------------------------------------------------------
