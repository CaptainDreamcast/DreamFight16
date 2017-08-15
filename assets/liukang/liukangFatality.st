;=================================================================
;***********************FATALITIES!*******************************
;=================================================================

;Statue
[StateDef 8571]
Type = S
Movetype = A
Physics = S
juggle = 4
poweradd = 50
ctrl = 0

[State 8571, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 8571, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 8571, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 8571, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 8571, ChangeAnim]
type = ChangeAnim
trigger1 = time = 30
value = 20 

[State 8571, 0]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 8571, ChangeAnim2]
type = ChangeState
trigger1 = p2bodydist X > 95 && p2bodydist X < 100
value = 8572

[Statedef 8572]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 8571
sprpriority = 9

[State 8572, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 8572, Fireball01]
type = playsnd
trigger1 = AnimElem = 2
value = 60,1

[State 8572, Statue]
type = Projectile
trigger1 = time = 90
projanim = 8572
id = 8572
postype = p2
offset = 0*const(size.xscale), -300*const(size.xscale)
velocity = 0, 9
remvelocity = 0, 0
; HitDef part of projectile info
attr = S, NP
damage    = 9999
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,0
sparkno = -1
guard.sparkno = -1
hitsound   = s4,21
guardsound = s0,0
ground.type = High
ground.slidetime = 25
ground.hittime  = 16
ground.velocity = -5
air.velocity = -1.5,-5
air.juggle = 3
p2stateno = 2500

[State 8572, Statue]
type = Explod
trigger1 = ProjContact(8572) = 1
anim = 8572
pos = 130*const(size.xscale), -15*const(size.xscale)
postype = p1
sprpriority = 7
ontop = 1
ownpal = 1

[State 8572, Shake]
type = EnvShake
trigger1 = ProjContact(8572) = 1
freq = 100
time = 25
ampl = -9

[State 8572, Blood1]
type = Explod
trigger1 = ProjContact(8572) = 1
anim = 6041
scale = 2,.8
id = 4242
pos = 120*const(size.xscale), 0*const(size.xscale)
postype = p1
sprpriority = -7
under = 1
ownpal = 0

[State 8572, Blood2]
type = Explod
trigger1 = ProjContact(8572) = 1
anim = 6040
scale = 3,.9
id = 4242
pos = 125*const(size.xscale), 0*const(size.xscale)
postype = p1
sprpriority = -2
under = 1
ownpal = 1

[State 8572, Blood Puddle]
type = Explod
trigger1 = ProjContact(8572) = 1
anim = 8573
id = 4242
pos = 135*const(size.xscale), -30*const(size.xscale)
postype = p1
sprpriority = -7
under = 1
ownpal = 0

[State 8572, Blood Sound]
type = PlaySnd
trigger1 = ProjContact(8572) = 1
value = 4, 24

[State 8572, Blood Sound2]
type = PlaySnd
trigger1 = ProjContact(8572) = 1
value = 1, 5

[State 8572, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 8572, NotFreindly]
type = VarSet
trigger1 = AnimElem = 4 
v = 5
value = 1

[State 8572, 10]
type = ChangeState
trigger1 = AnimTime = 0
ctrl = 1
value = 0

;Dragon
[Statedef 3601]
Type = S
Movetype = A
Physics = S
juggle = 4
poweradd = 50
ctrl = 0

[State 3601, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 3601, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 3601, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 3601, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 3601, ChangeAnim]
type = ChangeAnim
trigger1 = time = 30
value = 20 

[State 3601, 0]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 3601, ChangeAnim2]
type = ChangeState
trigger1 = p2bodydist X > 75 && p2bodydist X < 80
value = 4242

[StateDef 4242]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 999
sprpriority = 5

[State 4242, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 4242, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 4242, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 4242, Super C]
type = PowerAdd
trigger1 = Time = 1
value = -2000

[State 4242, Dragon1]
type = playsnd
trigger1 = time = 0
value = 60,3

[State 4242, Dragon2]
type = playsnd
trigger1 = time = 350
value = 60,7

[State 4242, Roar]
type = Playsnd
trigger1 = time = 20
value = 60, 21

[State 4242, Snap]
type = playsnd
trigger1 = movehit = 1
value = 1, 2

[State 4242, Snap2]
type = playsnd
trigger1 = movehit = 1
value = 31, 40

[State 4242, Dragon]
type = Projectile
trigger1 = time = 0
projanim = 4242
projid = 4242
projremove = 0
postype = p1
offset = 0*const(size.xscale), 0*const(size.xscale)
velocity = 0, 0
remvelocity = 0, 0
attr = S, SP
damage    = 9999
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = MA
pausetime = 0,30
sparkno = -1
guard.sparkno = -1
hitsound   = s31,35
guardsound = s1313,0
ground.type = High
ground.slidetime = 0
ground.hittime  = 16
ground.velocity = 0, 0
air.velocity = 0, 0
envshake.time = 30
envshake.freq = 190
envshake.ampl = -6
p2stateno = 2500
fall = 1

[State 9830, Blood1]
type = Explod
trigger1 = ProjContact(4242) = 1
anim = 6041
scale = 1,1.3
id = 4242
pos = 120*const(size.xscale), -70*const(size.xscale)
postype = p1
sprpriority = 7
ontop = 1
ownpal = 1

[State 9830, Blood2]
type = Explod
trigger1 = ProjContact(4242) = 1
anim = 6040
scale = 1,1.5
id = 4242
pos = 125*const(size.xscale), -63*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 0
ownpal = 1

[State 9830, Blood3]
type = Explod
trigger1 = ProjContact(4242) = 1
anim = 6040
id = 4242
pos = 115*const(size.xscale), -65*const(size.xscale)
postype = p1
sprpriority = 7
ontop = 0
ownpal = 1

[State 3601, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 3601, 9]
type = Changestate
trigger1 = time = 388
value = 0
ctrl = 1


;Fire Dragon
[Statedef 2355601]
Type = S
Movetype = A
Physics = S
juggle = 4
poweradd = 50
ctrl = 0

[State 2355601, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 2355601, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 2355601, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 2355601, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 2355601, ChangeAnim]
type = ChangeAnim
trigger1 = time = 30
value = 20 

[State 2355601, 5]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 2355601, ChangeAnim2]
type = ChangeState
trigger1 = p2bodydist X > 110 && p2bodydist X < 115
value = 2355602

[StateDef 2355602]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 999
sprpriority = 5

[State 2355602, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 2355602, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 2355602, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 2355602, Super C]
type = PowerAdd
trigger1 = Time = 1
value = -2000

[State 2355602, Dragon1]
type = playsnd
trigger1 = time = 0
value = 60,3

[State 2355602, Dragon2]
type = playsnd
trigger1 = time = 300
value = 60,7

[State 2355602, Burning]
type = Playsnd
trigger1 = projcontact = 1
value = 60,17

[State 2355602, Roar]
type = Playsnd
trigger1 = time = 20
value = 60, 21

[State 2355602, Dragon]
type = Projectile
trigger1 = time = 0
projanim = 2355602
projremove = 0
postype = p1
sprpriority = 4
offset = 0*const(size.xscale), 0*const(size.xscale)
velocity = 0, 0
remvelocity = 0, 0
attr = S, SP
damage    = 999
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = MA
pausetime = 0,30
sparkno = -1
guard.sparkno = -1
hitsound   = s31,35
guardsound = s0,0
ground.type = High
ground.slidetime = 0
ground.hittime  = 16
ground.velocity = 0, 0
air.velocity = 0, 0
envshake.time = 40
envshake.freq = 190
envshake.ampl = -6
p2stateno = 2500

[State 2355602, Fire]
type = Explod
trigger1 = time = 130
anim = 2355600
id = 2355600
scale = 2,2
pos = 75*const(size.xscale), -125*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 0

[State 2355602, Fire]
type = Playsnd
trigger1 = time = 135
value = 60, 16

[State 2355602, Bones]
type = Playsnd
trigger1 = time = 300
value = 20, 21

[State 2355602, Skeleton]
type = Explod
trigger1 = projcontact = 1
anim = 2355601
pos = 130*const(size.xscale), 0*const(size.xscale)
postype = p1
sprpriority = -6
under = 1
facing = 1

[State 2355602, Fired]
type = Explod
trigger1 = projcontact = 1
anim = 5116
scale = 1.3, 2
pos = 130*const(size.xscale), 0*const(size.xscale)
postype = p1
sprpriority = 9
ontop = 1
facing = 1

[State 2355602, 8]
type = VarRandom
trigger1 = Time = 5
v = 11
range = 1,3

[State 2355602, Male Screaming]
type = Playsnd
triggerall = Var(11) = 1
triggerall = projcontact = 1
trigger1 = p2name != "kitana"
trigger2 = p2name != "mileena"
trigger3 = p2name != "jade"
trigger4 = p2name != "cammy"
trigger5 = p2name != "chun li"
trigger6 = p2name != "kyla"
trigger7 = p2name != "sakura"
trigger8 = p2name != "juli"
trigger9 = p2name != "sonya"
trigger10 = p2name != "juni"
trigger11 = p2name != "evil chun li"
trigger12 = p2name != "mai"
trigger13 = p2name != "jenet"
trigger14 = p2name != "rose"
trigger15 = p2name != "Athena"
trigger16 = p2name != "Shermie"
trigger17 = p2name != "Karin"
trigger18 = p2name != "Vanessa"
trigger19 = p2name != "Mai"
trigger20 = p2name != "Kasumi"
trigger21 = p2name != "Blue Mary"
trigger22 = p2name != "Hinako"
trigger23 = p2name != "Leona"
trigger24 = p2name != "Chizuru"
trigger25 = p2name != "Storm"
trigger26 = p2name != "Psylocke"
trigger27 = p2name != "Rogue"
trigger28 = p2name != "King"
trigger29 = p2name != "Vice"
trigger30 = p2name != "Mature"
trigger31 = p2name != "Yuri"
trigger32 = p2name != "Mugena"
trigger33 = p2name != "Kula"
trigger34 = p2name != "BBH"
trigger35 = p2name != "Charlote"
trigger36 = p2name != "Nakoruru"
trigger37 = p2name != "rim"
trigger38 = p2name != "lilith"
trigger39 = p2name != "felicia"
trigger40 = p2name != "hsien-ko"
trigger41 = p2name != "q bee"
trigger42 = p2name != "r. mika"
trigger43 = p2name != "Janne"
trigger44 = p2name != "Aska"
trigger45 = p2name != "Eiko Magami"
trigger46 = p2name != "Biko Daitokuji"
trigger47 = p2name != "Princess Peach"
trigger48 = p2name != "Nega Peach"
trigger49 = p2name != "Dark Chun Li"
trigger50 = p2name != "Neo Dark Chun Li"
trigger51 = p2name != "Anita"
trigger52 = p2name != "Tamikav"
trigger53 = p2name != "Shorasha"
trigger54 = p2name != "Ainsley"
trigger55 = p2name != "Polaris"
trigger56 = p2name != "Buttercup"
trigger57 = p2name != "Baby Bonnie Hood"
trigger58 = p2name != "Bulleta"
trigger59 = p2name != "Hsien-Ko"
trigger60 = p2name != "Lei-Lei"
trigger61 = p2name != "Tessa"
value = 3,11

