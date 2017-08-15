;AI
;--------------------------------------------------------

[Command]
name = "CPU0"
command = U,U,D,D,B,F,B,F,s,s
time = 0

[Command]
name = "CPU1"
command = U,U,D,D,B,F,B,F,a,a
time = 0

[Command]
name = "CPU2"
command = U,U,D,D,B,F,B,F,b,b
time = 0

[Command]
name = "CPU3"
command = U,U,D,D,B,F,B,F,c,c
time = 0

[Command]
name = "CPU4"
command = U,U,D,D,B,F,B,F,x,x
time = 0

[Command]
name = "CPU5"
command = U,U,D,D,B,F,B,F,y,y
time = 0

[Command]
name = "CPU6"
command = U,U,D,D,B,F,B,F,z,z
time = 0

[Command]
name = "CPU7"
command = U,U,D,D,B,F,B,F,a,b
time = 0

[Command]
name = "CPU8"
command = U,U,D,D,B,F,B,F,b,c
time = 0

[Command]
name = "CPU9"
command = U,U,D,D,B,F,B,F,a,c
time = 0

[Command]
name = "CPU10"
command = U,U,D,D,B,F,B,F,x,y
time = 0

[Command]
name = "CPU11"
command = U,U,D,D,B,F,B,F,y,z
time = 0

[Command]
name = "CPU12"
command = U,U,D,D,B,F,B,F,x,z
time = 0

[Command]
name = "CPU13"
command = U,U,D,D,B,F,B,F,a,x
time = 0

[Command]
name = "CPU14"
command = U,U,D,D,B,F,B,F,a,y
time = 0

[Command]
name = "CPU15"
command = U,U,D,D,B,F,B,F,a,z
time = 0

[Command]
name = "CPU16"
command = U,U,D,D,B,F,B,F,b,x
time = 0

[Command]
name = "CPU17"
command = U,U,D,D,B,F,B,F,b,y
time = 0

[Command]
name = "CPU18"
command = U,U,D,D,B,F,B,F,b,z
time = 0

[Command]
name = "CPU19"
command = U,U,D,D,B,F,B,F,c,x
time = 0

[Command]
name = "CPU20"
command = U,U,D,D,B,F,B,F,c,y
time = 0

[Command]
name = "CPU21"
command = U,U,D,D,B,F,B,F,c,z
time = 0

[Command]
name = "CPU22"
command = U,U,D,D,B,F,B,F,x,a
time = 0

[Command]
name = "CPU23"
command = U,U,D,D,B,F,B,F,x,b
time = 0

[Command]
name = "CPU24"
command = U,U,D,D,B,F,B,F,x,c
time = 0

[Command]
name = "CPU25"
command = U,U,D,D,B,F,B,F,y,a
time = 0

[Command]
name = "CPU26"
command = U,U,D,D,B,F,B,F,y,b
time = 0

[Command]
name = "CPU27"
command = U,U,D,D,B,F,B,F,y,c
time = 0

[Command]
name = "CPU28"
command = U,U,D,D,B,F,B,F,z,a
time = 0

[Command]
name = "CPU29"
command = U,U,D,D,B,F,B,F,z,b
time = 0

[Command]
name = "CPU30"
command = U,U,D,D,B,F,B,F,z,c
time = 0

[Command]
name = "CPU31"
command = U,U,D,D,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU32"
command = U,U,D,D,B,B,B,B,s,s,s
time = 0

[Command]
name = "CPU33"
command = U,U,D,D,B,F,B,B,s,s,s

[Command]
name = "CPU34"
command = U,U,D,D,B,B,B,F,s,s
time = 0

[Command]
name = "CPU35"
command = U,U,U,U,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU36"
command = U,U,D,U,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU37"
command = D,D,D,D,D,D,B,F,s,s,s
time = 0

[Command]
name = "CPU38"
command = D,D,D,D,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU39"
command = D,U,D,D,B,F,B,F,s,s
time = 0

