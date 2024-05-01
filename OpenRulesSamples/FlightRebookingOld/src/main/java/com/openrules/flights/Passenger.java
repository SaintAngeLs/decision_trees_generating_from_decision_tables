/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.openrules.flights;

import java.util.Arrays;

public class Passenger extends com.openrules.core.ComparableDecisionVariable {

	String name;
	String status;
	int miles;
	String departureAirport;
	String arrivalAirport;
	Flight flight;
	String newFlight;
	Flight[] flights;
	
	public Passenger() {
	    
	}

	public Passenger(String name, String status, int miles, String flightNumber, Flight[] flights) {
		super();
		this.name = name;
		this.status = status;
		this.miles = miles;
		this.flights = flights;
		flight = null;
		for (Flight f : flights) {
			if (f.number.equals(flightNumber)) {
				this.flight = f;
				break;
			}
		}
		if (flight == null)
			throw new RuntimeException("CANNOT FIND FLIGHT " + flightNumber);
		newFlight = null;
		departureAirport = flight.from;
		arrivalAirport = flight.to;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public int getMiles() {
		return miles;
	}

	public void setMiles(int miles) {
		this.miles = miles;
	}

	public Flight getFlight() {
		return flight;
	}

	public void setFlight(Flight flight) {
		this.flight = flight;
	}

	public String getNewFlight() {
		return newFlight;
	}

	public void setNewFlight(String newFlight) {
		this.newFlight = newFlight;
	}

	public Flight[] getFlights() {
		return flights;
	}

	public void setFlights(Flight[] flights) {
		this.flights = flights;
	}

	public String getDepartureAirport() {
		return departureAirport;
	}

	public void setDepartureAirport(String departureAirport) {
		this.departureAirport = departureAirport;
	}

	public String getArrivalAirport() {
		return arrivalAirport;
	}

	public void setArrivalAirport(String arrivalAirport) {
		this.arrivalAirport = arrivalAirport;
	}

    @Override
	public String toString() {
		return "Passenger [name=" + name + ", status=" + status + ", miles=" + miles + ", flight="
				+ flight.getNumber() + "] - rebooked to " + newFlight;
	}
	
	public void sortFlights() {
		Arrays.sort(flights);
	}

}