[State 2355602, Male Screaming2]
type = Playsnd
triggerall = Var(11) = 2
triggerall = projcontact = 1
trigger1 = p2name != "kitana"
trigger2 = p2name != "mileena"
trigger3 = p2name != "jade"
trigger4 = p2name != "cammy"
trigger5 = p2name != "chun li"
trigger6 = p2name != "kyla"
trigger7 = p2name != "sakura"
trigger8 = p2name != "juli"
trigger9 = p2name != "sonya"
trigger10 = p2name != "juni"
trigger11 = p2name != "evil chun li"
trigger12 = p2name != "mai"
trigger13 = p2name != "jenet"
trigger14 = p2name != "rose"
trigger15 = p2name != "Athena"
trigger16 = p2name != "Shermie"
trigger17 = p2name != "Karin"
trigger18 = p2name != "Vanessa"
trigger19 = p2name != "Mai"
trigger20 = p2name != "Kasumi"
trigger21 = p2name != "Blue Mary"
trigger22 = p2name != "Hinako"
trigger23 = p2name != "Leona"
trigger24 = p2name != "Chizuru"
trigger25 = p2name != "Storm"
trigger26 = p2name != "Psylocke"
trigger27 = p2name != "Rogue"
trigger28 = p2name != "King"
trigger29 = p2name != "Vice"
trigger30 = p2name != "Mature"
trigger31 = p2name != "Yuri"
trigger32 = p2name != "Mugena"
trigger33 = p2name != "Kula"
trigger34 = p2name != "BBH"
trigger35 = p2name != "Charlote"
trigger36 = p2name != "Nakoruru"
trigger37 = p2name != "rim"
trigger38 = p2name != "lilith"
trigger39 = p2name != "felicia"
trigger40 = p2name != "hsien-ko"
trigger41 = p2name != "q bee"
trigger42 = p2name != "r. mika"
trigger43 = p2name != "Janne"
trigger44 = p2name != "Aska"
trigger45 = p2name != "Eiko Magami"
trigger46 = p2name != "Biko Daitokuji"
trigger47 = p2name != "Princess Peach"
trigger48 = p2name != "Nega Peach"
trigger49 = p2name != "Dark Chun Li"
trigger50 = p2name != "Neo Dark Chun Li"
trigger51 = p2name != "Anita"
trigger52 = p2name != "Tamikav"
trigger53 = p2name != "Shorasha"
trigger54 = p2name != "Ainsley"
trigger55 = p2name != "Polaris"
trigger56 = p2name != "Buttercup"
trigger57 = p2name != "Baby Bonnie Hood"
trigger58 = p2name != "Bulleta"
trigger59 = p2name != "Hsien-Ko"
trigger60 = p2name != "Lei-Lei"
trigger61 = p2name != "Tessa"
value = 3,10

[State 2355602, Male Screaming3]
type = Playsnd
triggerall = Var(11) = 3
triggerall = projcontact = 1
trigger1 = p2name != "kitana"
trigger2 = p2name != "mileena"
trigger3 = p2name != "jade"
trigger4 = p2name != "cammy"
trigger5 = p2name != "chun li"
trigger6 = p2name != "kyla"
trigger7 = p2name != "sakura"
trigger8 = p2name != "juli"
trigger9 = p2name != "sonya"
trigger10 = p2name != "juni"
trigger11 = p2name != "evil chun li"
trigger12 = p2name != "mai"
trigger13 = p2name != "jenet"
trigger14 = p2name != "rose"
trigger15 = p2name != "Athena"
trigger16 = p2name != "Shermie"
trigger17 = p2name != "Karin"
trigger18 = p2name != "Vanessa"
trigger19 = p2name != "Mai"
trigger20 = p2name != "Kasumi"
trigger21 = p2name != "Blue Mary"
trigger22 = p2name != "Hinako"
trigger23 = p2name != "Leona"
trigger24 = p2name != "Chizuru"
trigger25 = p2name != "Storm"
trigger26 = p2name != "Psylocke"
trigger27 = p2name != "Rogue"
trigger28 = p2name != "King"
trigger29 = p2name != "Vice"
trigger30 = p2name != "Mature"
trigger31 = p2name != "Yuri"
trigger32 = p2name != "Mugena"
trigger33 = p2name != "Kula"
trigger34 = p2name != "BBH"
trigger35 = p2name != "Charlote"
trigger36 = p2name != "Nakoruru"
trigger37 = p2name != "rim"
trigger38 = p2name != "lilith"
trigger39 = p2name != "felicia"
trigger40 = p2name != "hsien-ko"
trigger41 = p2name != "q bee"
trigger42 = p2name != "r. mika"
trigger43 = p2name != "Janne"
trigger44 = p2name != "Aska"
trigger45 = p2name != "Eiko Magami"
trigger46 = p2name != "Biko Daitokuji"
trigger47 = p2name != "Princess Peach"
trigger48 = p2name != "Nega Peach"
trigger49 = p2name != "Dark Chun Li"
trigger50 = p2name != "Neo Dark Chun Li"
trigger51 = p2name != "Anita"
trigger52 = p2name != "Tamikav"
trigger53 = p2name != "Shorasha"
trigger54 = p2name != "Ainsley"
trigger55 = p2name != "Polaris"
trigger56 = p2name != "Buttercup"
trigger57 = p2name != "Baby Bonnie Hood"
trigger58 = p2name != "Bulleta"
trigger59 = p2name != "Hsien-Ko"
trigger60 = p2name != "Lei-Lei"
trigger61 = p2name != "Tessa"
value = 20,26

[State 2355602, Female Screaming]
type = VarRandom
triggerall = Time = 5
trigger1 = p2name = "kitana"
trigger2 = p2name = "mileena"
trigger3 = p2name = "jade"
trigger4 = p2name = "cammy"
trigger5 = p2name = "chun li"
trigger6 = p2name = "kyla"
trigger7 = p2name = "sakura"
trigger8 = p2name = "juli"
trigger9 = p2name = "sonya"
trigger10 = p2name = "juni"
trigger11 = p2name = "evil chun li"
trigger12 = p2name = "mai"
trigger13 = p2name = "jenet"
trigger14 = p2name = "rose"
trigger15 = p2name = "Athena"
trigger16 = p2name = "Shermie"
trigger17 = p2name = "Karin"
trigger18 = p2name = "Vanessa"
trigger19 = p2name = "Mai"
trigger20 = p2name = "Kasumi"
trigger21 = p2name = "Blue Mary"
trigger22 = p2name = "Hinako"
trigger23 = p2name = "Leona"
trigger24 = p2name = "Chizuru"
trigger25 = p2name = "Storm"
trigger26 = p2name = "Psylocke"
trigger27 = p2name = "Rogue"
trigger28 = p2name = "King"
trigger29 = p2name = "Vice"
trigger30 = p2name = "Mature"
trigger31 = p2name = "Yuri"
trigger32 = p2name = "Mugena"
trigger33 = p2name = "Kula"
trigger34 = p2name = "BBH"
trigger35 = p2name = "Charlote"
trigger36 = p2name = "Nakoruru"
trigger37 = p2name = "rim"
trigger38 = p2name = "lilith"
trigger39 = p2name = "felicia"
trigger40 = p2name = "hsien-ko"
trigger41 = p2name = "q bee"
trigger42 = p2name = "r. mika"
trigger43 = p2name = "Janne"
trigger44 = p2name = "Aska"
trigger45 = p2name = "Eiko Magami"
trigger46 = p2name = "Biko Daitokuji"
trigger47 = p2name = "Princess Peach"
trigger48 = p2name = "Nega Peach"
trigger49 = p2name = "Dark Chun Li"
trigger50 = p2name = "Neo Dark Chun Li"
trigger51 = p2name = "Anita"
trigger52 = p2name = "Tamikav"
trigger53 = p2name = "Shorasha"
trigger54 = p2name = "Ainsley"
trigger55 = p2name = "Polaris"
trigger56 = p2name = "Buttercup"
trigger57 = p2name = "Baby Bonnie Hood"
trigger58 = p2name = "Bulleta"
trigger59 = p2name = "Hsien-Ko"
trigger60 = p2name = "Lei-Lei"
trigger61 = p2name = "Tessa"
v = 11
range = 3,5