[Command]
name = "CPU40"
command = B,B,B,B,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU41"
command = U,U,D,B,B,F,B,F,s,s
time = 0

[Command]
name = "CPU42"
command = D,U,D,D,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU43"
command = D,D,D,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU44"
command = F,D,D,B,F,B,F,s,s,s
time = 0

[Command]
name = "CPU45"
command = F,U,D,D,B,F,B,F,s,s,s
time = 0

;---Super Moves-----------------------

[Command]
name = "anglekick"
command = ~D, F, D, F, c
time = 30

[Command]
name = "kombo"
command = ~D, F, D, F, b
time = 30

[Command]
name = "fireblast"
command = ~50a
time = 30


;---Special Moves--------------------

[Command]
name = "fireball"
command = ~F, F, x

[Command]
name = "lowfireball"
command = ~F, F, a

[Command]
name = "flyingkick"
command = ~F, F, y

[Command]
name = "bike"
command = ~50b

[Command]
name = "cartwheel"
command = ~90y

;-| Double Tap |-----------------------------------------------------------
[Command]
name = "FF";Required (do not remove)
command = F, F
time = 10

[Command]
name = "BB";Required (do not remove)
command = B, B
time = 10

;-| 2/3 Button Combination |-----------------------------------------------
[Command]
name = "recovery";Required (do not remove)
command = a+b
time = 1

[Command]
name = "ab"
command = a+b
time = 1

;-| Dir + Button |---------------------------------------------------------
[Command]
name = "fwd_a"
command = /F,a
time = 1

[Command]
name = "fwd_b"
command = /F,b
time = 1

[Command]
name = "bck_x"
command = /B,x
time = 1

[Command]
name = "fwd_x"
command = /F,x
time = 1

[Command]
name = "downfwd_a"
command = /DF,a
time = 1

[Command]
name = "downfwd_b"
command = /DF,b
time = 1

[Command]
name = "back_a"
command = /B,a
time = 1

[Command]
name = "back_b"
command = /B,b
time = 1

[Command]
name = "back_y"
command = /B,y
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
name = "fwd_ab"
command = /F, a+b
time = 1

[Command]
name = "back_ab"
command = /B, a+b
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
name = "Holding_x"
command = /$x

[Command]
name = "y"
command = y
time = 1

[Command]
name = "z"
command = z
time = 1

[Command]
name = "HoldingStart"
command = /$s ;	Start!
time = 1


;-| Hold Dir |--------------------------------------------------------------
[Command]
name = "holdfwd";Required (do not remove)
command = /$F
time = 1

[Command]
name = "holdback";Required (do not remove)
command = /$B
time = 1

[Command]
name = "holdup";Required (do not remove)
command = /$U
time = 1

[Command]
name = "holdback1";Required (do not remove)
command = /$B
time = 1

[Command]
name = "holdback2";Required (do not remove)
command = c
time = 1

[Command]
name = "holddown";Required (do not remove)
command = /$D
time = 1

;--------------------------------------
[Statedef -1]

