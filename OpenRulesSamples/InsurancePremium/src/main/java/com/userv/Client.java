/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.userv;


public class Client {
	String 		name;
	Driver[] 	drivers;
	Car[] 		cars;
	double		autoPremium;
	double		driverPremium;
	double		basePremium;
	double		totalPremium;
	int			eligibilityScore;
	String		eligibilityRating;
	boolean 	longTermClient;
	String		segment;
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public Driver[] getDrivers() {
		return drivers;
	}
	public void setDrivers(Driver[] drivers) {
		this.drivers = drivers;
	}
	public Car[] getCars() {
		return cars;
	}
	public void setCars(Car[] cars) {
		this.cars = cars;
	}
	public double getAutoPremium() {
		return autoPremium;
	}
	public void setAutoPremium(double autoPremium) {
		this.autoPremium = autoPremium;
	}
	
	public double getDriverPremium() {
		return driverPremium;
	}
	public void setDriverPremium(double driverPremium) {
		this.driverPremium = driverPremium;
	}
	public double getTotalPremium() {
		return totalPremium;
	}
	public void setTotalPremium(double totalPremium) {
		this.totalPremium = totalPremium;
	}
	public int getEligibilityScore() {
		return eligibilityScore;
	}
	public void setEligibilityScore(int eligibilityScore) {
		this.eligibilityScore = eligibilityScore;
	}
	public String getSegment() {
		return segment;
	}
	public void setSegment(String segment) {
		this.segment = segment;
	}
	public String getEligibilityRating() {
		return eligibilityRating;
	}
	public void setEligibilityRating(String eligibilityRating) {
		this.eligibilityRating = eligibilityRating;
	}
	public boolean isLongTermClient() {
		return longTermClient;
	}
	public void setLongTermClient(boolean longTermClient) {
		this.longTermClient = longTermClient;
	}
	
	public double getBasePremium() {
		return basePremium;
	}
	public void setBasePremium(double basePremium) {
		this.basePremium = basePremium;
	}
	@Override
	public String toString() {
		StringBuffer buf = new StringBuffer();
		buf.append("Client:" 
				+ "\nlongTermClient=" + longTermClient 
				+ "\nsegment=" + segment
				+ "\neligibilityScore="	+ eligibilityScore 
				+ "\neligibilityRating=" + eligibilityRating
				+ "\ndriverPremium=" + driverPremium 
				+ "\nbasePremium=" + basePremium
				+ "\nautoPremium=" + autoPremium
				+ "\nTOTAL PREMIUM=" + totalPremium);
		for (int i = 0; i < drivers.length; i++) {
			buf.append(drivers[i]);
		}
		for (int i = 0; i < cars.length; i++) {
			buf.append(cars[i]);
		}
		return buf.toString();
	}
	
	
	
}