[State 2355602, Female Screaming2]
type = Playsnd
triggerall = Var(11) = 3
triggerall = projcontact = 1
trigger1 = p2name = "kitana"
trigger2 = p2name = "mileena"
trigger3 = p2name = "jade"
trigger4 = p2name = "cammy"
trigger5 = p2name = "chun li"
trigger6 = p2name = "kyla"
trigger7 = p2name = "sakura"
trigger8 = p2name = "juli"
trigger9 = p2name = "sonya"
trigger10 = p2name = "juni"
trigger11 = p2name = "evil chun li"
trigger12 = p2name = "mai"
trigger13 = p2name = "jenet"
trigger14 = p2name = "rose"
trigger15 = p2name = "Athena"
trigger16 = p2name = "Shermie"
trigger17 = p2name = "Karin"
trigger18 = p2name = "Vanessa"
trigger19 = p2name = "Mai"
trigger20 = p2name = "Kasumi"
trigger21 = p2name = "Blue Mary"
trigger22 = p2name = "Hinako"
trigger23 = p2name = "Leona"
trigger24 = p2name = "Chizuru"
trigger25 = p2name = "Storm"
trigger26 = p2name = "Psylocke"
trigger27 = p2name = "Rogue"
trigger28 = p2name = "King"
trigger29 = p2name = "Vice"
trigger30 = p2name = "Mature"
trigger31 = p2name = "Yuri"
trigger32 = p2name = "Mugena"
trigger33 = p2name = "Kula"
trigger34 = p2name = "BBH"
trigger35 = p2name = "Charlote"
trigger36 = p2name = "Nakoruru"
trigger37 = p2name = "rim"
trigger38 = p2name = "lilith"
trigger39 = p2name = "felicia"
trigger40 = p2name = "hsien-ko"
trigger41 = p2name = "q bee"
trigger42 = p2name = "r. mika"
trigger43 = p2name = "Janne"
trigger44 = p2name = "Aska"
trigger45 = p2name = "Eiko Magami"
trigger46 = p2name = "Biko Daitokuji"
trigger47 = p2name = "Princess Peach"
trigger48 = p2name = "Nega Peach"
trigger49 = p2name = "Dark Chun Li"
trigger50 = p2name = "Neo Dark Chun Li"
trigger51 = p2name = "Anita"
trigger52 = p2name = "Tamikav"
trigger53 = p2name = "Shorasha"
trigger54 = p2name = "Ainsley"
trigger55 = p2name = "Polaris"
trigger56 = p2name = "Buttercup"
trigger57 = p2name = "Baby Bonnie Hood"
trigger58 = p2name = "Bulleta"
trigger59 = p2name = "Hsien-Ko"
trigger60 = p2name = "Lei-Lei"
trigger61 = p2name = "Tessa"
value = 3, 12 

[State 2355602, Female Screaming3]
type = Playsnd
triggerall = Var(11) = 4
triggerall = projcontact = 1
trigger1 = p2name = "kitana"
trigger2 = p2name = "mileena"
trigger3 = p2name = "jade"
trigger4 = p2name = "cammy"
trigger5 = p2name = "chun li"
trigger6 = p2name = "kyla"
trigger7 = p2name = "sakura"
trigger8 = p2name = "juli"
trigger9 = p2name = "sonya"
trigger10 = p2name = "juni"
trigger11 = p2name = "evil chun li"
trigger12 = p2name = "mai"
trigger13 = p2name = "jenet"
trigger14 = p2name = "rose"
trigger15 = p2name = "Athena"
trigger16 = p2name = "Shermie"
trigger17 = p2name = "Karin"
trigger18 = p2name = "Vanessa"
trigger19 = p2name = "Mai"
trigger20 = p2name = "Kasumi"
trigger21 = p2name = "Blue Mary"
trigger22 = p2name = "Hinako"
trigger23 = p2name = "Leona"
trigger24 = p2name = "Chizuru"
trigger25 = p2name = "Storm"
trigger26 = p2name = "Psylocke"
trigger27 = p2name = "Rogue"
trigger28 = p2name = "King"
trigger29 = p2name = "Vice"
trigger30 = p2name = "Mature"
trigger31 = p2name = "Yuri"
trigger32 = p2name = "Mugena"
trigger33 = p2name = "Kula"
trigger34 = p2name = "BBH"
trigger35 = p2name = "Charlote"
trigger36 = p2name = "Nakoruru"
trigger37 = p2name = "rim"
trigger38 = p2name = "lilith"
trigger39 = p2name = "felicia"
trigger40 = p2name = "hsien-ko"
trigger41 = p2name = "q bee"
trigger42 = p2name = "r. mika"
trigger43 = p2name = "Janne"
trigger44 = p2name = "Aska"
trigger45 = p2name = "Eiko Magami"
trigger46 = p2name = "Biko Daitokuji"
trigger47 = p2name = "Princess Peach"
trigger48 = p2name = "Nega Peach"
trigger49 = p2name = "Dark Chun Li"
trigger50 = p2name = "Neo Dark Chun Li"
trigger51 = p2name = "Anita"
trigger52 = p2name = "Tamikav"
trigger53 = p2name = "Shorasha"
trigger54 = p2name = "Ainsley"
trigger55 = p2name = "Polaris"
trigger56 = p2name = "Buttercup"
trigger57 = p2name = "Baby Bonnie Hood"
trigger58 = p2name = "Bulleta"
trigger59 = p2name = "Hsien-Ko"
trigger60 = p2name = "Lei-Lei"
trigger61 = p2name = "Tessa"
value = 20,14

[State 2355602, Female Screaming4]
type = Playsnd
triggerall = Var(11) = 5
triggerall = projcontact = 1
trigger1 = p2name = "kitana"
trigger2 = p2name = "mileena"
trigger3 = p2name = "jade"
trigger4 = p2name = "cammy"
trigger5 = p2name = "chun li"
trigger6 = p2name = "kyla"
trigger7 = p2name = "sakura"
trigger8 = p2name = "juli"
trigger9 = p2name = "sonya"
trigger10 = p2name = "juni"
trigger11 = p2name = "evil chun li"
trigger12 = p2name = "mai"
trigger13 = p2name = "jenet"
trigger14 = p2name = "rose"
trigger15 = p2name = "Athena"
trigger16 = p2name = "Shermie"
trigger17 = p2name = "Karin"
trigger18 = p2name = "Vanessa"
trigger19 = p2name = "Mai"
trigger20 = p2name = "Kasumi"
trigger21 = p2name = "Blue Mary"
trigger22 = p2name = "Hinako"
trigger23 = p2name = "Leona"
trigger24 = p2name = "Chizuru"
trigger25 = p2name = "Storm"
trigger26 = p2name = "Psylocke"
trigger27 = p2name = "Rogue"
trigger28 = p2name = "King"
trigger29 = p2name = "Vice"
trigger30 = p2name = "Mature"
trigger31 = p2name = "Yuri"
trigger32 = p2name = "Mugena"
trigger33 = p2name = "Kula"
trigger34 = p2name = "BBH"
trigger35 = p2name = "Charlote"
trigger36 = p2name = "Nakoruru"
trigger37 = p2name = "rim"
trigger38 = p2name = "lilith"
trigger39 = p2name = "felicia"
trigger40 = p2name = "hsien-ko"
trigger41 = p2name = "q bee"
trigger42 = p2name = "r. mika"
trigger43 = p2name = "Janne"
trigger44 = p2name = "Aska"
trigger45 = p2name = "Eiko Magami"
trigger46 = p2name = "Biko Daitokuji"
trigger47 = p2name = "Princess Peach"
trigger48 = p2name = "Nega Peach"
trigger49 = p2name = "Dark Chun Li"
trigger50 = p2name = "Neo Dark Chun Li"
trigger51 = p2name = "Anita"
trigger52 = p2name = "Tamikav"
trigger53 = p2name = "Shorasha"
trigger54 = p2name = "Ainsley"
trigger55 = p2name = "Polaris"
trigger56 = p2name = "Buttercup"
trigger57 = p2name = "Baby Bonnie Hood"
trigger58 = p2name = "Bulleta"
trigger59 = p2name = "Hsien-Ko"
trigger60 = p2name = "Lei-Lei"
trigger61 = p2name = "Tessa"
value = 20,20

[State 2355602, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 2355602, 9]
type = Changestate
trigger1 = time = 330
value = 0
ctrl = 1


;Kombo Blast
[Statedef 21135]
Type = S
Movetype = A
Physics = S
juggle = 4
poweradd = 50
ctrl = 0

[State 21135, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 21135, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 21135, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 21135, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 21135, ChangeAnim]
type = ChangeAnim
trigger1 = time = 30
value = 20 

[State 21135, 0]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 21135, ChangeAnim2]
type = ChangeState
trigger1 = p2bodydist X > 15 && p2bodydist X < 20
value = 21136

[Statedef 21136]
type    = S
movetype= A
physics = S
juggle  = 1
velset = 0,0
anim = 3100
ctrl = 0

[State 21136, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 21136, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[state 21136, 1c]
type = hitdef
trigger1 = AnimElem = 4
trigger2 = AnimElem = 10
trigger3 = AnimElem = 16
trigger4 = AnimElem = 22
trigger5 = AnimElem = 27
trigger6 = AnimElem = 33
trigger7 = AnimElem = 40
attr = S, HA
animtype  = light
priority  = 2
damage = 0
guardflag = A
sparkxy = -20, -80
pausetime = 1,35
hitsound   = s27,27
guardsound = s0,0
ground.type = high
ground.slidetime = 25
ground.hittime  = 25
ground.velocity = 0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall.recover = 0
numhits = 1

[State 21136, Super 1]
type = PlaySnd
trigger1 = AnimElem = 4
value = 60, 6

[State 21136, Super 2]
type = PlaySnd
trigger1 = AnimElem = 16
value = 31,38 

[State 21136, Super 4]
type = PlaySnd
trigger1 = AnimElem = 40
value = 20,25 

[state 21136, 1i]
type = Helper
trigger1 = AnimElem = 4
trigger2 = AnimElem = 16
trigger3 = AnimElem = 33
trigger4 = AnimElem = 40
stateno = 6040
name = "Roundhouse"
pos = 70*const(size.xscale),-92*const(size.xscale)
postype = P1
helpertype = normal
sprpriority = 0
keyctrl = 0
ownpal = 0

[state 21136, 2b]
type = changestate
trigger1 = movehit = 1
trigger1 = animtime = 0
value = 21137
ctrl = 0

[StateDef 21137]
type    = S
movetype= A
physics = S
juggle  = 1
ctrl = 0
anim = 430
sprpriority = 2

[State 21137, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 21137, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 21137, 0]
type = PlaySnd
trigger1 = Time = 1
value = 4, 9

[State 21137, 1]
type = PlaySnd
trigger1 = Time = 3
value = 4, 16

[State 21137, 2]
type = HitDef
trigger1 = !MoveContact 
attr = C, NA
damage    = 0
animtype  = Heavy
givepower = 50, 50
hitflag = MAF ;HAF
guardflag = MA
sparkno = -1
guard.sparkno = -1
hitsound   = s20,32
guardsound = S0,0
ground.type = High
ground.slidetime = 5
ground.hittime  = 11
ground.velocity = -1.5, -9.5          
air.velocity = -1.5,-11
down.hittime = 20
fall = 1
fall.recover = 0

[State 21137, NotFriendly]
type = VarSet
trigger1 = MoveGuarded = 1
v = 5
value = 1

[State 21137, 4]
type = ChangeState
trigger1 = AnimTime = 0
value = 21138
ctrl = 1

[StateDef 21138]
type    = S
movetype= A
physics = S
juggle  = 2
ctrl = 0
velset = 0,0,0
anim = 131302
sprpriority = 9

[State 21138, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 21138, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 21138, Fireball Chuck]
type = playsnd
trigger1 = AnimElem = 2
value = 4,10

[State 21138, Fireball]
type = playsnd
trigger1 = AnimElem = 3
value = 60,7

[State 21138, Fire Explode]
type = Explod
trigger1 = AnimElem = 3
anim = 131401
id = 131401
scale = 1.1, 1.1
pos = -10*const(size.xscale), -105*const(size.xscale)
postype = p1
sprpriority = 2
ontop = 1
ownpal = 1

[State 21138, High Fireball]
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
damage    = 999
animtype = Hard
air.animtype = Back
air.fall = 1
guardflag = M
hitflag = MAF
pausetime = 0,0
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
p1stateno = 9830

[State 21138, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 21138, 10]
type = ChangeState
trigger1 = AnimTime = 0
ctrl = 1
value = 0


;Brutality
[Statedef 22236]
type    = S    
movetype= A                      ;Move-type: A-attack, I-idle, H-gethit
physics = S                      ;Physics: S-stand, C-crouch, A-air
velset = 0,0                     ;Set velocity (x,y) (Def: no change)
ctrl = 0                         ;Set ctrl (Def: no change)
poweradd = 20   

[State 22236, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 22236, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 22236, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 22236, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 22236, ChangeAnim]
type = ChangeAnim
trigger1 = time = 30
value = 20 

[State 22236, 0]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 22236, Brutality]
type = PlaySnd
trigger1 = Time = 40
value = 90210,0
volume = 255