[State -1]
type = VarSet
triggerall = statetype != A
triggerall = 1
trigger1 = command = "CPU0"
trigger2 = command = "CPU1"
trigger3 = command = "CPU2"
trigger4 = command = "CPU3"
trigger5 = command = "CPU4"
trigger6 = command = "CPU5"
trigger7 = command = "CPU6"
trigger8 = command = "CPU7"
trigger9 = command = "CPU8"
trigger10 = command = "CPU9"
trigger11 = command = "CPU10"
trigger12 = command = "CPU11"
trigger13 = command = "CPU12"
trigger14 = command = "CPU13"
trigger15 = command = "CPU14"
trigger16 = command = "CPU15"
trigger17 = command = "CPU16"
trigger18 = command = "CPU17"
trigger19 = command = "CPU18"
trigger20 = command = "CPU19"
trigger21 = command = "CPU20"
trigger22 = command = "CPU21"
trigger23 = command = "CPU22"
trigger24 = command = "CPU23"
trigger25 = command = "CPU24"
trigger26 = command = "CPU25"
trigger27 = command = "CPU26"
trigger28 = command = "CPU27"
trigger29 = command = "CPU28"
trigger30 = command = "CPU29"
trigger31 = command = "CPU30"
trigger32 = command = "CPU31"
trigger33 = command = "CPU32"
trigger34 = command = "CPU33"
trigger35 = command = "CPU34"
trigger36 = command = "CPU35"
trigger37 = command = "CPU36"
trigger38 = command = "CPU37"
trigger39 = command = "CPU38"
trigger40 = command = "CPU39"
trigger41 = command = "CPU40"
trigger42 = command = "CPU41"
trigger43 = command = "CPU42"
trigger44 = command = "CPU43"
trigger45 = command = "CPU44"
trigger46 = command = "CPU45"
v = 50
value = 1

[State -1]
type = VarSet
triggerall = statetype = A
triggerall = 1
trigger1 = command = "CPU0"
trigger2 = command = "CPU1"
trigger3 = command = "CPU2"
trigger4 = command = "CPU3"
trigger5 = command = "CPU4"
trigger6 = command = "CPU5"
trigger7 = command = "CPU6"
trigger8 = command = "CPU7"
trigger9 = command = "CPU8"
trigger10 = command = "CPU9"
trigger11 = command = "CPU10"
trigger12 = command = "CPU11"
trigger13 = command = "CPU12"
trigger14 = command = "CPU13"
trigger15 = command = "CPU14"
trigger16 = command = "CPU15"
trigger17 = command = "CPU16"
trigger18 = command = "CPU17"
trigger19 = command = "CPU18"
trigger20 = command = "CPU19"
trigger21 = command = "CPU20"
trigger22 = command = "CPU21"
trigger23 = command = "CPU22"
trigger24 = command = "CPU23"
trigger25 = command = "CPU24"
trigger26 = command = "CPU25"
trigger27 = command = "CPU26"
trigger28 = command = "CPU27"
trigger29 = command = "CPU28"
trigger30 = command = "CPU29"
trigger31 = command = "CPU30"
trigger32 = command = "CPU31"
trigger33 = command = "CPU32"
trigger34 = command = "CPU33"
trigger35 = command = "CPU34"
trigger36 = command = "CPU35"
trigger37 = command = "CPU36"
trigger38 = command = "CPU37"
trigger39 = command = "CPU38"
trigger40 = command = "CPU39"
trigger41 = command = "CPU40"
trigger42 = command = "CPU41"
trigger43 = command = "CPU42"
trigger44 = command = "CPU43"
trigger45 = command = "CPU44"
trigger46 = command = "CPU45"
v = 50
value = 1

;--------------------------------

[State -1, AI Never Stand Still]
type = null
triggerall = Var(50) = 1
triggerall = ctrl = 1
triggerall = p2stateno != 9010
trigger1 = 1
value = 0

[State -1, AI No Jump on Fatality]
type = ChangeState
value = 400
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = S
triggerall = ctrl = 1
triggerall = p2life < 10
trigger1 = p2stateno = 9010

[State -1, Dragon]
type = Changestate
value = 3601
triggerall = command = "y"
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 80

[State -1, AI Dragon]
type = ChangeState
value = 3601
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 80

[State -1, Dragon Fire]
type = Changestate
value = 2355601
triggerall = command = "a"
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 115

[State -1, AI Dragon Fire]
type = ChangeState
value = 2355601
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 115

[State -1, Statue]
type = Changestate
value = 8571
triggerall = command = "c"
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 100

[State -1, AI Statue]
type = ChangeState
value = 8571
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 100


[State -1, Kombo Blast]
type = Changestate
value = 21135
triggerall = command = "x"
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 20

