/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.openrules.flights;

public class Problem {
	
	Passenger[] passengers;
	Flight[] flights;
	
	public Problem() {
			
			flights = new Flight[] {
			    new Flight("UA123", "SFO", "SNA", "1/1/17 6:00 PM", "1/1/17 7:00 PM", 5, "cancelled"),
				new Flight("UA456", "SFO", "SNA", "1/1/17 7:00 PM", "1/1/17 8:00 PM", 2, "scheduled"),
				new Flight("UA789", "SFO", "SNA", "1/1/17 9:00 PM", "1/1/17 11:00 PM", 2, "scheduled"),
				new Flight("UA1001", "SFO", "SNA", "1/1/17 11:00 PM", "1/2/17 5:00 AM", 0, "scheduled"),
				new Flight("UA1111", "SFO", "LAX", "1/1/17 11:00 PM", "1/2/17 5:00 AM", 2, "scheduled")
			};
			
			passengers = new Passenger[] {
				new Passenger("Tom", "BRONZE", 10, "UA123", flights),
				new Passenger("Igor", "GOLD", 50000, "UA123", flights),
				new Passenger("Jenny", "GOLD", 500000, "UA123",flights),
				new Passenger("Harry", "GOLD", 100000, "UA123",flights),
				new Passenger("Dick", "SILVER", 100, "UA123",flights)
			};
			
			System.out.println("====  Passengers");
			for(Passenger p : passengers) {
				System.out.println(p);
			}
			System.out.println("====  Flights");
			for(Flight f : flights) {
				System.out.println(f);
			}
	}

	public Passenger[] getPassengers() {
		return passengers;
	}

	public void setPassengers(Passenger[] passengers) {
		this.passengers = passengers;
	}

	public Flight[] getFlights() {
		return flights;
	}

	public void setFlights(Flight[] flights) {
		this.flights = flights;
	}

}
