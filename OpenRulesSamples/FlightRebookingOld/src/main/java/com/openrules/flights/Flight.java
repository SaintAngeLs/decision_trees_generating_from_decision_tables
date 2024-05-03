/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.openrules.flights;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Flight extends com.openrules.core.ComparableDecisionVariable {

	String number;
	String from;
	String to;
	Date departureTime;
	Date arrivaTime;
	int capacity;
	String status;
	boolean suitable; // for a particular passenger
	
	public Flight() {
	}

	public Flight(String number, String from, String to, String departure, String arrival, int capacity,
			String status) {
		super();
		this.number = number;
		this.from = from;
		this.to = to;
		this.capacity = capacity;
		this.status = status;
		DateFormat formatter = new SimpleDateFormat("MM/dd/yy hh:mm aaa");
		try {
			this.departureTime = formatter.parse(departure);
			this.arrivaTime = formatter.parse(arrival);
		} catch (Exception e) {
			System.out.println("Invalid Date format: " + departure + " or " + arrival);
			throw new RuntimeException(e);
		}

	}
	
	public String toString() {
		return "Flight [number=" + number + ", from=" + from + ", to=" + to + ", departure=" + departureTime + ", arrival="
				+ arrivaTime + ", capacity=" + capacity + ", status=" + status + "]";
	}

	public String getNumber() {
		return number;
	}

	public void setNumber(String number) {
		this.number = number;
	}

	public String getFrom() {
		return from;
	}

	public void setFrom(String from) {
		this.from = from;
	}

	public String getTo() {
		return to;
	}

	public void setTo(String to) {
		this.to = to;
	}

	public Date getDepartureTime() {
		return departureTime;
	}

	public void setDepartureTime(Date departure) {
		this.departureTime = departure;
	}

	public Date getArrivalTime() {
		return arrivaTime;
	}

	public void setArrivalTime(Date arrival) {
		this.arrivaTime = arrival;
	}

	public int getCapacity() {
		return capacity;
	}

	public void setCapacity(int capacity) {
		this.capacity = capacity;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public boolean isSuitable() {
		return suitable;
	}

	public void setSuitable(boolean suitable) {
		this.suitable = suitable;
	}
}
