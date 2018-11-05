open util/ordering[DATE] as D
open util/ordering[TIME] as T

/* ------------------- SETS  ------------------- */

abstract sig STATE {}
one sig INITIALSTATE extends STATE {}
one sig PAYEDSTATE extends STATE {}
one sig CONFIRMEDSTATE extends STATE {}

abstract sig ROOMTYPE {}
one sig SINGLE extends ROOMTYPE {}
one sig DOUBLE extends ROOMTYPE {}

abstract sig INVOICETYPE {}
one sig BUSINESS extends INVOICETYPE {}
one sig LEISURE extends INVOICETYPE{}

sig TIME {}

sig DATE {}

sig CLIENT {

	clientAccounts: ACCOUNT set -> TIME
}

sig BROKER in CLIENT {

	roomReservations: ROOMRESERVATION set -> TIME,
	activityReservations: ACTIVITYRESERVATION set -> TIME,
	adventures: ADVENTURE set -> TIME
}

sig BANK {
	
	bankAccounts: set ACCOUNT
}{
	all a: bankAccounts | a.bank = this
}

sig ACCOUNT {

	bank: one BANK,
	client: one CLIENT,
	balance: Int one ->  TIME
}{
	this in bank.bankAccounts
}

sig HOTEL {
	
	rooms: some ROOM
}{
	all r: rooms | r.hotel = this
}

sig ROOM {
	
	hotel: one HOTEL,
	roomType: one ROOMTYPE	
}{
	this in hotel.rooms
}

sig ROOMRESERVATION {
	
	room: one ROOM,
	client: one CLIENT,
	arrival: one DATE,
	departure: one DATE,
}

sig ACTIVITYPROVIDER {
	
	activityOffers: ACTIVITYOFFER set ->  TIME
}

sig ACTIVITY {
	
	activityProvider: one ACTIVITYPROVIDER,
	capacity: one Int,
}{
	capacity > 0
}

sig ACTIVITYOFFER {

	activity: one ACTIVITY,
	begin: one DATE,
	end: one DATE,
	availability: Int one -> TIME
}

sig ACTIVITYRESERVATION {
	
	activityOffer: one ACTIVITYOFFER,
	client: one CLIENT,
	participants: one Int
}

sig ADVENTURE {
	
	payer: one CLIENT,
	participants: one Int,
	broker: one BROKER,
	roomReservations: set ROOMRESERVATION,
	activityReservation: one ACTIVITYRESERVATION,
	cost: one Int,
	payerAccount: one ACCOUNT,
	brokerAccount: one ACCOUNT,
	state: STATE one -> TIME
}

sig INVOICE {

	client: one CLIENT,
	invoiceType: one INVOICETYPE,
	amount: one Int,
	tax: one Int,
	registered: one TIME
}

one sig IRS {}

/* ------------- FACTS ---------------------*/


/* ------------- PREDICATES ------------- */