[State -1, AI Kombo Blast]
type = ChangeState
value = 21135
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 20

[State -1, Brutality]
type = Changestate
value = 22236
triggerall = command = "b"
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 20

[State -1, AI Brutality]
type = ChangeState
value = 22236
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 20

[State -1, ThePits]
type = Changestate
value = 9400
triggerall = P2Stateno = 9010
triggerall = P2Life <= 10
trigger1 = ctrl = 1
trigger1 = statetype != A
trigger1 = p2bodydist X > 0
triggerall = command = "z"

[State -1, AI ThePits]
type = ChangeState
value = 9400
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 900 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2stateno = 9010
triggerall = p2life < 10
trigger1 = p2bodydist X > 0

[State -1, Angle Kick]
type = Changestate
value = 4162
triggerall = command = "anglekick"
triggerall = power >= 2000
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, AI Angle Kick]
type = ChangeState
value = 4162
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = S
triggerall = p2statetype = S
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = power >= 2000
triggerall = p2stateno != 9010
trigger1 = p2bodydist X < 30
trigger2 = stateno = 2515
trigger2 = movehit = 1
trigger3 = p2bodydist X < 30

[State -1, Fireball]
type = Changestate
value = 8850
triggerall = command = "fireball"
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, AI Fireball]
type = ChangeState
value = 88500
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype = S
triggerall = p2statetype = S
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X = 50
trigger2 = p2bodydist X = 120
trigger3 = p2bodydist X = 142
trigger4 = p2bodydist X = 200
trigger5 = p2bodydist X = 240
trigger6 = stateno = 251514
trigger6 = movehit = 1
trigger7 = stateno = 4172
trigger7 = movehit = 1
trigger7 = time > 5

[State -1, AirFireball]
type = Changestate
value = 88500
triggerall = command = "fireball"
trigger1 = statetype = A
trigger1 = ctrl = 1

[State -1, AI Air Fireball]
type = ChangeState
value = 88500
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype = A
triggerall = p2statetype = A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X < 100
trigger2 = stateno = 600
trigger2 = movehit = 1
trigger3 = stateno = 610
trigger3 = movehit = 1
trigger4 = stateno = 620
trigger4 = movehit = 1

[State -1, Low Fireball]
type = Changestate
value = 32101
triggerall = command = "lowfireball"
trigger1 = ctrl = 1
trigger1 = statetype != A

[State -1, AI Low Fireball]
type = ChangeState
value = 32101
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype != A
triggerall = p2statetype != A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = power >= 1000
triggerall = p2stateno != 9010
trigger1 = p2bodydist X = 107
trigger2 = p2bodydist X = 133
trigger3 = p2bodydist X = 200
trigger4 = p2bodydist X = 240
trigger5 = stateno = 2515
trigger5 = movehit = 1

[State -1, Flying Kick]
type = Changestate
value = 131304
triggerall = command = "flyingkick"
trigger1 = statetype != A
trigger1 = ctrl = 1

[State -1, AI Flying Kick]
type = ChangeState
value = 131304
triggerall = Var(50) = 1
;triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = power >= 1000
triggerall = p2stateno != 9010
trigger1 = p2bodydist X = 87
trigger2 = p2bodydist X = 143
trigger3 = p2bodydist X = 165
trigger4 = p2bodydist X = 195
trigger5 = stateno = 610
trigger5 = movehit = 1
trigger6 = stateno = 620
trigger6 = movehit = 1
trigger7 = stateno = 251514
trigger7 = movehit = 1
trigger8 = stateno = 0
trigger8 = time > 20

[State -1, Bicycle Kick]
type = Changestate
value = 2515
triggerall = command = "bike"
triggerall = prevstateno != 2515
trigger1 = statetype != A
trigger1 = ctrl = 1

