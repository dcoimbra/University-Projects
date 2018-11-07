open util/ordering[DATE] as D
open util/ordering[TIME] as T
open util/integer

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

	clientAccounts: ACCOUNT set -> TIME //6
}

sig BROKER in CLIENT {

	roomReservations: ROOMRESERVATION set -> TIME,
	activityReservations: ACTIVITYRESERVATION set -> TIME,
	adventures: ADVENTURE set -> TIME
}

sig BANK {
	
	bankAccounts: set ACCOUNT //5
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
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', none]
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none]
 	noAdventureChangeExcept[t, t', none]
	noStateChangeExcept[t, t', none]
	//noNewInvoice[t, t']
}

pred clientDeposit [t, t' : TIME, a: ACCOUNT, amount: Int] {
	
	//pre-conditions
	a in CLIENT.clientAccounts.t
	
	//post-conditions
	deposit[t, t', a, amount]

	//frame-conditions
	noBalanceChangeExcept[t, t', a]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none] 
	noAvailabilityChangeExcept[t, t', none] 
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none] 
 	noAdventureChangeExcept[t, t', none]
	noStateChangeExcept[t, t', none]
	//noNewInvoice[t, t']
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

	 offer.activity.activityProvider.activityOffers.t' = offer.activity.activityProvider.activityOffers.t + offer

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', offer]
	noAvailabilityChangeExcept[t, t', offer] 
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none]
	noAdventureChangeExcept[t, t', none]
	noStateChangeExcept[t, t', none]
//	noNewInvoice[t, t']
}


pred createAdventure [t, t': TIME, adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReserv: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT] {
 
	//pre-conditions
	fromAccount in cli.clientAccounts.t
	toAccount in bro.clientAccounts.t
	adv not in BROKER.adventures.t
	actReserv not in BROKER.activityReservations.t
	roomReserv not in BROKER.roomReservations.t

	num > 0
	amount > 0

	//post-conditions
	consistentAdventure[adv, cli, num, bro, actReserv, roomReserv, amount, fromAccount, toAccount]
	adv.state.t' = INITIALSTATE

	bro.adventures.t' = bro.adventures.t + adv

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', none] 
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none]
	noAdventureChangeExcept[t, t', bro]
	noStateChangeExcept[t, t', adv]
//	noNewInvoice[t, t'] 
} 


pred payAdventure [t, t': TIME, adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReservs: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT, invoice: INVOICE] {

	//pre-conditions
	cli != bro
	fromAccount != toAccount
	fromAccount in cli.clientAccounts.t
	toAccount in bro.clientAccounts.t
	adv in bro.adventures.t
	adv.state.t = INITIALSTATE
	consistentAdventure[adv, cli, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount]
	invoice.client = cli
	num > 0
	amount > 0

	//post-conditions
	reserveActivity[t, t', actReserv, actReserv.activityOffer, cli, num]
	one arr, dep: DATE | reserveRooms[t, t', roomReservs, roomReservs.room, cli, arr, dep] 
	adv.state.t' = PAYEDSTATE
	let tax = calculateTax[amount, LEISURE] { 
									      deposit[t, t', toAccount, amount] 
									      deposit[t, t', fromAccount, negate[add[amount, tax]]]
								               makeInvoice[t, t', invoice, cli, LEISURE, amount, tax] 
									    }

	//frame-conditions
	let accounts = fromAccount + toAccount | noBalanceChangeExcept[t, t', accounts]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', actReserv.activityOffer] 
	noRoomReservationsChangeExcept[t, t', bro]
	noActivityReservationChangeExcept[t, t', bro]
	noAdventureChangeExcept[t, t', none]
	noStateChangeExcept[t, t', adv]
//	onlyNewInvoice[t, t', invoice] 
}

pred cancelAdventure [t, t': TIME, adv: ADVENTURE, inv: INVOICE] {

	//pre-conditions
	adv in BROKER.adventures.t
	adv.state.t = PAYEDSTATE
	inv in registered.t

	//post-conditions
	adv.state.t' = INITIALSTATE
	cancelInvoice[t, t', inv]
	deposit[t, t', adv.brokerAccount, negate[inv.amount]]
	deposit[t, t', adv.payerAccount, add[inv.amount, mul[inv.amount, inv.tax]]]
	cancelRoomReservations[t, t', adv.roomReservations]
	cancelActivityReservation[t, t', adv.activityReservation]
	BROKER.adventures.t' = BROKER.adventures.t - adv

	//frame-conditions
	let accounts = adv.brokerAccount + adv.payerAccount | noBalanceChangeExcept[t, t', accounts]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', adv.activityReservation.activityOffer]
	noRoomReservationsChangeExcept[t, t', adv.broker]
	noActivityReservationChangeExcept[t, t', adv.broker]
	noAdventureChangeExcept[t, t', adv.broker]
	noStateChangeExcept[t, t', adv]
//	noNewInvoice[t, t']
}

pred confirmAdventure[t, t': TIME, adv: ADVENTURE] {
	
	//pre-conditions
	adv in adv.broker.adventures.t
	adv.state.t = PAYEDSTATE

	//post-conditions
	adv.state.t' = CONFIRMEDSTATE

	//frame-conditions
	noBalanceChangeExcept[t, t', none]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', none]
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none]
	noAdventureChangeExcept[t, t', none] 
	noStateChangeExcept[t, t', adv]
// 	noNewInvoice[t, t'] 
}

pred makeAnualTaxRed [t, t': TIME, accs: set ACCOUNT] {
	
	//pre-condition
	accs in CLIENT.clientAccounts.t

	//post-condition
	let clients = accs.client :> INVOICE.client {
					//38
		all client: clients | one account: client.clientAccounts.t | deposit[t, t', account, calculateTaxRed[t, client]]
	}

	all inv: registered.t | cancelInvoice[t, t', inv]

	//frame-conditions
	noBalanceChangeExcept[t, t', accs]
	noOpenAccountsChange[t, t']
	noActivityOffersChangeExcept[t, t', none]
	noAvailabilityChangeExcept[t, t', none]
	noRoomReservationsChangeExcept[t, t', none]
	noActivityReservationChangeExcept[t, t', none] 
	noAdventureChangeExcept[t, t', none]
//	noNewInvoice[t, t'] 
}

/*-------AUX PREDICATES-----*/
pred deposit [t, t' : TIME, a: ACCOUNT, amount: Int] {	

	//pre-condition
	 plus[a.balance.t, amount] >= 0

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
	offer.availability.t' = sub[offer.availability.t, particip]
	reserv.activityOffer = offer
	reserv.client = cli
	reserv.participants = particip
	
	BROKER.activityReservations.t' = BROKER.activityReservations.t + reserv
}


pred cancelActivityReservation [t, t': TIME, reserv: ACTIVITYRESERVATION] {

	//pre-conditions
	reserv in BROKER.activityReservations.t
	
	//post-conditions
	reserv.activityOffer.availability.t' = plus[reserv.activityOffer.availability.t, reserv.participants]
	BROKER.activityReservations.t' = BROKER.activityReservations.t - reserv
}


pred reserveRooms [t, t': TIME, reservs: set ROOMRESERVATION, rooms: set ROOM, cli: CLIENT, arr: DATE, dep: DATE] {

	//pre-conditions
	#reservs = #rooms
	reservs not in BROKER.roomReservations.t
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
	some cli.clientAccounts.t
	price > 0
	tx > 0

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
	inv not in registered.t'
}

pred consistentAdventure[adv: ADVENTURE, cli: CLIENT, num: Int, bro: BROKER, actReserv: ACTIVITYRESERVATION, roomReservs: set ROOMRESERVATION, amount: Int, fromAccount: ACCOUNT, toAccount: ACCOUNT] {
	cli != bro
	fromAccount != toAccount
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

pred noActivityOffersChangeExcept[t, t': TIME, offer: ACTIVITYOFFER] {

	ACTIVITYPROVIDER.activityOffers.t' = ACTIVITYPROVIDER.activityOffers.t + offer
}
 
pred noAvailabilityChangeExcept[t, t': TIME, offer: ACTIVITYOFFER] {
	
	all off: ACTIVITYOFFER - offer | off.availability.t' = off.availability.t
}

pred noRoomReservationsChangeExcept[t, t': TIME, bro: set BROKER] {
	
	all brok: BROKER - bro | brok.roomReservations.t' = brok.roomReservations.t
}

pred noActivityReservationChangeExcept[t, t': TIME, bro: set BROKER] {
	
	all brok: BROKER - bro | brok.activityReservations.t' = brok.activityReservations.t
}

pred noAdventureChangeExcept[t, t': TIME, bro: set BROKER] {

	all brok: BROKER - bro | brok.adventures.t' = brok.adventures.t
}

pred noStateChangeExcept[t, t': TIME, adv: ADVENTURE] {
	
	all advt: ADVENTURE - adv | advt.state.t' = advt.state.t
}

pred noNewInvoice[t, t': TIME] {

	registered.t' in registered.t
}

pred onlyNewInvoice[t, t': TIME, inv: INVOICE] {
	
	registered.t' = registered.t + inv
}

/*---------- FUNCTIONS ------------------*/
fun calculateTax[price: Int, type: INVOICETYPE]: Int { //37
	
	{value: one Int {
		type = BUSINESS => value = rem[price, 2]
		type = LEISURE => value = rem[price, 3]
	}}
}

fun calculateTaxRed[t: TIME, cli: CLIENT]: Int {
	
	let inv = { invoice: registered.t | invoice.client = cli} | sum[inv.tax] 
}	

fun getDateInterval[date1, date2: DATE] : set DATE { 

	date1 + (nexts[date1] & prevs[date2]) + date2
}



/*------ VALIDATION ----------*/
pred A1 { //1
	
	some t: TIME | all account: ACCOUNT |  some b: BANK, c: CLIENT | account not in BROKER.clientAccounts.t => openAccount[t, t.next, account, c, b]
}

assert A2 { //2

	some t: TIME | no a: ACCOUNT | some b: BANK, c: CLIENT {
		a in CLIENT.clientAccounts.t
		openAccount[t, t.next, a, c, b]
	}
}

assert A3 { //3
	
	all t: TIME | all a: CLIENT.clientAccounts.t {

		one clientAccounts.t :> a
	}
}

assert A4 { //4

	 all t: TIME {
								   //bankAccounts: bank -> account; bankAccounts.openAcc are the banks associated with an account openAcc
		all openAcc: CLIENT.clientAccounts.t | one bankAccounts.openAcc 
	}
}

pred A5 { //5
	
	some b: BANK | #(b.bankAccounts) >= 2  
}

pred A6 { //6

	some t: TIME, c: CLIENT | #(c.clientAccounts.t) >= 2
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

pred A10 { //10
	
	some h: HOTEL | #(h.rooms) >= 2
}

assert A11 { //11
                                            //rooms: hotel -> room
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

assert A17 { //17

	all t: TIME, activityOffer: ACTIVITYPROVIDER.activityOffers.t | activityOffer.availability.t >= 0 and activityOffer.activity.capacity >= activityOffer.availability.t 
}

assert A18 { //18
	
	all t: TIME, activityReservation: BROKER.activityReservations.t | activityReservation.participants > 0
}

assert A19 { //19
	
	all t: TIME - T/last, activityReservation: ACTIVITYRESERVATION - BROKER.activityReservations.t | some offer: ACTIVITYPROVIDER.activityOffers.t | some client: CLIENT | some num: Int {
		reserveActivity[t, t.next, activityReservation, offer, client, num] => offer.availability.(t.next) = sub[offer.availability.t, num]
	}
}

assert A20 { //20

	all t: TIME - T/last | ACTIVITYPROVIDER.activityOffers.t in ACTIVITYPROVIDER.activityOffers.(t.next)
}

assert A21 { //21

	all t: TIME,  adventure: BROKER.adventures.t {
		
		let s = adventure.state.t |
			(s = INITIALSTATE and s != PAYEDSTATE and s != CONFIRMEDSTATE) or
			(s = PAYEDSTATE and s != INITIALSTATE and s !=  CONFIRMEDSTATE) or
			(s = CONFIRMEDSTATE and s != PAYEDSTATE and s != INITIALSTATE) 
	}
}

assert A22 { //22

	all t: TIME, adventure: BROKER.adventures.t | adventure.participants > 0
}

assert A23 { //23

	all t: TIME, adventure: BROKER.adventures.t {
		
		all reserv: adventure.roomReservations | adventure.payer = reserv.client
		adventure.payer = adventure.activityReservation.client
	}  
}

assert A24 { //24 
	
	all t: TIME, adventure: BROKER.adventures.t {

		all reserv, reserv': adventure.roomReservations | reserv.room.hotel = reserv'.room.hotel
	}
}

assert A25 { //25
	
	all t: TIME, adventure: BROKER.adventures.t {

		adventure.activityReservation.participants = add[#(adventure.roomReservations.room :> roomType.SINGLE), mul[#(adventure.roomReservations.room :> roomType.DOUBLE), 2]]
	}
}

assert A26 { //26

	all t: TIME - T/last | no a: ADVENTURE | some num: Int, amount: Int,  c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t, inv: INVOICE - registered.t  {
		a not in BROKER.adventures.t
		payAdventure[t, t.next, a, c, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount, inv]
	}
}

assert A27 { //27
	
	some t: TIME - T/last | all a: BROKER.adventures.t | some c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t, inv: INVOICE - registered.t  {

		payAdventure[t, t.next, a, c, 2, bro, actReserv, roomReservs, 1, fromAccount, toAccount, inv] => fromAccount in c.clientAccounts.t
	}
}

assert A28 { //28
	
	some t: TIME - T/last | all a: BROKER.adventures.t | some num: Int, amount: Int, c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t, inv: INVOICE - registered.t  {

		payAdventure[t, t.next, a, c, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount, inv] => toAccount in bro.clientAccounts.t
	}
}


assert A29 { //29

	all t: TIME - T/last | no a: BROKER.adventures.t | some inv: registered.t {
		a.state.t != PAYEDSTATE
		cancelAdventure[t, t.next, a, inv]
	}
}

assert A30 { //30
	
	all t: TIME - T/last, adv: BROKER.adventures.t | some inv: registered.t | cancelAdventure[t, t.next, adv, inv] => BROKER.adventures.(t.next) = BROKER.adventures.t - adv 
																				 		     else BROKER.adventures.(t.next) = BROKER.adventures.t 
}

assert A31 { //31

	some t: TIME - T/last | all adv: ADVENTURE | some num: Int, amount: Int, c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t, inv: INVOICE  {
			payAdventure[t, t.next, adv, c, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount, inv] => adv in BROKER.adventures.t
																								    
	}
}

assert A32 { //32

	some t: TIME - T/last | all adv: ADVENTURE |{
		confirmAdventure[t, t.next, adv] => (adv.state.t = PAYEDSTATE)
	}
}

assert A33 { //33
	
	all t: TIME, inv: registered.t | some inv.client.clientAccounts
}

assert A34 {//34

	some t: TIME | #(registered.(t.next)) < #(registered.t)
}

assert A35 {//35

	all t: TIME - T/last, inv: INVOICE | some adv: ADVENTURE, num: Int, amount: Int, c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t  {
			payAdventure[t, t.next, adv, c, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount, inv] => inv in registered.(t.next)																					    
	}	
}

assert A36 { //36

	all t: TIME - T/last,  inv: INVOICE | some adv: ADVENTURE, num: Int, amount: Int, c: CLIENT, bro: BROKER, actReserv: bro.activityReservations.t, roomReservs: bro.roomReservations.t, fromAccount: c.clientAccounts.t, toAccount: bro.clientAccounts.t  {
		not payAdventure[t, t.next, adv, c, num, bro, actReserv, roomReservs, amount, fromAccount, toAccount, inv] => not inv in registered.(t.next)																					    
	}
} 

assert A37 { //37
	
	all t: TIME, inv: registered.t | inv.tax >= 0
}


assert A39 { //39

	some t: TIME| no account: CLIENT.clientAccounts.t | makeAnualTaxRed[t, t.next, account] => account.balance.(t.next) < account.balance.t 
}

assert A40 { //40

	some t: TIME| some account: CLIENT.clientAccounts.t | 
		makeAnualTaxRed[t, t.next, account] => account.balance.(t.next) > account.balance.t 
}

assert A41 { //41

	all t: TIME| no account: ACCOUNT -  (registered.t).client.clientAccounts.t | makeAnualTaxRed[t, t.next, account]
}


/* ----------INITIALIZATION ------------- */
pred init [t: TIME] {

	no CLIENT.clientAccounts.t 
	no BROKER.roomReservations.t
	no ACTIVITYPROVIDER.activityOffers.t
	no BROKER.activityReservations.t
	no BROKER.adventures.t
	no registered.t
}

/* ------- TRANSITION RELATION ------ */
pred trans [t, t': TIME] {
	
	some depos: Int, avail: Int, num: Int, price: Int, account, fromAccount, toAccount: ACCOUNT, cli: CLIENT, bk: BANK, off: ACTIVITYOFFER, act: ACTIVITY, beg: DATE, ed: DATE, adv: ADVENTURE, bro: BROKER, actRes: ACTIVITYRESERVATION, roomRes: ROOMRESERVATION, inv: INVOICE, accs: set ACCOUNT {

		openAccount[t, t', account, cli, bk] or
		clientDeposit[t, t', account, depos] or
		makeActivityOffer[t, t', off, act, beg, ed, avail] or
		createAdventure[t, t', adv, cli, num, bro, actRes, roomRes, price, fromAccount, toAccount] or
		payAdventure[t, t', adv, cli, num, bro, actRes, roomRes, price, fromAccount, toAccount, inv] or
		cancelAdventure[t, t', adv, inv] or
		confirmAdventure[t, t', adv] or
		makeAnualTaxRed[t, t', accs]
	}
}



/* ------- EXECUTION -------- */

fact {
	init [T/first]
	all t: TIME - T/last | trans [t, T/next[t]]
}

run A1
check A2
check A3 
check A4
run A5
run A6
check A7
check A8 
check A9 
run A10
check A11  
check A12  
check A13 
check A14 
check A15 
check A16  
check A17  
check A18  
check A19
check A20 
check A21  
check A22  
check A23  
check A24  
check A25  
check A26 
check A27
check A28
check A29 
check A30 
check A31
check A32
check A33
check A34
check A35
check A36
check A37
check A39
check A40
check A41

/*
run openAccount for 2 but exactly 2 TIME
run deposit for 2 but exactly 2 TIME
run reserveActivity for 2 but exactly 2 TIME
run cancelActivityReservation for 2 but exactly 3 TIME
run reserveRooms for 2 but exactly 2 TIME
run cancelRoomReservations for 2 but exactly 3 TIME
run makeInvoice for 2 but exactly 3 TIME
run cancelInvoice for 2 but exactly 2 TIME
*/