[State 22236,2]
type = PlaySnd
trigger1 = Time = 60
value = 4000,11

[State 22236, ChangeAnim2]
type = ChangeState
trigger1 = p2bodydist X > 1 && p2bodydist X < 5
value = 22237

[StateDef 22237]
Type = S
Movetype = A
Physics = S
juggle = 4
poweradd = 50
Anim = 90210
ctrl = 0

[State 22237, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 22237, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 22237, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 22237, Super 1]
type = PlaySnd
trigger1 = AnimElem = 4
value = 60, 6

[State 22237, Super 2]
type = PlaySnd
trigger1 = AnimElem = 16
value = 31,38 

[State 22237, Super 4]
type = PlaySnd
trigger1 = AnimElem = 40
value = 20,25 

[State 22237, Super D]
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
time = 300

[state 22237, 1]
type = hitdef
trigger1 = AnimElem = 3 
trigger2 = animelem = 8
trigger3 = AnimElem = 13 
trigger4 = animelem = 20
trigger5 = animelem = 27
trigger6 = animelem = 33
trigger7 = animelem = 40
trigger8 = animelem = 48
trigger9 = animelem = 52
trigger10 = animelem = 57
trigger11 = animelem = 61
trigger12 = animelem = 66
trigger13 = animelem = 70
trigger14 = animelem = 76
trigger15 = animelem = 83
attr = S, HA
animtype  = light
priority  = 2
persistent = 2
guardflag = A
damage = 0
sparkxy = -5,-100
hitsound   = s27,27
guardsound = s0,0
pausetime = 0,35
ground.type = high
ground.slidetime = 25
ground.hittime  = 25
ground.velocity = 0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)
fall.recover = 0
envshake.time = 10
envshake.freq = 190
envshake.ampl = -4


[state 22237, 2]
type = hitdef
trigger1 = AnimElem = 91
attr = S, HA
animtype  = light
priority  = 2
persistent = 2
guardflag = A
damage = 0
sparkxy = -5,-100
hitsound   = s27,27
guardsound = s0,0
pausetime = 0,35
ground.type = high
ground.slidetime = 25
ground.hittime  = 25
ground.velocity = 0
fall.recover = 0
envshake.time = 10
envshake.freq = 190
envshake.ampl = -4

[State 22237, 10]
type = ChangeState
trigger1 = AnimTime = 0
ctrl = 1
value = 22238

[StateDef 22238]
type = A
movetype= A
physics = N
juggle = 5
ctrl = 0
anim = 1300

[State 22238, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 22238, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 22238, 2]
type = Velset
trigger1 = AnimElem = 1
trigger1 = movehit = 0
x = 9
y = .1

[State 22238, 2]
type = Velset
trigger1 = movehit = 1
x = 6
y = .02

[State 22238, 1]
type = VarRandom
trigger1 = AnimElem = 1
v = 11
range = 1, 2

[State 22238, 3.3]
type = Playsnd
trigger1 = Var(11) = 1
trigger1 = AnimElem = 1
value = 60,0

[State 22238, 3.4]
type = Playsnd
trigger1 = Var(11) = 2
trigger1 = AnimElem = 1
value = 20,6

[State 22238, Super D]
type = AfterImage
trigger1 = time = 1
FrameGap = 1
length = 16
timegap = 2
PalBright   =   0,  0,  0
PalContrast = 80,80,80
PalAdd      =   10,  0,  0
PalMul      =  1,1,1
Trans = Add1
time = 95

[State 22238, 2]
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
pausetime = 1,3
sparkxy = -40, -80
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
envshake.time = 10
envshake.freq = 190
envshake.ampl = -4
fall.recover = 0
kill       = (Var(9) = 0)
fall.kill  = (Var(9) = 0)
guard.kill = (Var(9) = 0)

[State 22238, End]
type = Changestate
trigger1 = AnimTime = 0
trigger1 = Pos Y >= 0
trigger1 = Vel Y > 0
value = 22239
ctrl = 1

[Statedef 22239]
type    = S
movetype= A
physics = S
juggle  = 1
ctrl = 0
anim = 430
sprpriority = 2

[State 22239, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 22239, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 22239, 0]
type = PlaySnd
trigger1 = Time = 1
value = 4, 9

[State 22239, 1]
type = PlaySnd
trigger1 = Time = 3
value = 4, 16

[State 22239, 2]
type = HitDef
trigger1 = !MoveContact 
attr = C, NA
damage    = 90, 35
animtype  = Heavy
givepower = 50, 50
hitflag = MAF ;HAF
guardflag = MA
sparkno = -1
guard.sparkno = -1
hitsound   = s20,32
guardsound = S0,0
ground.type = High
ground.slidetime = 5
ground.hittime  = 11
ground.velocity = -2.5, -9.5          
air.velocity = -2.0,-11
down.hittime = 20
fall = 1
fall.recover = 0
p1stateno = 9830

[State 22239, NotFriendly]
type = VarSet
trigger1 = MoveGuarded = 1
v = 5
value = 1

[State 3601, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1


[State 22239, 4]
type = ChangeState
trigger1 = AnimTime = 0
value = 431
ctrl = 1

; Get rid of P2
[Statedef 2500]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 2500, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow
flag2 = nokosnd
flag3 = Invisible

[State 2500, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 2500, NoSlow2]
type = AssertSpecial
trigger1 = Time >= 0
flag = NoShadow

[State 2500, 0.3]
type = SprPriority
trigger1 = time = 0
value = 0

[State 2500, 2.6]
type = Posset
trigger1 = time = 0
Y = 900

[State 2500, OkayYouCanStop]
type = PalFX
trigger1 = time = 0
time = 1
add = 0, 0, 0
mul = 0, 0, 0

[State 2500, NoBGM]
type = AssertSpecial
trigger1 = Time >= 9
flag = nokoslow

[State 2500, 3]
type = Lifeadd
trigger1 = Time = 9
Value = -9999

[State 2500, 3]
type = Selfstate
trigger1 = time = 5
value = 5150
ctrl = 1


;-----------------------------------------
; Player 1, You do the blow-up
[Statedef 2600]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 2600, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 2600, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 2600, 0.3]
type = SprPriority
trigger1 = time = 0
value = 2

[State 2600, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 1,5

[State 2600, 1]
type = Explod
trigger1 = projcontact = 1
trigger1 = p2stateno = 2500
anim = 2203
id = 2203
pos = 80*const(size.xscale), 220*const(size.xscale) 
postype = left
bindtime = -1
facing = 1
sprpriority = 2
ontop = 1
ownpal = 0

[State 2600, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 2600, 2]
type = ChangeState
trigger1 = Time = 0
value = 0
ctrl = 1

[Statedef 9000]
type    = S
movetype= A
physics = S
juggle  = 1
velset = 0,0
anim = 9000
ctrl = 0

[State 9000, 1]
type = Explod
trigger1 = time = 0
anim = 7040
id = 7050
pos = 156*const(size.xscale),90*const(size.xscale)
postype = left
bindtime = -1
facing = 1
sprpriority = 2
ontop = 1
ownpal = 0

[State 9000, 5]
type = HitDef
trigger1 = p2life < 5
trigger1 = p2movetype != H
trigger1 = p2statetype != A
attr = S, HA
numhits = 0
priority  = 7
sparkno = -1
p2stateno = 9010
kill = 0

[State 9000, 5.5]
type = ParentVarset
trigger1 = time = 0
V = 13
value = 400

[State 9000, 6]
type = Destroyself
trigger1 = p2stateno = 9010

;------------------------------------------------
;Fatality time
[Statedef 9010]
type    = S
movetype= H
physics = N
juggle  = 1
velset = 0,0
ctrl = 0

[State 9010, 0.1]
type = Changeanim
trigger1 = animexist(5300)
trigger1 = anim != 5300
value = 5300

[State 9010, 1]
type = Changeanim2
trigger1 = !animexist(5300)
trigger1 = anim != 9010
value = 9010

;Fatality time ends
[Statedef 9011]
type    = S
movetype= H
physics = S
juggle  = 1

[State 9011, NoSlow]
type = AssertSpecial
trigger1 = time = [0, 5000]
flag = nokoslow

[State 9011, 2]
type = Lifeadd
trigger1 = time = 1
value = -9999

[State 9011, 3]
type = Selfstate
trigger1 = time = 1
value = 5070
ctrl = 0

[Statedef 9020]
type    = S
movetype= A
juggle  = 1
anim = 9000
ctrl = 0

[State 9020, 5]
type = HitDef
trigger1 = time = 0
attr = S, HA
;Nothing = 0
numhits = 0
priority  = 7
sparkno = -1
p2stateno = 9011

[State 9020, 6]
type = Destroyself
trigger1 = time = 1


[Statedef 9101]
type    = S
movetype= I
juggle  = 0
anim = 9150
velset = 0,0,0
ctrl = 0

[State 9101, bindit]
type = ScreenBound
trigger1 = 1
value = 1
;MoveCamera= 1, 0

[State 9101, StartSnd1]
type = Playsnd
trigger1 = Time = 100
value = 4,6

[Statedef 9124]
type    = S
movetype= H
physics = N
velset = 0,0,0
ctrl = 0

[State 9124, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9124, Need to use the patch]
type = Explod
trigger1 = time >= 10
anim = 90001 
pos = 0*const(size.xscale),-70*const(size.xscale)
postype = p1
ontop = 1

[State 9124, Flash]
type = PalFX
trigger1 = time = 0
time = 1000
add = 0, 0, 0
mul = 188, 355, 155
sinadd = 0, 90, 90, 7

[State 9124, EndOfFAT]
type = Changestate
trigger1 = Time = 300
value = 9125
ctrl = 0


[Statedef 9125]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9125, Black]
type = BGPalFX
trigger1 = time = 3
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 9215, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9125, 0.3]
type = SprPriority
trigger1 = time = 0
value = 0