pred openAccount [t, t' : TIME, account: ACCOUNT, cli: CLIENT, bk: BANK] {

	// pre-conditions
	account not in CLIENT.clientAccounts.t
	account.bank = bk
	account in bk.bankAccounts
	account.client = cli

	//post-conditions
	clientAccounts.t' = clientAccounts.t + (cli -> account)
	account.balance.t' = 0

	//frame-conditions
	noBalanceChangeExcept[t, t', account]
}

pred clientDeposit [t, t' : TIME, a: ACCOUNT, amount: Int] {
	
	//pre-conditions
	a in CLIENT.clientAccounts.t
	
	//post-conditions
	deposit[t, t', a, amount]
	a.balance.t' >= 0

	//frame-conditions
	noBalanceChangeExcept[t, t', a]
	noOpenAccountsChange[t, t']
	
}

pred makeActivityOffer [t, t': TIME, offer: ACTIVITYOFFER, act: ACTIVITY, beg: DATE, en: DATE, avail: Int] {
	
	//pre-conditions
	offer not in ACTIVITYPROVIDER.activityOffers.t
	avail > 0
	avail <= act.capacity
	beg in prevs[en]	
	
	//post-conditions
	offer.activity = act
	offer.begin = beg
	offer.end = en

	offer.availability.t' = avail

	one activityProvider: ACTIVITYPROVIDER | activityProvider.activityOffers.t' = activityProvider.activityOffers.t + offer

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
}


pred createAdventure [t, t': TIME, adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReserv: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT] {
 
	//pre-conditions
	fromAccount in cli.clientAccounts.t
	fromAccount.client = cli
	toAccount in bro.clientAccounts.t
	toAccount.client = bro
	adv not in BROKER.adventures.t
	num > 0
	amount > 0

	//post-conditions
	consistentAdventure[adv, cli, num, bro, actReserv, roomReserv, amount, fromAccount, toAccount]
	adv.state.t' = INITIALSTATE

	bro.adventures.t' = bro.adventures.t + adv

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
} 


pred payAdventure [t, t': TIME, adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReservs: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT, invoice: INVOICE] {

	//pre-conditions
	adv in bro.adventures.t
	adv.state.t = INITIALSTATE
	consistentAdventure[adv, cli, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount]
	//DO TAX STUFF

	//post-conditions
	reserveActivity[t, t', actReserv, actReserv.activityOffer, cli, num]
	one arr, dep: DATE | reserveRooms[t, t', roomReservs, roomReservs.room, cli, arr, dep] 
	adv.state.t' = PAYEDSTATE

	//frame-conditions
	let accounts = fromAccount + toAccount | noBalanceChangeExcept[t, t', accounts]
	noOpenAccountsChange[t, t']
}

pred cancelAdventure [t, t': TIME, adv: ADVENTURE, inv: INVOICE] {

	//pre-conditions
	adv in adv.broker.adventures.t
	adv.state.t = PAYEDSTATE
	inv in registered.t

	//post-conditions
	adv.state.t' = INITIALSTATE
	cancelInvoice[t, t', inv]
	clientDeposit[t, t', adv.brokerAccount, sub[0, inv.amount]]
	clientDeposit[t, t', adv.payerAccount, add[inv.amount, mul[inv.amount, inv.tax]]]
	cancelRoomReservations[t, t', adv.roomReservations]
	cancelActivityReservation[t, t', adv.activityReservation]
	BROKER.adventures.t' = BROKER.adventures.t - adv

	//frame-conditions
	let accounts = adv.brokerAccount + adv.payerAccount | noBalanceChangeExcept[t, t', accounts]
	noOpenAccountsChange[t, t']
}

pred confirmAdventure[t, t': TIME, adv: ADVENTURE] {
	
	//pre-conditions
	one broker: BROKER | adv in broker.adventures.t

	//post-conditions
	adv.state.t' = CONFIRMEDSTATE

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
}

/*-------AUX PREDICATES-----*/
pred deposit [t, t' : TIME, a: ACCOUNT, amount: Int] {	

	//post-condition
	a.balance.t' = plus[a.balance.t, amount]
}


pred reserveActivity [t, t': TIME, reserv: ACTIVITYRESERVATION, offer: ACTIVITYOFFER, cli: CLIENT, particip: Int] {

	//pre-conditions
	reserv not in BROKER.activityReservations.t
	offer in ACTIVITYPROVIDER.activityOffers.t
	particip > 0
	particip <= offer.availability.t
	
	//post-conditions
	offer.availability.t' = minus[offer.availability.t, particip]
	reserv.activityOffer = offer
	reserv.client = cli
	reserv.participants = particip
	
	one broker: BROKER | broker.activityReservations.t' = broker.activityReservations.t + reserv
}


pred cancelActivityReservation [t, t': TIME, reserv: ACTIVITYRESERVATION] {

	//pre-conditions
	one broker: BROKER | reserv in broker.activityReservations.t
	
	//post-conditions
	reserv.activityOffer.availability.t' = plus[reserv.activityOffer.availability.t, reserv.participants]
	BROKER.activityReservations.t' = BROKER.activityReservations.t - reserv
}


pred reserveRooms [t, t': TIME, reservs: set ROOMRESERVATION, rooms: set ROOM, cli: CLIENT, arr: DATE, dep: DATE] {

	//pre-conditions
	#reservs = #rooms
	no reserv: reservs | reserv in BROKER.roomReservations.t
	arr in prevs[dep]
	all ro: rooms | no reserv : BROKER.roomReservations.t |
					reserv.room = ro and
					overlaps[getDateInterval[arr, dep], getDateInterval[reserv.arrival, reserv.departure]]
	
	
	//post-conditions
	all reserv: reservs, ro: rooms {  
		reserv.room = ro
		reserv.client = cli
		reserv.arrival = arr
		reserv.departure = dep
	}

	BROKER.roomReservations.t' = BROKER.roomReservations.t + reservs
}


pred cancelRoomReservations [t, t': TIME, reservs: set ROOMRESERVATION] {

	//pre-conditions
	all reserv: reservs | reserv in BROKER.roomReservations.t
	
	//post-conditions
	BROKER.roomReservations.t' = BROKER.roomReservations.t - reservs
}

pred makeInvoice [t, t': TIME, inv: INVOICE, cli: CLIENT, tp: INVOICETYPE, price: Int, tx: Int] {
	
	//pre-conditions
	inv not in registered.t

	//post-conditions 
	inv.client = cli
	inv.invoiceType = tp
	inv.amount = price
	inv.tax = tx
	registered.t' = registered.t + inv
}

pred cancelInvoice[t, t': TIME, inv: INVOICE] {

	//pre-conditions
	inv in registered.t

	//post_conditions
	registered.t' = registered.t - inv
}

pred consistentAdventure[adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReservs: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT] {

	adv.payer = cli
	adv.participants = num
	adv.broker = bro
	adv.activityReservation = actReserv
	adv.roomReservations = roomReservs
	adv.cost = amount
	adv.payerAccount = fromAccount
	adv.brokerAccount = toAccount
	adv.payer = actReserv.client
	all reserv : roomReservs | adv.payer = reserv.client
	adv.participants = actReserv.participants
	adv.participants = add[#(roomReservs.room :> roomType.SINGLE), mul[#(roomReservs.room :> roomType.DOUBLE), 2]]	
}

pred overlaps [interval1, interval2: set DATE] {

	some interval1 & interval2
}

pred noBalanceChangeExcept[t, t': TIME, account: set ACCOUNT] {

	all acc: ACCOUNT - account | acc.balance.t' = acc.balance.t
}

pred noOpenAccountsChange[t, t': TIME] {

	clientAccounts.t' = clientAccounts.t 
	
}

/*---------- FUNCTIONS ------------------*/
//fun calculateTax[price: Int, type: INVOICETYPE]

fun getDateInterval[date1, date2: DATE] : set DATE { 

	date1 + (nexts[date1] & prevs[date2]) + date2
}



/*------ ASSERTIONS ----------*/
assert A1 { //1	

	all a: ACCOUNT | some b: BANK, c: CLIENT {
		
		one t: TIME - T/last | openAccount[t, t.next, a, c, b]
	}
}

assert A2 { //2

	all t: TIME | no a: ACCOUNT | some b: BANK, c: CLIENT {
		a in CLIENT.clientAccounts.t
		openAccount[t, t.next, a, c, b]
	}
}

assert A3 { //3
	
	all t: TIME | all a: CLIENT.clientAccounts.t {

		#(clientAccounts.t :> a) <= 1
	}
}

assert A4 { //4

	 all t: TIME {
								   //bankAccounts: bank -> account; bankAccounts.openAcc are the banks associated with an account openAcc
		all openAcc: CLIENT.clientAccounts.t | #(bankAccounts.openAcc) = 1 
	}
}

assert A7 { //7
	
	all t: TIME - T/last | no closedAcc: ACCOUNT - CLIENT.clientAccounts.t {
	
		clientDeposit[t, t.next, closedAcc, 1]
	}
}

assert A8 { //8
	
	all t: TIME | all openAcc: CLIENT.clientAccounts.t | openAcc.balance.t >= 0
}

assert A9 { //9

	all t: TIME - T/last | clientAccounts.t in clientAccounts.(t.next)
}

assert A11 { //11

	all room: ROOM | #(rooms.room) = 1 
}

assert A12 { //12

	all room: ROOM | room.roomType = SINGLE or room.roomType = DOUBLE
}

assert A13 { //13

	all t: TIME | all roomReservs: BROKER.roomReservations.t | roomReservs.arrival in prevs[roomReservs.departure]
}

assert A14 { //14

	all t: TIME | no reserv, reserv' : BROKER.roomReservations.t |
				reserv.room = reserv'.room and
				overlaps[getDateInterval[reserv'.arrival, reserv'.departure], getDateInterval[reserv.arrival, reserv.departure]]
}

assert A15 { //15

	all activity: ACTIVITY | activity.capacity > 0
}

assert A16 { //16
	
	all t: TIME, activityOffer: ACTIVITYPROVIDER.activityOffers.t | activityOffer.begin in prevs[activityOffer.end] 
}
/* ----------INITIALIZATION ------------- */
pred init [t: TIME] {

	no CLIENT.clientAccounts.t 
	no BROKER.roomReservations.t
	no ACTIVITYPROVIDER.activityOffers.t
	no BROKER.activityReservations.t
	no BROKER.adventures.t
}

/* ------- TRANSITION RELATION ------ */
pred trans [t, t': TIME] {

	 some a: ACCOUNT, b: BANK, c: CLIENT, amount: Int |
		openAccount[t, t', a, c, b] or 
		clientDeposit[t, t', a, amount]

	or


	some o: ACTIVITYOFFER, a: ACTIVITY, b: DATE, e: DATE |
		makeActivityOffer[t, t', o, a, b, e, 4]

	or


	some adv:ADVENTURE, cli: CLIENT, bro:BROKER, actReserv: ACTIVITYRESERVATION, roomReserv: ROOMRESERVATION, fromAccount: ACCOUNT, toAccount: ACCOUNT |
		createAdventure[t, t', adv, cli, 1,bro, actReserv, roomReserv, 3, fromAccount, toAccount] or 
		confirmAdventure[t, t', adv] 

	or
	
	some adv: ADVENTURE, inv: INVOICE|
		cancelAdventure[t, t', adv, inv]
}



/* ------- EXECUTION -------- */
fact {
	init [T/first]
	all t: TIME - T/last | trans [t, T/next[t]]
}

run {} //1
check A2 //2
check A3 //3 
check A4 //4
run {} for 3 but exactly 1 BANK, exactly 2 ACCOUNT // 5
run {} for 3 but exactly 1 CLIENT, exactly 2 ACCOUNT //6
check A7 // 7
check A8 //8
check A9 //9
run {} for 3 but exactly 1 HOTEL, exactly 2 ROOM //10
check A11 {} //11 
check A12 {} //12
check A13 {} //13
check A14 {} //14
check A15 {} //15
check A16 {} //16