[State -1, AI Bicycle Kick]
type = ChangeState
value = 2515
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype != A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = prevstateno != 2515
triggerall = p2stateno != 9010
trigger1 = p2bodydist X = 50
trigger2 = p2bodydist X = 100
trigger3 = p2bodydist X = 120
trigger4 = p2bodydist X = 180
trigger5 = stateno = 251514
trigger5 = movehit = 1

[State -1, Carthwheel Kick]
type = Changestate
value = 3073
triggerall = command = "cartwheel"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger1 = p2bodydist X < 50

[State -1, AI Carthwheel Kick]
type = ChangeState
value = 3073
triggerall = Var(50) = 1
triggerall = random >= 280 && random < 299 || random >= 890 && random <= 999
triggerall = statetype = S
triggerall = p2statetype = S
triggerall = p2stateno != 9010
triggerall = ctrl = 1
triggerall = p2life > 3
trigger1 = p2bodydist X < 40

; Kick Kombo
[State -1, Kombo]
type = Changestate
value = 3100
triggerall = command = "kombo"
triggerall = power >= 2000
trigger1 = ctrl = 1
trigger1 = statetype = S

[State -1, AI Kombo]
type = ChangeState
value = 3100
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 910 && random <= 999
triggerall = statetype != A
triggerall = p2statetype != A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = power >= 2000
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 1 && p2bodydist X < 50

[State -1, Fireblast]
type = Changestate
value = 4172
triggerall = command = "fireblast"
triggerall = power >= 2000
trigger1 = statetype != A
trigger1 = ctrl = 1

[State -1, AI Fireblast]
type = ChangeState
value = 4172
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 850 && random <= 999
triggerall = statetype != A
triggerall = p2statetype != A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = power >= 2000
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 60 && p2bodydist X < 70
trigger2 = p2bodydist X > 130 && p2bodydist X < 140

;Throw
[State -1]
type = Changestate
value = 900
triggerall = P2bodydist X < 10
triggerall = P2statetype != A
triggerall = ctrl = 1
triggerall = statetype = S
trigger1 = command = "fwd_a"
trigger2 = command = "back_a"

[State -1, AI Throw]
type = ChangeState
value = 900
triggerall = Var(50) = 1
triggerall = random >= 200 && random < 299 || random >= 850 && random <= 999
triggerall = ctrl = 1
triggerall = statetype = S
triggerall = P2statetype = S
triggerall = p2stateno != 9010
trigger1 = p2bodydist X < 10

; Mini Uppercut
[State -1, CloseUpper]
type = Changestate
value = 230
triggerall = command = "x"
triggerall = P2bodyDist X < 10
triggerall = statetype = S
triggerall = ctrl = 1
trigger1 = command = "x"
trigger1 = P2StateType = S
trigger2 = command = "a"
trigger2 = P2StateType = S

[State -1, AI Mini Uppercut]
type = ChangeState
value = 230
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = S
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = P2bodyDist X < 10
triggerall = p2stateno != 9010
trigger1 = p2stateno != 9010

;Roundhouse
[State -1, RoundHouse]
type = Changestate
value = 240
triggerall = command = "y"
triggerall = command = "holdback"
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, AI Round]
type = ChangeState
value = 240
triggerall = Var(50) = 1
triggerall = random >= 200 && random < 299 || random >= 650 && random <= 999
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = statetype = S
triggerall = p2stateno != 9010
trigger1 = p2BodyDist X < 30

[State -1, AI Roundhouse]
type = ChangeState
value = 240
triggerall = Var(50) = 1
triggerall = random >= 100 && random < 299 || random >= 750 && random <= 999
triggerall = statetype = S
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2BodyDist X < 50
triggerall = p2stateno != 9010
trigger1 = stateno = 1010
trigger1 = movecontact = 1
trigger2 = p2stateno = 5300