[State 9125, 2.6]
type = Posset
trigger1 = time = 2
Y = 900

[State 9125, OkayYouCanStop]
type = PalFX
trigger1 = time = 6
time = 1
add = 0, 0, 0
mul = 0, 0, 0

[State 9215, NoBGM]
type = AssertSpecial
trigger1 = Time >= 9
flag = nokoslow

[State 9125, 3]
type = Lifeadd
trigger1 = Time = 9
Value = -9999

[State 9125, 2]
type = Selfstate
trigger1 = Time = 9
value = 5150


[Statedef 9130]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 9130, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9130, BOOM]
type = Playsnd
trigger1 = Time = 4
value = 4,21

[State 9130, 0.3]
type = SprPriority
trigger1 = time = 0
value = 2

[State 9130, 1.554]
type = Helper
trigger1 = Time = 0
stateno = 9200
name = "LotsaMeat"
postype = p2
pos = 0, const(size.mid.pos.y)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9130, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,Var(24)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9130, 1.556]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,(Var(24) / 2)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9130, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 9130, 2]
type = destroyself 
trigger1 = Time >= 0 
value = 0
ctrl = 1

;--------------------------------
[Statedef 9830]
type    = S
movetype= A
physics = N
velset = 0,0
ctrl = 0

[State 9830, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9830, NoBGM]
type = VarSet
trigger1 = Time >= 0
v = 26
value = 1

[State 9830, White]
type = BGPalFX
trigger1 = time = 0
time = 3
add = 230, 230, 230
ignorehitpause = 1

[State 9830, Black]
type = BGPalFX
trigger1 = Time >= 0
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 9830, Blood]
type = Explod
trigger1 = time = 0
anim = 6041
id = 6041
offset = 0*const(size.xscale), -50*const(size.xscale)
pos = 0*const(size.xscale), -50*const(size.xscale)
postype = p2
sprpriority = 7
ontop = 1
ownpal = 1

[State 9830, GoBoom]
type = TargetState
trigger1 = Time = 0
Value = 9125
pos = 170*const(size.xscale),0*const(size.xscale)

[State 9830, BOOM]
type = Playsnd
trigger1 = Time = 0
value = 4,21

[State 9830, BodyDamage1]
type = Playsnd
trigger1 = Time = 6
value = 1,5

[State 9830, BodyDamage2]
type = Playsnd
trigger1 = Time = 2
value = 31,35

[State 9830, 1]
type = SprPriority
trigger1 = time = 0
value = 2

[State 9830, 1.555]
type = Helper
trigger1 = Time = 0
stateno = 9200
name = "LotsaMeat"
postype = p2
pos = 0, const(size.mid.pos.y)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9830, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,Var(24)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9830, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,(Var(24) / 2)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9830, SetFatal]
type = Varset
trigger1 = Time = 1
V = 4
value = 1

[State 9830, EndOfFAT]
type = Changestate
trigger1 = AnimTime = 0 ;Time = 20
value = 0
ctrl = 1

[Statedef 9833]
type    = S
movetype= A
physics = N
velset = 0,0
ctrl = 0

[State 9833, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9833, BOOM]
type = Playsnd
trigger1 = Time = 4
value = 4,21

[State 9833, 0.3]
type = SprPriority
trigger1 = time = 0
value = 2

[State 9833, 1.555]
type = Helper
trigger1 = Time = 0
stateno = 9200
name = "LotsaMeat"
postype = p2
pos = 0, const(size.mid.pos.y)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9833, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,Var(24)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9833, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0,(Var(24) / 2)
Postype = P2
bindtime = 1
sprpriority = 3

[State 9833, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 5

[State 9833, GoBoom]
type = TargetState
trigger1 = AnimTime = 0
Value = 9125
pos = 170*const(size.xscale),0*const(size.xscale)

[State 9833, Oopsie]
type = TargetLifeAdd
trigger1 = AnimTime = 0
value = -9999

[State 9833, EndOfFAT]
type = Changestate
trigger1 = AnimTime = 0
value = 9130
ctrl = 0


;--------------------------------
; Measure Player Two
[StateDef 9200]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9150
ctrl = 0

[State 9200, Black]
type = BGPalFX
trigger1 = time = 3
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 9200, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9200, 5]
type = HitDef
trigger1 = Time = 0
attr = S, HA
numhits = 0
priority  = 7
sparkno = -1
guard.sparkno = -1
p2stateno = 9125
kill = 0

[State 9200, head]
type = BindtoTarget
trigger1 = time = 1
pos = 0,0,head
time = 1

[State 9200, Where]
type = ParentVarSet
trigger1 = time = 1
v = 24
value = Pos Y

[State 9200, mid]
type = BindtoTarget
trigger1 = time = 2
pos = 0,0,mid
time = 0

[State 9200, start]
type = changestate
trigger1 = time = 2
value = 9201

;--------------------------------------
; Blow chunks
[StateDef 9201]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9150
ctrl = 0

[State 9201, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9201, 10]
type = Helper
trigger1 = Random < 300
stateno = 9211
name = "Skull"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 10]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 20]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 30]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 10]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 20]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 10]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 20]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 30]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 10]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 20]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 30]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 10]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 20]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 30]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 40]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 50]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 60]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 70]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, 80]
type = Helper
trigger1 = 1
stateno = 9210
name = "MeatBit"
pos = 0,0
postype = p2 ;p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9201, done]
type = DestroySelf
trigger1 = time = 4

;-----------------------------------
;Meat Bits
[StateDef 9210]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9210, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9210, Select]
type = VarRandom
trigger1 = Time = 0
v = 11
Range = 9100, 9105

[State 9210, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = Var(11)

[State 9210, GO]
type = VelSet
trigger1 = Time = 0
X = 3.4-(Random/144)
Y = -4-(Random/99)

[State 9210, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9210, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0
value = -14

[State 9210, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9210, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9210, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;Meat Bits
[StateDef 9211]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9211, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9211, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9106

[State 9211, GO]
type = VelSet
trigger1 = Time = 0
X = 5-(Random/88)
Y = -4-(Random/99)

[State 9211, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9211, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9211, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9211, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9211, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-------------------------
; P2 Go BOOM!
[StateDef 9330]
type    = S
movetype= A
physics = N
ctrl = 0
anim = 9330

[State 9330, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9330, RemArm]
type = RemoveExplod
trigger1 = Time = 1
id = 9310

[State 9330, BOOM]
type = Playsnd
trigger1 = Time = 4
value = 4,21

[State 9330, 0.3]
type = SprPriority
trigger1 = time = 0
value = 2

[State 9330, 1.555]
type = Helper
trigger1 = Time = 0
stateno = 9340
name = "SomeMeat"
pos = 36*const(size.xscale), -81*const(size.xscale)
Postype = P1
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9330, SetFatal]
type = Varset
trigger1 = Time = 4
V = 4
value = 1

[State 9330, Dropum]
type = TargetState
trigger1 = Time = 4
ctrl = 0
value = 9399

[State 9330, 1.555]
type = Explod
trigger1 = Time = 5
anim = 6041
pos = 36*const(size.xscale), -121*const(size.xscale)
Postype = P1
bindtime = 1
sprpriority = 3

[State 9330, 1.555]
type = Explod
trigger1 = Time = 5
anim = 6041
pos = 36*const(size.xscale), -81*const(size.xscale)
Postype = P1
bindtime = 1
sprpriority = 3

[State 9330, 1.555]
type = Explod
trigger1 = Time = 5
anim = 6041
pos = 36*const(size.xscale), -41*const(size.xscale)
Postype = P1
bindtime = 1
sprpriority = 3

[State 9330, 2]
type = ChangeState
trigger1 = Time = 20
value = 0
ctrl = 1

;--------------------------------------
; Blow chunks
[StateDef 9340]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9150
ctrl = 0

[State 9340, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9340, 10]
type = Helper
trigger1 = Time = 0
stateno = 9341
name = "Skull"
pos = 0,0
postype = p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 10]
type = Helper
trigger1 = Time = 0
trigger1 = Time = 4
stateno = 9342
name = "Hand"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 20]
type = Helper
trigger1 = 1
;trigger1 = Var(12) = 0
stateno = 9343
name = "BONE"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 20]
type = Helper
trigger1 = 1
;trigger1 = Var(12) = 0
stateno = 9343
name = "BONE"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 30]
type = Helper
trigger1 = 1
;trigger1 = Var(12) = 0
stateno = 9344
name = "Guts"
pos = 0,0
postype = p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 10]
type = Helper
trigger1 = 1
;trigger1 = Var(12) = 0
stateno = 9345
name = "Hip"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 20]
type = Helper
trigger1 = 1
trigger1 = Time = 0
trigger2 = Time = 4
;trigger1 = Var(12) = 0
stateno = 9346
name = "Leg"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 30]
type = Helper
trigger1 = 1
;trigger1 = Var(12) = 0
stateno = 9347
name = "MEAT"
pos = 0,0
postype = p1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, 10]
type = Helper
trigger1 = Time = 0
trigger2 = Time = 4
;trigger1 = Var(12) = 0
stateno = 9348
name = "InvLeg"
pos = 0,0
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1
under = 1

