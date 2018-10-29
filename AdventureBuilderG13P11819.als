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

	accounts: ACCOUNT set -> TIME
}

sig BROKER in CLIENT {

	roomReservations: ROOMRESERVATION set -> TIME,
	activityReservations: ACTIVITYRESERVATION set -> TIME,
	adventures: ADVENTURE set -> TIME
}

sig BANK {
	
	accounts: set ACCOUNT
}{
	all a: accounts | a.bank = this
}

sig ACCOUNT {

	bank: one BANK,
	client: one CLIENT,
	balance: Int one ->  TIME
}{
	this in bank.accounts
}

sig HOTEL {
	
	rooms: some ROOM
}{
	all r: rooms | r.hotel = this
}

sig ROOM {
	
	hotel: one HOTEL,
	type: one ROOMTYPE	
}{
	this in hotel.rooms
}

sig ROOMRESERVATION {
	
	room: one ROOM,
	client: one CLIENT,
	arrival: one DATE,
	departure: one DATE,
}{
	departure in nexts[arrival]
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
}{
	end in nexts[begin]
}

sig ACTIVITYRESERVATION {
	
	activityOffer: one ACTIVITYOFFER,
	client: one CLIENT,
	participants: one Int
}{
	participants > 0
}

sig ADVENTURE {
	
	payer: one CLIENT,
	participants: one Int,
	broker: one BROKER,
	roomReservation: one ROOMRESERVATION,
	activityReservation: one ACTIVITYRESERVATION,
	cost: one Int,
	payerAccount: one ACCOUNT,
	brokerAccount: one ACCOUNT,
	state: STATE one -> TIME
}{
	cost > 0
	participants > 0
	some t: TIME | payerAccount in payer.accounts.t
	some t: TIME | brokerAccount in broker.accounts.t
	payerAccount.client = payer
	brokerAccount.client = broker
	payer = roomReservation.client and payer = activityReservation.client
}

sig INVOICE {

	client: one CLIENT,
	type: one INVOICETYPE,
	amount: one Int,
	tax: one Int,
	registered: one TIME
}

one sig IRS {}

/* ------------- FACTS ---------------------*/


/* ------------- PREDICATES ------------- */

pred openAccount [t, t' : TIME, account: ACCOUNT, cli: CLIENT, bk: BANK] {

	// pre-conditions
	account not in CLIENT.accounts.t
	account.bank = bk
	account in bk.accounts
	account.client = cli

	//post-conditions
	account in cli.accounts.t'
	account.client = cli
	account.balance.t' = 0

	//frame-conditions?
}