;Sweep
[State -1, Sweep]
type = Changestate
value = 245
;triggerall = Var(14) = 0
trigger1 = command = "b"
trigger1 = command = "holdback"
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, Sweep2]
type = ChangeState
value = 245
triggerall = Var(50) = 1
triggerall = random >= 100 && random < 299 || random >= 200 && random <= 999
triggerall = ctrl = 1
triggerall = statetype = S
triggerall = p2statetype = S
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 10 && p2bodydist X < 30

;Uppercut
[State -1, MKUppa]
type = ChangeState
value = 430
triggerall = command = "x"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = movecontact = 1
trigger2 = stateno = 410
trigger2 = time > 7

[State -1, AI Uppercut]
type = ChangeState
value = 430
triggerall = Var(50) = 1
triggerall = p2stateno != 9010
triggerall = random >= 100 && random < 299 || random >= 850 && random <= 999
triggerall = p2statetype = S
triggerall = statetype = S
triggerall = ctrl = 1
triggerall = p2life > 5
trigger1 = p2BodyDist X < 40
trigger2 = stateno = 1010
trigger2 = movecontact = 1
trigger3 = p2stateno = 5300

[State -1, AI Uppercut2]
type = ChangeState
value = 430
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = S
triggerall = p2statetype = A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2BodyDist X < 30

[State -1, AI Uppercut5]
type = ChangeState
value = 430
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = S
triggerall = ctrl = 1
triggerall = p2stateno != 9010
trigger1 = movecontact = 1
trigger1 = stateno = 410
trigger1 = time > 7

;Jump Side kick
[State -1, SideKick]
type = ChangeState
value = 620
triggerall = statetype = A
triggerall = ctrl = 1
triggerall = Vel X != 0 
trigger1 = command = "y"
trigger2 = command = "b"

[State -1, AI Jump Kick]
type = ChangeState
value = 620
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = A
triggerall = ctrl = 1
triggerall = Vel X != 0 
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 10 && p2bodydist X < 230

[State -1, AI Sliding Jump Kick]
type = ChangeState
value = 620
triggerall = Var(50) = 1
triggerall = random >= 200 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = A
triggerall = ctrl = 1
triggerall = p2stateno = 1521
triggerall = Vel X != 0 
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 100 && p2bodydist X < 200

;Jump Kick
[State -1, NKick]
type = ChangeState
value = 600
triggerall = statetype = A
triggerall = ctrl = 1
triggerall = Vel X = 0 
triggerall = p2stateno != 9010
trigger1 = command = "y"
trigger2 = command = "b"

[State -1, AI Jump Up Kick]
type = ChangeState
value = 600
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = A
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = Vel X = 0 
triggerall = p2BodyDist X < 15
triggerall = p2stateno != 9010
trigger1 = p2stateno != 9010

;MoonKick
[State -1, MoonKick]
type = Changestate
value = 235
triggerall = command != "holddown"
triggerall = P2bodyDist X < 15
triggerall = statetype = S
triggerall = ctrl = 1
trigger1 = command = "y"
trigger1 = P2StateType = S
trigger2 = command = "b"
trigger2 = P2StateType = S

[State -1, AI MoonKick]
type = ChangeState
value = 235
triggerall = Var(50) = 1
triggerall = random >= 100 && random < 299 || random >= 650 && random <= 999
triggerall = ctrl = 1
triggerall = P2statetype = S
triggerall = statetype = S
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 2 && p2bodydist X < 10

;HP - Part 1
[State -1, HLeftST]
type = ChangeState
value = 200
triggerall = command = "x"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 201
trigger2 = Time > 7
trigger3 = stateno = 2197

[State -1, AI HP]
type = ChangeState
value = 200
triggerall = Var(50) = 1
triggerall = random >= 100 && random < 299 || random >= 650 && random <= 999
triggerall = ctrl = 1
triggerall = P2statetype = S
triggerall = statetype = S
triggerall = p2stateno != 9010
triggerall = p2life > 5
trigger1 = p2bodydist X > 1 && p2bodydist X < 15