[State 9340, done]
type = DestroySelf
trigger1 = time = 4

;-----------------------------------
;SKULL
[StateDef 9341]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9341, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9106

[State 9341, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9341, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
;trigger1 = Vel Y != 0
type = VelAdd
y = .4

[State 9341, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0
value = -14

[State 9341, 0.5]
type = AngleDraw
trigger1 = Time >= 1
;trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0

[State 9341, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9341, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;Hand
[StateDef 9342]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9342, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9100

[State 9342, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9342, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9342, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9342, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9342, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9342, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;BONE
[StateDef 9343]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9343, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9101

[State 9343, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9343, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9343, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9343, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9343, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9343, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;Guts
[StateDef 9344]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9344, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9102

[State 9344, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9344, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
;trigger1 = Vel Y != 0
type = VelAdd
y = .4

[State 9344, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0
value = -14

[State 9344, 0.5]
type = AngleDraw
trigger1 = Time >= 1
;trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0

[State 9344, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9344, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;HIP
[StateDef 9345]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9345, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9103

[State 9345, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9345, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
;trigger1 = Vel Y != 0
type = VelAdd
y = .4

[State 9345, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9345, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9345, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9345, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;LEG
[StateDef 9346]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9346, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9104

[State 9346, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9346, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9346, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9346, 0.5]
type = AngleDraw
trigger1 = Time >= 1

[State 9346, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9346, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;MEAT
[StateDef 9347]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9347, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9105

[State 9347, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9347, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
;trigger1 = Vel Y != 0
type = VelAdd
y = .4

[State 9347, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0
value = -14

[State 9347, 0.5]
type = AngleDraw
trigger1 = Time >= 1
;trigger1 = Pos Y <= -7
;trigger1 = Vel Y < 0

[State 9347, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9347, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;-----------------------------------
;LEG2
[StateDef 9348]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9348, Draw]
type = ChangeAnim
trigger1 = Time = 0
value = 9114

[State 9348, GO]
type = VelSet
trigger1 = Time = 0
X = 2.6-(Random/188)
Y = -4-(Random/99)

[State 9348, Gravity]
trigger1 = Time >= 0
trigger1 = Pos Y <= -7
type = VelAdd
y = .4

[State 9348, .2]
type = AngleAdd
trigger1 = Time >= 1
trigger1 = Pos Y <= -7
value = -14

[State 9348, 0.5]
type = AngleDraw
trigger1 = Time >= 1


[State 9348, HitGroundSound]
type = PlaySnd
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
value = 1, 5

[State 9348, Halt]
type = VelSet
trigger1 = Pos Y > -7
trigger1 = Vel Y > 0
X = 0
Y = 0

;---------------------
; Zap Player two!!!
[StateDef 9380]
type    = A
movetype= H
physics = N
juggle  = 2
ctrl = 0

[State 9380, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9380, OW]
type = ChangeAnim2
trigger1 = time = 0
value = 1350

[State 9380, 6.7]
type = Explod 
trigger1 = time = 50
anim = 1616
name = "Smoke"
pos = 40*const(size.xscale),170*const(size.xscale)
postype = p1
bindtime = -1
helpertype = normal
sprpriority = 1
keyctrl = 0
ownpal = 1

[State 9380, Shockies]
type = PalFX
trigger1 = time = 0
time = 240
add = 0, 0, 0
mul = 128, 455, 455
sinadd = 0, 90, 90, 7

;----------------------------------------------
; Player Two Drops.
[StateDef 9390]
type    = A
movetype= H
physics = N
juggle  = 2
velset = .1,-.7,0
ctrl = 0

[State 9390, Gravity]
type = VelAdd
Trigger1 = 1
y = .4

[State 9390, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow
flag2 = nokosnd

[State 9390, 3]
type = Lifeadd
trigger1 = Time = 0
Value = -9999

[State 9390, Falling]
type = ChangeAnim2
trigger1 = time = 0
value = 1360

[State 9390, HitGround]
type = SelfState
trigger1 = Vel Y > 0
trigger1 = Pos Y >= 0
value = 5100 ;Hit ground

;-----------------------------------------
; Player 2, you're outta here!!!! (Quicker)
[Statedef 9399]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 9399, 0.3]
type = SprPriority
trigger1 = time = 0
value = 0

[State 9399, 2.6]
type = Posset
trigger1 = time = 0
Y = 900

[State 9399, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow

[State 9399, 3]
type = Lifeadd
trigger1 = Time = 0
Value = -9999

[State 9399, 2]
type = Selfstate
trigger1 = Time = 0
value = 5150


[Statedef 9400]
type    = S                      ;State-type: S-stand, C-crouch, A-air, L-liedown
movetype= A                      ;Move-type: A-attack, I-idle, H-gethit
physics = S                      ;Physics: S-stand, C-crouch, A-air
juggle  = 1                      ;Number of air juggle points move takes
velset = 0,0                     ;Set velocity (x,y) (Def: no change)
ctrl = 0                         ;Set ctrl (Def: no change)
poweradd = 20   

[State 9400, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay
flag2 = nokoslow

[State 9400, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 9400, DaDaDa]
type = Playsnd
trigger1 = Time = 1
value = 6,1

[State 9400, NoBGM]
type = VarSet
trigger1 = Time = 0
v = 26
value = 1

[State 9400, ChangeAnim]
type = ChangeAnim
trigger1 = time = 100
value = 20 

[State 9400, 1]
type = velset
trigger1 = time >= 0
trigger1 = anim = 20 
x = 2.5

[State 9400,3]
type = ChangeState
trigger1 = p2bodydist X < 13
value = 9401

;--------------------------------------
;----THE PIT--------------
;--------------------------------------

[Statedef 9401]
type    = S     
movetype= A     
physics = S    
juggle  = 1     
velset = 0,0
ctrl = 0
anim = 3073
poweradd = 40

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
ignorehitpause = 1
flag = NoBarDisplay

[State 9401, 1]
type = BGPalFX
trigger1 = Time >= 0  
time = 9999
add = -130, -130, -130
ignorehitpause = 1

[State 9401, NoBGM]
type = VarSet
trigger1 = Time >= 0
ignorehitpause = 1
v = 26
value = 1

[State 9401, 3]
type = PlaySnd
trigger1 = time = 0
value = 60,10

[State 9401, Velocity1]
type = Velset
trigger1 = AnimElem = 3
x = 4

[State 9401, Velocity2]
type = Velset
trigger1 = AnimElem = 4
x = -4

[State 9401, 4]
type = HitDef
trigger1 = AnimElem = 3
trigger2 = AnimElem = 4
trigger3 = AnimElem = 5
trigger4 = AnimElem = 6
attr = S, NA  
damage    = 0
animtype = Back
guardflag = M
hitflag = MAF
pausetime = 0,90
sparkno = -1
hitsound = s27,27
guardsound = s0,0
ground.slidetime = 50
ground.hittime  = 50
ground.Velocity = 3
air.Velocity = 2,-3
down.Velocity = 0,0
air.fall = 1

[State 9401, 7]
type = ChangeState
trigger1 = AnimTime = 0
value = 94011
ctrl = 1

[Statedef 94011]
type    = S
movetype= A
juggle  = 1
anim = 0
velset = 0, 0
ctrl = 0

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 94011, Spikes]
type = PlaySnd
trigger1 = Time = 5
trigger2 = Time = 9
value = 20,30

[State 94011, Spikey]
type = Explod
trigger1 = Time = 5
anim = 9400
pos = 3*const(size.xscale), 0*const(size.xscale)
Postype = Left
bindtime = 1
sprpriority = -1
ownpal = 1

[State 94011, Spikey2]
type = Explod
trigger1 = Time = 5
anim = 9401
pos = 3*const(size.xscale), 0*const(size.xscale)
Postype = Left
bindtime = 1
sprpriority = 1
ownpal = 1

[State 94011, 5]
type = Helper
trigger1 = animelem = 1
stateno = 9403
name = "Puncture"
id = 9101
pos = 3*const(size.xscale), -205*const(size.xscale)
Postype = Left
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 94011, StartAnim]
type = Changeanim
trigger1 = time = 0
value = 430  

[State 94011, PitHit]
type = HitDef
trigger1 = time > 1
attr = S, HA
damage    = 50,5
hitsound = S20,32
priority  = 7
sparkno = -1
guard.sparkno = -1
kill = 0
p1stateno = 431 
p2stateno = 9405
id = 6040

[State  94011, Var]
type = Varset
trigger1 = time = 1
V = 4
value = 1

[State 94011, stand]
type = Changestate
trigger1 = AnimTime = 0
value = 0
ctrl = 1
-----------------------------------------
; Player 1, You do the PIT
[Statedef 9402]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 9402, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9402, 0.3]
type = SprPriority
trigger1 = time = 0
value = 2

;----
[State 9402, 3.5]
type = Helper
trigger1 = Time = 0
stateno = 6040
name = "UPPERCUT"
id = 6040
pos = 57*const(size.xscale),-74*const(size.xscale)
postype = P1
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1
;--------

[State 9402, SetPit]
type = Varset
trigger1 = Time = 4
V = 4
value = 2

[State 9402, 2]
type = ChangeState
trigger1 = Time = 200
value = 0
ctrl = 1

;--------------------------------------
; With sharp spiky things!
[Statedef 9403]
type    = S
movetype= A
juggle  = 1
anim = 9403
velset = 0, 0
ctrl = 0

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9403, ca-chunk]
type = HitDef
trigger1 = Time > 0
attr = S, HA
;Nothing = 0
numhits = 0
priority  = 7
sparkno = -1
guard.sparkno = -1
p2stateno = 9406
kill = 0

[State 9403, StopTarget]
type = TargetVelSet
trigger1 = MoveHit = 1
X = 0
Y = 0

[State 9403, FindTarget]
type = Helper
trigger1 = MoveHit = 1
stateno = 9404
name = "Seive"
id = 9101
pos = 0, 0
Postype = p2
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9403, bye]
type = DestroySelf
trigger1 = MoveHit = 1


;--------------------------------------
; Hit the sharp spiky things!
[Statedef 9404]
type    = S
movetype= A
juggle  = 1
anim = 9404
velset = 0, 0
ctrl = 0

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9404, ca-chunk]
type = PlaySnd
trigger1 = Time = 0
value = 1,3

[State 9404, groan]
type = PlaySnd
trigger1 = Time = 0
value = 3,7

[State 9404, 1.555]
type = Explod
trigger1 = Time = 0
anim = 6041
pos = 0, 0
Postype = P1
bindtime = 1
sprpriority = 3

[State 9404, 3.5]
type = Helper
trigger1 = Time = 0
stateno = 6040
name = "Spiked"
id = 6040
pos = 0,0
postype = P1
helpertype = normal
sprpriority = 2
keyctrl = 0
ownpal = 0
under = 1

[State 9404, Select]
type = VarRandom
trigger1 = TimeMod = 45, 43
v = 22
Range = -20, 20

[State 9404, Left]
type = Helper
trigger1 = TimeMod = 45, 44
trigger1 = Var(22) <= 0
stateno = 6000
name = "Blood Left"
pos = Var(22), 0
postype = P1
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9404, Right]
type = Helper
trigger1 = TimeMod = 45, 44
trigger1 = Var(22) > 0
stateno = 6002
name = "Blood Right"
pos = Var(22), 0
postype = P1
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[Statedef 9405]
type    = A
movetype= H
physics = N
juggle  = 1
anim = 0
velset = 0, 0
ctrl = 0

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9405, bindit]
type = ScreenBound
trigger1 = 1
value = 0
MoveCamera= 0, 0

[State 9405, 0.3]
type = SprPriority
trigger1 = time = 0
value = 0

[State 9405, DieDieDie]
type = ChangeAnim2
trigger1 = Time = 0
value = 9405

[State 9405, Fly]
type = VelSet
trigger1 = Time = 0
Y = -10

[State 9405, 2]
type = Selfstate
trigger1 = time = 105
value = 5150

[Statedef 8910]
type = S
movetype= A
physics = N
ctrl = 0
anim = 8910

[State 9401, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 8910, NotFriendly]
type = VarSet
trigger1 = Time = 0
v = 5
value = 1

[State 8910, 0]
type = SprPriority
trigger1 = Time = 0
value = -1

[State 8910, Rise1]
type = TargetBind
trigger1 = time = 1 
pos = 37*const(size.xscale), -59*const(size.xscale)
bindtime = 1

[State 8910, Rise2]
type = TargetBind
trigger1 = AnimElem = 2
pos = 37*const(size.xscale), -79*const(size.xscale)
bindtime = 1

[State 8910, Lift]
type = TargetBind
trigger1 = AnimElem = 3
pos = 34*const(size.xscale), -89*const(size.xscale)
bindtime = 1

[State 8910, Heave]
type = TargetBind
trigger1 = AnimElem = 4
pos = 26*const(size.xscale), -102*const(size.xscale)
bindtime = 1

[State 8910, Over]
type = TargetBind
trigger1 = AnimElem = 5
pos = 19*const(size.xscale), -117*const(size.xscale)
bindtime = 1

[State 8910, HUU]
type = PlaySnd
trigger1 = AnimElem = 7
value = 4, 12

[State 8910, Pressed]
type = TargetBind
trigger1 = AnimElem = 6
pos = 4*const(size.xscale), -112*const(size.xscale)
bindtime = 1

[State 8910, Left]
type = Helper
trigger1 = AnimElemtime(5) = 30 
stateno = 6000
name = "Blood Left"
pos = -50*const(size.xscale), 50*const(size.xscale)
postype = P2
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 8910, Right]
type = Helper
trigger1 = AnimElemtime(5) = 20 
stateno = 6002
name = "Blood Right"
pos = 50*const(size.xscale), 50*const(size.xscale)
postype = P2
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 8910, Pressed]
type = explod
trigger1 = AnimElemtime(5) >= 0 && AnimElemtime(5) < 20
anim = 1230
postype = p2
pos = -50*const(size.xscale), 50*const(size.xscale)
bindtime = -11
facing = random-500
vfacing = random-500
random = random / 50,random / 100
scale = random/1000.0,random/1000.0
ontop = 1
vel = 2*random/1000.0,2*random/1000.0
accel = 2*random/1000.0,2*random/1000.0
removetime = -2

[State 8910, Toss] ; <----- sends player 2 into state 921, without using a hitdef
type = TargetState
trigger1 = AnimElem = 7, = 0
value = 8921
ctrl = 0

[State 8910, kill P2]
type = HitDef
trigger1 = time = 78
attr = S, HA
hitflag = HLAFD
damage    = 500,5
priority  = 7
kill       = 1
fall.kill  = 1
guard.kill = 1

[State 8910, 2]
type = Changestate
trigger1 = AnimTime = 0
value = 0
ctrl = 1

;-----------------------------------
;THROWEE
[Statedef 920]
type = S
movetype = H
physics = N
ctrl = 0

[State 920, bindit]
type = ScreenBound
trigger1 = 1
value = 0
MoveCamera= 0, 0

[State 920, 0]
type = SprPriority
trigger1 = Time = 0
value = 1

[State 920, 1]
type = Changeanim2
trigger1 = time = 0
value = 920

[State 920, Just in case]
type = Selfstate
trigger1 = Time >= 60
value = 5100
ctrl = 0
;-----------------------------------
;THROWEE
[Statedef 921]
type = S
movetype = H
physics = N
velset = 15, 15
ctrl = 0

[State 921, bindit]
type = ScreenBound
trigger1 = 1
value = 0
MoveCamera= 0, 0

[State 921, 0]
type = SprPriority
trigger1 = Time = 0
value = 1

[State 921, .5]
type = Turn
trigger1 = time = 0

[State 921, .5]
type = Changeanim2
trigger1 = time = 0
value = 921

[State 921, 4]
type = Selfstate
trigger1 = Pos Y >= 0
value = 5100
ctrl = 0
;--------------------
[Statedef 9406]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 0
ctrl = 0

[State 9406, DieDieDie]
type = ChangeAnim2
trigger1 = Time = 0
value = 9406

[State 9406, bindit]
type = ScreenBound
trigger1 = 1
value = 0
MoveCamera= 0, 0

[State 9406, Clone]
type = Explod
trigger1 = time = 0
anim = 5110
pos = 0*const(size.xscale),0*const(size.xscale)
bindtime = 1
removetime = -1
sprpriority = 0
shadow = -1

[State 9406, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow

[State 9406, 3]
type = Lifeadd
trigger1 = time = 10
Value = -9999

[State 9406, 2.6]
type = Posset
trigger1 = time = 10
Y = 900

[State 9406, 2]
type = Selfstate
trigger1 = time = 23
value = 5150

;--------------------
[Statedef 9415]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 0
ctrl = 0

[State 9415, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9415, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9415, 0.3]
type = SprPriority
trigger1 = time = 0
value = -3

[State 9415, Anim]
type = ChangeAnim2
trigger1 = time = 0
value = 9415

[State 9415, Anim]
type = VelSet
trigger1 = time = 0
X = -0.4
Y = -10

[State 9415, Gravity]
type = VelAdd
trigger1 = time >= 0
Y = .4

[State 9415, drop]
type = changestate
trigger1 = Vel Y > 0
value = 9416

;--------------------
[Statedef 9416]
type    = A
movetype= H
physics = N
juggle  = 1
ctrl = 0

[State 9416, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9416, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9416, Anim]
type = ChangeAnim2
trigger1 = time = 0
value = 9416

[State 9416, Gravity]
type = VelAdd
trigger1 = time >= 0
Y = .4

[State 9416, 1.5]
type = AssertSpecial
trigger1 = Pos Y > 0
flag = noshadow

[State 9416, 1.5]
type = EnvColor
trigger1 = Pos Y > 300
value = 0,0,0
time = 1455

[State 9416, drop]
type = changestate
trigger1 = Pos Y > 600
value = 9417

;--------------------
[Statedef 9417]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 0
ctrl = 0

[State 9417, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9417, 2.6]
type = Posset
trigger1 = time = 0
Y = 900

[State 9417, Fall]
type = Explod
trigger1 = TimeMod = 2,0
trigger1 = Time < 20
anim = 5110
pos = 160*const(size.xscale),150*const(size.xscale)
postype = Left
bindtime = 1
removetime = 2
sprpriority = 0
ontop = 1
shadow = -1
Scale = (((20 - Time)/3) + 1), (((20 - Time)/3) + 1)

[State 9417, Splat]
type = Explod
trigger1 = TimeMod = 2,0
trigger1 = Time = 20
anim = 5110
pos = 160*const(size.xscale),150*const(size.xscale)
postype = Left
bindtime = 1
removetime = -1
sprpriority = 0
ontop = 1
shadow = -1

[State 9417, 3]
type = Lifeadd
trigger1 = time = 20
Value = -9999

[State 9417, 2]
type = Selfstate
trigger1 = time = 20
value = 5150


;-----------------------------------------
; Player 1, You do the PIT
[Statedef 9421]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 9421, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9421, 0.3]
type = SprPriority
trigger1 = time = 0
value = -2

[State 9421, 5]
type = Helper
trigger1 = Time = 70
stateno = 9422
name = "Splat"
id = 9421
pos = 0*const(size.xscale),0*const(size.xscale)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9421, SetPit]
type = Varset
trigger1 = Time = 4
V = 4
value = 2

[State 9421, 2]
type = ChangeState
trigger1 = Time = 200
value = 0
ctrl = 1

;-----------------------------------------
; Player 1, You do the PIT
[Statedef 9422]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9420
ctrl = 0

[State 9422, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9422, PitHit]
type = HitDef
trigger1 = time > 1
attr = S, HA
damage    = 50,5
hitsound = S4,21
priority  = 7
sparkno = -1
guard.sparkno = -1
kill = 0
p2stateno = 9428

[State 9422, 0.3]
type = EnvColor
trigger1 = MoveHit = 1
value = 255, 255, 255
Time = 7

[State 9422, 1.555]
type = Helper
trigger1 = MoveHit = 1
stateno = 9201
name = "LotsaMeat"
postype = p2
pos = 0*const(size.xscale), 6*const(size.xscale)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9422, 3.5]
type = Helper
trigger1 = MoveHit = 1
stateno = 6040
name = "Crunched"
id = 6040
pos = 0*const(size.xscale),0*const(size.xscale)
postype = P2
helpertype = normal
sprpriority = 2
keyctrl = 0
ownpal = 0
under = 1

[State 9422, SetPit]
type = Varset
trigger1 = Time = 4
V = 4
value = 2

[State 9422, 3.5]
type = DestroySelf
trigger1 = MoveHit = 1

[State 9422, 2]
type = Selfstate
trigger1 = Time > 300
value = 5150

;--------------------
[Statedef 9425]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 0
ctrl = 0

[State 9425, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9425, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9425, 0.3]
type = SprPriority
trigger1 = time = 0
value = -3

[State 9425, Anim]
type = ChangeAnim2
trigger1 = time = 0
value = 9415

[State 9425, Anim]
type = VelSet
trigger1 = time = 0
X = -0.4
Y = -10

[State 9425, Gravity]
type = VelAdd
trigger1 = time >= 0
Y = .4

[State 9425, drop]
type = changestate
trigger1 = Vel Y > 0
value = 9426

;--------------------
[Statedef 9426]
type    = A
movetype= H
physics = N
juggle  = 1
ctrl = 0

[State 9426, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9426, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9426, Anim]
type = ChangeAnim2
trigger1 = time = 0
value = 9416

[State 9426, Gravity]
type = VelAdd
trigger1 = time >= 0
Y = .4

[State 9426, 1.5]
type = AssertSpecial
trigger1 = Pos Y > 0
flag = noshadow

[State 9426, drop]
type = changestate
trigger1 = Pos Y > 600
value = 9427

;--------------------
[Statedef 9427]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 10
ctrl = 0

[State 9427, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9427, nosha]
type = AssertSpecial
trigger1 = Time >= 0
flag = noshadow

[State 9427, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9427, 2.6]
type = Posset
trigger1 = time = 5
Y = -900

[State 9427, 3]
type = Lifeadd
trigger1 = Time > 200
Value = -9999

[State 9427, 2]
type = Selfstate
trigger1 = Time > 200
value = 5150

;--------------------
[Statedef 9428]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 10
ctrl = 0

[State 9428, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9428, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow
flag2 = noshadow

[State 9428, 2.6]
type = Posset
trigger1 = time = 5
Y = 600

[State 9428, 3]
type = Lifeadd
trigger1 = time = 5
Value = -9999

[State 9428, 2]
type = Selfstate
trigger1 = time = 5
value = 5150

;-----------------------------------------
; Player 1, You do the PIT
[Statedef 9431]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 9431, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9431, 5]
type = Helper
trigger1 = Time = 0
trigger1 = Var(11) < 80
stateno = 9432
name = "Puddle"
id = 9421
pos = -67*const(size.xscale),0*const(size.xscale)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 1
under = 1

[State 9431, 5]
type = Helper
trigger1 = Time = 0 
trigger1 = Var(11) >= 80
stateno = 9432
name = "Puddle"
id = 9421
pos = 97*const(size.xscale),0*const(size.xscale)
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 1
under = 1

[State 9431, SetPit]
type = Varset
trigger1 = Time = 4
V = 4
value = 2

[State 9431, ClearTemp]
type = Varset
trigger1 = Time = 75
V = 11
value = 0

[State 9431, 2]
type = ChangeState
trigger1 = Time = 200
value = 0
ctrl = 1

;--------------------
[Statedef 9435]
type    = A
movetype= H
physics = N
juggle  = 1
velset = 0, 0
ctrl = 0

[State 9435, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9425, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 9435, NoSlow]
type = AssertSpecial
trigger1 = Time >= 0
flag = nokoslow
flag2 = noshadow

[State 9435, 2.6]
type = Posset
trigger1 = time = 0
Y = 900

[State 9435, 3]
type = Lifeadd
trigger1 = time = 0
Value = -9999

[State 9435, 2]
type = Selfstate
trigger1 = time = 0
value = 5150


[Statedef 9440]
type    = S
movetype= A
juggle  = 1
anim = 0
velset = 0, 0
ctrl = 0

[State 9440, StartAnim]
type = Changeanim
trigger1 = time = 0
value = 9200

[State 9440, PitHitCorner]
type = HitDef
trigger1 = time > 1
trigger1 = FrontEdgeBodyDist < 80
attr = S, HA
hitsound = S6,3
priority  = 7
sparkno = -1
guard.sparkno = -1
animtype  = up
ground.velocity = 0.7, -60
air.velocity = 2,-10
down.hittime = 30
fall = 1
fall.recover = 0
fall.damage = 9999
p1stateno = 9441

[State 9440, PitHit]
type = HitDef
trigger1 = time > 1
trigger1 = FrontEdgeBodyDist >= 80
attr = S, HA
hitsound = S6,3
priority  = 7
sparkno = -1
guard.sparkno = -1
animtype  = up
ground.velocity = -0.6, -60
air.velocity = -2,-10
down.hittime = 30
fall = 1
fall.recover = 0
fall.damage = 9999
p1stateno = 9441

[State 9440, stand]
type = Changestate
trigger1 = AnimTime = 0
value = 0
ctrl = 1


[Statedef 9441]
type    = S
movetype= A
physics = N
velset = 0,0
anim = 9210
ctrl = 0

[State 9441, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 9441, 3.5]
type = Helper
trigger1 = Time = 0
stateno = 6040
name = "UPPERCUT"
id = 6040
pos = 57*const(size.xscale),-74*const(size.xscale)
postype = P1
helpertype = normal
sprpriority = -1
keyctrl = 0
ownpal = 0
under = 1

[State 9441, SetPit]
type = Varset
trigger1 = Time = 4
V = 4
value = 2

[State 9441, 2]
type = ChangeState
trigger1 = Time = 150
value = 0
ctrl = 1

;------------------------------------------------------------------------
;What the????
[Statedef 8110]
type    = S
movetype= H
physics = N
velset = 0,0
anim = 0
ctrl = 0

[State 8110, move]
type = posadd
trigger1 = time = 2
trigger1 = BackEdgeBodyDist <= 40
x = 29


[Statedef 8111]
type    = S
movetype= H
physics = N
velset = 0,0
anim = 41
ctrl = 0

[State 8111, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 8111, NoNuthin]
type = AssertSpecial
trigger1 = 1
flag = nokoslow
flag2 = nokosnd

[State 8111, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 8111, 3]
type = Lifeadd
trigger1 = time = 0
Value = -9999

[State 8111, 3]
type = VelSet
trigger1 = time = 0
Y = -10

[State 8111, 3]
type = ChangeState
trigger1 = time = 60
value = 8112

[Statedef 8112]
type    = S
movetype= H
physics = N
anim = 44
ctrl = 0

[State 8112, bindit]
type = ScreenBound
trigger1 = 1
value = 1
MoveCamera= 0, 0

[State 8112, turn]
type = Turn
trigger1 = time = 0

[State 8112, move]
type = posset
trigger1 = time = 0
trigger1 = FrontEdgeDist >= 50
x = -50

[State 8112, move2]
type = posset
trigger1 = time = 0
trigger1 = FrontEdgeDist < 50
x = 50

[State 8112, gravity]
type = VelAdd
trigger1 = Vel Y < 10
Y = .4

[State 8112, 3]
type = ChangeState
trigger1 = Pos Y >= 0
value = 8113


[Statedef 8113]
type    = S
movetype= H
physics = N
velset = 0,0
ctrl = 0

[State 8113, AssertSpecial1]
type = AssertSpecial
trigger1 = Time >= 0  
flag = NoBarDisplay

[State 8113, move2]
type = posset
trigger1 = time = 0
Y = 0

[State 8113, 2]
type = changeanim
trigger1 = time = 0
value = 170
ctrl = 0

[State 8113, Defeat]
type = Explod
trigger1 = time = 0
trigger1 = AnimExist(170)
anim = 170
pos = 0*const(size.xscale),0*const(size.xscale)
postype = p1
bindtime = 1
removetime = -1
sprpriority = 3

[State 8113, Stun]
type = Explod
trigger1 = time = 0
trigger1 = !animexist(170)
trigger1 = animexist(5300)
anim = 5300
pos = 0*const(size.xscale),0*const(size.xscale)
postype = p1
bindtime = 1
removetime = -1
sprpriority = 3

[State 8113, 1.555]
type = Explod
trigger1 = time = 0
trigger1 = !animexist(170)
trigger1 = !animexist(5300)
anim = 0
pos = 0*const(size.xscale),0*const(size.xscale)
postype = p1
bindtime = 1
removetime = -1
sprpriority = 3

[State 8113, 2.6]
type = Posset
trigger1 = time = 3
Y = 900

[State 8113, 2]
type = Selfstate
trigger1 = time = 3
value = 5150

;p2's state
[Statedef 9104]
type = S
movetype= H
physics = S
velset = 0,0

[State 9104, 0.1]
type = Changeanim
trigger1 = (anim != 5300) && ( animexist(5300))
value = 5300

[State 9104, 1]
type = Changeanim2
trigger1 = (!animexist(5300)) && (anim != 9010)
value = 9010


[Statedef 9106]
type    = S
movetype= H
physics = S
velset = 0,0

[State 9106, Love Hurts but Fatalities hurt worse]
type = LifeAdd
trigger1 = 1
value = -9999

[State 9106, Go Bye Bye]
type = SelfState
trigger1 = 1
value = 5150


;p2's state
[Statedef 9104]
type = S
movetype= H
physics = S
velset = 0,0

[State 9104, 0.1]
type = Changeanim
trigger1 = (anim != 5300) && ( animexist(5300))
value = 5300

[State 9104, 1]
type = Changeanim2
trigger1 = (!animexist(5300)) && (anim != 9010)
value = 9010

;----------------------------
[Statedef 9106]
type    = S
movetype= H
physics = S
velset = 0,0

[State 9106, Love Hurts but Fatalities hurt worse]
type = LifeAdd
trigger1 = 1
value = -9999

[State 9106, Go Bye Bye]
type = SelfState
trigger1 = 1
value = 5150