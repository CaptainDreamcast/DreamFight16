[Command]
name = "Hooch"
command = a+x

[Command]
name = "Hooch"
command = b+y

[Command]
name = "Hooch"
command = c+z

[Command]
name = "FF"     
command = F, F
time = 10

[Command]
name = "BB"     
command = B, B
time = 10

[Command]
name = "recovery"
command = x+y
time = 1

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
name = "start"
command = s
time = 1

[Command]
name = "holdfwd"
command = /$F
time = 1

[Command]
name = "holdback"
command = /$B
time = 1

[Command]
name = "holdup" 
command = /$U
time = 1

[Command]
name = "holddown"
command = /$D
time = 1

[Statedef -1]

[State -1, Laugh]
type = ChangeState
value = 195
triggerall = command = "start"
trigger1 = statetype != A
trigger1 = ctrl

[State -1, Hooch]
type = ChangeState
value = 1000
triggerall = command = "Hooch"
triggerall = power >= 3000
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Run Fwd]
type = ChangeState
value = 100
trigger1 = command = "FF"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Run Back]
type = ChangeState
value = 105
trigger1 = command = "BB"
trigger1 = statetype = S
trigger1 = ctrl

[State -1, Big]
type = ChangeState
value = 200
triggerall = command = "x"
triggerall = NumHelper(201) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, Big Air]
type = ChangeState
value = 203
triggerall = command = "x"
triggerall = NumHelper(201) < 1
trigger1 = statetype = A
trigger1 = ctrl

[State -1, The Green Mile]
type = ChangeState
value = 205
triggerall = command = "y"
triggerall = NumHelper(206) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, The Green Mile Air]
type = ChangeState
value = 207
triggerall = command = "y"
triggerall = NumHelper(206) < 1
trigger1 = statetype = A
trigger1 = ctrl

[State -1, Forest Gump]
type = ChangeState
value = 210
triggerall = command = "z"
triggerall = NumHelper(211) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, Forest Gump Air]
type = ChangeState
value = 212
triggerall = command = "z"
triggerall = NumHelper(211) < 1
trigger1 = statetype = A
trigger1 = ctrl

[State -1, Sleepless in Seattle]
type = ChangeState
value = 215
triggerall = command = "a"
triggerall = NumHelper(216) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, Sleepless in Seattle Air]
type = ChangeState
value = 217
triggerall = command = "a"
triggerall = NumHelper(216) < 1
trigger1 = statetype = A
trigger1 = ctrl

[State -1, Cast Away]
type = ChangeState
value = 220
triggerall = command = "b"
triggerall = NumHelper(221) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, Cast Away Air]
type = ChangeState
value = 222
triggerall = command = "b"
triggerall = NumHelper(221) < 1
trigger1 = statetype = A
trigger1 = ctrl

[State -1, Saving Private Ryan]
type = ChangeState
value = 225
triggerall = command = "c"
triggerall = NumHelper(226) < 1
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = statetype = C
trigger2 = ctrl

[State -1, Saving Private Ryan Air]
type = ChangeState
value = 227
triggerall = command = "c"
triggerall = NumHelper(226) < 1
trigger1 = statetype = A
trigger1 = ctrl