pred clientDeposit [t, t' : TIME, a: ACCOUNT, amount: Int] {
	
	//pre-conditions
	a in CLIENT.accounts.t
	
	//post-conditions
	deposit[t, t', a, amount]
}

pred makeActivityOffer [t, t': TIME, offer: ACTIVITYOFFER, act: ACTIVITY, beg: DATE, en: DATE, avail: Int] {
	
	//pre-conditions
	offer not in ACTIVITYPROVIDER.activityOffers.t
    
	avail <= act.capacity
	
	//post-conditions
	offer.activity = act
	offer.begin = beg
	offer.end = en

	offer.availability.t' = avail

	one activityProvider: ACTIVITYPROVIDER | offer in activityProvider.activityOffers.t'
}


pred createAdventure [t, t': TIME, adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReserv: ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT] {
 
	//pre-conditions
	//fromAccount in cli.accounts.t
	//fromAccount.client = cli
	//toAccount in bro.accounts.t
	//toAccount.client = bro
	adv not in BROKER.adventures.t

	//post-conditions
	adv.payer = cli
	adv.participants = num
	adv.broker = bro
	adv.activityReservation = actReserv
	adv.roomReservation = roomReserv
	adv.cost = amount
	adv.payerAccount = fromAccount
	adv.brokerAccount = toAccount
	adv.state.t' = INITIALSTATE

	adv in bro.adventures.t'
} 

pred confirmAdventure[t, t': TIME, adv: ADVENTURE] {
	
	//pre-conditions
	one broker: BROKER | adv in broker.adventures.t

	//post-conditions
	adv.state.t' = CONFIRMEDSTATE
		
}

/*-------AUX PREDICATES-----*/
pred deposit [t, t' : TIME, a: ACCOUNT, amount: Int] {
	
	a.balance.t' = plus[a.balance.t, amount]
}


pred reserveActivity [t, t': TIME, reserv: ACTIVITYRESERVATION, offer: ACTIVITYOFFER, cli: CLIENT, particip: Int] {

	//pre-conditions
	reserv not in BROKER.activityReservations.t
	offer in ACTIVITYPROVIDER.activityOffers.t
	particip <= offer.availability.t
	
	//post-conditions
	offer.availability.t' = minus[offer.availability.t, particip]
	reserv.activityOffer = offer
	reserv.client = cli
	reserv.participants = particip
	
	one broker: BROKER | reserv in broker.activityReservations.t'
}


pred cancelActivityReservation [t, t': TIME, reserv: ACTIVITYRESERVATION] {

	//pre-conditions
	one broker: BROKER | reserv in broker.activityReservations.t
	
	//post-conditions
	reserv.activityOffer.availability.t' = plus[reserv.activityOffer.availability.t, reserv.participants]
	reserv not in BROKER.activityReservations.t'
}


pred reserveRooms [t, t': TIME, reservs: set ROOMRESERVATION, rooms: set ROOM, cli: CLIENT, arr: DATE, dep: DATE] {

	//pre-conditions
	#reservs = #rooms
	no reserv: reservs | reserv in BROKER.roomReservations.t
	
	//post-conditions
	all reserv: reservs, ro: rooms {  
		reserv.room = ro
		reserv.client = cli
		reserv.arrival = arr
		reserv.departure = dep
	}

	all reserv: reservs | reserv in BROKER.roomReservations.t'
}


pred cancelRoomReservations [t, t': TIME, reservs: set ROOMRESERVATION] {

	//pre-conditions
	all reserv: reservs | reserv in BROKER.roomReservations.t
	
	//post-conditions
	no reserv: reservs | reserv in BROKER.roomReservations.t'
}

pred makeInvoice [t, t': TIME, inv: INVOICE, cli: CLIENT, tp: INVOICETYPE, price: Int, tx: Int] {
	
	//pre-conditions
	inv not in registered.t

	//post-conditions 
	inv.client = cli
	inv.type = tp
	inv.amount = price
	inv.tax = tx
	inv in registered.t'
}

pred cancelInvoice[t, t': TIME, inv: INVOICE] {

	//pre-conditions
	inv in registered.t

	//post_conditions
	inv not in registered.t'
}

/* ----------INITIALIZATION ------------- */
pred init [t: TIME] {

	no CLIENT.accounts.t 
	no BROKER.roomReservations.t
	no ACTIVITYPROVIDER.activityOffers.t
	no BROKER.activityReservations.t
}

/* ------- TRANSITION RELATION ------ */
pred trans [t, t': TIME] {

/*
	some a: ACCOUNT, b: BANK, c: CLIENT |
		openAccount[t, t', a, c, b] or 
		clientDeposit[t, t', a, 1]

	or
*/

/*
	some o: ACTIVITYOFFER, a: ACTIVITY, b: DATE, e: DATE |
		makeActivityOffer[t, t', o, a, b, e, 4]

	or
*/

/*
	some reservs: ROOMRESERVATION, rooms: ROOM,  c: CLIENT, a: DATE, d: DATE |
		  reserveRooms[t, t', reservs, rooms, c, a, d]
		 // or cancelRoomReservations [t, t', reservs]
	
	or


*/

/*
	some reservs: ACTIVITYRESERVATION, offers: ACTIVITYOFFER, c: CLIENT |
		reserveActivity[t, t', reservs, offers, c, 3] or
		cancelActivityReservation[t, t', reservs]
	
	or

*/
/*
	some inv: INVOICE, cli: CLIENT, tp: INVOICETYPE |
		makeInvoice[t, t', inv, cli, tp, 2, 1] or
		cancelInvoice [t, t', inv]

*/

/*
	some adv:ADVENTURE, cli: CLIENT, bro:BROKER, actReserv: ACTIVITYRESERVATION, roomReserv: ROOMRESERVATION, fromAccount: ACCOUNT, toAccount: ACCOUNT |
		createAdventure[t, t', adv, cli, 1,bro, actReserv, roomReserv, 3, fromAccount, toAccount] or 
		confirmAdventure[t, t', adv]

*/
}



/* ------- EXECUTION -------- */
fact {
	init [T/first]
	all t: TIME - T/last | trans [t, T/next[t]]
}

run {} for 3 