;HP - Part 2
[State -1, HRightCONT]
type = ChangeState
value = 201
triggerall = command = "x"
trigger1 = stateno = 200
trigger1 = Time > 7
trigger2 = stateno = 2197

;LP - Part 1
[State -1, LLeftST]
type = ChangeState
value = 220
triggerall = command = "a"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 200
trigger2 = Time > 10
trigger3 = stateno = 2197

[State -1, AI LP]
type = ChangeState
value = 220
triggerall = Var(50) = 1
triggerall = random >= 100 && random < 299 || random >= 650 && random <= 999
triggerall = ctrl = 1
triggerall = P2statetype = S
triggerall = statetype = S
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2bodydist X > 1 && p2bodydist X < 15

;LP - Part 2
[State -1, LRightCONT]
type = ChangeState
value = 200
triggerall = command = "a"
trigger1 = stateno = 220
trigger1 = Time > 10
trigger2 = stateno = 2197

;High Kick
[State -1]
type = ChangeState
value = 210
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, High Kick]
type = ChangeState
value = 210
triggerall = Var(50) = 1
triggerall = random >= 200 && random < 299 || random >= 850 && random <= 999
triggerall = ctrl = 1
triggerall = statetype = S
triggerall = p2life > 5
triggerall = p2stateno != 9010
trigger1 = p2BodyDist X < 22
trigger1 = ctrl = 1

;Low Mid Kick
[State -1]
type = ChangeState
value = 225
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1

[State -1, AI Low Mid Kick]
type = ChangeState
value = 225
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 750 && random <= 999
triggerall = ctrl = 1
triggerall = P2statetype = S
triggerall = p2stateno != 9010
triggerall = statetype = S
triggerall = p2life > 5
trigger1 = p2bodydist X > 2 && p2bodydist X < 20

;Low Kick
[State -1]
type = ChangeState
value = 400
triggerall = command = "b"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = movecontact = 1
trigger2 = stateno = 420
trigger2 = time > 7

[State -1, AI Low Kick0]
type = ChangeState
value = 400
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = C
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2BodyDist X < 15
triggerall = p2stateno != 9010
trigger1 = p2stateno != 9010
trigger2 = movecontact = 1
trigger2 = stateno = 420
trigger2 = time > 7

;Low High Kick
[State -1]
type = ChangeState
value = 410
triggerall = command = "y"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = movecontact = 1
trigger2 = stateno = 400
trigger2 = time > 7


[State -1, AI Low Kick]
type = ChangeState
value = 410
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = C
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2BodyDist X < 15
triggerall = p2stateno != 9010
trigger1 = p2stateno != 9010
trigger2 = movecontact = 1
trigger2 = stateno = 400
trigger2 = time > 7

;Low Crouch
[State -1]
type = ChangeState
value = 420
triggerall = command = "a"
triggerall = command = "holddown"
trigger1 = statetype = C
trigger1 = ctrl = 1

[State -1, AI Crouch Punch]
type = ChangeState
value = 420
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = C
triggerall = p2stateno != 9010
triggerall = ctrl = 1
triggerall = p2life > 5
triggerall = p2BodyDist X < 15
trigger1 = p2stateno != 9010

;Jump Punch
[State -1, Punch]
type = ChangeState
value = 610
triggerall = statetype = A
triggerall = ctrl = 1
trigger1 = command = "a"
trigger2 = command = "x"

[State -1, AI Jump Punch]
type = ChangeState
value = 610
triggerall = Var(50) = 1
triggerall = random >= 250 && random < 299 || random >= 950 && random <= 999
triggerall = statetype = A
triggerall = p2stateno != 9010
triggerall = ctrl = 1
triggerall = p2life > 5
trigger1 = p2BodyDist X < 15
trigger1 = p2stateno != 9010

[State -1, AI No Jump]
type = null
triggerall = Var(50) = 1
trigger1 = p2stateno = 9010
value = 40